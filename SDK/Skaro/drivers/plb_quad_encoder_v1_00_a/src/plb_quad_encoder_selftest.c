/*****************************************************************************
* Filename:          /data/projects/EDK11/drivers/plb_quad_encoder_v1_00_a/src/plb_quad_encoder_selftest.c
* Version:           1.00.a
* Description:       Contains a diagnostic self-test function for the plb_quad_encoder driver
* Date:              Thu Jan 14 11:53:28 2010 (by Create and Import Peripheral Wizard)
*****************************************************************************/


/***************************** Include Files *******************************/

#include "plb_quad_encoder.h"
#include "Timer.h"
/************************** Constant Definitions ***************************/


/************************** Variable Definitions ****************************/


/************************** Function Definitions ***************************/

/**
 *
 * Run a self-test on the driver/device. Note this may be a destructive test if
 * resets of the device are performed.
 *
 * If the hardware system is not built correctly, this function may never
 * return to the caller.
 *
 * @param   baseaddr_p is the base address of the PLB_QUAD_ENCODER instance to be worked on.
 *
 * @return
 *
 *    - XST_SUCCESS   if all self-test code passed
 *    - XST_FAILURE   if any self-test code failed
 *
 * @note    Caching must be turned off for this function to work.
 * @note    Self test may fail if data memory and device are not on the same bus.
 *
 */
XStatus EncoderTest()
{
  Xuint32 baseaddr;
  Xuint32 Reg32Value;
  
  /*
   * Check and get the device address
   */
  //XASSERT_NONVOID(baseaddr_p != XNULL);
  baseaddr = (Xuint32)XPAR_PLB_QUAD_ENCODER_0_BASEADDR;

  xil_printf("******************************\n\r");
  xil_printf("* User Peripheral Self Test\n\r");
  xil_printf("******************************\n\n\r");

  /*
   * Reset the device to get it back to the default state
   */
  xil_printf("Soft reset test...\n\r");
  PLB_QUAD_ENCODER_mReset(baseaddr);
  xil_printf("   - write 0x%08x to software reset register\n\r", SOFT_RESET);
  xil_printf("   - soft reset passed\n\n\r");

	xil_printf("Reading encoder, run encoder to 200 ticks to test clear\r\n");
  while(1)
  {
	//Reg32Value = PLB_QUAD_ENCODER_mReadSlaveReg0(baseaddr, 0);
	Reg32Value = getTicks();
	xil_printf("   - read %d from register 0 word 0\r", Reg32Value);
	ClockDelay(100000000);
	if((int)Reg32Value >= 200)
	{
		xil_printf("\r\n200 Ticks, clearing encoder!");
		break;
	}
  }
  clearEncoder();
  Reg32Value = getTicks();
  xil_printf("\r\nEncoder cleared, value in register is now: %d\r\n", Reg32Value);

  return XST_SUCCESS;
}
