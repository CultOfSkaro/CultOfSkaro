#include "xparameters.h"

#include "xcache_l.h"

#include "xuartlite_l.h"

#include "stdio.h"

#include "xutil.h"

#include "USB_IO.h"

//====================================================

int main (void) {

   XCache_EnableICache(0x00000001);
   XCache_EnableDCache(0x00000001);
   print("-- Entering main() --\r\n");
   print("Initializing USB\r\n");
   USB_init();
   print("USB Status: ");
   printf("%x\r\n", USB_getStatus());
	print("Testing writeReady of USB: ");
	if (!USB_writeReady())
	{
		print("FAILED\r\n");
	}
	else
	{
		print("PASSED\r\n\r\n");
	}
	
	// single reads
	print("Testing single reads\r\n");
	print("please enter sequence \"1234\" over USB and press enter on STDIN\r\n");
	print("\r\n");
	char ch;
	read(0,&ch,1);
	while (!XUartLite_IsReceiveEmpty((int) stdin)) getchar();

	if (USB_readWord() != '1' || USB_readWord() != '2' ||
		USB_readWord() != '3' || USB_readWord() != '4' || USB_readReady()) {
		
		print("TEST single reads : FAILED\r\n");
	} else {
		print("TEST single reads : PASSED\r\n");
	}
	print("\r\n");

	while(USB_readReady())
		USB_readWord();
		
	
	// burst 16 bit reads
	print("Testing burst16 reads\r\n");
	print("please enter sequence \"1234\" over USB and press enter on STDIN\r\n");
	print("\r\n");
	
	read(0,&ch,1);
	while (!XUartLite_IsReceiveEmpty((int) stdin)) getchar();
		
	u16 c[4];
	u32 length = USB_burstRead16(c, 4);
	if (c[0] != '1' || c[1] != '2' ||
		c[2] != '3' || c[3] != '4' || length != 4 || USB_readReady()) {
		
		print("TEST burst16 reads : FAILED\r\n");
	} else {
		print("TEST burst16 reads : PASSED\r\n");
	}
	print("\r\n");
	
	while(USB_readReady())
		USB_readWord();
		
	
	// burst 32 bit reads
	print("Testing burst16 reads\r\n");
	print("please enter sequence \"1234\" over USB and press enter on STDIN\r\n");
	
	read(0,&ch,1);
	while (!XUartLite_IsReceiveEmpty((int) stdin)) getchar();
	u32 d[2];
	length = USB_burstRead32(d, 2);

	if (((u16*) d)[0] != '1' || ((u16*) d)[1] != '2' ||
		((u16*) d)[2] != '3' || ((u16*) d)[3] != '4' || 
		length != 2 || USB_readReady()) {
		
		print("TEST burst32 reads : FAILED\r\n");
	} else {
		print("TEST burst32 reads : PASSED\r\n");
	}
	print ("\r\n");


	print("Testing  single write\r\n");

	USB_singleWrite('1');
	USB_singleWrite('2');
	USB_singleWrite('3');
	USB_singleWrite('4');
	USB_forceCommit();

	
	print("Did \"1234\" print in the USB terminal (Y/N) :");
	do {
		read(0,&ch,1);
	} while (tolower(ch) != 'y' && tolower(ch) != 'n');
	
	if (tolower(ch) == 'y') {
		print("\r\nTEST single write : PASSED");
	} else {
		print("\r\nTEST single write : FAILED");
	}
	while (!XUartLite_IsReceiveEmpty((int) stdin)) getchar();

	print("\r\n");
	
	
	USB_setBurstSize(128);

	u32* longdword = " T h i s   s e n t e n c e   h a s   e x a c t l y   o n e   h u n d r e d   f i f t y   t w o   c h a r a c t e r s   i n   i t   ( i n c l u d i n g   s p a c e s ,   p a r e n t h e s e ,   c o m m a s ,   a n d   t h e   f i v e   e x c l a m a t i o n   m a r k s ,   o f   c o u r s e ) ! ! ! ! ! !";

	USB_blockWrite(longdword, 152);
	USB_forceCommit();


	print("Did the message print correctly (ending with 6 !s) in the USB terminal (Y/N) :");
	do {
		read(0,&ch,1);
	} while (tolower(ch) != 'y' && tolower(ch) != 'n');
	
	if (tolower(ch) == 'y') {
		print("\r\nTEST single write : PASSED");
	} else {
		print("\r\nTEST single write : FAILED");
	}
	//while (!XUartLite_mIsReceiveEmpty(stdin)) getchar();

	print("\r\n");
	
	print("Tests Completed");
	
	
   print("-- Exiting main() --\r\n");
   XCache_DisableDCache();
   XCache_DisableICache();
   return 0;
}

