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
/* Set this macro-definition to 1 if you want to reset all the keys */
#define ERASE_ALL_KEYS	0

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


uint32_t starttime __attribute__((section (".customSection")));

const uint8_t Iv[] = {
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};

const uint8_t chall[] = {
		0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };


uint8_t uid[15];
uint8_t mac[16];
uint8_t status_reg[15];

bool verifystatus;

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

  bool keyLoaded;
  uint8_t key[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
              0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};

  /* Initialize CSEc driver */
  CSEC_DRV_Init(&csecState);

  /* Initialize Flash for CSEc operation */
  flash_init_for_csec = initFlashForCsecOperation();

  /* Load the MASTER_ECU key with a known value, which will be used as Authorization
   * key (a secret key known by the application in order to configure other user keys) */
  setAuthKey();


  CSEC_DRV_GetID(chall,uid,  status_reg, mac);

  /* Load the selected key */
  /* First load => counter == 1 */
  keyLoaded = loadKey(CSEC_KEY_1, key, 15);
  if (keyLoaded)
  {
      uint8_t i;

      status_t stat;
      bool encryptionOk = true;
      uint8_t cipherText[16];
      uint8_t *Mac_Generated;
      uint8_t cipherText_encrypted[16];
      uint8_t plainText[16] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
      0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};

      uint8_t message[32];

      uint8_t expectedCipherText[16] = {0x69, 0xc4, 0xe0, 0xd8, 0x6a, 0x7b, 0x04,
      0x30, 0xd8, 0xcd, 0xb7, 0x80, 0x70, 0xb4, 0xc5, 0x5a};

      starttime = OSIF_GetMilliseconds();
//      stat = CSEC_DRV_EncryptCBC(CSEC_KEY_1, plainText, 16U, Iv, cipherText, 1U);
//
//      stat = CSEC_DRV_DecryptCBC(CSEC_KEY_1, cipherText, 16U, Iv, cipherText_encrypted, 1U);
//
//      if (stat == STATUS_SUCCESS)
//      {
//          /* Check if the decrypted cipher text is same as plain text */
//          for (i = 0; i < 16; i++)
//          {
//              if (cipherText_encrypted[i] != plainText[i])
//              {
//                  encryptionOk = false;
//                  break;
//              }
//          }
//      }



//      stat = CSEC_DRV_GenerateMAC(CSEC_RAM_KEY, plainText, 16U, cipherText, 1U);
//
//
//    stat = CSEC_DRV_VerifyMAC(CSEC_RAM_KEY, plainText, 16U, cipherText, 16U, &verifystatus, 1U );


      stat = CSEC_DRV_EncryptCBC(CSEC_KEY_1, plainText, 16U, Iv, cipherText, 1U);

      stat = CSEC_DRV_GenerateMAC(CSEC_RAM_KEY, cipherText, 16U, Mac_Generated, 1U);

for ( int i = 0; i<32 ; i++)
{
	if (i<16)
	message [i] = plainText[i];
	else if( i > 15)
	message [i] = Mac_Generated[i];
}

// SEND MESSAGE BUFFER VIA CAN AT MESSAGE ID: 0x800
    stat = CSEC_DRV_VerifyMAC(CSEC_RAM_KEY, cipherText, 16U, Mac_Generated, 16U, &verifystatus, 1U );

      if (encryptionOk)
      {
          PINS_DRV_ClearPins(LED_PORT, 1 << LED_OK);
      }
  }
  else
  {
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

