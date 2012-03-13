/**
\file
\author Barrett Edwards

FILE:				ISR.c
AUTHOR:				Barrett Edwards
CREATED:			26 Sep 2006
PLATFORM:			Helios Computing Platform
FRAMEWORK:			Communication Framework
COMPANION FILES:	ISR.h
DESCRIPTION:		See ISR.h

	Related Functions & Groups 
	- Init
		- void ISR_Init();
	- ISRs
		- void ISR_Timer(void *ptr);
		- void ISR_Serial();
		- void ISR_Wireless();
		- void ISR_TTL();
		- void ISR_USB();
		- void ISR_Correlator();
	- Handlers 
		- void ISR_HandleAll();
		- void ISR_Handler(ISRInfo *info);

CHANGE LOG
- 01 Aug 2007 BBE Added Doxygen style comments

*/

/* Includes -----------------------------------------------------------------*/
#include <string.h>

#include <xtime_l.h>
#include <USB_IO.h>
#include <xgpio.h>
//#include <plb_correlator.h>

#include "InterruptControl.h"
#include "BufferStore.h"
#include "Comm.h"
#include "Packet.h"
#include "State.h"
#include "LoopScheduler.h"
#include "ISR.h"
#include "HeliosIO.h"
#include "ServoControl.h"


/* Macros -------------------------------------------------------------------*/
/* Enumerations -------------------------------------------------------------*/
/* Structs ------------------------------------------------------------------*/
/* Global Memory  -----------------------------------------------------------*/
//ISRInfo isriSerial;		///< Incomming communication buffer information for RS-232 Serial Port
//ISRInfo isriWireless;	///< Incomming communication buffer information for Wireless Serial Port
ISRInfo isriUSB;		///< Incomming communication buffer information for USB Port
// globals for the game system 
extern XGpio Gpio;
extern uint8 GAMEENABLED;
extern uint8 GAMEFLAG;
extern uint8 GAMEWTS;
extern uint8 GAMEPAUSED;
extern int32 str;
extern int32 thr;
Xuint32 oldBits;


/* External Memory-----------------------------------------------------------*/
/* Function Prototypes ------------------------------------------------------*/
/* Code ---------------------------------------------------------------------*/

/** Alloacte & initialize the incomming communication memory storage buffers
	The ISRInfo structs contain a pointer to a memory buffer where incomming
	communication data is temporarily stored until the entire message is 
	received and processed.
*/
void ISR_Init()
{
	XStatus Status;
	//isriSerial.buffer	= (uint8*) MemAlloc(ISR_BUFFER_SIZE);
	//isriWireless.buffer	= (uint8*) MemAlloc(ISR_BUFFER_SIZE);
	isriUSB.buffer		= (uint8*) MemAlloc(ISR_BUFFER_SIZE);
	//isriSerial.index	= 0;
	//isriWireless.index	= 0;
	isriUSB.index		= 0;
}





/** Timer Tick ISR - happens every 1 ms

	#Call the ISRs for the serial ports here!#
	The maximum baud rate of a standard RS-232 connection is 115200 Baud
	115200 / 10 bits per symbol = 11520 bytes per second.
	At 11520 bytes per second, a single byte transfer over the RS-232
	line takes 87 micro-seconds.
	Since the Xilinx opb_uartlite we are using has a 16 byte receive buffer
	we need to service the uart (read the data out) at least every 
	16 * 87 usec = 1.3 milliseconds 
	So if we call the ISR routines every millisecond we should be safe.

	The reason we do this is so that we do NOT have to hook up the interrupt
	lies of the uart FPGA core. The problem with doing this is that the serial
	port is so slow. for example:
	- one byte will arrive at the serial port 
	- uart interrupt will go off in the PowerPC
	- PowerPC will save state and call the ISR
	- ISR will read out #only 1 byte# and then return since no other data has arrived yet
	- PowerPC will resume exeuction
	- 87 micro seconds later....
	- one more byte will arrive and the cycles repeats
	
	If alot of serial data is sent over the uart the above method will thrash 
	the PowerPC. The PowerPC will not have enough time to do anything useful. 
	So by unhooking the interrupt line of the uart and explicitly calling these
	ISR functions at a regular timed interval, we can reduce the thrashing on 
	the	PowerPC.

	NOTE: We do not call the USB interrupt handler here. We use the interrupt 
	signaling method on the USB because the USB is so fast. When the first 
	word has arrived, by the time the PowerPC saves state and gets to calling 
	the ISR, more of the data packet will have already arrived. It has been my 
	experience (BBE) that when a command in sent to the Helios from the GUI 
	over the USB, that the entire packet (usually 10 to 20 bytes long) will 
	only cause a single interrupt. Oh and the USB also uses the interrupt to 
	signal that it has completed a DMA burst transaction. So we have to use 
	the interrupt.
*/
void ISR_Timer(void *ptr)
{
	CPU_MSR msr;
	msr = DISABLE_INTERRUPTS();
	{
		M_uint[U_MS]++;

		//ISR_Serial();
		//ISR_Wireless();

		LoopSchedulerTimerTick();
	}
	RESTORE_INTERRUPTS(msr);

	XTime_PITClearInterrupt(); // Clear the PIT's interrupt bit
}

