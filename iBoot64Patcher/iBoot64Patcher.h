//
//  iBoot64Patcher.h
//  iBoot64Patcher
//
//  Created by tihmstar on 27.09.19.
//  Copyright © 2019 tihmstar. All rights reserved.
//

#ifndef iBoot64Patcher_h
#define iBoot64Patcher_h

#include <stdint.h>
#include <unistd.h>

#define IBOOT_VERS_STR_OFFSET 0x286

#define bswap32 __builtin_bswap32
#define bswap16 __builtin_bswap16

#define GET_IBOOT_FILE_OFFSET(iboot_in, x) ((uint8_t*)x - (uint8_t*) iboot_in->buf)
#define GET_IBOOT_ADDR(iboot_in, x) ((uint8_t*)x - (uint8_t*) iboot_in->buf) + get_iboot_base_address(iboot_in->buf)


struct iboot_img {
    char* buf;
    size_t len;
    uint32_t VERS;
} __attribute__((packed));

#endif /* iBoot64Patcher_h */
