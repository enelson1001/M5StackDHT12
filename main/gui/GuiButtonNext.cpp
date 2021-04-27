/****************************************************************************************
 * GuiButtonNext.h - A class that creates a NEXT gui button
 *
 * Created on Jan. 04, 2020
 * Modified on March 02, 2021 - Updated to lvgl to v7.10, moved button style to GuiButton class
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
#include "gui/GuiButtonNext.h"

namespace redstone
{
    // Constructor
    GuiButtonNext::GuiButtonNext(ViewController& view_ctrl) : view_controller(view_ctrl)
    {
    }

    // The On-Clicked button event
    void GuiButtonNext::on_clicked(lv_obj_t* btn)
    {
        view_controller.show_next_view();
    }

    // Create the NEXT button
    lv_obj_t* GuiButtonNext::create(lv_obj_t* parent)
    {
        // create style for button
        create_button_style();

        // create the button
        lv_obj_t* btn = create_btn(parent);
        lv_obj_set_size(btn, 60, 24);
        lv_obj_reset_style_list(btn, LV_BTN_PART_MAIN);
        lv_obj_add_style(btn, LV_BTN_PART_MAIN, &gui_btn_style);

        // set the text for the button label
        lv_obj_t* label = lv_label_create(btn, NULL);
        lv_label_set_text(label, "NEXT");

        // sets the GuiButton callback to call this on_clicked function
        btn->user_data = this;

        return btn;
    }
}
