/* ----------------------------------------------------------------------    
* Copyright (C) 2010 ARM Limited. All rights reserved.    
*    
* $Date: 13/10/11 2:55p $Revision: 	V1.1.0  
*    
* Project: 	    CMSIS DSP Library    
* Title:        arm_fir_decimate_init_q15.c    
*    
* Description:  Initialization function for the Q15 FIR Decimator.    
*    
* Target Processor: Cortex-M4/Cortex-M3/Cortex-M0
*  
* Version 1.1.0 2012/02/15 
*    Updated with more optimizations, bug fixes and minor API changes.  
*   
* Version 1.0.10 2011/7/15  
*    Big Endian support added and Merged M0 and M3/M4 Source code.   
*    
* Version 1.0.3 2010/11/29   
*    Re-organized the CMSIS folders and updated documentation.    
*     
* Version 1.0.2 2010/11/11    
*    Documentation updated.     
*    
* Version 1.0.1 2010/10/05     
*    Production release and review comments incorporated.    
*    
* Version 1.0.0 2010/09/20     
*    Production release and review comments incorporated    
*    
* Version 0.0.7  2010/06/10     
*    Misra-C changes done    
* ------------------------------------------------------------------- */

#include "arm_math.h"

/**    
 * @ingroup groupFilters    
 */

/**    
 * @addtogroup FIR_decimate    
 * @{    
 */

/**    
 * @brief  Initialization function for the Q15 FIR decimator.    
 * @param[in,out] *S points to an instance of the Q15 FIR decimator structure.    
 * @param[in] numTaps  number of coefficients in the filter.    
 * @param[in] M  decimation factor.    
 * @param[in] *pCoeffs points to the filter coefficients.    
 * @param[in] *pState points to the state buffer.    
 * @param[in] blockSize number of input samples to process per call.    
 * @return    The function returns ARM_MATH_SUCCESS if initialization was successful or ARM_MATH_LENGTH_ERROR if    
 * <code>blockSize</code> is not a multiple of <code>M</code>.    
 *    
 * <b>Description:</b>    
 * \par    
 * <code>pCoeffs</code> points to the array of filter coefficients stored in time reversed order:    
 * <pre>    
 *    {b[numTaps-1], b[numTaps-2], b[N-2], ..., b[1], b[0]}    
 * </pre>    
 * \par    
 * <code>pState</code> points to the array of state variables.    
 * <code>pState</code> is of length <code>numTaps+blockSize-1</code> words where <code>blockSize</code> is the number of input samples    
 * to the call <code>arm_fir_decimate_q15()</code>.    
 * <code>M</code> is the decimation factor.    
 */

arm_status arm_fir_decimate_init_q15(
  arm_fir_decimate_instance_q15 * S,
  uint16_t numTaps,
  uint8_t M,
  q15_t * pCoeffs,
  q15_t * pState,
  uint32_t blockSize)
{

  arm_status status;

  /* The size of the input block must be a multiple of the decimation factor */
  if((blockSize % M) != 0u)
  {
    /* Set status as ARM_MATH_LENGTH_ERROR */
    status = ARM_MATH_LENGTH_ERROR;
  }
  else
  {
    /* Assign filter taps */
    S->numTaps = numTaps;

    /* Assign coefficient pointer */
    S->pCoeffs = pCoeffs;

    /* Clear the state buffer.  The size of buffer is always (blockSize + numTaps - 1) */
    memset(pState, 0, (numTaps + (blockSize - 1u)) * sizeof(q15_t));

    /* Assign state pointer */
    S->pState = pState;

    /* Assign Decimation factor */
    S->M = M;

    status = ARM_MATH_SUCCESS;
  }

  return (status);

}

/**    
 * @} end of FIR_decimate group    
 */
