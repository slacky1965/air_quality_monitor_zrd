/********************************************************************************************************
 * @file    version_cfg.h
 *
 * @brief   This is the header file for version_cfg
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *			All rights reserved.
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/
#pragma once

#include "../common/comm_cfg.h"

#if defined(MCU_CORE_826x)
    #if (CHIP_8269)
        #define CHIP_TYPE               TLSR_8269
    #else
        #define CHIP_TYPE               TLSR_8267
    #endif
#elif defined(MCU_CORE_8258)
        #define CHIP_TYPE               TLSR_8258_512K//TLSR_8258_1M
    #if (CHIP_TYPE == TLSR_8258_1M)
        #define FLASH_CAP_SIZE_1M       1
    #endif
#elif defined(MCU_CORE_8278)
        #define CHIP_TYPE               TLSR_8278
        #define FLASH_CAP_SIZE_1M       1
#elif defined(MCU_CORE_B91)
        #define CHIP_TYPE               TLSR_B91
        #define FLASH_CAP_SIZE_1M       1
#elif defined(MCU_CORE_B92)
        #define CHIP_TYPE               TLSR_B92
        #define FLASH_CAP_SIZE_1M       1
#elif defined(MCU_CORE_TL721X)
        #define CHIP_TYPE               TLSR_TL721X
        #define FLASH_CAP_SIZE_1M       1
#elif defined(MCU_CORE_TL321X)
        #define CHIP_TYPE               TLSR_TL321X
        #define FLASH_CAP_SIZE_1M       1
#endif

#define APP_RELEASE                     0x10//app release 1.0
#define APP_BUILD                       0x01//app build 01
#define STACK_RELEASE                   0x30//stack release 3.0
#define STACK_BUILD                     0x01//stack build 01

/*********************************************************************************************
 * During OTA upgrade, the upgraded device will check the rules of the following three fields.
 * Refer to ZCL OTA specification for details.
 */
#define MANUFACTURER_CODE_TELINK        0x1141//Telink ID
#define	IMAGE_TYPE                      ((CHIP_TYPE << 8) | IMAGE_TYPE_BOOTLOADER)
#define	FILE_VERSION                    ((APP_RELEASE << 24) | (APP_BUILD << 16) | (STACK_RELEASE << 8) | STACK_BUILD)

/* Pre-compiled link configuration. */
#define IS_BOOT_LOADER_IMAGE            1
#define RESV_FOR_APP_RAM_CODE_SIZE      0x2000
#define IMAGE_OFFSET                    BOOT_LOADER_IMAGE_ADDR

#if !defined(BOOT_LOADER_MODE) || (BOOT_LOADER_MODE == 0)
	#error "Please define BOOT_LOADER_MODE = 1"
#endif
