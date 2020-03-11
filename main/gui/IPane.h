/****************************************************************************************
 * IPane.h - An abstract class that panes implement to be able to be show a pane, hide a
 *            pane and create a pane
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

namespace redstone
{
    class IPane
    {
        public:
            virtual ~IPane() {}

            virtual void show() = 0;

            virtual void hide() = 0;

            virtual void create(int width, int height) = 0;
    };
}
