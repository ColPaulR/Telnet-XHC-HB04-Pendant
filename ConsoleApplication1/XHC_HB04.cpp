#include "XHC_HB04.h"
#include <math.h>
#include <stdio.h>
//#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <libusb.h>



int XHC_HB04::iConnect()
{
	// TODO: Add your implementation code here.

	size_t list;
	libusb_device** devs;
	int ret=-1;

	printf("waiting for XHC-HB04 device\n");

	list = libusb_get_device_list(context, &devs);

	if (list < 0) {
		perror("libusb_get_device_list");
		return 1;
	}

	dev_handle = libusb_open_device_with_vid_pid(context, LB04_VID, LB04_PID);
	libusb_free_device_list(devs, 1);

	if (dev_handle) {
		printf("found XHC-HB04 device\n");

		if (libusb_kernel_driver_active(dev_handle, 0) == 1) {
			libusb_detach_kernel_driver(dev_handle, 0);
		}

		ret = libusb_claim_interface(dev_handle, 0);
		if (ret < 0) {
			perror("libusb_claim_interface");
			return ret;
		}

		ret = libusb_set_configuration(dev_handle, 1);
		
		// Show as connected
		bDo_Reconnect = FALSE;
	}
	else {
		// Show as disconnected
		bDo_Reconnect = TRUE;
	}

	// Return success
	return ret;
}

//int XHC_HB04::xhc_encode_float(float v, unsigned char* buf)
//{
//	unsigned int int_v = round(fabs(v) * 10000.0);
//	unsigned short int_part = int_v / 10000;
//	unsigned short fract_part = int_v % 10000;
//	if (v < 0) fract_part = fract_part | 0x8000;
//	*(short*)buf = int_part;
//	*((short*)buf + 1) = fract_part;
//	return 4;
//}
//
//int XHC_HB04::xhc_encode_s16(int v, unsigned char* buf)
//{
//	*(short*)buf = v;
//	return 2;
//}
//
//void XHC_HB04::xhc_display_encode(unsigned char* data, int len)
//{
//	unsigned char buf[6 * 7];
//	unsigned char* p = buf;
//	int i;
//	int packet;
//
//	assert(len == 6 * 8);
//
//	memset(buf, 0, sizeof(buf));
//
//	*p++ = 0xFE;
//	*p++ = 0xFD;
//	*p++ = 0x0C;
//
//	if (this->ubAxis == axis_a)
//		p += xhc_encode_float(round(1000 * 4) / 1000, p);
//	else
//		p += xhc_encode_float(round(1000 * 1) / 1000, p);
//	p += xhc_encode_float(round(1000 * 2) / 1000, p);
//	p += xhc_encode_float(round(1000 * 3) / 1000, p);
//	if (this->ubAxis == axis_a)
//		p += xhc_encode_float(round(1000 * 14) / 1000, p);
//	else
//		p += xhc_encode_float(round(1000 * 11) / 1000, p);
//	p += xhc_encode_float(round(1000 * 12) / 1000, p);
//	p += xhc_encode_float(round(1000 * 13) / 1000, p);
//
//	/*p += xhc_encode_s16(round(100.0 * *(xhc->hal->feedrate_override)), p);
//	p += xhc_encode_s16(round(100.0 * *(xhc->hal->spindle_override)), p);
//	p += xhc_encode_s16(round(60.0 * *(xhc->hal->feedrate)), p);
//	p += xhc_encode_s16(round(60.0 * *(xhc->hal->spindle_rps)), p);
//	*/
//
//	switch (this->ubScale) {
//	case 1:
//		buf[35] = 0x01;
//		break;
//	case 10:
//		buf[35] = 0x03;
//		break;
//	case 100:
//		buf[35] = 0x08;
//		break;
//	case 1000:
//		buf[35] = 0x0A;
//		break;
//	}
//
//	// Multiplex to 6 USB transactions
//
//	p = buf;
//	for (packet = 0; packet < 6; packet++) {
//		for (i = 0; i < 8; i++) {
//			if (i == 0)
//				data[i + 8 * packet] = 6;
//			else
//				data[i + 8 * packet] = *p++;
//		}
//	}
//}


XHC_HB04::XHC_HB04() {
	int ret;

	// Quit if 1
	this->bDo_Exit = FALSE;

	if (libusb_init(&context) < 0) {
		perror("libusb_init");
		return;
	}

	libusb_set_option(context, LIBUSB_OPTION_MAX);

	ret = iConnect();
}

XHC_HB04::~XHC_HB04() {
	printf("connection lost, cleaning up\n");
	//libusb_cancel_transfer(transfer_in);
	//libusb_free_transfer(transfer_in);
	libusb_release_interface(dev_handle, 0);
	libusb_close(dev_handle);
	libusb_exit(context);
}

