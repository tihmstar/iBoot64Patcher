//
//  patcher.cpp
//  iBoot64Patcher
//
//  Created by tihmstar on 27.09.19.
//  Copyright © 2019 tihmstar. All rights reserved.
//

#include "patcher.hpp"
#include <stdio.h>
#include "functions.hpp"

int patch_boot_args(struct iboot_img* iboot_in, const char* boot_args) {
    printf("%s: Entering...\n", __FUNCTION__);
    
    /* Find the pre-defined boot-args from iBoot "rd=md0 ..." */
    void* default_boot_args_str_loc = memstr(iboot_in->buf, iboot_in->len, DEFAULT_BOOTARGS_STR);
    if(!default_boot_args_str_loc) {
        printf("%s: Unable to find default boot-args string!\n", __FUNCTION__);
        return 0;
    }
    printf("%s: Default boot-args string is at %p\n", __FUNCTION__, (void*) GET_IBOOT_FILE_OFFSET(iboot_in, default_boot_args_str_loc));
    
    /* Find the boot-args string xref within the kernel load routine. */
    void* default_boot_args_xref = iboot_memmem(iboot_in, default_boot_args_str_loc);
    if(!default_boot_args_xref) {
        printf("%s: Unable to find default boot-args string xref!\n", __FUNCTION__);
        return 0;
    }
    printf("%s: boot-args xref is at %p\n", __FUNCTION__, (void*) GET_IBOOT_FILE_OFFSET(iboot_in, default_boot_args_xref));
//
//    /* If new boot-args length exceeds the pre-defined one in iBoot, we need to point the xref somewhere else... */
//    if(strlen(boot_args) > strlen(DEFAULT_BOOTARGS_STR)) {
//        printf("%s: Relocating boot-args string...\n", __FUNCTION__);
//
//        /* Find the "Reliance on this cert..." string. */
//        char* reliance_cert_str_loc = (char*) memstr(iboot_in->buf, iboot_in->len, RELIANCE_CERT_STR);
//        if(!reliance_cert_str_loc) {
//            printf("%s: Unable to find \"%s\" string!\n", __FUNCTION__, RELIANCE_CERT_STR);
//            return 0;
//        }
//        printf("%s: \"%s\" string found at %p\n", __FUNCTION__, RELIANCE_CERT_STR, GET_IBOOT_FILE_OFFSET(iboot_in, reliance_cert_str_loc));
//
//        /* Point the boot-args xref to the "Reliance on this cert..." string. */
//        printf("%s: Pointing default boot-args xref to %p...\n", __FUNCTION__, GET_IBOOT_ADDR(iboot_in, reliance_cert_str_loc));
//        *(uint32_t*)default_boot_args_xref = (uintptr_t) GET_IBOOT_ADDR(iboot_in, reliance_cert_str_loc);
//
//        default_boot_args_str_loc = reliance_cert_str_loc;
//    }
//    printf("%s: Applying custom boot-args \"%s\"\n", __FUNCTION__, boot_args);
//    strcpy(default_boot_args_str_loc, boot_args);
//
//    /* This is where things get tricky... (Might run into issues on older loaders)*/
//
//    /* Patch out the conditional branches... */
//    void* _ldr_rd_boot_args = ldr_to(default_boot_args_xref);
//    if(!_ldr_rd_boot_args) {
//        uintptr_t default_boot_args_str_loc_with_base = (uintptr_t) GET_IBOOT_FILE_OFFSET(iboot_in, default_boot_args_str_loc) + get_iboot_base_address(iboot_in->buf);
//
//        _ldr_rd_boot_args = find_next_LDR_insn_with_value(iboot_in, (uint32_t) default_boot_args_str_loc_with_base);
//        if(!_ldr_rd_boot_args) {
//            printf("%s: Error locating LDR Rx, =boot_args!\n", __FUNCTION__);
//            return 0;
//        }
//    }
//
//    struct arm32_thumb_LDR* ldr_rd_boot_args = (struct arm32_thumb_LDR*) _ldr_rd_boot_args;
//    printf("%s: Found LDR R%d, =boot_args at %p\n", __FUNCTION__, ldr_rd_boot_args->rd, GET_IBOOT_FILE_OFFSET(iboot_in, _ldr_rd_boot_args));
//
//    /* Find next CMP Rd, #0 instruction... */
//    void* _cmp_insn = find_next_CMP_insn_with_value(ldr_rd_boot_args, 0x100, 0);
//    if(!_cmp_insn) {
//        printf("%s: Error locating next CMP instruction!\n", __FUNCTION__);
//        return 0;
//    }
//
//    struct arm32_thumb* cmp_insn = (struct arm32_thumb*) _cmp_insn;
//    void* arm32_thumb_IT_insn = _cmp_insn;
//
//    printf("%s: Found CMP R%d, #%d at %p\n", __FUNCTION__, cmp_insn->rd, cmp_insn->offset, GET_IBOOT_FILE_OFFSET(iboot_in, _cmp_insn));
//
//    /* Find the next IT EQ/IT NE instruction following the CMP Rd, #0 instruction... (kinda hacky) */
//    while(*(uint16_t*)arm32_thumb_IT_insn != ARM32_THUMB_IT_EQ && *(uint16_t*)arm32_thumb_IT_insn != ARM32_THUMB_IT_NE) {
//        arm32_thumb_IT_insn++;
//    }
//
//    printf("%s: Found IT EQ/IT NE at %p\n", __FUNCTION__, GET_IBOOT_FILE_OFFSET(iboot_in, arm32_thumb_IT_insn));
//
//    /* MOV Rd, Rs instruction usually follows right after the IT instruction. */
//    struct arm32_thumb_hi_reg_op* mov_insn = (struct arm32_thumb_hi_reg_op*) (arm32_thumb_IT_insn + 2);
//
//    printf("%s: Found MOV R%d, R%d at %p\n", __FUNCTION__, mov_insn->rd, mov_insn->rs, GET_IBOOT_FILE_OFFSET(iboot_in, arm32_thumb_IT_insn + 2));
//
//    /* Find the last LDR Rd which holds the null string pointer... */
//    int null_str_reg = (ldr_rd_boot_args->rd == mov_insn->rs) ? mov_insn->rd : mov_insn->rs;
//
//    /* + 0x10: Some iBoots have the null string load after the CMP instruction... */
//    void* ldr_null_str = find_last_LDR_rd((uintptr_t) (_cmp_insn + 0x10), 0x200, null_str_reg);
//    if(!ldr_null_str) {
//        printf("%s: Unable to find LDR R%d, =null_str\n", __FUNCTION__, null_str_reg);
//        return 0;
//    }
//
//    printf("%s: Found LDR R%d, =null_str at %p\n", __FUNCTION__, null_str_reg, GET_IBOOT_FILE_OFFSET(iboot_in, ldr_null_str));
//
//    /* Calculate the new PC relative load from the default boot args xref to the LDR Rd, =null_string location. */
//    uint32_t diff = (uint32_t) (GET_IBOOT_FILE_OFFSET(iboot_in, default_boot_args_xref) - GET_IBOOT_FILE_OFFSET(iboot_in, ldr_null_str));
//
//    /* T1 LDR PC-based instructions use the immediate 8 bits multiplied by 4. */
//    struct arm32_thumb_LDR* ldr_rd_null_str = (struct arm32_thumb_LDR*) ldr_null_str;
//    printf("%s: Pointing LDR R%d, =null_str to boot-args xref...\n", __FUNCTION__, ldr_rd_null_str->rd);
//    ldr_rd_null_str->imm8 = (diff / 0x4);
//
//    printf("%s: Leaving...\n", __FUNCTION__);
    return 1;
}
