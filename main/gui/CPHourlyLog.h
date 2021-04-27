/****************************************************************************************
 * CPHourlyLog.h - A content pane that displays a temperature chart
 *
 * Created on Jan. 04, 2020
 * Modified on March 02, 2021 - Updated to lvgl v7.10
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
#include <memory>  // for shared pointer
#include <lvgl/lvgl.h>
#include <smooth/core/ipc/IEventListener.h>
#include <smooth/core/ipc/SubscribingTaskEventQueue.h>
#include "gui/IPane.h"
#include "model/EnvirValue.h"
#include "sntp/TimeValue.h"

namespace redstone
{
    class CPHourlyLog : public IPane,
        public smooth::core::ipc::IEventListener<EnvirValue>,
        public smooth::core::ipc::IEventListener<TimeValue>
    {
        public:
            /// Constructor
            /// \param task_lvgl The task this class is running under
            CPHourlyLog(smooth::core::Task& task_lvgl);

            /// Show the content pane
            void show() override;

            // Hide the content pane
            void hide() override;

            /// Create the content pane
            /// \param width The width of the content pane
            /// \param height The height of the content pane
            void create(int width, int height) override;

            /// The EnvirValue events that this instance listens for
            void event(const EnvirValue& event) override;

            /// The TimeValue events that this instance listens for
            void event(const TimeValue& ev) override;

        private:
            /// Update the chart
            void update_chart();

            /// clear the graph
            void clear_chart();

            // Subscriber's queue's
            using SubQEnvirValue = smooth::core::ipc::SubscribingTaskEventQueue<EnvirValue>;
            std::shared_ptr<SubQEnvirValue> subr_queue_envir_value;

            using SubQTimeValue = smooth::core::ipc::SubscribingTaskEventQueue<TimeValue>;
            std::shared_ptr<SubQTimeValue> subr_queue_time_value;

            lv_style_t plain_style;
            lv_style_t content_container_style;
            lv_style_t chart_bg_style;
            lv_style_t series_bg_style;
            lv_style_t series_style;

            lv_obj_t* content_container;
            lv_obj_t* temperature_chart;
            lv_chart_series_t* temperature_ser;

            float temperature;
            int current_hour{ -1 };
    };
}
