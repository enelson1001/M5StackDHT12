/****************************************************************************************
 * GuiButtonPrev.h - A class that creates a PREV gui button
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
#include "gui/GuiButtonPrev.h"

namespace redstone
{
    // Constructor
    GuiButtonPrev::GuiButtonPrev(ViewController& view_ctrl) : view_controller(view_ctrl)
    {
    }

    // The On-Clicked button event
    void GuiButtonPrev::on_clicked(lv_obj_t* btn)
    {
        view_controller.show_previous_view();
    }

    // Create the PREV button
    lv_obj_t* GuiButtonPrev::create(lv_obj_t* parent)
    {
        // create the button
        lv_obj_t* btn = create_btn(parent);

        // sets the GuiButton callback to call this on_clicked function
        btn->user_data = this;

        // create style for button label
        lv_style_copy(&gui_btn_style, &lv_style_plain);
        gui_btn_style.text.font = &lv_font_roboto_12;
        gui_btn_style.text.color = LV_COLOR_WHITE;

        // set the size of the button
        lv_obj_set_size(btn, 60, 24);

        // set the text for the button label
        lv_obj_t* label = lv_label_create(btn, NULL);
        lv_obj_set_style(label, &gui_btn_style);
        lv_label_set_text(label, "PREV");

        return btn;
    }
}