/** Interrupt Service Routine for GPIO core
  This function is called when any interrupt happens on the GPIO module.  Inside this
  function, which pin caused the interrupt is determined and the appropriate function is
  called.
  */
void ISR_GPIO()
{
	Xuint32 interruptStatus;
	CPU_MSR msr;
	msr = DISABLE_INTERRUPTS();
	
	//XGpio_InterruptDisable(&Gpio, GAME_SYSTEM_GPIO_INT);
	interruptStatus = XGpio_InterruptGetStatus(&Gpio);
	//get GPIO bits
	if(interruptStatus | GAME_SYSTEM_GPIO_INT)
	{
		Xuint32 bits = XGpio_DiscreteRead(&Gpio, GAME_SYSTEM_GPIO_CHANNEL);
		Xuint32 changes = bits ^ oldBits;
		if(changes != 0)
		{
			//xil_printf("INT changed: 0x%x\r\n", changes);		
			/*if(changes & bits & GAME_SHOT_STATUS_READY_M)
			{
				if( bits & GAME_HIT_M )
					Game_Hit();
				else
					Game_Miss();
			}*/
			/*if(changes & GAME_READY_TO_SHOOT_M)
			{
				if( (bits & GAME_READY_TO_SHOOT_M) ) 
				{
					xil_printf("Clear to fire\r\n");
					xil_printf("Game state: %x", XGpio_DiscreteRead(&Gpio, GAME_SYSTEM_GPIO_CHANNEL));
				}
				if( !(bits & GAME_READY_TO_SHOOT_M) )
				{
					xil_printf("Shot Fired!\r\n");
					//deassert shots
					Game_ClearShots();
					//xil_printf("Game state: %x", XGpio_DiscreteRead(&Gpio, GAME_SYSTEM_GPIO_CHANNEL));
				}
			}*/
			if(changes & GAME_TRUCK_ALIVE_M)
			{
				if((bits & GAME_TRUCK_ALIVE_M))
				{
					xil_printf("Truck revived!\r\n");
				}
				else
				{
					xil_printf("You died :(\r\n");
					str = 0;
					thr = 0;
					SetServo(RC_STR_SERVO, 0);
					SetServo(RC_VEL_SERVO, 0);
				}
			}
			if(changes & GAME_HAVE_FLAG_M)
			{
				if((bits & GAME_HAVE_FLAG_M))
				{
					xil_printf("You have the flag.\r\n");
				}
				else
				{
					xil_printf("You no longer have the flag.\r\n");
				}
			}
			if(changes & GAME_STATE_M)
			{
				if((bits & GAME_STATE_M) == GAME_STATE_STOP)
				{
					xil_printf("Game reset.\r\n");
				}
				else if((bits & GAME_STATE_M) == GAME_STATE_GO)
				{
					xil_printf("Game has started.\r\n");
					Game_UnPaused();
				}
				else if((bits & GAME_STATE_M) == GAME_STATE_PAUSE)
				{
					xil_printf("Game paused.\r\n");
					Game_Paused();
				}
				else if((bits & GAME_STATE_M) == GAME_STATE_WIN)
				{
					xil_printf("Game over.\r\n");
					Game_FlagCaptured();
				}
			}
		}
		oldBits = bits;
	}
	else
	{
		xil_printf("Ch1 INT\r\n");
		Game_Shoot(GAME_KILL_SHOT);
	}
	XGpio_InterruptClear(&Gpio, XGPIO_IR_MASK);
	//XGpio_InterruptEnable(&Gpio, GAME_SYSTEM_GPIO_INT);
	RESTORE_INTERRUPTS(msr);
}

