/*****************************************************************************
* Filename:          /data/projects/EDK11/drivers/plb_quad_encoder_v1_00_a/src/plb_quad_encoder.c
* Version:           1.00.a
* Description:       plb_quad_encoder Driver Source File
* Date:              Thu Jan 14 11:53:28 2010 (by Create and Import Peripheral Wizard)
*****************************************************************************/


/***************************** Include Files *******************************/

#include "plb_quad_encoder.h"

Xuint32 baseaddr = (Xuint32)XPAR_PLB_QUAD_ENCODER_0_BASEADDR;

Xuint32 getTicks()
{
	return PLB_QUAD_ENCODER_mReadSlaveReg0(baseaddr, 0);
}

void clearEncoder()
{
	PLB_QUAD_ENCODER_mReset(baseaddr);
}
/************************** Function Definitions ***************************/

