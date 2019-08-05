#include <libusb-1.0/libusb.h>

const int BUFLEN = 35;

void printdev(libusb_device *dev); // Function prototype
int bcd2int(char bcd); // Function prototype
short int obtain_usb_data(unsigned char* receive_buffer);