/****************************************************************************************
 * Wifi.h - The Wifi class
 *
 * Created on March 01, 2021
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
#include <smooth/core/Application.h>
#include <smooth/core/network/Wifi.h>

namespace redstone
{
    class Wifi
    {
        public:
            /// Constructor
            Wifi(smooth::core::Application& app);

            /// Start the wifi
            void start_wifi();

            /// Show network information
            void show_network_info();

            /// Show wifi information
            void show_wifi_information();

            /// show rssi signal strength
            void show_wifi_rssi();

            /// show local mac address
            void show_local_mac_address();

            /// Set the wifi credentials
            /// \param ssid The SSID name
            /// \param password The password used to connecto the AP
            void set_wifi_cred(const std::string& ssid, const std::string& password);

        private:
            smooth::core::Application& app;
            std::string ssid{};
            std::string password{};
    };
}
