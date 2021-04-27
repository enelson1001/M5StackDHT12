/****************************************************************************************
 * DataStore.h -
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
#pragma once

#include <smooth/core/filesystem/SPISDCard.h>

namespace redstone
{
    class DataStore
    {
        public:
            /// Constructor
            DataStore();

            /// initialize DataStore
            void init();

            /// deinitialize Datastore
            void deinitialize();

            /// Reads a string from a particular line in the file
            /// \param filename The name of the file to read the string from
            /// \param line_numer The line number in the file to read the string
            /// \return Return the string from the nth line in the file
            std::string read_nth_line(std::string filename, int line_number);

        private:
            std::unique_ptr<smooth::core::filesystem::SDCard> sd_card{};
            bool card_initialized{ false };
    };
}
