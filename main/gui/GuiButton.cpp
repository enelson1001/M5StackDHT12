/****************************************************************************************
 * GuiButton.h - A base class that GUI buttons implement
 *
 * Created on Jan. 04, 2020
 * Modified on March 02, 2021 - Updated to lvgl to v7.10
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
#include "gui/GuiButton.h"

namespace redstone
{
    // Constructor
    GuiButton::GuiButton()
    {
    }

    // The button callback for events
    void GuiButton::button_event_cb(lv_obj_t* obj, lv_event_t event)
    {
        GuiButton* the_user = reinterpret_cast<GuiButton*>(obj->user_data);

        if (event == LV_EVENT_CLICKED)
        {
            the_user->on_clicked(obj);
        }
    }

    // create the button
    lv_obj_t* GuiButton::create_btn(lv_obj_t* parent)
    {
        gui_button = lv_btn_create(parent, NULL);
        lv_obj_set_event_cb(gui_button, button_event_cb);

        return gui_button;
    }

    // Get the pointer to the gui button
    lv_obj_t* GuiButton::get_gui_btn_ptr()
    {
        return gui_button;
    }

    // Align the button
    void GuiButton::align(const lv_obj_t* base, lv_align_t align, lv_coord_t x_mod, lv_coord_t y_mod)
    {
        lv_obj_align(gui_button, base, align, x_mod, y_mod);
    }

    // Get the button's screen coordinates
    lv_area_t GuiButton::get_coords()
    {
        return gui_button->coords;
    }

    // Create button style
    void GuiButton::create_button_style()
    {
        // create style for button
        lv_style_init(&gui_btn_style);
        lv_style_set_bg_color(&gui_btn_style, LV_STATE_DEFAULT, lv_color_hex3(0x006));
        lv_style_set_bg_opa(&gui_btn_style, LV_STATE_DEFAULT, LV_OPA_COVER);
        lv_style_set_border_width(&gui_btn_style, LV_STATE_DEFAULT, 0);
        lv_style_set_radius(&gui_btn_style, LV_STATE_DEFAULT, 5);
        lv_style_set_text_color(&gui_btn_style, LV_STATE_DEFAULT, LV_COLOR_WHITE);
        lv_style_set_text_font(&gui_btn_style, LV_STATE_DEFAULT, &lv_font_montserrat_12);

        // use different color for button pressed state
        lv_style_set_bg_color(&gui_btn_style, LV_STATE_PRESSED, lv_color_hex3(0x33f));
    }
}
