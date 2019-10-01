//
//  functions.hpp
//  iBoot64Patcher
//
//  Created by tihmstar on 27.09.19.
//  Copyright © 2019 tihmstar. All rights reserved.
//

#ifndef functions_hpp
#define functions_hpp

#include "iBoot64Patcher.h"

#define KERNELCACHE_PREP_STRING "__PAGEZERO"

void* memstr(const void* mem, size_t size, const char* str);
void* iboot_memmem(struct iboot_img* iboot_in, void* pat);
uint64_t get_iboot_base_address(void* iboot_buf);

bool has_kernel_load(struct iboot_img* iboot_in);


#endif /* functions_hpp */
