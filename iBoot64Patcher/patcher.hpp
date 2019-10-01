//
//  patcher.hpp
//  iBoot64Patcher
//
//  Created by tihmstar on 27.09.19.
//  Copyright © 2019 tihmstar. All rights reserved.
//

#ifndef patcher_hpp
#define patcher_hpp

#include "iBoot64Patcher.h"


#define DEBUG_ENABLED_DTRE_VAR_STR "debug-enabled"
#define DEFAULT_BOOTARGS_STR "rd=md0 nand-enable-reformat=1 -progress"
#define RELIANCE_CERT_STR "Reliance on this certificate"


int patch_boot_args(struct iboot_img* iboot_in, const char* boot_args);


#endif /* patcher_hpp */
