set FLASH_FILE             "executable.elf.srec"   ;# Image file to program the flash with
set FLASH_BASEADDR         0x83000000              ;# Base address of flash device
set FLASH_PROG_OFFSET      0x00000000              ;# Offset at which the image should be programmed within flash
set FLASH_BUSWIDTH         16                      ;# Device bus width of all flash parts combined
set SCRATCH_BASEADDR       0xFFFF0000              ;# Base address of scratch memory
set SCRATCH_LEN            0x00010000              ;# Length of scratch memory
set XMD_CONNECT            "connect ppc hw -debugdevice cpunr 1" ;# Target Command to connect to XMD
set TARGET_TYPE            "PPC405"                ;# Target processor type
set PROC_INSTANCE          "ppc405_0"              ;# Processor Instance name

