/*****************************************************************************
* Filename:          /data/projects/EDK11/drivers/plb_quad_encoder_v1_00_a/src/plb_quad_encoder.h
* Version:           1.00.a
* Description:       plb_quad_encoder Driver Header File
* Date:              Thu Jan 14 11:53:28 2010 (by Create and Import Peripheral Wizard)
*****************************************************************************/

#ifndef PLB_QUAD_ENCODER_H
#define PLB_QUAD_ENCODER_H

/***************************** Include Files *******************************/

#include "xbasic_types.h"
#include "xstatus.h"
#include "xio.h"
#include "xparameters.h"
/************************** Constant Definitions ***************************/


/**
 * User Logic Slave Space Offsets
 * -- SLV_REG0 : user logic slave module register 0
 */
#define PLB_QUAD_ENCODER_USER_SLV_SPACE_OFFSET (0x00000000)
#define PLB_QUAD_ENCODER_SLV_REG0_OFFSET (PLB_QUAD_ENCODER_USER_SLV_SPACE_OFFSET + 0x00000000)

/**
 * Software Reset Space Register Offsets
 * -- RST : software reset register
 */
#define PLB_QUAD_ENCODER_SOFT_RST_SPACE_OFFSET (0x00000100)
#define PLB_QUAD_ENCODER_RST_REG_OFFSET (PLB_QUAD_ENCODER_SOFT_RST_SPACE_OFFSET + 0x00000000)

/**
 * Software Reset Masks
 * -- SOFT_RESET : software reset
 */
#define SOFT_RESET (0x0000000A)

/**************************** Type Definitions *****************************/


/***************** Macros (Inline Functions) Definitions *******************/

/**
 *
 * Write a value to a PLB_QUAD_ENCODER register. A 32 bit write is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is written.
 *
 * @param   BaseAddress is the base address of the PLB_QUAD_ENCODER device.
 * @param   RegOffset is the register offset from the base to write to.
 * @param   Data is the data written to the register.
 *
 * @return  None.
 *
 * @note
 * C-style signature:
 * 	void PLB_QUAD_ENCODER_mWriteReg(Xuint32 BaseAddress, unsigned RegOffset, Xuint32 Data)
 *
 */
#define PLB_QUAD_ENCODER_mWriteReg(BaseAddress, RegOffset, Data) \
 	XIo_Out32((BaseAddress) + (RegOffset), (Xuint32)(Data))

/**
 *
 * Read a value from a PLB_QUAD_ENCODER register. A 32 bit read is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is read from the register. The most significant data
 * will be read as 0.
 *
 * @param   BaseAddress is the base address of the PLB_QUAD_ENCODER device.
 * @param   RegOffset is the register offset from the base to write to.
 *
 * @return  Data is the data from the register.
 *
 * @note
 * C-style signature:
 * 	Xuint32 PLB_QUAD_ENCODER_mReadReg(Xuint32 BaseAddress, unsigned RegOffset)
 *
 */
#define PLB_QUAD_ENCODER_mReadReg(BaseAddress, RegOffset) \
 	XIo_In32((BaseAddress) + (RegOffset))


/**
 *
 * Write/Read 32 bit value to/from PLB_QUAD_ENCODER user logic slave registers.
 *
 * @param   BaseAddress is the base address of the PLB_QUAD_ENCODER device.
 * @param   RegOffset is the offset from the slave register to write to or read from.
 * @param   Value is the data written to the register.
 *
 * @return  Data is the data from the user logic slave register.
 *
 * @note
 * C-style signature:
 * 	void PLB_QUAD_ENCODER_mWriteSlaveRegn(Xuint32 BaseAddress, unsigned RegOffset, Xuint32 Value)
 * 	Xuint32 PLB_QUAD_ENCODER_mReadSlaveRegn(Xuint32 BaseAddress, unsigned RegOffset)
 *
 */
#define PLB_QUAD_ENCODER_mWriteSlaveReg0(BaseAddress, RegOffset, Value) \
 	XIo_Out32((BaseAddress) + (PLB_QUAD_ENCODER_SLV_REG0_OFFSET) + (RegOffset), (Xuint32)(Value))

#define PLB_QUAD_ENCODER_mReadSlaveReg0(BaseAddress, RegOffset) \
 	XIo_In32((BaseAddress) + (PLB_QUAD_ENCODER_SLV_REG0_OFFSET) + (RegOffset))

/**
 *
 * Reset PLB_QUAD_ENCODER via software.
 *
 * @param   BaseAddress is the base address of the PLB_QUAD_ENCODER device.
 *
 * @return  None.
 *
 * @note
 * C-style signature:
 * 	void PLB_QUAD_ENCODER_mReset(Xuint32 BaseAddress)
 *
 */
#define PLB_QUAD_ENCODER_mReset(BaseAddress) \
 	XIo_Out32((BaseAddress)+(PLB_QUAD_ENCODER_RST_REG_OFFSET), SOFT_RESET)

/************************** Function Prototypes ****************************/

// MACRO DEFINITIONS ////////////////////////////////////////

/*#define ENCODER_COUNT(BASE_ADDR) \
        (*((Xuint32 volatile *)((Xuint32)(BASE_ADDR))))*/
#define ENCODER_COUNT(BaseAddress) \
        XIo_In32((BaseAddress) + (PLB_QUAD_ENCODER_SLV_REG0_OFFSET))

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
XStatus PLB_QUAD_ENCODER_SelfTest(void * baseaddr_p);

Xuint32 getTicks();
void clearEncoder();

#endif /** PLB_QUAD_ENCODER_H */
