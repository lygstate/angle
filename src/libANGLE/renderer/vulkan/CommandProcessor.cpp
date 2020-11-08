//
// Copyright 2020 The ANGLE Project Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// CommandProcessor.cpp:
//    Implements the class methods for CommandProcessor.
//

#include "libANGLE/renderer/vulkan/CommandProcessor.h"
#include "libANGLE/renderer/vulkan/RendererVk.h"
#include "libANGLE/trace.h"

namespace rx
{
namespace vk
{
namespace
{
constexpr size_t kInFlightCommandsLimit = 100u;
constexpr bool kOutputVmaStatsString    = false;

void InitializeSubmitInfo(VkSubmitInfo *submitInfo,
                          const vk::PrimaryCommandBuffer &commandBuffer,
                          const std::vector<VkSemaphore> &waitSemaphores,
                          const std::vector<VkPipelineStageFlags> &waitSemaphoreStageMasks,
                          const vk::Semaphore *signalSemaphore)
{
    // Verify that the submitInfo has been zero'd out.
    ASSERT(submitInfo->signalSemaphoreCount == 0);
    ASSERT(waitSemaphores.size() == waitSemaphoreStageMasks.size());
    submitInfo->sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo->commandBufferCount = commandBuffer.valid() ? 1 : 0;
    submitInfo->pCommandBuffers    = commandBuffer.ptr();
    submitInfo->waitSemaphoreCount = static_cast<uint32_t>(waitSemaphores.size());
    submitInfo->pWaitSemaphores    = waitSemaphores.data();
    submitInfo->pWaitDstStageMask  = waitSemaphoreStageMasks.data();

    if (signalSemaphore)
    {
        submitInfo->signalSemaphoreCount = 1;
        submitInfo->pSignalSemaphores    = signalSemaphore->ptr();
    }
}

bool CommandsHaveValidOrdering(const std::vector<vk::CommandBatch> &commands)
{
    Serial currentSerial;
    for (const vk::CommandBatch &commands : commands)
    {
        if (commands.serial <= currentSerial)
        {
            return false;
        }
        currentSerial = commands.serial;
    }

    return true;
}
}  // namespace

void CommandProcessorTask::initTask()
{
    mTask                        = CustomTask::Invalid;
    mRenderPass                  = nullptr;
    mCommandBuffer               = nullptr;
    mSemaphore                   = nullptr;
    mOneOffFence                 = nullptr;
    mPresentInfo                 = {};
    mPresentInfo.pResults        = nullptr;
    mPresentInfo.pSwapchains     = nullptr;
    mPresentInfo.pImageIndices   = nullptr;
    mPresentInfo.pNext           = nullptr;
    mPresentInfo.pWaitSemaphores = nullptr;
    mOneOffCommandBufferVk       = VK_NULL_HANDLE;
}

// CommandProcessorTask implementation
void CommandProcessorTask::initProcessCommands(CommandBufferHelper *commandBuffer,
                                               const RenderPass *renderPass)
{
    mTask          = CustomTask::ProcessCommands;
    mCommandBuffer = commandBuffer;
    mRenderPass    = renderPass;
}

void CommandProcessorTask::copyPresentInfo(const VkPresentInfoKHR &other)
{
    if (other.sType == VK_NULL_HANDLE)
    {
        return;
    }

    mPresentInfo.sType = other.sType;
    mPresentInfo.pNext = other.pNext;

    if (other.swapchainCount > 0)
    {
        ASSERT(other.swapchainCount == 1);
        mPresentInfo.swapchainCount = 1;
        mSwapchain                  = other.pSwapchains[0];
        mPresentInfo.pSwapchains    = &mSwapchain;
        mImageIndex                 = other.pImageIndices[0];
        mPresentInfo.pImageIndices  = &mImageIndex;
    }

    if (other.waitSemaphoreCount > 0)
    {
        ASSERT(other.waitSemaphoreCount == 1);
        mPresentInfo.waitSemaphoreCount = 1;
        mWaitSemaphore                  = other.pWaitSemaphores[0];
        mPresentInfo.pWaitSemaphores    = &mWaitSemaphore;
    }

    mPresentInfo.pResults = other.pResults;

    void *pNext = const_cast<void *>(other.pNext);
    while (pNext != nullptr)
    {
        VkStructureType sType = *reinterpret_cast<VkStructureType *>(pNext);
        switch (sType)
        {
            case VK_STRUCTURE_TYPE_PRESENT_REGIONS_KHR:
            {
                const VkPresentRegionsKHR *presentRegions =
                    reinterpret_cast<VkPresentRegionsKHR *>(pNext);
                mPresentRegion = *presentRegions->pRegions;
                mRects.resize(mPresentRegion.rectangleCount);
                for (uint32_t i = 0; i < mPresentRegion.rectangleCount; i++)
                {
                    mRects[i] = presentRegions->pRegions->pRectangles[i];
                }
                mPresentRegion.pRectangles = mRects.data();

                mPresentRegions.sType          = VK_STRUCTURE_TYPE_PRESENT_REGIONS_KHR;
                mPresentRegions.pNext          = presentRegions->pNext;
                mPresentRegions.swapchainCount = 1;
                mPresentRegions.pRegions       = &mPresentRegion;
                mPresentInfo.pNext             = &mPresentRegions;
                pNext                          = const_cast<void *>(presentRegions->pNext);
                break;
            }
            default:
                ERR() << "Unknown sType: " << sType << " in VkPresentInfoKHR.pNext chain";
                UNREACHABLE();
                break;
        }
    }
}

void CommandProcessorTask::initPresent(egl::ContextPriority priority, VkPresentInfoKHR &presentInfo)
{
    mTask     = CustomTask::Present;
    mPriority = priority;
    copyPresentInfo(presentInfo);
}

void CommandProcessorTask::initFinishToSerial(Serial serial)
{
    // Note: sometimes the serial is not valid and that's okay, the finish will early exit in the
    // TaskProcessor::finishToSerial
    mTask   = CustomTask::FinishToSerial;
    mSerial = serial;
}

void CommandProcessorTask::initFlushAndQueueSubmit(
    std::vector<VkSemaphore> &&waitSemaphores,
    std::vector<VkPipelineStageFlags> &&waitSemaphoreStageMasks,
    const Semaphore *semaphore,
    egl::ContextPriority priority,
    GarbageList &&currentGarbage,
    Serial submitQueueSerial)
{
    mTask                    = CustomTask::FlushAndQueueSubmit;
    mWaitSemaphores          = std::move(waitSemaphores);
    mWaitSemaphoreStageMasks = std::move(waitSemaphoreStageMasks);
    mSemaphore               = semaphore;
    mGarbage                 = std::move(currentGarbage);
    mPriority                = priority;
    mSerial                  = submitQueueSerial;
}

void CommandProcessorTask::initOneOffQueueSubmit(VkCommandBuffer oneOffCommandBufferVk,
                                                 egl::ContextPriority priority,
                                                 const Fence *fence,
                                                 Serial submitQueueSerial)
{
    mTask                  = CustomTask::OneOffQueueSubmit;
    mOneOffCommandBufferVk = oneOffCommandBufferVk;
    mOneOffFence           = fence;
    mPriority              = priority;
    mSerial                = submitQueueSerial;
}

CommandProcessorTask &CommandProcessorTask::operator=(CommandProcessorTask &&rhs)
{
    if (this == &rhs)
    {
        return *this;
    }

    mRenderPass    = rhs.mRenderPass;
    mCommandBuffer = rhs.mCommandBuffer;
    std::swap(mTask, rhs.mTask);
    std::swap(mWaitSemaphores, rhs.mWaitSemaphores);
    std::swap(mWaitSemaphoreStageMasks, rhs.mWaitSemaphoreStageMasks);
    mSemaphore   = rhs.mSemaphore;
    mOneOffFence = rhs.mOneOffFence;
    std::swap(mGarbage, rhs.mGarbage);
    std::swap(mSerial, rhs.mSerial);
    std::swap(mPriority, rhs.mPriority);
    mOneOffCommandBufferVk = rhs.mOneOffCommandBufferVk;

    copyPresentInfo(rhs.mPresentInfo);

    // clear rhs now that everything has moved.
    rhs.initTask();

    return *this;
}

// CommandBatch implementation.
CommandBatch::CommandBatch() = default;

CommandBatch::~CommandBatch() = default;

CommandBatch::CommandBatch(CommandBatch &&other)
{
    *this = std::move(other);
}

CommandBatch &CommandBatch::operator=(CommandBatch &&other)
{
    std::swap(primaryCommands, other.primaryCommands);
    std::swap(commandPool, other.commandPool);
    std::swap(fence, other.fence);
    std::swap(serial, other.serial);
    return *this;
}

void CommandBatch::destroy(VkDevice device)
{
    primaryCommands.destroy(device);
    commandPool.destroy(device);
    fence.reset(device);
}

// CommandProcessor implementation.
void CommandProcessor::handleError(VkResult errorCode,
                                   const char *file,
                                   const char *function,
                                   unsigned int line)
{
    ASSERT(errorCode != VK_SUCCESS);

    std::stringstream errorStream;
    errorStream << "Internal Vulkan error (" << errorCode << "): " << VulkanResultString(errorCode)
                << ".";

    if (errorCode == VK_ERROR_DEVICE_LOST)
    {
        WARN() << errorStream.str();
        handleDeviceLost();
    }

    std::lock_guard<std::mutex> queueLock(mErrorMutex);
    Error error = {errorCode, file, function, line};
    mErrors.emplace(error);
}

CommandProcessor::CommandProcessor(RendererVk *renderer)
    : Context(renderer), mWorkerThreadIdle(false)
{
    std::lock_guard<std::mutex> queueLock(mErrorMutex);
    while (!mErrors.empty())
    {
        mErrors.pop();
    }
}

CommandProcessor::~CommandProcessor() = default;

Error CommandProcessor::getAndClearPendingError()
{
    std::lock_guard<std::mutex> queueLock(mErrorMutex);
    Error tmpError({VK_SUCCESS, nullptr, nullptr, 0});
    if (!mErrors.empty())
    {
        tmpError = mErrors.front();
        mErrors.pop();
    }
    return tmpError;
}

void CommandProcessor::queueCommand(Context *context, CommandProcessorTask *task)
{
    ANGLE_TRACE_EVENT0("gpu.angle", "CommandProcessor::queueCommand");
    // Grab the worker mutex so that we put things on the queue in the same order as we give out
    // serials.
    std::lock_guard<std::mutex> queueLock(mWorkerMutex);

    mTasks.emplace(std::move(*task));
    mWorkAvailableCondition.notify_one();
}

void CommandProcessor::processTasks()
{

    while (true)
    {
        bool exitThread      = false;
        angle::Result result = processTasksImpl(&exitThread);
        if (exitThread)
        {
            // We are doing a controlled exit of the thread, break out of the while loop.
            break;
        }
        if (result != angle::Result::Continue)
        {
            // TODO: https://issuetracker.google.com/issues/170311829 - follow-up on error handling
            // ContextVk::commandProcessorSyncErrorsAndQueueCommand and WindowSurfaceVk::destroy
            // do error processing, is anything required here? Don't think so, mostly need to
            // continue the worker thread until it's been told to exit.
            UNREACHABLE();
        }
    }
}

angle::Result CommandProcessor::processTasksImpl(bool *exitThread)
{
    ANGLE_TRY(mCommandQueue.init(this));

    while (true)
    {
        std::unique_lock<std::mutex> lock(mWorkerMutex);
        if (mTasks.empty())
        {
            mWorkerThreadIdle = true;
            mWorkerIdleCondition.notify_all();
            // Only wake if notified and command queue is not empty
            mWorkAvailableCondition.wait(lock, [this] { return !mTasks.empty(); });
        }
        mWorkerThreadIdle = false;
        CommandProcessorTask task(std::move(mTasks.front()));
        mTasks.pop();
        lock.unlock();

        ANGLE_TRY(processTask(&task));
        if (task.getTaskCommand() == CustomTask::Exit)
        {

            *exitThread = true;
            lock.lock();
            mWorkerThreadIdle = true;
            mWorkerIdleCondition.notify_one();
            return angle::Result::Continue;
        }
    }

    UNREACHABLE();
    return angle::Result::Stop;
}

angle::Result CommandProcessor::processTask(CommandProcessorTask *task)
{
    switch (task->getTaskCommand())
    {
        case CustomTask::Exit:
        {
            ANGLE_TRY(mCommandQueue.finishToSerial(this, Serial::Infinite(),
                                                   mRenderer->getMaxFenceWaitTimeNs()));
            // Shutting down so cleanup
            mCommandQueue.destroy(mRenderer);
            mCommandPool.destroy(mRenderer->getDevice());
            break;
        }
        case CustomTask::FlushAndQueueSubmit:
        {
            ANGLE_TRACE_EVENT0("gpu.angle", "processTask::FlushAndQueueSubmit");
            // End command buffer

            // Get shared submit fence. It's possible there are other users of this fence that
            // must wait for the work to be submitted before waiting on the fence. Reset the fence
            // immediately so we are sure to get a fresh one next time.
            Shared<Fence> fence;
            ANGLE_TRY(mRenderer->newSharedFence(this, &fence));

            // Call submitFrame()
            ANGLE_TRY(mCommandQueue.submitFrame(
                this, task->getPriority(), task->getWaitSemaphores(),
                task->getWaitSemaphoreStageMasks(), task->getSemaphore(), std::move(fence),
                std::move(task->getGarbage()), &mCommandPool, task->getQueueSerial()));

            ASSERT(task->getGarbage().empty());
            break;
        }
        case CustomTask::OneOffQueueSubmit:
        {
            ANGLE_TRACE_EVENT0("gpu.angle", "processTask::OneOffQueueSubmit");
            VkSubmitInfo submitInfo = {};
            submitInfo.sType        = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            if (task->getOneOffCommandBufferVk() != VK_NULL_HANDLE)
            {
                submitInfo.commandBufferCount = 1;
                submitInfo.pCommandBuffers    = &task->getOneOffCommandBufferVk();
            }

            // TODO: https://issuetracker.google.com/issues/170328907 - vkQueueSubmit should be
            // owned by TaskProcessor to ensure proper synchronization
            ANGLE_TRY(mCommandQueue.queueSubmit(this, task->getPriority(), submitInfo,
                                                task->getOneOffFence(), task->getQueueSerial()));
            ANGLE_TRY(mCommandQueue.checkCompletedCommands(this));
            break;
        }
        case CustomTask::FinishToSerial:
        {
            ANGLE_TRY(mCommandQueue.finishToSerial(this, task->getQueueSerial(),
                                                   mRenderer->getMaxFenceWaitTimeNs()));
            break;
        }
        case CustomTask::Present:
        {
            VkResult result =
                present(mRenderer->getVkQueue(task->getPriority()), task->getPresentInfo());
            if (ANGLE_UNLIKELY(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR))
            {
                // We get to ignore these as they are not fatal
            }
            else if (ANGLE_UNLIKELY(result != VK_SUCCESS))
            {
                // Save the error so that we can handle it.
                // Don't leave processing loop, don't consider errors from present to be fatal.
                // TODO: https://issuetracker.google.com/issues/170329600 - This needs to improve to
                // properly parallelize present
                handleError(result, __FILE__, __FUNCTION__, __LINE__);
            }
            break;
        }
        case CustomTask::ProcessCommands:
        {
            ASSERT(!task->getCommandBuffer()->empty());
            if (task->getRenderPass())
            {
                ANGLE_TRY(mCommandQueue.flushRenderPassCommands(this, *task->getRenderPass(),
                                                                task->getCommandBuffer()));
            }
            else
            {
                ANGLE_TRY(mCommandQueue.flushOutsideRPCommands(this, task->getCommandBuffer()));
            }
            ASSERT(task->getCommandBuffer()->empty());
            mRenderer->recycleCommandBufferHelper(task->getCommandBuffer());
            break;
        }
        case CustomTask::CheckCompletedCommands:
        {
            ANGLE_TRY(mCommandQueue.checkCompletedCommands(this));
            break;
        }
        default:
            UNREACHABLE();
            break;
    }

    return angle::Result::Continue;
}

void CommandProcessor::checkCompletedCommands(Context *context)
{
    CommandProcessorTask checkCompletedTask;
    checkCompletedTask.initTask(CustomTask::CheckCompletedCommands);
    queueCommand(this, &checkCompletedTask);
}

void CommandProcessor::waitForWorkComplete(Context *context)
{
    ANGLE_TRACE_EVENT0("gpu.angle", "CommandProcessor::waitForWorkComplete");
    std::unique_lock<std::mutex> lock(mWorkerMutex);
    mWorkerIdleCondition.wait(lock, [this] { return (mTasks.empty() && mWorkerThreadIdle); });
    // Worker thread is idle and command queue is empty so good to continue

    if (!context)
    {
        return;
    }

    // Sync any errors to the context
    while (hasPendingError())
    {
        Error workerError = getAndClearPendingError();
        if (workerError.mErrorCode != VK_SUCCESS)
        {
            context->handleError(workerError.mErrorCode, workerError.mFile, workerError.mFunction,
                                 workerError.mLine);
        }
    }
}

// TODO: https://issuetracker.google.com/170311829 - Add vk::Context so that queueCommand has
// someplace to send errors.
void CommandProcessor::shutdown(std::thread *commandProcessorThread)
{
    CommandProcessorTask endTask;
    endTask.initTask(CustomTask::Exit);
    queueCommand(this, &endTask);
    waitForWorkComplete(nullptr);
    if (commandProcessorThread->joinable())
    {
        commandProcessorThread->join();
    }
}

Serial CommandProcessor::getLastCompletedQueueSerial()
{
    std::lock_guard<std::mutex> lock(mQueueSerialMutex);
    return mCommandQueue.getLastCompletedQueueSerial();
}

Serial CommandProcessor::getLastSubmittedQueueSerial()
{
    std::lock_guard<std::mutex> lock(mQueueSerialMutex);
    return mCommandQueue.getLastSubmittedQueueSerial();
}

Serial CommandProcessor::getCurrentQueueSerial()
{
    std::lock_guard<std::mutex> lock(mQueueSerialMutex);
    return mCommandQueue.getCurrentQueueSerial();
}

Serial CommandProcessor::reserveSubmitSerial()
{
    std::lock_guard<std::mutex> lock(mQueueSerialMutex);
    return mCommandQueue.reserveSubmitSerial();
}

// Wait until all commands up to and including serial have been processed
void CommandProcessor::finishToSerial(Context *context, Serial serial)
{
    ANGLE_TRACE_EVENT0("gpu.angle", "CommandProcessor::finishToSerial");
    CommandProcessorTask finishToSerial;
    finishToSerial.initFinishToSerial(serial);
    queueCommand(context, &finishToSerial);

    // Wait until the worker is idle. At that point we know that the finishToSerial command has
    // completed executing, including any associated state cleanup.
    waitForWorkComplete(context);
}

void CommandProcessor::handleDeviceLost()
{
    ANGLE_TRACE_EVENT0("gpu.angle", "CommandProcessor::handleDeviceLost");
    std::unique_lock<std::mutex> lock(mWorkerMutex);
    mWorkerIdleCondition.wait(lock, [this] { return (mTasks.empty() && mWorkerThreadIdle); });

    // Worker thread is idle and command queue is empty so good to continue
    mCommandQueue.handleDeviceLost(mRenderer);
}

void CommandProcessor::finishAllWork(Context *context)
{
    ANGLE_TRACE_EVENT0("gpu.angle", "CommandProcessor::finishAllWork");
    // Wait for GPU work to finish
    finishToSerial(context, Serial::Infinite());
}

VkResult CommandProcessor::getLastAndClearPresentResult(VkSwapchainKHR swapchain)
{
    std::unique_lock<std::mutex> lock(mSwapchainStatusMutex);
    if (mSwapchainStatus.find(swapchain) == mSwapchainStatus.end())
    {
        // Wake when required swapchain status becomes available
        mSwapchainStatusCondition.wait(lock, [this, swapchain] {
            return mSwapchainStatus.find(swapchain) != mSwapchainStatus.end();
        });
    }
    VkResult result = mSwapchainStatus[swapchain];
    mSwapchainStatus.erase(swapchain);
    return result;
}

VkResult CommandProcessor::present(VkQueue queue, const VkPresentInfoKHR &presentInfo)
{
    std::lock_guard<std::mutex> lock(mSwapchainStatusMutex);
    ANGLE_TRACE_EVENT0("gpu.angle", "vkQueuePresentKHR");
    VkResult result = vkQueuePresentKHR(queue, &presentInfo);

    // Verify that we are presenting one and only one swapchain
    ASSERT(presentInfo.swapchainCount == 1);
    ASSERT(presentInfo.pResults == nullptr);
    mSwapchainStatus[presentInfo.pSwapchains[0]] = result;

    mSwapchainStatusCondition.notify_all();

    return result;
}

// CommandQueue implementation.
CommandQueue::CommandQueue() : mCurrentQueueSerial(mQueueSerialFactory.generate()) {}

CommandQueue::~CommandQueue() = default;

void CommandQueue::destroy(RendererVk *renderer)
{
    mLastCompletedQueueSerial = Serial::Infinite();
    clearAllGarbage(renderer);

    mPrimaryCommands.destroy(renderer->getDevice());
    mPrimaryCommandPool.destroy(renderer->getDevice());

    ASSERT(mInFlightCommands.empty() && mGarbageQueue.empty());
}

angle::Result CommandQueue::init(Context *context)
{
    RendererVk *renderer = context->getRenderer();

    // Initialize the command pool now that we know the queue family index.
    uint32_t queueFamilyIndex = renderer->getQueueFamilyIndex();
    ANGLE_TRY(mPrimaryCommandPool.init(context, queueFamilyIndex));

    return angle::Result::Continue;
}

angle::Result CommandQueue::checkCompletedCommands(Context *context)
{
    ANGLE_TRACE_EVENT0("gpu.angle", "CommandQueue::checkCompletedCommandsNoLock");
    RendererVk *renderer = context->getRenderer();
    VkDevice device      = renderer->getDevice();

    int finishedCount = 0;

    for (CommandBatch &batch : mInFlightCommands)
    {
        VkResult result = batch.fence.get().getStatus(device);
        if (result == VK_NOT_READY)
        {
            break;
        }
        ANGLE_VK_TRY(context, result);
        ++finishedCount;
    }

    if (finishedCount == 0)
    {
        return angle::Result::Continue;
    }

    return retireFinishedCommands(context, finishedCount);
}

angle::Result CommandQueue::retireFinishedCommands(Context *context, size_t finishedCount)
{
    ASSERT(finishedCount > 0);

    RendererVk *renderer = context->getRenderer();
    VkDevice device      = renderer->getDevice();

    for (size_t commandIndex = 0; commandIndex < finishedCount; ++commandIndex)
    {
        CommandBatch &batch = mInFlightCommands[commandIndex];

        mLastCompletedQueueSerial = batch.serial;
        renderer->resetSharedFence(&batch.fence);
        ANGLE_TRACE_EVENT0("gpu.angle", "command buffer recycling");
        batch.commandPool.destroy(device);
        ANGLE_TRY(mPrimaryCommandPool.collect(context, std::move(batch.primaryCommands)));
    }

    if (finishedCount > 0)
    {
        auto beginIter = mInFlightCommands.begin();
        mInFlightCommands.erase(beginIter, beginIter + finishedCount);
    }

    size_t freeIndex = 0;
    for (; freeIndex < mGarbageQueue.size(); ++freeIndex)
    {
        GarbageAndSerial &garbageList = mGarbageQueue[freeIndex];
        if (garbageList.getSerial() < mLastCompletedQueueSerial)
        {
            for (GarbageObject &garbage : garbageList.get())
            {
                garbage.destroy(renderer);
            }
        }
        else
        {
            break;
        }
    }

    // Remove the entries from the garbage list - they should be ready to go.
    if (freeIndex > 0)
    {
        mGarbageQueue.erase(mGarbageQueue.begin(), mGarbageQueue.begin() + freeIndex);
    }

    return angle::Result::Continue;
}

angle::Result CommandQueue::releaseToCommandBatch(Context *context,
                                                  PrimaryCommandBuffer &&commandBuffer,
                                                  CommandPool *commandPool,
                                                  CommandBatch *batch)
{
    ANGLE_TRACE_EVENT0("gpu.angle", "CommandQueue::releaseToCommandBatch");

    RendererVk *renderer = context->getRenderer();
    VkDevice device      = renderer->getDevice();

    batch->primaryCommands = std::move(commandBuffer);

    if (commandPool->valid())
    {
        batch->commandPool = std::move(*commandPool);
        // Recreate CommandPool
        VkCommandPoolCreateInfo poolInfo = {};
        poolInfo.sType                   = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags                   = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
        poolInfo.queueFamilyIndex        = renderer->getQueueFamilyIndex();

        ANGLE_VK_TRY(context, commandPool->init(device, poolInfo));
    }

    return angle::Result::Continue;
}

void CommandQueue::clearAllGarbage(RendererVk *renderer)
{
    for (GarbageAndSerial &garbageList : mGarbageQueue)
    {
        for (GarbageObject &garbage : garbageList.get())
        {
            garbage.destroy(renderer);
        }
    }
    mGarbageQueue.clear();
}

void CommandQueue::handleDeviceLost(RendererVk *renderer)
{
    ANGLE_TRACE_EVENT0("gpu.angle", "CommandQueue::handleDeviceLost");

    VkDevice device = renderer->getDevice();

    for (CommandBatch &batch : mInFlightCommands)
    {
        // On device loss we need to wait for fence to be signaled before destroying it
        VkResult status = batch.fence.get().wait(device, renderer->getMaxFenceWaitTimeNs());
        // If the wait times out, it is probably not possible to recover from lost device
        ASSERT(status == VK_SUCCESS || status == VK_ERROR_DEVICE_LOST);

        // On device lost, here simply destroy the CommandBuffer, it will fully cleared later
        // by CommandPool::destroy
        batch.primaryCommands.destroy(device);

        batch.commandPool.destroy(device);
        batch.fence.reset(device);
    }
    mInFlightCommands.clear();
}

bool CommandQueue::allInFlightCommandsAreAfterSerial(Serial serial) const
{
    return mInFlightCommands.empty() || mInFlightCommands[0].serial > serial;
}

angle::Result CommandQueue::finishToSerial(Context *context, Serial finishSerial, uint64_t timeout)
{
    if (mInFlightCommands.empty())
    {
        return angle::Result::Continue;
    }

    ANGLE_TRACE_EVENT0("gpu.angle", "CommandQueue::finishToSerial");

    // Find the serial in the the list. The serials should be in order.
    ASSERT(CommandsHaveValidOrdering(mInFlightCommands));

    size_t finishedCount = 0;
    while (finishedCount < mInFlightCommands.size() &&
           mInFlightCommands[finishedCount].serial <= finishSerial)
    {
        finishedCount++;
    }

    if (finishedCount == 0)
    {
        return angle::Result::Continue;
    }

    const CommandBatch &batch = mInFlightCommands[finishedCount - 1];

    // Wait for it finish
    VkDevice device = context->getDevice();
    VkResult status = batch.fence.get().wait(device, timeout);

    ANGLE_VK_TRY(context, status);

    // Clean up finished batches.
    ANGLE_TRY(retireFinishedCommands(context, finishedCount));
    ASSERT(allInFlightCommandsAreAfterSerial(finishSerial));

    return angle::Result::Continue;
}

Serial CommandQueue::reserveSubmitSerial()
{
    Serial returnSerial = mCurrentQueueSerial;
    mCurrentQueueSerial = mQueueSerialFactory.generate();
    return returnSerial;
}

angle::Result CommandQueue::submitFrame(
    Context *context,
    egl::ContextPriority priority,
    const std::vector<VkSemaphore> &waitSemaphores,
    const std::vector<VkPipelineStageFlags> &waitSemaphoreStageMasks,
    const Semaphore *signalSemaphore,
    Shared<Fence> &&sharedFence,
    GarbageList &&currentGarbage,
    CommandPool *commandPool,
    Serial submitQueueSerial)
{
    // Start an empty primary buffer if we have an empty submit.
    ANGLE_TRY(ensurePrimaryCommandBufferValid(context));
    ANGLE_VK_TRY(context, mPrimaryCommands.end());

    VkSubmitInfo submitInfo = {};
    InitializeSubmitInfo(&submitInfo, mPrimaryCommands, waitSemaphores, waitSemaphoreStageMasks,
                         signalSemaphore);

    ANGLE_TRACE_EVENT0("gpu.angle", "CommandQueue::submitFrame");

    RendererVk *renderer = context->getRenderer();
    VkDevice device      = renderer->getDevice();

    DeviceScoped<CommandBatch> scopedBatch(device);
    CommandBatch &batch = scopedBatch.get();
    batch.fence         = std::move(sharedFence);
    batch.serial        = submitQueueSerial;

    ANGLE_TRY(queueSubmit(context, priority, submitInfo, &batch.fence.get(), batch.serial));

    if (!currentGarbage.empty())
    {
        mGarbageQueue.emplace_back(std::move(currentGarbage), batch.serial);
    }

    // Store the primary CommandBuffer and command pool used for secondary CommandBuffers
    // in the in-flight list.
    ANGLE_TRY(releaseToCommandBatch(context, std::move(mPrimaryCommands), commandPool, &batch));

    mInFlightCommands.emplace_back(scopedBatch.release());

    ANGLE_TRY(checkCompletedCommands(context));

    // CPU should be throttled to avoid mInFlightCommands from growing too fast. Important for
    // off-screen scenarios.
    if (mInFlightCommands.size() > kInFlightCommandsLimit)
    {
        size_t numCommandsToFinish = mInFlightCommands.size() - kInFlightCommandsLimit;
        Serial finishSerial        = mInFlightCommands[numCommandsToFinish].serial;
        ANGLE_TRY(finishToSerial(context, finishSerial, renderer->getMaxFenceWaitTimeNs()));
    }

    return angle::Result::Continue;
}

angle::Result CommandQueue::waitForSerialWithUserTimeout(vk::Context *context,
                                                         Serial serial,
                                                         uint64_t timeout,
                                                         VkResult *result)
{
    // No in-flight work. This indicates the serial is already complete.
    if (mInFlightCommands.empty())
    {
        *result = VK_SUCCESS;
        return angle::Result::Continue;
    }

    // Serial is already complete.
    if (serial < mInFlightCommands[0].serial)
    {
        *result = VK_SUCCESS;
        return angle::Result::Continue;
    }

    size_t batchIndex = 0;
    while (batchIndex != mInFlightCommands.size() && mInFlightCommands[batchIndex].serial < serial)
    {
        batchIndex++;
    }

    // Serial is not yet submitted. This is undefined behaviour, so we can do anything.
    if (batchIndex >= mInFlightCommands.size())
    {
        WARN() << "Waiting on an unsubmitted serial.";
        *result = VK_TIMEOUT;
        return angle::Result::Continue;
    }

    ASSERT(serial == mInFlightCommands[batchIndex].serial);

    vk::Fence &fence = mInFlightCommands[batchIndex].fence.get();
    ASSERT(fence.valid());
    *result = fence.wait(context->getDevice(), timeout);

    // Don't trigger an error on timeout.
    if (*result != VK_TIMEOUT)
    {
        ANGLE_VK_TRY(context, *result);
    }

    return angle::Result::Continue;
}

angle::Result CommandQueue::ensurePrimaryCommandBufferValid(Context *context)
{
    if (mPrimaryCommands.valid())
    {
        return angle::Result::Continue;
    }

    ANGLE_TRY(mPrimaryCommandPool.allocate(context, &mPrimaryCommands));

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType                    = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags                    = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    beginInfo.pInheritanceInfo         = nullptr;
    ANGLE_VK_TRY(context, mPrimaryCommands.begin(beginInfo));

    return angle::Result::Continue;
}

angle::Result CommandQueue::flushOutsideRPCommands(Context *context,
                                                   CommandBufferHelper *outsideRPCommands)
{
    ANGLE_TRY(ensurePrimaryCommandBufferValid(context));
    return outsideRPCommands->flushToPrimary(context->getRenderer()->getFeatures(),
                                             &mPrimaryCommands, nullptr);
}

angle::Result CommandQueue::flushRenderPassCommands(Context *context,
                                                    const RenderPass &renderPass,
                                                    CommandBufferHelper *renderPassCommands)
{
    ANGLE_TRY(ensurePrimaryCommandBufferValid(context));
    return renderPassCommands->flushToPrimary(context->getRenderer()->getFeatures(),
                                              &mPrimaryCommands, &renderPass);
}

angle::Result CommandQueue::queueSubmit(Context *context,
                                        egl::ContextPriority contextPriority,
                                        const VkSubmitInfo &submitInfo,
                                        const Fence *fence,
                                        Serial submitQueueSerial)
{
    ANGLE_TRACE_EVENT0("gpu.angle", "CommandQueue::queueSubmit");

    RendererVk *renderer = context->getRenderer();

    if (kOutputVmaStatsString)
    {
        renderer->outputVmaStatString();
    }

    VkQueue queue       = renderer->getVkQueue(contextPriority);
    VkFence fenceHandle = fence ? fence->getHandle() : VK_NULL_HANDLE;
    ANGLE_VK_TRY(context, vkQueueSubmit(queue, 1, &submitInfo, fenceHandle));
    mLastSubmittedQueueSerial = submitQueueSerial;

    // Now that we've submitted work, clean up RendererVk garbage
    return renderer->cleanupGarbage(mLastCompletedQueueSerial);
}
}  // namespace vk
}  // namespace rx
