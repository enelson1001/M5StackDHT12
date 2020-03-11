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
#pragma once

#include <lvgl/lvgl.h>

namespace redstone
{
    class GuiButton
    {
        public:
            /// Constructor
            GuiButton();

            /// Destructor
            virtual ~GuiButton() {}

            /// A "C" style callback required by Lvgl for button events
            /// \param obj The lvgl button object
            /// \param event The event type
            static void button_event_cb(lv_obj_t* obj, lv_event_t event);

            /// Get the pointer to gui button
            /// \param return Returns the pointer to the lvgl button object
            lv_obj_t* get_gui_btn_ptr();

            /// Align the button
            /// \param base
            /// \param align The type of aligment to use on the button
            /// \param x_mod The x offset from the chosen alignment
            /// \param y_mod The y offset from the chosen alignment
            void align(const lv_obj_t* base, lv_align_t align, lv_coord_t x_mod, lv_coord_t y_mod);

            /// Get the button coordinates
            /// \param return Returns the button coordinates
            lv_area_t get_coords();

            /// Create the button
            /// \param parent The parent of this button
            /// \param return Returns the created lvgl button object
            virtual lv_obj_t* create(lv_obj_t* parent) = 0;

        protected:
            /// The ON Clicked Event - button pressed and released
            /// This function must be implemented by the child
            /// \param obj The lvgl button object
            virtual void on_clicked(lv_obj_t* obj) = 0;

            lv_obj_t* create_btn(lv_obj_t* parent);

            lv_obj_t* gui_button;
    };
}
