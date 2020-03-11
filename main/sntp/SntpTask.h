/****************************************************************************************
 * SntpTask.h - A task that runs periodically to update the local system time.
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

#include <smooth/core/Application.h>
#include <smooth/core/Task.h>
#include <smooth/core/sntp/Sntp.h>
#include <smooth/core/sntp/TimeSyncEvent.h>
#include <smooth/core/ipc/IEventListener.h>
#include <smooth/core/ipc/SubscribingTaskEventQueue.h>
#include "sntp/TimeValue.h"

namespace redstone
{
    class SntpTask : public smooth::core::Task,
        public smooth::core::ipc::IEventListener<smooth::core::sntp::TimeSyncEvent>
    {
        public:
            SntpTask(smooth::core::Application& app);

            void init() override;

            void tick() override;

            void event(const smooth::core::sntp::TimeSyncEvent& ev) override;

        private:
            using TimeSyncQueue = smooth::core::ipc::SubscribingTaskEventQueue<smooth::core::sntp::TimeSyncEvent>;

            smooth::core::Application& app;
            smooth::core::sntp::Sntp sntp;
            std::shared_ptr<TimeSyncQueue> sync_queue;
            TimeValue time_value{};
            std::array<char, 32> char_buf;
            bool has_synced{ false };
    };
}
