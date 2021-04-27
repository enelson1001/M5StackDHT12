/****************************************************************************************
 * Wifi.cpp - A class that is used to connect to a wifi network
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
#include "Wifi.h"
#include <smooth/core/network/Wifi.h>
#include <smooth/core/logging/log.h>

using namespace smooth::core::logging;

namespace redstone
{
    // Class Constants
    static const char* TAG = "Wifi";

    // Constructor
    Wifi::Wifi(smooth::core::Application& app) : app(app)
    {
    }

    // Startup wifi
    void Wifi::start_wifi()
    {
        Log::info(TAG, "Starting wifi...");
        smooth::core::network::Wifi& wifi = app.get_wifi();
        wifi.set_host_name("M5Stack-DHT12");
        wifi.set_auto_connect(true);
        wifi.set_ap_credentials(ssid, password);
        wifi.connect_to_ap();
    }

    // Show network information
    void Wifi::show_network_info()
    {
        Log::error(TAG, "My IP = {}", smooth::core::network::Wifi::get_local_ip_address());
        Log::error(TAG, "My Netmask = {}", smooth::core::network::Wifi::get_netmask());
        Log::error(TAG, "My Gateway = {}", smooth::core::network::Wifi::get_gateway());
    }

    // Show wifi information
    void Wifi::show_wifi_information()
    {
        wifi_ap_record_t wifi_data;

        if (esp_wifi_sta_get_ap_info(&wifi_data) == ESP_OK)
        {
            Log::error(TAG, "RSSI = {}", wifi_data.rssi);
            Log::error(TAG, "SSID = {}", wifi_data.ssid);
            Log::error(TAG, "AP Channel = {}", wifi_data.primary);
        }
    }

    // Show RSSI signal strength
    void Wifi::show_wifi_rssi()
    {
        wifi_ap_record_t wifi_data;

        if (esp_wifi_sta_get_ap_info(&wifi_data) == ESP_OK)
        {
            Log::error(TAG, "RSSI = {}", wifi_data.rssi);
            Log::error(TAG, "AP Channel = {}", wifi_data.primary);
        }
    }

    // Show local MAC address
    void Wifi::show_local_mac_address()
    {
        constexpr const char* mac_format = "{:02X}:{:02X}:{:02X}:{:02X}:{:02X}:{:02X}";
        std::string mac_str{};
        std::array<uint8_t, 6> mac{};

        if (smooth::core::network::Wifi::get_local_mac_address(mac))
        {
            mac_str = fmt::format(mac_format, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
            Log::error(TAG, "My MAC = {}", mac_str);
        }
        else
        {
            Log::error(TAG, "Could not get local MAC");
        }
    }

    // Set WIFI credentials
    void Wifi::set_wifi_cred(const std::string& my_ssid, const std::string& my_password)
    {
        ssid = my_ssid;
        password = my_password;
    }
}
