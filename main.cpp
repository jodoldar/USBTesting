#include <iostream>
#include <libusb-1.0/libusb.h>

using namespace std;

void printdev(libusb_device *dev); // Function prototype

int main() {
    libusb_device **devs;
    libusb_device_handle *dev_handle;
    libusb_context *ctx;

    int retValue;
    int iInterface;
    ssize_t counter;

    retValue = libusb_init(&ctx);
    if (retValue < 0) {
        cout << "Error initializing libusb" << endl;
        return 1;
    }

    //libusb_set_debug(ctx, 3); Deprecated

    counter = libusb_get_device_list(ctx, &devs);
    if (counter < 0) {
        cout << "Error listing the devices" << endl;
        return 2;
    }
    cout << "There are " << counter << " devices in the list" << endl;

    /*for (int i = 0; i < counter; i++) {
        printdev(devs[i]);
    }*/

    dev_handle = libusb_open_device_with_vid_pid(ctx, 4400, 26625);
    if (dev_handle == NULL) {
        cout << "Cannot open device" << endl;
    } else {
        cout << "Device opened" << endl;
    }

    libusb_free_device_list(devs, 1);

    if (libusb_kernel_driver_active(dev_handle, 0) == 1) {
        cout << "Kernel Driver Active" << endl;
        if (libusb_detach_kernel_driver(dev_handle, 0) == 0) {
            cout << "Kernel Driver Detached" << endl;
        }
    }

    libusb_detach_kernel_driver(dev_handle, 0);
    libusb_set_configuration(dev_handle, 1);

    iInterface = libusb_claim_interface(dev_handle, 0);
    if (iInterface < 0) {
        cout << "Cannot Claim Interface (" << libusb_strerror(libusb_error(iInterface)) << ")" << endl;
        return 3;
    }
    cout << "Interface claimed" << endl;

    libusb_close(dev_handle);
    libusb_exit(ctx);

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