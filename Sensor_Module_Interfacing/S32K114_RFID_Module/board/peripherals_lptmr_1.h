/***********************************************************************************************************************
 * This file was generated by the S32 Config Tools. Any manual edits made to this file
 * will be overwritten if the respective S32 Config Tools is used to update this file.
 **********************************************************************************************************************/

#ifndef lptmr_1_H
#define lptmr_1_H

/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.
 * The global macro will be used in function call of the module.
 *
 */
/*******************************************************************************
 * Included files 
 ******************************************************************************/
#include "lptmr_driver.h"

/*******************************************************************************
 * Definitions 
 ******************************************************************************/
#define INST_LPTMR_1  (0U)

/*******************************************************************************
 * Global variables 
 ******************************************************************************/

/* LPTMR Configuration 0 */
extern const lptmr_config_t lptmr_1_config0;

extern void LPTMR0_IRQHandler(void);

extern uint32_t GB_millis(void);

#endif /* lptmr_1_H */
