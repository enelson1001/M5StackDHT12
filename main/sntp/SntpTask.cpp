/****************************************************************************************
 * SntpTask.cpp - A task that runs periodically to update the local time system
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
#include <chrono>
#include <ctime>
#include <smooth/core/network/Wifi.h>
#include <smooth/core/ipc/Publisher.h>

#include "sntp/SntpTask.h"
#include "sntp/WifiSsidPasswd.h"

using namespace std::chrono;
using namespace smooth::core;
using namespace smooth::core::ipc;

namespace redstone
{
    // Class constants
    static const char* TAG = "SntpTask";

    // Constructor
    SntpTask::SntpTask(Application& app) :
            smooth::core::Task("SntpTask", 3200, 10, seconds(1)),

            // The Task Name = "SntpTask"
            // The stack size is 3200 bytes
            // The priority is set to 10
            // The tick interval is 1 sec
            app(app),
            sntp(std::vector<std::string>{ "0.us.pool.ntp.org", "1.us.pool.ntp.org" }),
            sync_queue(TimeSyncQueue::create(2, *this, *this))
    {
    }

    // Initialize the Sntp Task
    void SntpTask::init()
    {
        Log::info(TAG, "Initializing Sntp Task");
        setenv("TZ", "MST7MDT6,M3.2.0/2,M11.1.0", 1);
        tzset();

        sntp.start();
        Log::info(TAG, "Starting wifi...");

        network::Wifi& wifi = app.get_wifi();
        wifi.set_host_name("M5Stack-DHT12");
        wifi.set_auto_connect(true);
        wifi.set_ap_credentials(WIFI_SSID, WIFI_PASSWORD);
        wifi.connect_to_ap();
    }

    // The task tick event happens every 1 seconds
    void SntpTask::tick()
    {
        if (has_synced)
        {
            auto t = system_clock::to_time_t(system_clock::now());
            tm time{};
            localtime_r(&t, &time);

            std::strftime(char_buf.data(), char_buf.size(), "%I:%M %p", &time);
            std::string time_string(char_buf.data(), char_buf.size());
            time_value.set_time(time_string);

            std::strftime(char_buf.data(), char_buf.size(), "%a, %d %b %Y", &time);
            std::string date_string(char_buf.data(), char_buf.size());
            time_value.set_date(date_string);

            std::strftime(char_buf.data(), char_buf.size(), "%H", &time);
            std::string hour_string(char_buf.data(), char_buf.size());
            time_value.set_hour(hour_string);

            std::strftime(char_buf.data(), char_buf.size(), "%M", &time);
            std::string minute_string(char_buf.data(), char_buf.size());
            time_value.set_minute(minute_string);

            std::strftime(char_buf.data(), char_buf.size(), "%S", &time);
            std::string second_string(char_buf.data(), char_buf.size());
            time_value.set_second(second_string);

            Publisher<TimeValue>::publish(time_value);
        }
    }

    void SntpTask::event(const smooth::core::sntp::TimeSyncEvent& ev)
    {
        // update local time
        auto t = system_clock::to_time_t(ev.get_timePoint());
        tm time{};
        localtime_r(&t, &time);
        has_synced = true;
    }
}
