/****************************************************************************************
 * TimeValue.h - This class instance is published and received by subscribers
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

namespace redstone
{
    class TimeValue
    {
        public:
            TimeValue() {}

            /// Get time string
            /// \param return Return the time in a string
            std::string get_time() const
            {
                return str_time;
            }

            /// Set the time
            /// \param time_value A string time value
            void set_time(std::string time_value)
            {
                str_time = time_value;
            }

            /// Get the date
            /// \param return Return the date in a string
            std::string get_date() const
            {
                return str_date;
            }

            /// Set the date
            /// \param date_value A string date value
            void set_date(std::string date_value)
            {
                str_date = date_value;
            }

            /// Get the hour
            /// \param return Return a value of the hour.  The hour value is 0-23
            int16_t get_hour() const
            {
                return hour;
            }

            /// Set the hour
            /// \param str_hour A string hour value in range of 0-23
            void set_hour(std::string str_hour)
            {
                hour = std::stoi(str_hour);
            }

            /// Get the minute
            /// \param return Return the value of the minute.
            int16_t get_minute() const
            {
                return minute;
            }

            /// Set the minute
            /// \param str_minute A string minute value in range of 0-59
            void set_minute(std::string str_minute)
            {
                minute = std::stoi(str_minute);
            }

            /// Get the second
            /// \param return Return the value of the minute.
            int16_t get_second() const
            {
                return minute;
            }

            /// Set the second
            /// \param str_second A string minute value in range of 0-59
            void set_second(std::string str_second)
            {
                second = std::stoi(str_second);
            }

        private:
            std::string str_time;
            std::string str_date;
            int16_t hour;
            int16_t minute;
            int16_t second;
    };
}
