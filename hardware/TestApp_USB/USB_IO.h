

#ifndef USB_IO_H



#define USB_BASEADDR   			XPAR_PLB_USB_0_BASEADDR

#define USB_RD_STATUS_OFFSET		0x00000000
#define USB_RD_DATAREAD_OFFSET	0x00000008
#define USB_RD_STARTADDR_OFFSET	0x00000010
#define USB_RD_XFERSIZE_OFFSET	0x00000018

#define USB_WR_SEND_OFFSET			0x00000000
#define USB_WR_DATAWRITE_OFFSET	0x00000008
#define USB_WR_BURSTSIZE_OFFSET	0x00000010
#define USB_WR_XFERSIZE_OFFSET	0x00000018
#define USB_WR_STARTADDR_OFFSET	0x00000020


#define USB_COMMIT					0x00000001
#define USB_BURST_XFER				0x00000000

#define USB_DMA_SESSION_ACTIVE	0x00000020
#define USB_PBF_EMPTY				0x00000010
#define USB_USB_READY				0x00000008
#define USB_RECV_EMPTY				0x00000004
#define USB_SEND_EMPTY				0x00000002
#define USB_SEND_FULL				0x00000001

#define USB_READ_STATUS				(USB_USB_READY | USB_RECV_EMPTY) 
#define USB_VALID_READ				USB_USB_READY

#define USB_WRITE_STATUS			(USB_DMA_SESSION_ACTIVE|USB_PBF_EMPTY|USB_USB_READY\
											|USB_SEND_FULL)
#define USB_VALID_WRITE				(USB_PBF_EMPTY|USB_USB_READY)


#define USB_getStatus()  XIo_In32(USB_BASEADDR + USB_RD_STATUS_OFFSET)

// function definitions

void USB_init();

// status functions
u32 USB_writeReady();
u32 USB_readReady();

// read functions
u16 USB_readWord();
u32 USB_burstRead16(u16* startAddr, u32 nWords);
u32 USB_burstRead32(u32* startAddr, u32 nDWords);

// write functions
void USB_forceCommit();
void USB_singleWrite(u16 data);

int USB_setBurstSize(u32 size);
int USB_blockWrite(u32* startAddr, u32 xferSize);

#endif
