/****************************************************************************************
 * DisplayDriver.h - A LittlevGL Display Driver for ILI9341 - Resolution: 240×320
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

#include <lvgl/lvgl.h>
#include <smooth/application/display/ILI9341.h>
#include <smooth/application/display/DisplaySpi.h>
#include <smooth/core/io/spi/SpiDmaFixedBuffer.h>

namespace redstone
{
    class DisplayDriver
    {
        public:
            /// Constructor
            DisplayDriver();

            /// Initialize the Lvgl Display Driver
            bool initialize();

        private:
            /// The "C" style callback required by LittlevGL
            static void display_flush_cb(lv_disp_drv_t* drv, const lv_area_t* area, lv_color_t* color_map);

            /// Class function required by LittlevGL to flush the video display buffer (VDB)
            /// \param drv The Lvgl display driver
            /// \param area The area of the screen to flush the colors to
            /// \param color_map The pointer to the color_map (colors to flush to screen)
            void display_drv_flush(lv_disp_drv_t* drv, const lv_area_t* area, lv_color_t* color_map);

            /// Initialize the lcd display driver IC
            bool init_display();

            // Set the screen rotation
            void set_screen_rotation();

        private:
            // Note 1 - COLOR_SIZE is determined by the color depth chosen. LV_COLOR_DEPTH
            // is defined in LittlevGL lv_config.h file.
            //
            // Note 2 - LINES_TO_SEND defines the number of lines to send to the display on a
            // LittlevGL flush(). More lines means more memory used, but less overhead for
            // setting up and finishing transfers. Make sure LV_HOR_MAX is dividable by this.
            //
            // Note 3 - MAX_DMA_LEN is used to set the maximum transfer size in BYTES for a
            // spi transaction, it is also used to create buffer(s) required by LittlevGL
            // to draw screen content. LV_HOR_RES_MAX is defined in LittlevGL lv_config.h file
            static constexpr int COLOR_SIZE = LV_COLOR_DEPTH / 8;                               // See Note 1
            static constexpr int LINES_TO_SEND = 40;                                            // See Note 2
            static constexpr int MAX_DMA_LEN = (LV_HOR_RES_MAX * LINES_TO_SEND * COLOR_SIZE);   // See Note 3

            spi_host_device_t spi_host;
            smooth::core::io::spi::Master spi_master;

            std::unique_ptr<smooth::application::display::DisplaySpi> display{};
            bool display_initialized{ false };

            smooth::core::io::spi::SpiDmaFixedBuffer<uint8_t, MAX_DMA_LEN> video_display_buffer1{};
            lv_color1_t* vdb1;
            lv_disp_buf_t disp_buf;
            lv_disp_drv_t disp_drv;
    };
}
