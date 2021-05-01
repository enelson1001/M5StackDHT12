/****************************************************************************************
 * DataStore.cpp -
 *
 * Created on Feb. 15, 2021
 * Copyright (c) 2021 Ed Nelson (https://github.com/enelson1001)
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
#include <fstream>
#include <iostream>
#include <memory>                   // for unique_ptr
#include <smooth/core/io/spi/Master.h>
#include <smooth/core/logging/log.h>
#include "storage/DataStore.h"

using namespace smooth::core;
using namespace smooth::core::logging;
using namespace smooth::core::io::spi;
using namespace smooth::core::filesystem;

namespace redstone
{
    // Class constants
    static const char* TAG = "DataStore";

    // Constructor
    DataStore::DataStore()
    {
    }

    // Initialize the DataStore
    void DataStore::init()
    {
        Log::info(TAG, "Initializing DataStore");
        sd_card = std::make_unique<smooth::core::filesystem::SPISDCard>(VSPI_HOST,
                                                                        SPI_DMA_Channel::DMA_1,     // use DMA
                                                                        GPIO_NUM_19,                // miso gpio pin
                                                                        GPIO_NUM_23,                // mosi gpio pin
                                                                        GPIO_NUM_18,                // clock gpio pin
                                                                        GPIO_NUM_4);                // chip select

        // initialize sdcard do not format if sdcard cannot be mounted
        card_initialized = sd_card->init(SDCardMount::instance(), false, 5);
    }

    void DataStore::deinitialize()
    {
        sd_card->deinit();
    }

    // Read the nth line from file on DataStore
    std::string DataStore::read_nth_line(const std::string filename, int line_number)
    {
        std::string line;

        // Smmoth standard mount point is /sdcard so path will be /sdcard/filename
        auto path = SDCardMount::instance().mount_point() / filename;

        std::ifstream file_in{ path.str(), std::ios::binary | std::ios::in };

        if (file_in.good())
        {
            //skip N lines
            for (int i = 0; i < line_number; ++i)
            {
                std::getline(file_in, line);
            }

            std::getline(file_in, line);

            // if text file was created on Windows we need to remove the carriage return character that is at the end of
            // the line
            if (line.size() && line[line.size() - 1] == '\r')
            {
                line = line.substr( 0, line.size() - 1 );
            }
        }

        return line;
    }
}
