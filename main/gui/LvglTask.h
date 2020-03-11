/****************************************************************************************
 * LvglTask.h - A dedicated task for running LittlevGL
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

#include "gui/ViewController.h"
#include <smooth/core/Task.h>

namespace redstone
{
    class LvglTask : public smooth::core::Task
    {
        public:
            LvglTask();

            void init() override;

            void tick() override;

        private:
            ViewController view_controller;
    };
}
