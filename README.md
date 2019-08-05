# USBTesting
USBTesting for the WeatherStation project.

In order to compile the file in MacOS, libusb is necessary. As with the original project, to run the executable and obtain results it's necessary also a TE923 compatible device.

To install it, in a terminal, type:

`brew install libusb`

After that, to compile the file in MacOs:

`g++ -o Test main.cpp -I /usr/local/Cellar/libusb/1.0.22/include/ -I /usr/local/Cellar/libusb/1.0.22/lib/ -lusb-1.0`

in Linux, just:

`g++ -o Test main.cpp -lusb-1.0 -std=c++11`

Once compiled, to execute the file simply run the executable `./Test`.
