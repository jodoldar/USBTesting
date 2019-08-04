#include <iostream>
#include <chrono>
#include <thread>
#include <cstring>
#include <libusb-1.0/libusb.h>

#include "data_decoder.h"
#include "main.h"
#include "Observation.h"

using namespace std;

int main() {
    
    unsigned char receive_buffer[BUFLEN];
    Observation current_obs;
    int aux_counter = 0;

    current_obs = Observation();

    obtain_usb_data((unsigned char*)&receive_buffer);

    //decode pressure
    current_obs.setPressure(decode_pressure(receive_buffer));
    cout << "Current PRESSURE is " << current_obs.getPressure() << endl;

    //decode temperature and humidity from all sensors
    aux_counter = 0;
	for (auto it : decode_temperature(receive_buffer))
    {
        current_obs.setTemperature(it, aux_counter++);
        cout << "Current TEMPERATURE " << aux_counter-1 << " is " << current_obs.getTemperature(aux_counter-1) << endl;
    }

     aux_counter = 0;
	for (auto it : decode_humidity(receive_buffer))
    {
        current_obs.setHumidity(it, aux_counter++);
        cout << "Current HUMIDITY " << aux_counter-1 << " is " << current_obs.getHumidity(aux_counter-1) << endl;
    }

    //decode windchill
    int data_e = 0;
    // Part 1: Error management
    if ((bcd2int(receive_buffer[23] & 0xF0) > 90) || (bcd2int(receive_buffer[23] & 0x0F) > 9)) {
		if ((receive_buffer[23] == 0xAA) && (receive_buffer[24] == 0x8A))
			data_e = -1; //data->_wChill = -1;
		else if ((receive_buffer[23] == 0xBB) && (receive_buffer[24] == 0x8B))
			data_e = -2; //data->_wChill = -2;
		else if ((receive_buffer[23] == 0xEE) && (receive_buffer[24] == 0x8E))
			data_e = -3; //data->_wChill = -3;
		else
			data_e = -4; //data->_wChill = -4;
	}
	else
		data_e = 0; //data->_wChill = 0;

    // Part 2: Data Adquisition
    float temp_wChill = 0.0;
	if (((receive_buffer[24] & 0x40) != 0x40))
		cout << "data->_wChill = " << -2 << endl;
	if (data_e == 0) {
        temp_wChill = (bcd2int(receive_buffer[23]) / 10.0) + (bcd2int(receive_buffer[24] & 0x0F) * 10.0);
		if ((receive_buffer[24] & 0x20) == 0x20)
			temp_wChill += 0.05;
		if ((receive_buffer[24] & 0x80) != 0x80)
			temp_wChill *= -1;

        cout << "data->wChill = " << temp_wChill << endl;
	}
	else
		cout << "data->wChill = " << 0 << endl;
    cout << "End of the program" << endl;
    return 0;
}

void printdev (libusb_device *dev) {
    libusb_device_descriptor usbDes;

    int pDevDesc = libusb_get_device_descriptor(dev, &usbDes);
    if (pDevDesc < 0) {
        cout << "Error getting the USB descriptor" << endl;
        return;
    }

    cout << "Number of possible configurations: " << (int)usbDes.bNumConfigurations << "  ";
    cout << "Device Class: " << (int)usbDes.bDeviceClass << "  ";
    cout << "VendorID: " << usbDes.idVendor << "  ";
    cout << "ProductID: " << usbDes.idProduct << endl;

    libusb_config_descriptor *config;
    libusb_get_config_descriptor(dev, 0, &config);
    cout << "Interfaces: " << (int)config->bNumInterfaces << " ||| ";

    const libusb_interface *inter;
    const libusb_interface_descriptor *interdesc;
    const libusb_endpoint_descriptor *epdesc;

    for(int i = 0; i < (int)config->bNumInterfaces; i++) {
        inter = &config->interface[i];
        cout << "Number of alternate settings: " << inter->num_altsetting << " | ";

        for(int j = 0; j < inter->num_altsetting; j++) {
            interdesc = &inter->altsetting[j];
            cout << "Interface Number: " << (int)interdesc->bInterfaceNumber << " | ";
            cout << "Number of endpoints: " << (int)interdesc->bNumEndpoints << " | ";

            for(int k = 0; k < (int)interdesc->bNumEndpoints; k++) {
                epdesc = &interdesc->endpoint[k];
                cout << "Descriptor Type: " << (int)epdesc->bDescriptorType << " | ";
                cout << "EP Address: " << (int)epdesc->bEndpointAddress << " | ";
            }
        }
    }
    cout << endl << endl << endl;

    libusb_free_config_descriptor(config);
}


