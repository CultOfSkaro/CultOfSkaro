##############################################################################
## Filename:          /data/projects/SpencerBranch/drivers/plb_vision_v1_00_a/data/plb_vision_v2_1_0.tcl
## Description:       Microprocess Driver Command (tcl)
## Date:              Mon Feb  8 12:51:11 2010 (by Create and Import Peripheral Wizard)
##############################################################################

#uses "xillib.tcl"

proc generate {drv_handle} {
  xdefine_include_file $drv_handle "xparameters.h" "plb_vision" "NUM_INSTANCES" "DEVICE_ID" "C_BASEADDR" "C_HIGHADDR" 
}
