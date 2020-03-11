/****************************************************************************************
 * TitlePane.h - A class that creates a simple title pane
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

#include <string>
#include <lvgl/lvgl.h>
#include "gui/IPane.h"

namespace redstone
{
    class TitlePane : public IPane
    {
        public:
            TitlePane(std::string title);

            ~TitlePane() {}

            /// Create a title pane
            /// \param width The width of the title pane
            /// \param height The height of the title pane
            void create(int width, int height) override;

            /// Show the title pane
            void show() override;

            /// Hide the title pane
            void hide() override;

        private:
            lv_style_t title_style;
            lv_obj_t* title_container;
            lv_obj_t* title_label;
            std::string title;
    };
}
