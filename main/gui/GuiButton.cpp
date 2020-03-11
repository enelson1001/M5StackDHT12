/****************************************************************************************
 * GuiButton.h - A base class that GUI buttons implement
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
}
