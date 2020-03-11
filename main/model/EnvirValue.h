/****************************************************************************************
 * EnvirValue.h - This class instance is published and received by subscribers
 * 
 * Created on Jan. 04, 2020
 * Copyright (c) 2019 Ed Nelson (https://github.com/enelson1001)
 * Licensed under MIT License (see LICENSE file)
 *
 * Derivative Works
 * Smooth - A C++ framework for embedded programming on top of Espressif's ESP-IDF
 * Copyright 2019 Per Malmberg (https://gitbub.com/PerMalmberg)
 * Licensed under the Apache License, Version 2.0 (the "License");
 *
 * LittlevGL - A powerful and easy-to-use embedded GUI
 * Copyright (c) 2016 Gábor Kiss-Vámosi (https://github.com/littlevgl/lvgl)
 * Licensed under MIT License
 ***************************************************************************************/
#pragma once

#include <string>
#include <math.h>

namespace redstone
{
    class EnvirValue
    {
        public:
            EnvirValue() {}

            /// Get the temperature in degree fahrenheit
            /// \param return Return the temperature in degree farenheit
            float get_temperture_degree_F() const
            {
                return ((temperature * 9) / 5) + 32;
            }

            /// Get the temperature in degree celsius
            /// \param return Return the temperature in degree celsius
            float get_temperature_degree_C() const
            {
                return temperature;
            }

            /// Set the temperature in degree celsius
            /// \param value The temperature in degree celsius
            void set_temperture_degree_C(float value)
            {
                temperature = value;
            }

            /// Get the relative humidity
            /// \param return Return the realtive humidity
            float get_relative_humidity() const
            {
                return humidity;
            }

            /// Set the relative humidity
            /// \param value The relative humidity
            void set_relative_humidity(float value)
            {
                humidity = value;
            }

            /// Get the heat index fahrenheit
            /// \param return Return the heat index in fahrenheit
            float get_heat_index_fahrenheit() const
            {
                // Using both Rothfusz and Steadman's equations
	            // http://www.wpc.ncep.noaa.gov/html/heatindex_equation.shtml

                float heat_index;
                float temp_deg_f = get_temperture_degree_F();

                heat_index = 0.5 * (temp_deg_f + 61.0 +((temp_deg_f -68) * 1.2) + (humidity * 0.094));

                if (heat_index > 79)
                {
                    heat_index = -42.379 + 2.04901523 * temp_deg_f + 10.14333127 * humidity
				                 + -0.22475541 * temp_deg_f * humidity
				                 + -0.00683783 * pow(temp_deg_f, 2)
				                 + -0.05481717 * pow(humidity, 2)
				                 + 0.00122874 * pow(temp_deg_f, 2) * humidity
				                 + 0.00085282 * temp_deg_f * pow(humidity, 2)
				                 + -0.00000199 * pow(temp_deg_f, 2) * pow(humidity, 2);

                    if (humidity<13 && temp_deg_f>=80.0 && temp_deg_f<=112.0)
                    {
                        heat_index -= ((13.0 - humidity) * 0.25) * sqrt((17.0 - abs(temp_deg_f - 95.0)) * 0.05882);
                    }

                    if (humidity>85.0 && temp_deg_f>=80.0 && temp_deg_f<=87.0)
                    {
                        heat_index += ((humidity - 85.0) * 0.1) * ((87.0 - temp_deg_f) * 0.2);
                    }
                }

                return heat_index;
            }

            /// Get the heat index celsius
            /// \param return Return the heat index in celsius
            float get_heat_index_celsius() const
            {
                return (get_heat_index_fahrenheit() -32) * 0.55555;
            }

            /// Get the dew point value in celsius
            /// \return Retuen the dew point value in celsius
            float get_dew_point_celsius() const
            {
                // dewPoint function NOAA
                // reference (1) : http://wahiduddin.net/calc/density_algorithms.htm
                // reference (2) : http://www.colorado.edu/geography/weather_station/Geog_site/about.htm
                // Good approximation for 0 ... +70 °C with max. deviation less than 0.25 °C

                float dew_point = temperature - (14.55 + 0.114 * temperature) * (1 - (0.01 * humidity)) 
                                  - pow(((2.5 + 0.007 * temperature) * (1 - (0.01 * humidity))),3) 
                                  - (15.9 + 0.117 * temperature) * pow((1 - (0.01 * humidity)), 14);

                return dew_point;
            }

            /// Get the dew point value in fahrenheit
            /// \return Retuen the dew point value in fahrenheit
            float get_dew_point_fahrenheit() const
            {
                return get_dew_point_celsius() * 1.8 + 32;
            }

        private:
            float temperature;
            float humidity;
    };
}
