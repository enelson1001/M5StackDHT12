/****************************************************************************************
 * TitlePane.cpp - A class that creates a simple title pane
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
#include "gui/TitlePane.h"
#include <smooth/core/logging/log.h>

using namespace smooth::core::logging;

namespace redstone
{
    // Class constants
    static const char* TAG = "TitlePane";

    // Constructor
    TitlePane::TitlePane(std::string title) : title(title)
    {
    }

    // Create the Title Pane
    void TitlePane::create(int width, int height)
    {
        Log::info(TAG, "Creating the Title Pane");

        // create style for the titlepain
        lv_style_copy(&title_style, &lv_style_plain);
        title_style.body.main_color = lv_color_hex3(0x036);
        title_style.body.grad_color = lv_color_hex3(0x036);
        title_style.text.font = &lv_font_roboto_22;
        title_style.text.color = LV_COLOR_WHITE;

        // create container for title pane
        title_container = lv_cont_create(lv_scr_act(), NULL);
        lv_obj_set_size(title_container, width, height);
        lv_cont_set_layout(title_container, LV_LAYOUT_CENTER);
        lv_obj_align(title_container, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
        lv_cont_set_style(title_container, LV_CONT_STYLE_MAIN, &title_style);
        lv_obj_set_hidden(title_container, true);

        // create a title label and place in title container
        title_label = lv_label_create(title_container, NULL);
        lv_obj_set_style(title_label, &title_style);
        lv_label_set_text(title_label, title.c_str());
        lv_obj_align(title_label, NULL, LV_ALIGN_CENTER, 0, 0);
    }

    // Show the title pane
    void TitlePane::show()
    {
        lv_obj_set_hidden(title_container, false);
    }

    // Hide the title pane
    void TitlePane::hide()
    {
        lv_obj_set_hidden(title_container, true);
    }
}