short int obtain_usb_data(unsigned char* receive_buffer)
{
    libusb_device **devs;
    libusb_device_handle *dev_handle;
    libusb_context *ctx;

    int retValue;
    int iInterface;
    ssize_t counter;
    unsigned char control_data[] = {0x05, 0x0AF, 0x00, 0x00, 0x00, 0x00, 0xAF, 0xFE};
    int control_addr = 0x020001;
    int transferred_len = 0;
    int bytes_transferred = 0;
    int total_transferred = 0;
    unsigned char crc;
    bool finish = false;

    retValue = libusb_init(&ctx);
    if (retValue < 0) {
        cerr << "Error initializing libusb" << endl;
        return 1;
    }

    //libusb_set_debug(ctx, 3); Deprecated

    counter = libusb_get_device_list(ctx, &devs);
    if (counter < 0) {
        cerr << "Error listing the devices" << endl;
        return 2;
    }
    #ifdef DEBUG 
        cout << "There are " << counter << " devices in the list" << endl;
        for (int i = 0; i < counter; i++) {
            printdev(devs[i]);
        }
    #endif

    dev_handle = libusb_open_device_with_vid_pid(ctx, 4400, 26625);
    if (dev_handle == NULL) {
        cerr << "Cannot open device" << endl;
    } else {
        #ifdef DEBUG 
            cout << "Device opened" << endl;
        #endif
    }

    libusb_free_device_list(devs, 1);

    retValue = libusb_set_auto_detach_kernel_driver(dev_handle, 1);
    if (retValue < 0) {
        cerr << " Error auto-setting the kernel detach: " << libusb_strerror(libusb_error(retValue)) << endl;
        if (libusb_kernel_driver_active(dev_handle, 0) == 1) {
            cerr << "Kernel Driver Active" << endl;
            if (libusb_detach_kernel_driver(dev_handle, 0) == 0) {
                cerr << "Kernel Driver Detached" << endl;
            } else {
                cerr << "Error detaching the Kernel Driver" << endl;
            }
        } else {
            cerr << "Kernel Driver not loaded" << endl;
        }
    }
    
    iInterface = libusb_claim_interface(dev_handle, 0);
    if (iInterface < 0) {
        cerr << "Cannot Claim Interface (" << libusb_strerror(libusb_error(iInterface)) << ")" << endl;
        return 3;
    }
    #ifdef DEBUG 
        cout << "Interface claimed" << endl;
    #endif

    retValue = libusb_set_interface_alt_setting(dev_handle, 0, 0);
    if (retValue < 0) {
        cerr << "Alternative setting not configured: " << libusb_strerror(libusb_error(retValue)) << endl;
    }

    do {

    // Control transferece
    control_data[4] = control_addr / 0x10000;
    control_data[3] = ( control_addr - ( control_data[4] * 0x10000 ) ) / 0x100;
    control_data[2] = control_addr - ( control_data[4] * 0x10000 ) - ( control_data[3] * 0x100 );
    control_data[5] = ( control_data[1] ^ control_data[2] ^ control_data[3] ^ control_data[4] );

    retValue = libusb_control_transfer(dev_handle, 0x21 & 0xff, 0x09 & 0xff, 0x0200 & 0xffff, 0x0000 & 0xffff, control_data, 0x08 & 0xffff, 50);
    if (retValue < 0) {
        cerr << "Error sending request: " << libusb_strerror(libusb_error(retValue)) << endl;
    } else {
        std::this_thread::sleep_for(std::chrono::milliseconds(300)); // Wait for 30 ms
        #ifdef DEBUG 
            cout << "Message sent" << endl;
        #endif
    }

    // Receive transference
    retValue = libusb_interrupt_transfer(dev_handle, 0x81 & 0xff, control_data, 0x8, &transferred_len, 50);
    if (retValue < 0) {
        //return -4;
        cerr << "Error in the receive transfer." << endl;
    }
    while ( transferred_len > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(15)); // Wait for 0.15s
        bytes_transferred = (int)(control_data[0]);
        #ifdef DEBUG 
        cout << "Control data 0: " << hex << (int)(control_data[0]) << dec << endl;
        cout << "Data transferred: " << transferred_len << endl;
        #endif
        if (( total_transferred + bytes_transferred ) < BUFLEN )
            memcpy (receive_buffer + total_transferred, control_data + 1, bytes_transferred);
        total_transferred += bytes_transferred;
        libusb_interrupt_transfer(dev_handle, 0x81 & 0xff, control_data, 0x8, &transferred_len, 50);
    }
    #ifdef DEBUG 
    cout << "Message received" << endl;

    for (int i = 0; i < BUFLEN; i++) {
        cout << "Char [" << i << "]: ";
        cout << hex << static_cast<int>(receive_buffer[i]) << dec << endl;
    }
    #endif
    // CRC Calculation
    crc = 0x00;
    for (int i = 0; i <= 32; i++ ) {
        crc = crc ^ receive_buffer[i];
    }

    if (crc == receive_buffer[33]){
        finish = true;
    } 
    if (crc == 0x5a) {
        finish = true;
    }

    #ifdef DEBUG
    cout << "CRC: " << hex << (int)crc << dec << endl;
    cout << "Rbuf[33]: " << hex << (int)receive_buffer[33] << dec << endl;
    cout << "Rbuf[0]: " << hex << (int)receive_buffer[0] << dec << endl;
    #endif

    } while (finish == false);

    libusb_close(dev_handle);
    libusb_exit(ctx);
}