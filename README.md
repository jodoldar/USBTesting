# USBTesting
USBTesting for the WeatherStation project.

In order to compile the file in MacOS, libusb is necessary. To install it, in a terminal, type:

`brew install libusb`

After that, to compile the file:

`g++ -o Test main.cpp -I /usr/local/Cellar/libusb/1.0.22/include/ -I /usr/local/Cellar/libusb/1.0.22/lib/ -lusb-1.0`

Once compiled, to execute the file simply run the executable `./Test`.