/** Interrupt Service Routine for RS-232 Serial Port UART
	This function determines if any incomming data has arrived on the RS-232 
	Serial port. If data has arrived, it is copied into the ISRInfo buffer.
	The received data is not processed here. It is processed at a later time
	in the Handler function, which is called from #outside# the Interrupt 
	Service Routine

	STEPS:
	-# Disable interrupts so nothing is interrupted
	-# Check to see if new data has arrived 
	-# If new data has arrived then increment the ISR counter. This ISR Counter
		represents the number of times that this function has been called and 
		successfuly capture new incomming communication data.
	-# Grab each incomming byte and stick into the ISRInfo buffer.
	-# If the ISRInfo buffer overflows, then we cannot accept the data and it 
		gets dropped
*/
/* THIS IS UNUSED, if you want to use this instead of something else,
 * uncomment the definitions of the isriSerial. ISRInfo isriSerial, and the function calls to this
 * function inside ISR_Timer()
 
void ISR_Serial()
{	
	CPU_MSR msr;
	int val;
	
	msr = DISABLE_INTERRUPTS();
	{
		if(SER_DataReady(SER_BASEADDR))
			M_uint[U_ISR_COUNT_SER]++;

		while(SER_DataReady(SER_BASEADDR))
		{
			val = SER_getch(SER_BASEADDR);
			if(isriSerial.index < ISR_BUFFER_SIZE)
				isriSerial.buffer[isriSerial.index++] = val;
		}
	}
	RESTORE_INTERRUPTS(msr);
}
*/



/** Interrupt Service Routine for Wireless Serial Port UART
	This function determines if any incomming data has arrived on the Wireless
	Serial port. If data has arrived, it is copied into the ISRInfo buffer.
	The received data is not processed here. It is processed at a later time
	in the Handler function, which is called from #outside# the Interrupt 
	Service Routine

	STEPS:
	-# Disable interrupts so nothing is interrupted
	-# Check to see if new data has arrived 
	-# If new data has arrived then increment the ISR counter. This ISR Counter
		represents the number of times that this function has been called and 
		successfuly capture new incomming communication data.
	-# Grab each incomming byte and stick into the ISRInfo buffer.
	-# If the ISRInfo buffer overflows, then we cannot accept the data and it 
		gets dropped
*/
/* THIS IS UNUSED, if you want to use this instead of something else,
 * uncomment the definitions of the isriWireless. ISRInfo isriWireless, and the function calls to this
 * function inside ISR_Timer()

void ISR_Wireless()
{	
	CPU_MSR msr;
	int val;
	
	msr = DISABLE_INTERRUPTS();
	{
		if(SER_DataReady(WIR_BASEADDR))
			M_uint[U_ISR_COUNT_WIR]++;

		while(SER_DataReady(WIR_BASEADDR))
		{
			val = SER_getch(WIR_BASEADDR);
			if(isriWireless.index < ISR_BUFFER_SIZE)
				isriWireless.buffer[isriWireless.index++] = val;
		}
	}
	RESTORE_INTERRUPTS(msr);
}
*/


/** Interrupt Service Routine for USB
	This function determines if any incomming data has arrived on the USB
	port. If data has arrived, it is copied into the ISRInfo buffer.
	The received data is not processed here. It is processed at a later time
	in the Handler function, which is called from #outside# the Interrupt 
	Service Routine

	The USB pcore also asserts an interrupt to indicate that has completed a 
	DMA based transmission. This function clears a flag in the pcore to tell it
	that the PowerPC now knows it is finished with that transaction.

	STEPS:
	-# Disable interrupts so nothing is interrupted
	-# Clear the USB DMA interrupt flag
	-# Check to see if new data has arrived 
	-# If new data has arrived then increment the ISR counter. This ISR Counter
		represents the number of times that this function has been called and 
		successfuly capture new incomming communication data.
	-# Grab each incomming byte and stick into the ISRInfo buffer.
	-# If the ISRInfo buffer overflows, then we cannot accept the data and it 
		gets dropped
*/
void ISR_USB()
{
	CPU_MSR msr;
	uint32  val;
	
	msr = DISABLE_INTERRUPTS();
	{		
		//USB_ClearDMAInterrupt();
		//xil_printf("USBINT\r\n");
		if(USB_readReady())
			M_uint[U_ISR_COUNT_USB]++;

		while(USB_readReady())
		{			
			val = USB_readWord();
			if(isriUSB.index < ISR_BUFFER_SIZE)
			{
				isriUSB.buffer[isriUSB.index++] = val >> 8;
				isriUSB.buffer[isriUSB.index++] = val & 0x00FF;
			}
		}						
	}
	RESTORE_INTERRUPTS(msr);	
}


/** Wrapper function to handle data on all incomming comunication channels
	This function simply calls ISR_Handler() on each of the communication 
	channels:
	- RS-232 Serial
	- Wireless 
	- TTL
	- USB
*/
void ISR_HandleAll()
{
	//ISR_Handler(&isriSerial);
	//ISR_Handler(&isriWireless);
	ISR_Handler(&isriUSB);
}





