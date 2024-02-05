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

#define CSEc_FLASH_PARTION_ALREADY_DONE 0xAA
#define CSEC_FLASH_PARTION_NOT_DONE 0x55
#define ERASE_ALL_KEYS	1

status_t initFlashForCsecOperation(void)
{
	flash_ssd_config_t flashSSDConfig;
	status_t flash_partition, flash_init;

	flash_init = FLASH_DRV_Init(&flash1_InitConfig0, &flashSSDConfig);

	/* If EEESize variable is equal to 0, it means that  Flash memory partitioning for
	 * emulated EEEPROM is not done. And Flash memory partitioning is mandatory step to do
	 * CSEc Operation. */
	if (flashSSDConfig.EEESize == 0)
	{
#ifdef FLASH_TARGET
		/* Flash partitioning for CSEc operation must only be ran with RAM configuration.
		 * The first time when running the example on the board, or after a key erase,
		 * this example should be ran from RAM, in order to enable CSEc operation. Please
		 * refer to the documentation for more information. */
		PINS_DRV_ClearPins(LED_PORT, 1 << LED_OK);

		//If flash configuration is selected then memory partitioning cant be done
		flash_partition = CSEC_FLASH_PARTION_NOT_DONE;
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

		flash_partition = FLASH_DRV_DEFlashPartition(&flashSSDConfig, 0x2, 0x4, 0x3, false, true);

		#endif /* FLASH_TARGET */
	} else if (flashSSDConfig.EEESize != 0)
	{
		// Means flash partitioning is already done
		flash_partition = CSEc_FLASH_PARTION_ALREADY_DONE;
	}

	return flash_partition;

}

uint8_t key_counter = 0;

int main(void)
{
  /* Write your code here */
  /* Initialize and configure clocks
   * 	-	see clock manager component for details
   */
  CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT,
						g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
  CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_FORCIBLE);

  status_t flash_init_for_csec;
  /* Initialize pins */
  PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);

  /* Turn off the leds */
  PINS_DRV_SetPins(LED_PORT, (1 << LED_ERROR) | (1 << LED_OK));


    /* Initialize CSEc driver */
    CSEC_DRV_Init(&csecState);

    /* Initialize Flash for CSEc operation */
    flash_init_for_csec = initFlashForCsecOperation();

    /* Load the MASTER_ECU key with a known value, which will be used as Authorization
     * key (a secret key known by the application in order to configure other user keys) */
    setAuthKey();

      if (eraseKeys())
       {
           PINS_DRV_ClearPins(LED_PORT, 1 << LED_OK);
           PINS_DRV_ClearPins(LED_PORT, 1 << LED_ERROR);
       }

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

