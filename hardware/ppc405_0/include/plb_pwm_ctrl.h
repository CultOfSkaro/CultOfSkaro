/*****************************************************************************
* Filename:          C:\Documents and Settings\Administrator\Desktop\My Dropbox\Documents\2010\winter\Senior Project\svn\EDK11\Drew/drivers/plb_pwm_ctrl_v1_00_a/src/plb_pwm_ctrl.h
* Version:           1.00.a
* Description:       plb_pwm_ctrl Driver Header File
* Date:              Fri Jan 22 12:28:25 2010 (by Create and Import Peripheral Wizard)
*****************************************************************************/

#ifndef PLB_PWM_CTRL_H
#define PLB_PWM_CTRL_H

/***************************** Include Files *******************************/

#include "xbasic_types.h"
#include "xstatus.h"
#include "xio.h"

/************************** Constant Definitions ***************************/


/**
 * User Logic Slave Space Offsets
 * -- SLV_REG0 : user logic slave module register 0
 * -- SLV_REG1 : user logic slave module register 1
 */
#define PLB_PWM_CTRL_USER_SLV_SPACE_OFFSET (0x00000000)
#define PLB_PWM_CTRL_SLV_REG0_OFFSET (PLB_PWM_CTRL_USER_SLV_SPACE_OFFSET + 0x00000000)
#define PLB_PWM_CTRL_SLV_REG1_OFFSET (PLB_PWM_CTRL_USER_SLV_SPACE_OFFSET + 0x00000004)
#define PLB_PWM_CTRL_SLV_REG2_OFFSET (PLB_PWM_CTRL_USER_SLV_SPACE_OFFSET + 0x00000008)
#define PLB_PWM_CTRL_SLV_REG3_OFFSET (PLB_PWM_CTRL_USER_SLV_SPACE_OFFSET + 0x0000000C)

/**************************** Type Definitions *****************************/


/***************** Macros (Inline Functions) Definitions *******************/

/**
 *
 * Read a value from a PLB_PWM_CTRL register. A 32 bit read is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is read from the register. The most significant data
 * will be read as 0.
 *
 * @param   BaseAddress is the base address of the PLB_PWM_CTRL device.
 * @param   RegOffset is the register offset from the base to write to.
 *
 * @return  Data is the data from the register.
 *
 * @note
 * C-style signature:
 * 	Xuint32 PLB_PWM_CTRL_mReadReg(Xuint32 BaseAddress, unsigned RegOffset)
 *
 */
#define PLB_PWM_CTRL_mReadReg(BaseAddress, RegOffset) \
 	XIo_In32((BaseAddress) + (RegOffset))


/**
 *
 * Write/Read 32 bit value to/from PLB_PWM_CTRL user logic slave registers.
 *
 * @param   BaseAddress is the base address of the PLB_PWM_CTRL device.
 * @param   RegOffset is the offset from the slave register to write to or read from.
 * @param   Value is the data written to the register.
 *
 * @return  Data is the data from the user logic slave register.
 *
 * @note
 * C-style signature:
 * 	void PLB_PWM_CTRL_mWriteSlaveRegn(Xuint32 BaseAddress, unsigned RegOffset, Xuint32 Value)
 * 	Xuint32 PLB_PWM_CTRL_mReadSlaveRegn(Xuint32 BaseAddress, unsigned RegOffset)
 *
 */
#define SET_PWM_DUTY(BaseAddress, RegOffset, Value) \
 	XIo_Out32((BaseAddress) + (RegOffset*8), (Xuint32)(Value))
#define SET_PWM_PERIOD(BaseAddress, RegOffset, Value) \
 	XIo_Out32((BaseAddress) + (RegOffset*8+4), (Xuint32)(Value))

#define GET_PWM_DUTY(BaseAddress, RegOffset) \
 	XIo_In32((BaseAddress) + (RegOffset*8))
#define GET_PWM_PERIOD(BaseAddress, RegOffset) \
 	XIo_In32((BaseAddress) + (RegOffset*8+4))

#endif /** PLB_PWM_CTRL_H */
