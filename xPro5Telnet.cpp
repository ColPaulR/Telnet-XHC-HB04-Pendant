//#include <stdlib.h>
//#include <stdio.h>
//#include <string.h>
//#include <math.h>
//#include <assert.h>
//#include <signal.h>
//#include <string.h>
//#include <libusb.h>
//#include <unistd.h>
//#include <synchapi.h>
//#include <stdarg.h>
//#include <iostream>

#include <unistd.h>
#include <iostream> 
#include "XHC_HB04.cpp"

XHC_HB04 myPendant;

void hexdump(unsigned char *data, int len)
{
	int i;

	for (i=0; i<len; i++) printf("%02X ", data[i]);
}

int main()
{
	unsigned char myReadBuff[32];
	int iReturn;

	while (!myPendant.bDo_Exit) {
		iReturn=myPendant.ReadXHC(myReadBuff);
		if (iReturn==0) {
			// Debug print out data
			hexdump((unsigned char*)&in_buf, iLen);
			//printf(" button1 = %x button2 = %x axis = %x scale = %x jog = %x\n", this->ubButton1, this->ubButton2, this->ubAxis,this->ubScale, this->cJog);
		}
		usleep(100000);
	}
	return 0;
}