/**	Find & Handle packets in an incomming communication data stream
	Incomming communication data can be represented as a uint8 array. 
	This function looks at a uint8 array and attempts to locate packtes of data
	that are formatted according the to HeliosPacketHeader communication 
	protocol. 

	In this communication protocol, the HeliosPacketHeader struct is sent 
	first, which is then followed by a variable length data buffer, which is 
	posibly length 0. The HeliosPacketHeader is currently 10 bytes in size, so 
	the	smallest an incomming data packet could bt is 10 bytes. This is why 
	there is the number '10' all over this function. 

	STEPS:
	-# Disable interrupts. The ISRInfo::index could change...
	-# Determine if there is enough information in the incomming data buffer 
	   to fit a HeliosPacketHeader (10 bytes) (while(index >= 10))
	-# If there is enough data to possibly represent a HeliosPacketHeader, then
	   we need to check to see if the first 10 bytes in the buffer are a 
	   HeliosPacketHeader. To do this we call BufferToHeader() and then test the
	   header using verifyHeader(). The verifyHeader() function tests the Magic
	   number, and the header checksum calculation. If verifyHeader() returns 
	   TRUE then the first 10 bytes in the buffer are a HeliosPacketHeader.
	-# If we get here then we must have a HeliosPacketHeader. Now we need to 
	   look and the header and see how long the data buffer is supossed to be. 
	   If there is enough received data in the buffer to hold both the header and 
	   the accompanying data (if(index >= 10 + header.buffer_size)) then we need to
	   test the checksum on the accompanying packet data. 
	-# If the computed checksum of the packet data matches the checksum in the 
	   header if(verifyDataBuffer(&header, &buffer[10] , header.buffer_size)) then
	   we have successfuly received a communication packet. We then pass the 
	   verified packet the Comm.c file for processing 
	   (RX_ProcessPacket(&header, &buffer[10]);).

	#Error Handling:# In any type of communication system, the possibility of 
	data corruption must handled to ensure accurate reception of data. In this 
	implementation, we use a predefined packet header size and checksums on both
	the header and the packet data.

	Verification Error Handling
	- If the first byte is not the magic number MAGIC, then we have 'gotten 
	  lost' in the data stream. So we need to resyncronize with the incomming 
	  data stream. To resyncronize with the data stream we simply keep looking 
	  down the stream until we find the possibility of the start of a new 
	  packet, which will begin with the magic number MAGIC (0xBE). We then run 
	  the verifyHeader() check again to see if we have found the start of a new
	  packet or if we have just found a random 0xBE in the data stream. We keep
	  searching like this until we find the start of a real packet. Once we 
	  find the possible start of a new packet, we shift the data in the array 
	  toward the front of the array using memmove(). Doing this ensures that the
	  start of a HeliosPacketHeader is always in the buffer[0] spot. This is 
	  how we "syncronize" with the data stream.
	- If we succesfuly verify a packet header and then test the packet data and
	  the checksum fails, then something has happened and we cannot use this
	  packet. So we know for sure that the first 10 bytes in the incomming data
	  stream need to be trashed. We then start searching the rest of the data 
	  stream for a magic number. But this time we start at byte 11 since we
	  know that the header is bad. Once we find the magic number we shift the
	  remaining data in the buffer to the start of the buffer and try again.
*/
void ISR_Handler(ISRInfo *info)
{
	HeliosPacketHeader header;	  
	uint8* buffer = info->buffer; // This is used so we don't have to use info->buffer everywhere
	CPU_MSR msr;					
	
	msr = DISABLE_INTERRUPTS();
	{//A
		while(info->index >= 10)
		{//B
			BufferToHeader(&header,buffer);			
			if(verifyHeader(&header))
			{//C
				if(info->index >= 10 + header.buffer_size)
				{//E
					if(verifyDataBuffer(&header, &buffer[10] , header.buffer_size))
					{//F
						RX_ProcessPacket(&header, &buffer[10]); 
						info->index -= 10 + header.buffer_size;	
						memmove(&buffer[0], &buffer[10 + header.buffer_size], info->index);
					}//F
					else 
					{//G
						// Data checksum test failed. So these data bytes are not a valid
						int index2;
						for(index2 = 10 ; index2 < info->index ; index2++)
							if(buffer[index2] == MAGIC)
								break;
						memmove(&buffer[0], &buffer[index2], info->index - index2);
						info->index -= index2;
					}//G
				}//E
				else 
				{// Not enough bytes of the data buffer have been read in yet. So we will have to wait  
					break;
				}//End: if(index >= 10 + header.buffer_size) else{}
			}//C
			else 
			{//D
				// Header checksum test failed. So these 10 bytes are not a valid header.
				// Search for the next start of a packet (find magic number MAGIC == 0xBE)
				int index2;
				for(index2 = 1 ; index2 < info->index ; index2++)
					if(buffer[index2] == MAGIC)
						break;
				memmove(&buffer[0], &buffer[index2], info->index - index2);
				info->index -= index2;
			}//D
		}//B
	}//A
	RESTORE_INTERRUPTS(msr);
}
