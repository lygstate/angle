// GENERATED FILE - DO NOT EDIT.
// Generated by gen_vk_internal_shaders.py.
//
// Copyright 2020 The ANGLE Project Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// shaders/gen/ConvertVertex.comp.00000006.inc:
//   Pre-generated shader for the ANGLE Vulkan back-end.

#pragma once
constexpr uint8_t kConvertVertex_comp_00000006[] = {
    0x1f,0x8b,0x08,0x00,0x00,0x00,0x00,0x00,0x02,0xff,0x9d,0x98,0x79,0x94,0xce,0x65,
    0x14,0xc7,0xdf,0xdf,0x2c,0xef,0x8c,0x99,0x61,0x66,0x2c,0x23,0x59,0x32,0x51,0x8d,
    0x4c,0x66,0x92,0x35,0x4d,0x07,0xc9,0x52,0x93,0x8a,0x70,0x48,0x31,0x99,0x16,0xca,
    0x16,0xa5,0x24,0xc9,0x16,0x59,0x42,0x96,0x50,0x21,0x24,0x4b,0xd6,0x24,0xa4,0x42,
    0xe9,0xa8,0x9c,0x43,0x96,0x36,0xa5,0xd5,0x21,0x4b,0x93,0xca,0x16,0xe9,0xb9,0xef,
    0xfb,0xb9,0xcd,0xf5,0x9c,0xf9,0xab,0x39,0xe7,0x77,0x7e,0xbf,0xfb,0xbd,0xeb,0xf3,
    0x7d,0xee,0xb3,0xbc,0x13,0x1b,0x53,0x23,0x21,0x14,0x0a,0x42,0x49,0xa1,0xc4,0x50,
    0x85,0x20,0x14,0xf9,0x4b,0x0f,0xc5,0x84,0xe4,0x33,0x39,0x14,0x8e,0xbc,0x5b,0xe5,
    0xb7,0xcf,0xaf,0x33,0x60,0x60,0x61,0x9d,0x7a,0xf5,0x73,0x45,0x5f,0x26,0x14,0x1b,
    0xb1,0x13,0x5d,0xaa,0xb3,0x89,0x77,0xef,0x38,0xf7,0xf4,0x2e,0xe8,0xd9,0x47,0xf0,
    0x7d,0xee,0x49,0x73,0x78,0x5c,0x24,0x56,0x28,0xd4,0x14,0x5b,0x79,0x5a,0x3b,0xeb,
    0x2b,0xa2,0x69,0x42,0x35,0x78,0x2b,0x16,0x80,0xc5,0x19,0x2c,0x06,0x2c,0xd1,0x60,
    0xb1,0x60,0x29,0x06,0x8b,0x03,0x4b,0x33,0x58,0x3c,0x58,0x79,0x83,0x85,0xc1,0x2e,
    0x31,0x58,0x02,0x58,0x15,0x83,0x25,0x82,0x55,0x37,0x58,0x29,0xb0,0x9a,0x06,0x4b,
    0x02,0xcb,0x32,0x58,0x32,0x58,0xb6,0xc1,0x52,0xc0,0x72,0x0d,0x56,0x1a,0xac,0x9e,
    0x70,0xec,0x46,0xa5,0xe3,0x6d,0xe5,0x46,0xd3,0x9d,0x5a,0x95,0x8b,0x02,0x8f,0x33,
    0xb1,0x2f,0x80,0x0b,0xb1,0xef,0xe1,0xde,0x97,0xff,0xa7,0x8b,0xca,0x99,0x70,0x2a,
    0xf2,0x4e,0x2f,0xde,0xae,0x12,0xe2,0xed,0x32,0xf1,0x76,0x7b,0xf1,0x76,0x13,0x4f,
    0xe5,0x7d,0x8c,0xb3,0x0a,0x72,0xf9,0x20,0x2a,0x57,0x72,0x4f,0x39,0x37,0x8a,0x98,
    0x88,0x7d,0x6c,0x24,0x9e,0x7c,0x57,0x70,0x36,0x61,0xf8,0x0c,0x45,0xde,0x71,0x11,
    0xde,0x13,0xa8,0x2b,0xd3,0xd5,0x94,0xc8,0xb7,0xe2,0x19,0xce,0x3b,0x1d,0x9f,0x4c,
    0x67,0x5f,0x96,0x7e,0x4a,0xc0,0x3f,0x83,0xef,0x74,0xf4,0x15,0xf9,0xce,0x20,0x5e,
    0x65,0x13,0x2f,0x03,0x9b,0xea,0xd4,0x23,0x58,0x35,0xd7,0x2d,0xda,0x13,0xff,0xe7,
    0x91,0x1a,0xae,0xa4,0x2f,0x24,0x4e,0x13,0xe4,0xab,0xc0,0x64,0xcc,0x59,0xd4,0x2f,
    0xf3,0x50,0x1b,0xb9,0x96,0xf1,0xbf,0x1a,0xdb,0x30,0x7a,0x79,0xe7,0x30,0x4f,0x6a,
    0x5f,0x97,0x9a,0x55,0xdf,0x84,0xfe,0x54,0x7d,0x73,0x4f,0xce,0xc7,0xbf,0xbc,0xf3,
    0xba,0xdd,0xf8,0xb5,0x63,0x7e,0x45,0x16,0x9e,0x3a,0x31,0x17,0x55,0x1d,0xaa,0xfd,
    0x56,0x8d,0xbe,0xea,0x4e,0x7d,0xf7,0x91,0xbb,0x80,0xf1,0x89,0xdc,0xc3,0xd4,0x23,
    0xf9,0x0a,0xcd,0x9c,0x3e,0x88,0x2e,0x6c,0xf4,0xbd,0xe8,0x7f,0xad,0xa3,0x37,0xf6,
    0xaa,0x1f,0xc8,0x3a,0x52,0x79,0xb0,0xa9,0x53,0xe4,0xa7,0xdc,0x73,0x21,0xb6,0x58,
    0x1e,0xca,0xda,0xd1,0x78,0xa3,0x8c,0xbf,0xc8,0x93,0x3c,0xbe,0x66,0x33,0x07,0x52,
    0xdf,0xab,0xcc,0x7d,0x96,0x89,0xff,0x9a,0xc4,0x77,0x7f,0x2a,0x2f,0x30,0xf3,0x25,
    0xfe,0x6f,0xb1,0xdb,0x29,0x6f,0xef,0x46,0xe4,0x61,0x0d,0x85,0xb7,0x9d,0x86,0x37,
    0x59,0x3f,0x3b,0xc9,0xf3,0x39,0x35,0xec,0x82,0x37,0x91,0x77,0x83,0x55,0x74,0xf2,
    0x1e,0xfc,0x62,0xb1,0xdf,0x4b,0xce,0x3d,0xd8,0xef,0x65,0x1f,0x0d,0xd0,0x7f,0xc1,
    0xb7,0xed,0x93,0x22,0xaf,0xce,0x72,0x41,0x74,0xaf,0xcd,0x76,0x92,0xc4,0x91,0x35,
    0x29,0x58,0x11,0xb6,0x0d,0xdc,0x9a,0x88,0xa1,0x37,0x42,0xe4,0x3e,0xed,0x90,0x78,
    0x72,0x0a,0x2f,0x87,0xe0,0x47,0xe5,0xc3,0x9e,0x7c,0xc2,0xc8,0xb2,0xa6,0x2e,0x78,
    0x7a,0x29,0xc8,0xea,0x63,0x3d,0x39,0x2e,0xb8,0xd8,0x3e,0xc1,0xd3,0x97,0xf2,0xf4,
    0x69,0xc8,0xcd,0x5c,0x95,0xc2,0xc1,0xf7,0xf0,0x22,0x3d,0x94,0xc7,0xb8,0x7f,0x00,
    0x17,0x1b,0x59,0x4f,0x3f,0xb2,0x06,0x0b,0x8d,0xcd,0x4f,0xe0,0xcb,0x9d,0x8d,0xac,
    0x89,0x9f,0xf1,0x13,0xfc,0x94,0x63,0xe2,0x20,0x9c,0x9c,0x75,0xf6,0xa2,0xfb,0xc5,
    0x3d,0x07,0xe1,0x47,0xbe,0xcf,0xbb,0x81,0xc9,0xb7,0x60,0x37,0x3a,0xfb,0x43,0xd4,
    0x20,0xdf,0x87,0xf9,0x3e,0xe3,0xf4,0xbf,0xe2,0x23,0xef,0x93,0x2e,0xd6,0x51,0xf7,
    0x3e,0x46,0x6c,0xd1,0x1f,0x47,0x7f,0xdc,0xd4,0xf6,0x1b,0x3c,0xaf,0xa4,0xb6,0xdf,
    0xc1,0x8a,0xa8,0x47,0xe4,0x23,0xee,0x39,0x8a,0xef,0x11,0xc3,0xc7,0x1f,0x25,0xf0,
    0xf1,0x27,0xf8,0x48,0x67,0x23,0xf2,0x5f,0x60,0x45,0xc6,0xe6,0x24,0x39,0x87,0x61,
    0x73,0x0a,0xbb,0x93,0x8c,0xe9,0x04,0x98,0xda,0x9f,0x66,0xee,0x95,0xe3,0x33,0x70,
    0xbc,0xc0,0xd8,0x9c,0x05,0x57,0x8e,0xff,0xc6,0xef,0x2c,0x1c,0x9f,0x37,0x1c,0x8b,
    0xee,0x5c,0x84,0xd7,0xe8,0x98,0xce,0xc1,0x8f,0x8e,0xf1,0xbc,0x89,0x1b,0x04,0xd1,
    0xdc,0x52,0x97,0xf4,0x97,0xc8,0x8d,0x5d,0x0e,0x59,0x8b,0x31,0x41,0xf4,0x7c,0x17,
    0x5c,0xf4,0x17,0xc0,0xf2,0x58,0xab,0xf1,0x41,0xb4,0x47,0x45,0x27,0xbd,0x17,0x6f,
    0x7c,0xc3,0x41,0xf4,0xbc,0x8a,0xc3,0x57,0x7a,0x35,0x1c,0x14,0xe7,0x4d,0x34,0x79,
    0xa5,0x4f,0x13,0x4d,0xdc,0xa4,0x20,0xda,0xdb,0xa2,0x93,0x9e,0x4d,0x8a,0xc4,0x0d,
    0x47,0xfc,0x92,0x03,0xd9,0x0f,0xa2,0xbd,0x5d,0xca,0xc4,0x4b,0x09,0xa2,0xeb,0xeb,
    0x23,0x38,0x2f,0x1d,0x44,0xb1,0xe4,0xa0,0xb8,0xa7,0x04,0x13,0x1e,0x8e,0xc1,0xc3,
    0x31,0xc3,0x43,0x99,0xe0,0xe2,0x39,0x4b,0x0d,0xa2,0xd8,0x02,0xd3,0x87,0xa9,0xc1,
    0xc5,0x7d,0x78,0xd4,0xf8,0xa7,0x93,0x5f,0x6c,0x65,0x5d,0xa5,0xc3,0x85,0xec,0x09,
    0x65,0x83,0xe8,0x79,0x2f,0xb8,0xf4,0x7a,0x23,0xf7,0x34,0x20,0x4f,0x32,0xf3,0x26,
    0x67,0x4c,0x43,0xe1,0x82,0x33,0x4b,0xbf,0x93,0xc8,0x95,0x62,0x72,0xd5,0xc4,0x46,
    0xfb,0xa5,0x36,0xfd,0x52,0xcb,0xd8,0x64,0x83,0x6b,0x9f,0x5e,0x83,0x5f,0xb6,0xb1,
    0xa9,0x43,0x7c,0xb5,0xc9,0x05,0xcb,0x31,0x3c,0x5c,0x8b,0x6f,0xae,0xc9,0x77,0x1d,
    0xf9,0xea,0x9a,0x58,0xf5,0xc0,0xd5,0xaf,0x3e,0xbe,0x82,0xff,0xe3,0xea,0x17,0xd9,
    0x8e,0x3b,0xb5,0x84,0x71,0x97,0x36,0xdf,0x65,0x18,0x77,0x9a,0xd9,0xaf,0x1a,0xb1,
    0x1f,0x6a,0xce,0xc6,0xf8,0x8c,0x25,0xe6,0xf5,0x60,0x39,0x66,0x4c,0x37,0x80,0x37,
    0x61,0x6e,0x1a,0x81,0x69,0x8c,0x3c,0xe2,0x4a,0x8d,0x79,0xa6,0xc6,0x74,0xee,0xb6,
    0xf2,0x57,0xd6,0xd4,0x55,0x8e,0xba,0x2a,0x98,0xba,0x9a,0x7a,0xfb,0x74,0x4b,0x4f,
    0xee,0xec,0xc9,0x5d,0x3c,0xb9,0xab,0x27,0x77,0xf3,0xe4,0xfe,0x9e,0x3c,0xc8,0x93,
    0x47,0x7a,0xf2,0x38,0x4f,0x9e,0xe1,0xc9,0x33,0x8d,0x2c,0x67,0xf6,0x1c,0x4f,0xbf,
    0xc6,0x3b,0x87,0x36,0x7a,0xf2,0x26,0x6f,0x1e,0x9a,0xc1,0x8b,0xf6,0xc7,0x4d,0xf4,
    0x47,0x73,0x63,0xd3,0x02,0x7c,0x34,0xf3,0x72,0x33,0x7e,0x2d,0x98,0x97,0xa6,0x60,
    0x6a,0xdf,0xca,0x8b,0xd9,0xba,0x84,0x98,0x6d,0xc0,0x75,0xfe,0x6f,0xc1,0xaf,0x0d,
    0x31,0x5b,0x82,0xa9,0xfd,0xad,0xcc,0x8d,0xc6,0xbc,0x8d,0x98,0xf9,0xc6,0xa6,0x2d,
    0xb8,0xee,0xb3,0x77,0xe0,0xd7,0xd6,0xd8,0xdc,0x49,0x1c,0x3d,0x53,0xda,0x83,0xc9,
    0x1d,0x70,0x11,0xd8,0x5d,0xf8,0xb6,0x67,0x7f,0xee,0x68,0xf6,0x67,0xd1,0x75,0x70,
    0x4f,0x47,0x7a,0xa9,0x03,0xfd,0xd7,0x09,0xb9,0xa3,0xc9,0x75,0x37,0xfd,0x25,0xe3,
    0xe9,0x82,0xac,0xba,0x7b,0xa8,0x43,0x74,0x5d,0x91,0x75,0x9f,0xbc,0x97,0xbd,0xa5,
    0x0b,0xfd,0x25,0x36,0x9d,0xc1,0xd5,0xff,0x7e,0x7a,0x53,0xe7,0xe4,0x01,0xb0,0x9c,
    0x08,0x47,0xe1,0xc8,0x7d,0xf3,0x21,0xee,0xa4,0x85,0xe8,0xd5,0xb7,0x27,0x3a,0x89,
    0xdb,0x0d,0x59,0x79,0x7d,0x18,0x5e,0x7b,0x19,0xfb,0x47,0xc0,0x97,0xc2,0x4f,0x1f,
    0xb0,0xde,0xf0,0xd3,0xcf,0xf0,0x23,0xba,0xbe,0xee,0x99,0x0e,0x1f,0x7d,0x4d,0x9c,
    0x47,0x19,0xf3,0x12,0xe2,0x0c,0x00,0x13,0xee,0x17,0x3b,0x0b,0xb9,0x63,0x3e,0x06,
    0x5e,0x97,0xfb,0xef,0x10,0x7c,0x1f,0x47,0x27,0x35,0xf7,0x47,0xd6,0xb8,0x4f,0x78,
    0x71,0x9f,0x04,0xb3,0x71,0x87,0x80,0x0f,0xe6,0xde,0xac,0x71,0x9f,0x46,0x27,0x71,
    0x07,0x21,0x2b,0x17,0xcf,0xc0,0xc5,0x50,0x93,0x6b,0x18,0xb8,0x72,0xf1,0x2c,0x98,
    0x72,0x31,0xc2,0x70,0x21,0xba,0xe1,0xee,0x19,0x03,0x17,0xc3,0x4d,0x9c,0xd1,0xd4,
    0xac,0x7b,0xdd,0x73,0xdc,0xd7,0x47,0x53,0xcb,0x48,0x30,0x39,0xb3,0x46,0xe0,0x3f,
    0xc6,0xf8,0x8f,0xf5,0xc6,0xfc,0x3c,0x58,0x3b,0xea,0x98,0x60,0xea,0x10,0xdd,0x78,
    0xf7,0x4c,0x24,0xce,0x78,0x72,0x8c,0x33,0xf7,0xb3,0x09,0xe8,0x26,0x9a,0x1c,0x2f,
    0xb0,0x7f,0xa9,0x3c,0x99,0x9c,0xc3,0xa9,0x79,0x0a,0xbf,0x29,0x26,0x9b,0x71,0xbc,
    0x88,0xdf,0x14,0x73,0x9e,0x4c,0x05,0x9f,0x64,0xf2,0x4e,0xf5,0xf2,0x4e,0x30,0x79,
    0xa6,0xb1,0x0f,0x2a,0x0f,0xd3,0x3c,0x1e,0x46,0x20,0xf7,0x43,0x9e,0x6e,0x7c,0x5f,
    0x62,0x5d,0x88,0xef,0x0c,0x64,0xf9,0x9e,0xc9,0xda,0xd0,0x35,0x36,0x8b,0x73,0x6c,
    0x06,0x7b,0xaa,0xe6,0x9a,0x65,0x7a,0x6c,0xb6,0x89,0xfb,0x32,0x5c,0x28,0xdf,0xaf,
    0x80,0xcd,0x86,0xef,0x79,0x86,0x6f,0xd1,0xcd,0x75,0xcf,0x7c,0xea,0x9b,0x4b,0xcc,
    0x39,0xfc,0xa6,0x92,0xda,0xe7,0xa1,0x9b,0x6f,0x72,0x2c,0x24,0xc7,0x87,0x2e,0x87,
    0xf4,0xec,0xeb,0xdc,0x23,0x17,0xc2,0x79,0x56,0x64,0x8f,0x2a,0xc6,0x35,0xe6,0x22,
    0x2f,0xe6,0x3c,0x62,0x8a,0xfd,0x1b,0x9c,0x11,0xda,0xef,0x8b,0xc1,0xb4,0xdf,0x17,
    0x7b,0x5c,0xf6,0x33,0xf5,0x2c,0xf1,0xf6,0xdc,0xa5,0x25,0xec,0xb9,0xcb,0xc0,0x75,
    0xcf,0x7d,0x13,0xbf,0x65,0xc6,0x66,0xb9,0xd7,0xab,0x2b,0xc0,0xec,0x9e,0xbb,0x12,
    0xdf,0x15,0xf0,0xb9,0xda,0xf0,0x29,0xba,0x55,0xee,0x59,0x4b,0x8d,0xab,0xa8,0x7f,
    0x0d,0xbf,0x41,0xa5,0xfe,0xd5,0xe8,0xd6,0x9a,0xbc,0x6f,0x73,0x1e,0xab,0xbc,0x8e,
    0xf3,0x76,0x0b,0x7d,0xf9,0x0e,0x36,0xeb,0x8c,0xcd,0x7a,0xce,0xe8,0x8f,0xb1,0xd9,
    0x80,0xdd,0x7a,0x93,0x73,0x83,0x97,0x73,0x35,0xba,0x8d,0xfc,0x06,0xd6,0x58,0xef,
    0x71,0x1e,0x0f,0x61,0x2e,0xde,0x07,0xeb,0xcb,0x5d,0xf9,0x03,0x30,0xbd,0x3b,0x6f,
    0xe6,0xbc,0x1e,0xc5,0x1d,0x66,0x0b,0x36,0x9b,0x89,0xbf,0x09,0x4c,0xed,0xb7,0x72,
    0x9e,0xcb,0xf9,0xb3,0xd5,0xbb,0xff,0x54,0x82,0xbf,0x8a,0xdc,0x7f,0x32,0xf8,0xdf,
    0x9e,0xd4,0x7b,0xa9,0x89,0xb1,0x0d,0x5c,0x62,0x6c,0xf3,0xee,0x79,0xd5,0x88,0x51,
    0xd9,0xdc,0xa1,0xaa,0x98,0x78,0x55,0x89,0x77,0x99,0xb9,0x7f,0x6c,0x37,0xf7,0x0b,
    0x89,0xff,0x09,0x76,0xca,0xc1,0xa7,0x60,0xda,0x8f,0x9f,0x81,0xc9,0xf8,0xb6,0x23,
    0x2b,0x7f,0x3b,0x88,0x27,0xb5,0xed,0x30,0xb5,0xc5,0x98,0xff,0xc1,0x55,0x37,0xb5,
    0x65,0x52,0x4f,0x0d,0xf3,0x3b,0xf0,0xcb,0x12,0x7e,0x07,0x7e,0x05,0xae,0x7d,0xfd,
    0x35,0x7d,0x3d,0xd8,0xd8,0x7c,0x03,0xae,0xe7,0xeb,0x7e,0x30,0x7b,0xbf,0xfe,0x96,
    0x58,0xfb,0x8d,0xdf,0x77,0xd4,0xa1,0x67,0xf0,0x01,0xfe,0xbf,0x51,0x88,0xbd,0x8c,
    0xf3,0x00,0x76,0xfa,0x7b,0xe2,0x5f,0x28,0x36,0xbc,0x92,0x6c,0x16,0x00,0x00
};

