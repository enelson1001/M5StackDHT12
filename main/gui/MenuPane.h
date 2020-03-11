/****************************************************************************************
 * MenuPane.h - A class that creates menu pane that contains menu buttons
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

#include <array>
#include <unordered_map>
#include <memory>               // for unique_ptr
#include <lvgl/lvgl.h>
#include "gui/IPane.h"
#include "gui/GuiButton.h"
#include "gui/HwPushButton.h"

namespace redstone
{
    class MenuPane : public IPane
    {
        public:
            // Constants & Enums
            enum BtnID : int
            {
                LeftButtonID = 0,
                MiddleButtonID = 1,
                RightButtonID = 2
            };

            static int constexpr ButtonQtyMax = 3;
            static int constexpr NoButtonPressed = -1;

            /// Constructor
            MenuPane();

            /// Initialize the LV Input Device Driver
            void initialize();

            /// Create the Menu Pane
            /// \param menu_btns An object that contains the gui menu buttons used in menu pane
            /// \param width The width of the menu pane
            /// \param height The height of the menu pane
            //void create(lv_obj_t* parent, int width, int height);
            void create(int width, int height) override;

            /// Show the menu pane
            void show() override;

            /// Hide the menu pane
            void hide() override;

            /// Add a hw button
            /// \param btn_id The menu button id
            /// \param gui_btn The gui button part that makes up a menu button
            /// \param hw_btn The hardware button part that makes up a menu button
            void add_menu_button(BtnID btn_id, std::unique_ptr<GuiButton> gui_btn,
                                 std::unique_ptr<HwPushButton> hw_btn);

            /// Add screen location of a gui button to array
            /// \param btn_id The id of the button
            /// \param btn_scr_coords The buttons screen coordinates
            void add_screen_location_of_button(int btn_id, lv_area_t btn_scr_coords);

        private:

            /// Static "C" style callback required by the Lvgl input driver
            /// \param input_device_driver The lvgl input device driver
            /// \param data The data from the input driver
            static bool button_read_cb(lv_indev_drv_t* input_device_driver, lv_indev_data_t* data);

            /// Read the hardware buttons to see if one is pressed
            int read_hardware_buttons();

            /// Get the middle point between to points
            /// \param start_point The starting point
            /// \param end_point The ending point
            /// \return Return the middle between the start point and end poit
            lv_coord_t get_mid_point(lv_coord_t start_point, lv_coord_t end_point);

            lv_indev_drv_t input_device_driver;
            lv_indev_t* input_device_button;
            lv_style_t menu_style;
            lv_obj_t* menu_pane_container;

            std::unordered_map<int, std::unique_ptr<HwPushButton>> hw_buttons;
            std::unordered_map<int, std::unique_ptr<GuiButton>> gui_buttons;
            std::array<lv_point_t, ButtonQtyMax> screen_locations_of_buttons;
    };
}
