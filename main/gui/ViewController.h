/****************************************************************************************
 * ViewController.h - Controls which view will be displayed on the M5Stack
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

#include <memory>                   // for unique_ptr
#include <unordered_map>
#include <smooth/core/Task.h>
#include "gui/DisplayDriver.h"
#include "gui/MenuPane.h"
#include "gui/IPane.h"

namespace redstone
{
    class ViewController
    {
        public:
            // Constants & Enums
            enum ViewID : int
            {
                Measurements = 0,
                Table,
                HourlyLog,
                TimeDate
            };

            // Constructor
            ViewController(smooth::core::Task& task_lvgl);

            /// Initialize the view controller
            void init();

            /// Show the new view
            void show_new_view();

            /// Hide the current view
            void hide_current_view();

            /// Show the next view
            void show_next_view();

            /// Show the previous view
            void show_previous_view();

        private:
            smooth::core::Task& task_lvgl;
            DisplayDriver display_driver{};

            std::unique_ptr<IPane> content_pane;
            std::unique_ptr<IPane> title_pane;
            MenuPane menu_pane{};

            std::unordered_map<ViewID, std::unique_ptr<IPane>> content_panes;
            std::unordered_map<ViewID, std::unique_ptr<IPane>> title_panes;
            ViewID current_view_id{ Measurements };
            ViewID new_view_id{ Measurements };
    };
}
