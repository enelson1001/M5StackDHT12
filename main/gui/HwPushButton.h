/****************************************************************************************
 * HwPushButton.h - A class that creates a hardware PUSHBUTTON
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

#include <smooth/core/io/Input.h>

namespace redstone
{
    class HwPushButton
    {
        public:
            /// Constructor
            HwPushButton(gpio_num_t pin, bool pullup, bool pulldn) : hw_btn(pin, pullup, pulldn) {}

            /// Is button pressed
            bool is_button_pressed()
            {
                return !hw_btn.read();
            }

        private:
            smooth::core::io::Input hw_btn;
    };
}
