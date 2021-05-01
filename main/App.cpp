/****************************************************************************************
 * App.cpp - The Application class
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

//*****************************************************************************************************************
// Typical output on M5Stack
//
//  MemStat: Mem type |  8-bit free | Smallest block | Minimum free | 32-bit free | Smallest block | Minimum free
//  MemStat: INTERNAL |       68588 |          32768 |        12728 |       96680 |          32768 |        40808
//  MemStat:      DMA |       68596 |          32768 |        12728 |       68596 |          32768 |        12728
//  MemStat:   SPIRAM |           0 |              0 |            0 |           0 |              0 |            0
//  MemStat:
//  MemStat:             Name |      Stack |  Min free stack |  Max used stack
//  MemStat:        Dht12Task |       3300 |             744 |            2556
//  MemStat:         MainTask |      16384 |           12060 |            4324
//  MemStat:         SntpTask |       3200 |             924 |            2276
//  MemStat: SocketDispatcher |      20480 |           17900 |            2580
//  MemStat:         LvglTask |       4096 |             356 |            3740
//
// Esp32-IDF version: v4.3-beta3 - commit e9cf9e2 - April 14, 2021
// Toolchain version: esp-2020r3-8.4.0/xtensa-esp32-elf
// Lvgl version:  v7.11.0 - commit: ec9de51, March, 2021
// Smooth version: master - commit: 5578b8b, April 15, 2021
// Bin file size: 1,516,992  bytes
//******************************************************************************************************************
#include "App.h"
#include <chrono>
#include <thread>
#include <smooth/core/task_priorities.h>
#include <smooth/core/logging/log.h>
#include <smooth/core/SystemStatistics.h>

using namespace smooth::core;
using namespace std::chrono;
using namespace smooth::core::io::spi;
using namespace smooth::core::filesystem;
using namespace smooth::application::sensor;

namespace redstone
{
    // Class Constants
    static const char* TAG = "APP";

    // Constructor
    App::App() : Application(APPLICATION_BASE_PRIO, seconds(20)),
            wifi(*this),

            sntp_task(*this)
    {
    }

    // Initialize the application
    void App::init()
    {
        Log::warning(TAG, "============ Starting APP  ===========");
        Application::init();
        lvgl_task.start();

        // allow lvgl task time to initialize the spi bus
        std::this_thread::sleep_for(seconds{ 2 });

        // initialize sdcard
        data_store.init();

        // allow sdcard to be initialized
        std::this_thread::sleep_for(seconds{ 3 });
        wifi.set_wifi_cred(data_store.read_nth_line("wifi_cred.txt", 0), data_store.read_nth_line("wifi_cred.txt", 1));
        wifi.start_wifi();
        dht12_task.start();
        sntp_task.start();

        // allow time for wifi to connect
        std::this_thread::sleep_for(seconds{ 10 });
        wifi.show_network_info();
        wifi.show_local_mac_address();
        wifi.show_wifi_information();
    }

    // Tick event happens every 60 seconds
    void App::tick()
    {
        Log::warning(TAG, "============ M5StackDHT12 TICK  =============");

        if (!heap_caps_check_integrity_all(true))
        {
            Log::error(TAG, "========= Heap Corrupted  ===========");
        }

        SystemStatistics::instance().dump();
    }
}
