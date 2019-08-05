#include <list>

float decode_pressure (unsigned char* raw_data);
std::list<float> decode_temperature (unsigned char* raw_data);
std::list<float> decode_humidity (unsigned char* raw_data);
float decode_wind_chill (unsigned char* raw_data);
float decode_wind_gust (unsigned char* raw_data);
float decode_wind_speed (unsigned char* raw_data);
float decode_wind_dir (unsigned char* raw_data);

int bcd2int(char bcd);