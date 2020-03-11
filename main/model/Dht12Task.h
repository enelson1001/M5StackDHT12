/****************************************************************************************
 * Dht12Task.h - A task that runs periodically to retreive DHT12 measurements.
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

#include "model/EnvirValue.h"
#include <smooth/core/Task.h>
#include <smooth/core/ipc/IEventListener.h>
#include <smooth/core/ipc/SubscribingTaskEventQueue.h>
#include <smooth/core/io/i2c/Master.h>
#include <smooth/application/io/i2c/DHT12.h>

namespace redstone
{
    class Dht12Task : public smooth::core::Task
    {
        public:
            Dht12Task();

            void init() override;

            void tick() override;

        private:
            bool init_i2c_dht12();

            smooth::core::io::i2c::Master i2c_master;
            std::unique_ptr<smooth::application::sensor::DHT12> sensor{};
            bool dht12_initialized{ false };
            EnvirValue envir_value{};
    };
}
