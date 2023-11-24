#pragma once
#include <libusb.h>

#define LB04_VID 4302
#define LB04_PID 60307

typedef enum {
	axis_off = 0x00,
	axis_x = 0x11,
	axis_y = 0x12,
	axis_z = 0x13,
	axis_a = 0x18,
	axis_spindle = 0x14,
	axis_feed = 0x15
} xhc_axis_t;


class XHC_HB04
{
	unsigned char ubAxis;
	unsigned char ubScale;
	char cJog;
	
	// USB variables
	libusb_context* context = NULL;
	libusb_device_handle* dev_handle;

	int iConnect();
	int xhc_encode_float(float v, unsigned char* buf);
	int xhc_encode_s16(int v, unsigned char* buf);
	void xhc_display_encode(unsigned char* data, int len);

public:
	bool bDo_Exit;
	bool bDo_Reconnect;

	XHC_HB04(void);
	~XHC_HB04(void);
	//int SetDisplay(unsigned char* data);
	int ReadXHC(unsigned char in_buf[]);
};