//int XHC_HB04::SetDisplay(unsigned char* data) {
//	//unsigned char data[6*8];
//	int packet;
//	int ret;
//
//
//	// Check to see if connected
//	if (this->bDo_Reconnect) {
//		ret = iConnect();
//		if (ret != 0)
//			return ret;
//	}
//
//	// Format buffer
//	xhc_display_encode(unsigned char* data, sizeof(data));
//
//	// Assumed connected here
//	for (packet = 0; packet < 6; packet++) {
//		ret = libusb_control_transfer(dev_handle, 0x21, 0x09, 0x0306, 0x00, data + 8 * packet, 8, 0))
//		if (ret < 0) {
//			// Transferred less than 0 bytes
//			bDo_Reconnect = 1;
//			return ret;
//		}
//	}
//
//}
//
int XHC_HB04::ReadXHC(unsigned char in_buf[]) {
	int iLen;
	int ret;

	//unsigned char in_buf[6];

	// Check to see if connected
	if (this->bDo_Reconnect) {
		ret = iConnect();
		if (ret != 0) return ret;
	}
	else {
		// Assumed connected here
		switch (ret = libusb_bulk_transfer(dev_handle, (0x01 | LIBUSB_ENDPOINT_IN), in_buf, sizeof(in_buf), &iLen, 0))
		{
			case 0:
				// 0 on success (and populates transferred)
				if (iLen != sizeof(in_buf)) {
					std::cerr << "Wrong number of bytes returned: " + iLen;
					return iLen;
				}
				return 0;
			case LIBUSB_ERROR_NO_DEVICE: // if the device has been disconnected
				bDo_Reconnect = 1;
			case LIBUSB_ERROR_TIMEOUT://if the transfer timed out (and populates transferred)
			case LIBUSB_ERROR_PIPE: // if the endpoint halted
			case LIBUSB_ERROR_OVERFLOW: // if the device offered more data, see Packets and overflows
			case LIBUSB_ERROR_BUSY: // if called from event handling context
			case LIBUSB_ERROR_OTHER: // code on other failures
				return ret;
		}
	}
	return ret;
}

//int XHC_HB04::xhc_encode_float(float v, unsigned char* buf)
//{
//	unsigned int int_v = round(fabs(v) * 10000.0);
//	unsigned short int_part = int_v / 10000;
//	unsigned short fract_part = int_v % 10000;
//	if (v < 0) fract_part = fract_part | 0x8000;
//	*(short*)buf = int_part;
//	*((short*)buf + 1) = fract_part;
//	return 4;
//}
//
//int XHC_HB04::xhc_encode_s16(int v, unsigned char* buf)
//{
//	*(short*)buf = v;
//	return 2;
//}
//
//void XHC_HB04::xhc_display_encode(unsigned char* data, int len)
//{
//	unsigned char buf[6 * 7];
//	unsigned char* p = buf;
//	int i;
//	int packet;
//
//	assert(len == 6 * 8);
//
//	memset(buf, 0, sizeof(buf));
//
//	*p++ = 0xFE;
//	*p++ = 0xFD;
//	*p++ = 0x0C;
//
//	if (this->ubAxis == axis_a)
//		p += xhc_encode_float(round(1000 * 4) / 1000, p);
//	else
//		p += xhc_encode_float(round(1000 * 1) / 1000, p);
//	p += xhc_encode_float(round(1000 * 2) / 1000, p);
//	p += xhc_encode_float(round(1000 * 3) / 1000, p);
//	if (this->ubAxis == axis_a)
//		p += xhc_encode_float(round(1000 * 14) / 1000, p);
//	else
//		p += xhc_encode_float(round(1000 * 11) / 1000, p);
//	p += xhc_encode_float(round(1000 * 12) / 1000, p);
//	p += xhc_encode_float(round(1000 * 13) / 1000, p);
//
//	/*p += xhc_encode_s16(round(100.0 * *(xhc->hal->feedrate_override)), p);
//	p += xhc_encode_s16(round(100.0 * *(xhc->hal->spindle_override)), p);
//	p += xhc_encode_s16(round(60.0 * *(xhc->hal->feedrate)), p);
//	p += xhc_encode_s16(round(60.0 * *(xhc->hal->spindle_rps)), p);
//	*/
//
//	switch (this->ubScale) {
//	case 1:
//		buf[35] = 0x01;
//		break;
//	case 10:
//		buf[35] = 0x03;
//		break;
//	case 100:
//		buf[35] = 0x08;
//		break;
//	case 1000:
//		buf[35] = 0x0A;
//		break;
//	}
//
//	// Multiplex to 6 USB transactions
//
//	p = buf;
//	for (packet = 0; packet < 6; packet++) {
//		for (i = 0; i < 8; i++) {
//			if (i == 0)
//				data[i + 8 * packet] = 6;
//			else
//				data[i + 8 * packet] = *p++;
//		}
//	}
//}

