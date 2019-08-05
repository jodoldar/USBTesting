/**
 * @file main.h
 * @author Josep Dols (jodoldar@gmail.com)
 * @brief Header of the main.cpp file
 * @version 1.0.0
 * @date 2019-08-05
 * 
 * @copyright Copyright (c) 2019
 * 
 * Header of the main file of the program. In includes the definition of the
 * size of the array where the USB data is stored and the functions headers.
 */

#include <libusb-1.0/libusb.h>

/**
 * @brief Size of the data buffer where USB data is stored
 */
const int BUFLEN = 35;

void printdev(libusb_device *dev);
short int obtain_usb_data(unsigned char* receive_buffer);