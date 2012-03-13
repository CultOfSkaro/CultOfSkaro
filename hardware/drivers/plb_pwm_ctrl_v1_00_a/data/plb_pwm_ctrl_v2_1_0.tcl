##############################################################################
## Filename:          C:\Documents and Settings\Administrator\Desktop\My Dropbox\Documents\2010\winter\Senior Project\svn\EDK11\Drew/drivers/plb_pwm_ctrl_v1_00_a/data/plb_pwm_ctrl_v2_1_0.tcl
## Description:       Microprocess Driver Command (tcl)
## Date:              Fri Jan 22 12:28:25 2010 (by Create and Import Peripheral Wizard)
##############################################################################

#uses "xillib.tcl"

proc generate {drv_handle} {
  xdefine_include_file $drv_handle "xparameters.h" "plb_pwm_ctrl" "NUM_INSTANCES" "DEVICE_ID" "C_BASEADDR" "C_HIGHADDR" 
}
