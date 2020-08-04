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

#include "Observation.h"

/* Functions definition */
int obtain_current_uv_index();
int write_into_DB(Observation obs);