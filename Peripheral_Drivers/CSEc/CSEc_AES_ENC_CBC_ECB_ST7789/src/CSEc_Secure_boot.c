/*
 * Copyright 2022 Gettobyte
 * All rights reserved.
 *
 */

/* Including needed modules to compile this module/procedure */
#include "sdk_project_config.h"
#include "flash_driver.h"
#if CPU_INIT_CONFIG
  #include "Init_Config.h"
#endif

volatile int exit_code = 0;

#include <stdint.h>
#include <stdbool.h>
#include "csec_utils.h"

/* Flash configuration */
static const flash_user_config_t flash1_InitConfig0 = {
    .PFlashBase  = 0x00000000U,                     /* Base address of Program Flash block */
    .PFlashSize  = 0x00080000U,                     /* Size of Program Flash block         */
    .DFlashBase  = 0x10000000U,                     /* Base address of Data Flash block    */
    .EERAMBase   = 0x14000000U,                     /* Base address of FlexRAM block */
    /* If using callback, any code reachable from this function must not be placed in a Flash block targeted for a program/erase operation.*/
    .CallBack    = NULL_CALLBACK
};

#define LED_PORT       PTD
#define LED_OK         16U
#define LED_ERROR      15U

#define CSEc_FLASH_PARTION_ALREADY_DONE 0xAA   // CSEc memory partionting is already done, previous time. To do it again with new value, run CSEC_Crypto_Erase_Keys.c example
#define CSEC_FLASH_PARTION_CANT_BE_DONE 0x55   // CSEc memory partionting to EEEPROM cant be done because RAM configuration is selected or CSEc flash configuration is not done yet, run CSEC_Vrypto_Engine_Init.c example

status_t initFlashForCsecOperation(void)
{
	flash_ssd_config_t flashSSDConfig;
	status_t flash_partition, flash_init;

	FLASH_DRV_Init(&flash1_InitConfig0, &flashSSDConfig);

	if (flashSSDConfig.EEESize == 0)
	{
#ifdef FLASH_TARGET
		/* Flash partitioning for CSEc operation must only be ran with RAM configuration.
		 * The first time when running the example on the board, or after a key erase,
		 * this example should be ran from RAM, in order to enable CSEc operation. Please
		 * refer to the documentation for more information. */
		PINS_DRV_ClearPins(LED_PORT, 1 << LED_OK);
		//If flash configuration is selected then memory partitioning cant be done
		flash_partition = CSEC_FLASH_PARTION_CANT_BE_DONE;

#else
		uint32_t address;
		uint32_t size;
#if (FEATURE_FLS_HAS_PROGRAM_PHRASE_CMD == 1u)
		uint8_t unsecure_key[FTFx_PHRASE_SIZE] = {0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFEu, 0xFFu, 0xFFu, 0xFFu};
#else   /* FEATURE_FLASH_HAS_PROGRAM_LONGWORD_CMD */
		uint8_t unsecure_key[FTFx_LONGWORD_SIZE] = {0xFEu, 0xFFu, 0xFFu, 0xFFu};
#endif  /* FEATURE_FLS_HAS_PROGRAM_PHRASE_CMD */

		/* First, erase all Flash blocks to ensure the IFR region is blank
		 * before partitioning FlexNVM and FlexRAM */
		FLASH_DRV_EraseAllBlock(&flashSSDConfig);
		/* Reprogram secure byte in Flash configuration field */
#if (FEATURE_FLS_HAS_PROGRAM_PHRASE_CMD == 1u)
		address = 0x408u;
		size = FTFx_PHRASE_SIZE;
#else   /* FEATURE_FLASH_HAS_PROGRAM_LONGWORD_CMD == 1u */
		address = 0x40Cu;
		size = FTFx_LONGWORD_SIZE;
#endif /* FEATURE_FLS_HAS_PROGRAM_PHRASE_CMD */
		FLASH_DRV_Program(&flashSSDConfig, address, size, unsecure_key);

        FLASH_DRV_DEFlashPartition(&flashSSDConfig, 0x2, 0x4, 0x3, false, true);
#endif /* FLASH_TARGET */
	} else if (flashSSDConfig.EEESize != 0)
	{
		// Means flash partitioning is already done
		flash_partition = CSEc_FLASH_PARTION_ALREADY_DONE;
	}

	return flash_partition;

}

status_t flash_init_for_csec;


bool keyLoaded;
uint8_t boot_key[16] = {0x10, 0x31, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
            0x09, 0x0a, 0x0b, 0x0c, 0x9d, 0x0e, 0x0f};

uint32_t u32BootSize = 128*1024*8;

status_t statusVal;

int main(void)
{
  /* Initialize CSEc driver */
  CSEC_DRV_Init(&csecState);

// keyLoaded = setAuthKey();

 statusVal = CSEC_DRV_BootFailure();

// statusVal =  CSEC_DRV_BootOK();

// load the BOOT_MAC_KEY

  keyLoaded = loadKey(CSEC_BOOT_MAC_KEY, boot_key, 1);
  statusVal = CSEC_DRV_BootDefine(u32BootSize, CSEC_BOOT_SERIAL);





  for(;;) {
    if(exit_code != 0) {
      break;
    }
  }
  return exit_code;

} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
** @}
*/

