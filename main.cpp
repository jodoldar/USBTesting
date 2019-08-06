/**
 * @file main.cpp
 * @author Josep Dols (jodoldar@gmail.com)
 * @brief Main file of the Weather Station v3 project
 * @version 1.0.1
 * @date 2019-08-05
 * 
 * @copyright Copyright (c) 2019
 * 
 * Central file of the Weather Station v3 project. From the main() function,
 * the data retrieval process is called. After that, a new Observation object
 * is created and filled with all the information available.
 * 
 * TODO:
 *  - Daemonize the execution in order to keep the program executed while the
 *      Raspberry Pi is on.
 *  - Integrate the WeatherUndergroun API call from the previous Weather Station
 *      v2 project.
 *  - Implement the DB call in order to save the retrieved data.
 */

#include <iostream>
#include <chrono>
#include <thread>
#include <cstring>
#include <libusb-1.0/libusb.h>

#include "data_decoder.h"
#include "main.h"
#include "network_utils.hpp"
#include "Observation.h"

using namespace std;

/**
 * @brief Main function from where the different calls to the modules are done
 * and coordinated to obtain the data, process it, and send it to the outside.
 * 
 * @return int Result of the program execution
 */
int main() {
    /* Variables declaration */
    unsigned char receive_buffer[BUFLEN];
    Observation current_obs;
    int aux_counter = 0;
    int iterCounter = 0;
    short int retValue = 0;
    bool keepRecording = true;
    int uv_index = 0;

    while (keepRecording)
    {
        if ((iterCounter % 120) == 0)
        {
            /* Call to the UV API to get current value */
            uv_index = obtain_current_uv_index();
            if (uv_index < 0)
                uv_index = 0;
        }
        /* Variables initialization */
        current_obs = Observation();

        /* Obtain the data from the USB device */
        retValue = obtain_usb_data((unsigned char*)&receive_buffer);
        if (retValue < 0)
        {
            this_thread::sleep_for(chrono::seconds(30));
            iterCounter++;
            continue;
        }

        /* Process & decode the pressure value */
        current_obs.setPressure(decode_pressure(receive_buffer));
        cout << "Current PRESSURE is " << current_obs.getPressure() << endl;

        /* Process & decode the temperature values */
        aux_counter = 0;
        for (auto it : decode_temperature(receive_buffer))
        {
            current_obs.setTemperature(it, aux_counter++);
            cout << "Current TEMPERATURE " << aux_counter-1 << " is " << current_obs.getTemperature(aux_counter-1) << endl;
        }

        /* Process & decode the humidity values */
        aux_counter = 0;
        for (auto it : decode_humidity(receive_buffer))
        {
            current_obs.setHumidity(it, aux_counter++);
            cout << "Current HUMIDITY " << aux_counter-1 << " is " << current_obs.getHumidity(aux_counter-1) << endl;
        }

        /* Process & decode the wind chill value */
        current_obs.setWindChill(decode_wind_chill(receive_buffer));
        cout << "Current WIND CHILL is " << current_obs.getWindChill() << endl;

        /* Process & decode the wind gust value */
        current_obs.setWindGust(decode_wind_gust(receive_buffer));
        cout << "Current WIND GUST is " << current_obs.getWindGust() << endl;

        /* Process & decode the wind speed value */
        current_obs.setWindSpeed(decode_wind_speed(receive_buffer));
        cout << "Current WIND SPEED is " << current_obs.getWindSpeed() << endl;

        /* Process & decode the wind direction value */
        current_obs.setWindDir(decode_wind_dir(receive_buffer));
        cout << "Current WIND DIR is " << current_obs.getWindDir() << endl;

        /* Calculate the dew point from the current observation */
        current_obs.calculateDewPoint();
        cout << "Calculated DEW POINT is " << current_obs.getDewPoint() << endl;

        /* Calculate the RealFeel© from the current observation */
        current_obs.calculateRealFeel(uv_index);
        cout << "Calculated RealFeel© is " << current_obs.getRealFeel() << endl;

        this_thread::sleep_for(chrono::seconds(30));
        iterCounter++;
    }

    cout << "End of the program" << endl;
    return 0;
}

/**
 * @brief From a USB device, it shows its information
 * 
 * Given a USB device, this function obtains all the available info from
 * it, and shows it via the standard console output. Apart from that, it
 * iterates over all the available interfaces of the device, showing also
 * their information in the console.
 * 
 * @param dev Device from where to obtain the information
 */
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

/**
 * @brief Function to obtain the info from the USB device
 * 
 * Through this function, via the libusb, a dump signal is sent to the specified
 * USB device. After that, the USB device returns a message with the information
 * related to the Weather Station, coded into an array of unsigned chars.
 * 
 * @param receive_buffer Buffer where the retreived information is stored. 
 * @return short int Result of the execution of the function.
 */
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
    int transfer_attempts;

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

    transfer_attempts = 0;

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

        transfer_attempts++;

    } while (finish == false && transfer_attempts < 10);

    libusb_close(dev_handle);
    libusb_exit(ctx);

    if (transfer_attempts == 10)
        return -1;
}