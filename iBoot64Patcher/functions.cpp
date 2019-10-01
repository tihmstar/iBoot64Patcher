//
//  functions.cpp
//  iBoot64Patcher
//
//  Created by tihmstar on 27.09.19.
//  Copyright © 2019 tihmstar. All rights reserved.
//

#include "functions.hpp"
#include <string.h>
#include "iBoot64Patcher.h"

void* memstr(const void* mem, size_t size, const char* str) {
    return (void*) memmem(mem, size, str, strlen(str));
}

void* iboot_memmem(struct iboot_img* iboot_in, void* pat) {
    uint32_t new_pat = (uintptr_t) GET_IBOOT_ADDR(iboot_in, pat);
    
    return (void*) memmem(iboot_in->buf, iboot_in->len, &new_pat, sizeof(uint32_t));
}

uint64_t get_iboot_base_address(void* iboot_buf) {
    if(iboot_buf) {
        return *(uint64_t*)((uint8_t*)iboot_buf + 0x318);
    }
    return 0;
}

bool has_kernel_load(struct iboot_img* iboot_in){
    void* debug_enabled_str = memstr(iboot_in->buf, iboot_in->len, KERNELCACHE_PREP_STRING);
    return (bool) (debug_enabled_str != NULL);
}
