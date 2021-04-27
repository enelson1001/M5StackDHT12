/****************************************************************************************
 * MenuPane.h - A class that creates a menu pane that contains menu buttons
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
#include "gui/MenuPane.h"
#include <smooth/core/logging/log.h>

using namespace smooth::core::logging;

namespace redstone
{
    // Class constants
    static const char* TAG = "MenuPane";

    // Constructor
    MenuPane::MenuPane()
    {
    }

    // Initialize the lv input device driver - handles button debounce and causes
    // gui button to display the pressed style when the hardware button is pressed
    void MenuPane::initialize()
    {
        lv_indev_drv_init(&input_device_driver);
        input_device_driver.read_cb = button_read_cb;
        input_device_driver.type = LV_INDEV_TYPE_BUTTON;
        input_device_driver.user_data = this;
        input_device_button = lv_indev_drv_register(&input_device_driver);
        lv_indev_set_button_points(input_device_button, screen_locations_of_buttons.data());
    }

    // Create the Menu Pane
    void MenuPane::create(int width, int height)
    {
        Log::info(TAG, "Creating the Menu Pane");

        // create style for the menu pane container
        lv_style_init(&menu_style);
        lv_style_set_bg_color(&menu_style, LV_STATE_DEFAULT, lv_color_hex3(0x036));
        lv_style_set_border_width(&menu_style, LV_STATE_DEFAULT, 0);
        lv_style_set_radius(&menu_style, LV_STATE_DEFAULT, 0);

        // create a container to hold the menu buttons
        menu_pane_container = lv_cont_create(lv_scr_act(), NULL);
        lv_obj_set_size(menu_pane_container, width, height);
        lv_obj_set_size(menu_pane_container, 320, 40);
        lv_cont_set_layout(menu_pane_container, LV_LAYOUT_OFF);
        lv_obj_align(menu_pane_container, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
        lv_obj_add_style(menu_pane_container, LV_CONT_PART_MAIN, &menu_style);
        lv_obj_set_hidden(menu_pane_container, true);

        // create left button
        gui_buttons[LeftButtonID]->create(menu_pane_container);
        gui_buttons[LeftButtonID]->align(NULL, LV_ALIGN_CENTER, -95, +5);

        // create right button
        gui_buttons[RightButtonID]->create(menu_pane_container);
        gui_buttons[RightButtonID]->align(NULL, LV_ALIGN_CENTER, 95, +5);

        // add button locations on screen
        add_screen_location_of_button(LeftButtonID, gui_buttons[LeftButtonID]->get_coords());
        add_screen_location_of_button(RightButtonID, gui_buttons[RightButtonID]->get_coords());
    }

    // Add a menu button - A menu button consist of a gui button and a hardware button.
    // Pressing the hardware button cause the gui button to display being pressed and
    // create an on-clicked event when the pressed harware button is released.  The
    // debounce of the hardware button is handled in lv_indev_drv
    void MenuPane::add_menu_button(BtnID id, std::unique_ptr<GuiButton> gui_btn, std::unique_ptr<HwPushButton> hw_btn)
    {
        gui_buttons[id] = std::move(gui_btn);
        hw_buttons[id] = std::move(hw_btn);
    }

    // Add screen location of a gui button to screen_locations_of_buttons array used by Lvgl input driver
    void MenuPane::add_screen_location_of_button(int btn_id, lv_area_t btn_scr_coords)
    {
        lv_point_t scr_loc_of_btn = { get_mid_point(btn_scr_coords.x1, btn_scr_coords.x2),
                                      get_mid_point(btn_scr_coords.y1, btn_scr_coords.y2) };

        screen_locations_of_buttons[btn_id] = scr_loc_of_btn;
    }

    // Get the midpoint between to coordinates
    lv_coord_t MenuPane::get_mid_point(lv_coord_t start_point, lv_coord_t end_point)
    {
        return (end_point - start_point) / 2 + start_point;
    }

    // Read the hardware buttons and get id of button if pressed, used by Lvgl
    int MenuPane::read_hardware_buttons()
    {
        int btn_id = NoButtonPressed;

        for (auto const& x:hw_buttons)
        {
            if (x.second->is_button_pressed())
            {
                btn_id = x.first;
                break;
            }
        }

        return btn_id;
    }

    // Button read callback for the Lvgl input driver
    bool IRAM_ATTR MenuPane::button_read_cb(lv_indev_drv_t* input_device_driver, lv_indev_data_t* data)
    {
        uint8_t last_button = 0;
        MenuPane* driver = reinterpret_cast<MenuPane*>(input_device_driver->user_data);
        int button_pressed = driver->read_hardware_buttons();

        if (button_pressed >= 0)
        {
            data->state = LV_INDEV_STATE_PR;
            last_button = button_pressed;
        }
        else
        {
            data->state = LV_INDEV_STATE_REL;
        }

        // Save the last pressed button's ID
        data->btn_id = last_button;

        // Return `false` because we are not buffering and no more data to read
        return false;
    }

    // Show the menu pane
    void MenuPane::show()
    {
        lv_obj_set_hidden(menu_pane_container, false);
    }

    // Hide the menu pane
    void MenuPane::hide()
    {
        lv_obj_set_hidden(menu_pane_container, true);
    }
}
