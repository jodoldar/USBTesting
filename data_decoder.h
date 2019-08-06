/**
 * @file data_decoder.h
 * @author Josep Dols (jodoldar@gmail.com)
 * @brief Library to decode the information from the USB
 * @version 1.0.0
 * @date 2019-08-06
 * 
 * @copyright Copyright (c) 2019
 * 
 * Library with the necessary functions to decode the information retreived
 * from the USB device into a readable and usable format.
 */

#include <list>

float decode_pressure (unsigned char* raw_data);
std::list<float> decode_temperature (unsigned char* raw_data);
std::list<float> decode_humidity (unsigned char* raw_data);
float decode_wind_chill (unsigned char* raw_data);
float decode_wind_gust (unsigned char* raw_data);
float decode_wind_speed (unsigned char* raw_data);
float decode_wind_dir (unsigned char* raw_data);

int bcd2int(char bcd);