#include "data_decoder.h"

#include <cstring>
#include <iostream>
#include <list>

using namespace std;

float decode_pressure (unsigned char* raw_data)
{
    float f_press;

    #ifdef DEBUG
    cout << "[DEBUG] PRS BUF[20]=" << hex << static_cast<int>(raw_data[20]) << " BUF[21]=" << static_cast<int>(raw_data[21]) << dec << endl;
    #endif
    if (( raw_data[21] & 0xF0 ) == 0xF0 ) {
        cerr << "[DEBUG] PRS = 0.0 (No data available)" << endl;
        return 0.0;
    } else {
        f_press = static_cast<int>(raw_data[21] * 0x100 + raw_data[20]) * 0.0625;
        #ifdef DEBUG
        cout << "[DEBUG] PRS = " << f_press << endl;
        #endif
        return f_press;
    }
}

std::list<float> decode_temperature (unsigned char* raw_data)
{
    std::list<float> lcl_temps(0);
    int data_err = 0;
    float tmp_data = 0.0;

    for (int i = 0; i <= 2; i++) {
		int offset = i * 3;
        data_err = 0;
        tmp_data = 0.0;

        #ifdef DEBUG
        cout << "[DEBUG] TMP " << i << " BUF[" << 0 + offset << "]=" << hex << raw_data[offset] << " BUF[" << dec << 1 + offset << "]=" << hex << raw_data[1 + offset] << " BUF[" << dec << 2 + offset << "]=" << hex << raw_data[2 + offset] << endl;
        #endif
		if (bcd2int(raw_data[offset] & 0x0F) > 9) {
            cerr << "[DEBUG] TMP buffer 0 & 0x0F > 9" << endl;
			if (((raw_data[offset] & 0x0F) == 0x0C) || ((raw_data[offset] & 0x0F) == 0x0B)) {
				cerr << "[DEBUG] TMP buffer 0 & 0x0F = (0x0C or 0x0B)" << endl;
                data_err = -2;
			} else {
				cerr << "[DEBUG] TMP other error in buffer 0" << endl;
                data_err = -1;
			}
		}
		if (((raw_data[1 + offset] & 0x40) != 0x40) && i > 0) {
			cerr << "[DEBUG] TMP buffer 1 bit 6 set" << endl;
            data_err = -2;
		}

		if (!data_err) {
			tmp_data = (bcd2int(raw_data[offset]) / 10.0) + (bcd2int(raw_data[1 + offset] & 0x0F) * 10.0);
			
			if ((raw_data[1 + offset] & 0x20) == 0x20)
				tmp_data += 0.05;
			if ((raw_data[1 + offset] & 0x80) != 0x80)
				tmp_data *= -1;

            #ifdef DEBUG
			cout << "[DEBUG] TMP " << i << " is " << tmp_data << endl;
            #endif
		}

        lcl_temps.push_back(tmp_data);
	}

    return lcl_temps;
}

std::list<float> decode_humidity (unsigned char* raw_data)
{
    std::list<float> lcl_hums(0);
    int data_err = 0;
    float tmp_humid = 0.0;

    for (int i = 0; i <= 2; i++) {
		int offset = i * 3;
        data_err = 0;
        tmp_humid = 0.0;

        if (bcd2int(raw_data[offset] & 0x0F) > 9) {
            cerr << "[DEBUG] TMP buffer 0 & 0x0F > 9" << endl;
            if (((raw_data[offset] & 0x0F) == 0x0C) || ((raw_data[offset] & 0x0F) == 0x0B)) {
                cerr << "[DEBUG] TMP buffer 0 & 0x0F = (0x0C or 0x0B)" << endl;
                data_err = -2;
            } else {
                cerr << "[DEBUG] TMP other error in buffer 0" << endl;
                data_err = -1;
            }
        }
        if (((raw_data[1 + offset] & 0x40) != 0x40) && i > 0) {
            cerr << "[DEBUG] TMP buffer 1 bit 6 set" << endl;
            data_err = -2;
        }

        if (data_err <= -2) {
			cerr << "[DEBUG] HMY error. Data not valid" << endl;
		} else if (bcd2int(raw_data[2 + offset] & 0x0F) > 9) {
			cerr << "[DEBUG] HMY buffer 0 & 0x0F > 9" << endl;
		} else {
            tmp_humid = bcd2int(raw_data[2 + offset]);
			
            #ifdef DEBUG
			cout << "[DEBUG] HMY " << i << " is " << tmp_humid << endl;
            #endif
		}

        lcl_hums.push_back(tmp_humid);
    }

    return lcl_hums;
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

int bcd2int(char bcd) {
	return ((int)((bcd & 0xF0) >> 4) * 10 + (int)(bcd & 0x0F));
}