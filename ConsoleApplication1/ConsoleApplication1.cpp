// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
//#include <stdlib.h>
#include <stdio.h>
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
#include "XHC_HB04.h"


void hexdump(unsigned char* data, int len)
{
	int i;

	for (i = 0; i < len; i++) printf("%02X ", data[i]);
	printf("\n");
}

int main()
{
	unsigned char myReadBuff[32];
	int iReturn;

	XHC_HB04 myPendant;


	while (!myPendant.bDo_Exit) {
		iReturn = myPendant.ReadXHC(myReadBuff);
		if (iReturn == 0) {
			// Debug print out data
			hexdump((unsigned char*)&myReadBuff, 16);
			//printf(" button1 = %x button2 = %x axis = %x scale = %x jog = %x\n", this->ubButton1, this->ubButton2, this->ubAxis,this->ubScale, this->cJog);
		}
		//usleep(100000);
	}
	return 0;
}



//int main()
//{
//    std::cout << "Hello World!\n";
//}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
