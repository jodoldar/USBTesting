#include "data_decoder.h"
#include <list>

float decode_pressure (unsigned char* raw_data)
{
    return 0.0;
}

std::list<float> decode_temperature (unsigned char* raw_data)
{
    return std::list<float>(3,0.0);
}

std::list<float> decode_humidity (unsigned char* raw_data)
{
    return std::list<float>(3,0.0);
}

float decode_wind_chill (unsigned char* raw_data)
{
    return 0.0;
}

float decode_wind_gust (unsigned char* raw_data)
{
    return 0.0;
}

float decode_wind_speed (unsigned char* raw_data)
{
    return 0.0;
}

int decode_wind_dir (unsigned char* raw_data)
{
    return 0;
}