// Generated from:
//
// #version 450 core
//
// layout(local_size_x = 64, local_size_y = 1, local_size_z = 1)in;
//
// layout(set = 0, binding = 0)buffer dest
// {
//     uint destData[];
// };
//
// layout(set = 0, binding = 1)buffer src
// {
//     uint srcData[];
// };
//
// layout(push_constant)uniform PushConstants
// {
//
//     uint outputCount;
//
//     uint componentCount;
//
//     uint srcOffset;
//     uint destOffset;
//
//     uint Ns;
//     uint Bs;
//     uint Ss;
//     uint Es;
//
//     uint Nd;
//     uint Bd;
//     uint Sd;
//     uint Ed;
//
//     bool isSrcHDR;
//     bool isSrcA2BGR10;
// } params;
//
// uint getSourceComponentOffset(uint vertex, uint component)
// {
//     return vertex * params . Ss + component * 4 + params . srcOffset;
// }
//
// uint getDestinationComponentOffset(uint vertex, uint component)
// {
//     return vertex * params . Sd + component * 4 + params . destOffset;
// }
//
// uint getShiftBits(uint offset, uint B)
// {
//
//     uint shift =(offset % 4)* 8;
//
//     return shift;
// }
//
//       float loadSourceComponent(uint cd)
// {
//
//     uint vertex = cd / params . Nd;
//     uint component = cd % params . Nd;
//
//     if(component >= params . Ns && component < 3)
//     {
//         return 0;
//     }
//
//     uint offset = getSourceComponentOffset(vertex, component);
//     uint block = srcData[offset / 4];
//
//     uint shiftBits;
//     uint valueBits;
//     uint valueMask;
//
//     if(params . isSrcHDR)
//     {
//         valueBits = component == 3 ? 2 : 10;
//         valueMask = component == 3 ? 0x03 : 0x3FF;
//         if(params . isSrcA2BGR10)
//         {
//             shiftBits = 10 * component;
//         }
//         else
//         {
//
//             shiftBits = component == 3 ? 0 :(valueBits *(2 - component)+ 2);
//         }
//     }
//     else
//     {
//         shiftBits = getShiftBits(offset, 4);
//         valueBits = 4 * 8;
//         valueMask = valueBits == 32 ? - 1 :(1 << valueBits)- 1;
//     }
//
//     uint valueAsUint;
//
//     if(component >= params . Ns && component == 3)
//     {
//
//         valueAsUint = 0x10000;
//
//     }
//     else
//     {
//         valueAsUint =(block >> shiftBits)& valueMask;
//     }
//
//     float divisor = 1.0f / 65536.0f;
//           float value = int(valueAsUint)* divisor;
//
//     return value;
// }
//
//        float convertComponent(float srcValue)
// {
//
//     return srcValue;
// }
//
// uint makeDestinationComponent(uint cd, float value)
// {
//
//     uint valueAsUint = floatBitsToInt(value);
//
//     return valueAsUint;
// }
//
// void storeDestinationComponents(uint valueAsUint)
// {
//
//     destData[gl_GlobalInvocationID . x + params . destOffset / 4]= valueAsUint;
// }
//
// void main()
// {
//     if(gl_GlobalInvocationID . x >= params . outputCount)
//         return;
//
//     uint valueOut = 0;
//     for(uint i = 0;i < 1;++ i)
//     {
//         uint cd = gl_GlobalInvocationID . x * 1 + i;
//         if(cd >= params . componentCount)
//         {
//             break;
//         }
//
//               float srcValue = loadSourceComponent(cd);
//                float destValue = convertComponent(srcValue);
//         valueOut |= makeDestinationComponent(cd, destValue);
//     }
//
//     storeDestinationComponents(valueOut);
// }
