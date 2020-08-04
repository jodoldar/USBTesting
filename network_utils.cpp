/**
 * @file network_utils.cpp
 * @author Josep Dols (jodoldar@gmail.com)
 * @brief Group of utilities related to network usage
 * @version 1.1.0
 * @date 2019-08-06
 * 
 * @copyright Copyright (c) 2019
 * 
 * Group of functions to be used related to those functions which require some
 * interaction with external APIs through the Internet.
 * 
 * For now, the expected usage is:
 *  - Obtain the current UV index given a position.
 *  - Send the observation data to the Weather Underground API
 */

#include <curl/curl.h>
#include <iostream>
#include <sstream>
#include <string>
#include "json.hpp"

#include "Observation.h"

using json = nlohmann::json;

/**
 * @brief Callback to the cURL HTTP request
 * 
 * @param contents Pointer to the content received
 * @param size Size of the data received
 * @param nmemb Number of bytes received
 * @param userp Pointer to the user buffer
 * @return size_t Size of the data received
 */
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

/**
 * @brief Function to obtain the current UV index
 * 
 * Using the cURL utilities via libcurl4, it makes a GET API request to
 * retrieve the lastest information available given a position. With that
 * information, it also comes some info about the sun position, and sunrise
 * or sunset, all of that in JSON format.
 * 
 * The API used is available at https://www.openuv.io/uvindex
 * 
 * @return int Current UV index retrieved
 */
int obtain_current_uv_index()
{
    CURL *curl_unit;
    struct curl_slist *list = NULL;
    std::string readBuffer;
    int callResult;

    curl_unit = curl_easy_init();

    if(curl_unit)
    {   
        curl_easy_setopt(curl_unit, CURLOPT_URL, "https://api.openuv.io/api/v1/uv?lat=39.4409&lng=-0.5953");
        list = curl_slist_append(list, "x-access-token: 3cd3fd48dbfab663bda353241ae70871");
        curl_easy_setopt(curl_unit, CURLOPT_HTTPHEADER, list);

        curl_easy_setopt(curl_unit, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl_unit, CURLOPT_WRITEDATA, &readBuffer);

        callResult = curl_easy_perform(curl_unit);
        std::cerr << "[DEBUG] Result of the call: " << callResult << std::endl;

        curl_slist_free_all(list); /* free the list again */
        curl_easy_cleanup(curl_unit);

        #ifdef DEBUG
            std::cout << readBuffer << std::endl;
        #endif

        json apiResult = json::parse(readBuffer);
        std::cerr << "Calculed UV value is " << apiResult["result"]["uv"] << std::endl;
        return static_cast<int>(apiResult["result"]["uv"]);
    }
    else
    {
        std::cerr << "cURL library could not be initialized." << std::endl;
        return -1;
    }
}


/**
 * @brief Function to register an observation into DB
 * 
 * Using the cURL utilities via libcurl4, it makes a POST API request to
 * store the values registered into a InfluxDB present in the same machine
 * with the comma-separated pairs of sensor=value
 * 
 * More info at https://docs.influxdata.com/influxdb/v1.8/guides/write_data/
 * 
 * @return int Status of the DB insertion
 */
int write_into_DB(Observation obs)
{
    CURL *curl_unit;
    struct curl_slist *list = NULL;
    std::stringstream ssBuffer;
    std::string readBuffer;
    int callResult;
    wchar_t outMsg[1024];

    curl_unit = curl_easy_init();
    ssBuffer.str("");
    memset(outMsg, '\0', 1024);

    if (curl_unit)
    {
        ssBuffer << "curl -i -XPOST \"http://localhost:8086/write?db=demo\" --data-binary \"observation temp=" << obs.getTemperature(1) << ",humid=" << obs.getHumidity(1) << "i,press=" << obs.getPressure() \
        << ",wind_dir=" << obs.getWindDir() << ",wind_spd=" << obs.getWindSpeed() << ",wind_gst=" << obs.getWindGust() << ",rfel=" \
        << obs.getRealFeel() << "\"";

        std::cerr << "[DEBUG] Insert: " << ssBuffer.str() << std::endl;
/*
        curl_easy_setopt(curl_unit, CURLOPT_URL, "http://localhost:8086/write?db=demo");
        list = curl_slist_append(list, "Content-type: charset=ASCII");
        curl_easy_setopt(curl_unit, CURLOPT_HTTPHEADER, list);
        curl_easy_setopt(curl_unit, CURLOPT_POSTFIELDS, outMsg);
        curl_easy_setopt(curl_unit, CURLOPT_POSTFIELDSIZE, strlen((char*)outMsg));

        curl_easy_setopt(curl_unit, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl_unit, CURLOPT_WRITEDATA, &readBuffer);

        callResult = curl_easy_perform(curl_unit);
        std::cerr << "[DEBUG] Result of the call: " << callResult << std::endl;
        std::cerr << "[DEBUG] Result: " << readBuffer << std::endl;

        long response_code;
        curl_easy_getinfo(curl_unit, CURLINFO_RESPONSE_CODE, &response_code);

        std::cerr << "[DEBUG] Code: " << response_code << std::endl;

        curl_slist_free_all(list);
        curl_easy_cleanup(curl_unit);*/

        system(ssBuffer.str().c_str());
    }
    else
    {
        std::cerr << "cURL library could not be initialized." << std::endl;
        return -1;
    }

    return callResult;
}
