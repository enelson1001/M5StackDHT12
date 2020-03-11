/****************************************************************************************
 * CPHourlyLog.cpp - A content pane that displays a temperature chart
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
#include <sstream>
#include <iomanip>  // for set precision
#include "gui/CPHourlyLog.h"

#include <smooth/core/logging/log.h>
using namespace smooth::core::logging;

namespace redstone
{
    // Class constants
    static const char* TAG = "CPHourlyLog";
    static const char* TICKS_Y_AXIS = "60F\n65F\n70F\n75F\n80F";
    static const char* TICKS_X_AXIS = "12a\n4a\n8a\n12p\n4p\n8p\nhr";

    // Constructor
    CPHourlyLog::CPHourlyLog(smooth::core::Task& task_lvgl) :
            subr_queue_envir_value(SubQEnvirValue::create(2, task_lvgl, *this)),

            // Create Subscriber Queue (SubQ) so this content pane can listen for
            // EnvirValue events
            // the queue will hold up to 2 items
            // the "task_lvgl" is this task which to signal when an event is available.
            // the "*this" is the class instance that will receive the events

            subr_queue_time_value(SubQTimeValue::create(2, task_lvgl, *this))

            // Create Subscriber Queue (SubQ) so this content pane can listen for
            // TimeValue events
            // the queue will hold up to 2 items
            // the "task_lvgl" is this task which to signal when an event is available.
            // the "*this" is the class instance that will receive the events
    {
    }

    // Create the content pane
    void CPHourlyLog::create(int width, int height)
    {
        Log::info(TAG, "Creating CPHourlyLog");

        // create style for the content container
        lv_style_copy(&content_container_style, &lv_style_plain);
        content_container_style.body.main_color = lv_color_hex3(0xaaa);
        content_container_style.body.grad_color = lv_color_hex3(0xaaa);

        // create style for temperature chart
        lv_style_copy(&temperature_chart_style, &lv_style_plain);
        temperature_chart_style.text.font = &lv_font_roboto_12;
        temperature_chart_style.text.color = lv_color_hex3(0x606);
        temperature_chart_style.body.border.width = 2;  
        temperature_chart_style.line.width = 1;

        // create a content container
        content_container = lv_cont_create(lv_scr_act(), NULL);
        lv_obj_set_size(content_container, width, height);
        lv_obj_align(content_container, NULL, LV_ALIGN_CENTER, 0, 0);
        lv_cont_set_style(content_container, LV_CONT_STYLE_MAIN, &content_container_style);
        lv_obj_set_hidden(content_container, true);

        // create the temperature chart
        temperature_chart = lv_chart_create(content_container, NULL);
        lv_chart_set_style(temperature_chart, LV_CHART_STYLE_MAIN, &temperature_chart_style);

        // create a margin for text labels
        lv_chart_set_margin(temperature_chart, 42);

        // The x-axis of 219 = 2*2px (border) + 23*1px (23 inside tick marks) + 24*8px (24 spaces between tick marks, space = 8px)
        // The y-axis of 103 = 2*2px (border) + 19*1px (19 inside tick marks) + 20*4px (20 spaces between tick marks, space = 4px)
        // Even though size calculates out to be width=219 and height=103 you need to set size to 217,101 else points are off by 1px
        lv_obj_set_size(temperature_chart, 217, 101);

        // Offset chart to fit content pane better
        lv_obj_align(temperature_chart, NULL, LV_ALIGN_CENTER, 20, -10);

        // Show lines and points
        lv_chart_set_type(temperature_chart, LV_CHART_TYPE_POINT | LV_CHART_TYPE_LINE);

        // The opacity of the chart
        lv_chart_set_series_opa(temperature_chart, LV_OPA_70);

        // The chart will use update mode circular                      
        lv_chart_set_update_mode(temperature_chart, LV_CHART_UPDATE_MODE_CIRCULAR);

        // The line width is 2px and point radius is 2px - has no effect if temperature_chart_style has style.line.width = 1
        lv_chart_set_series_width(temperature_chart, 2); 

        // The 3 horizontal division lines will line up with 65F, 70F, 75F and 80F.  
        // The 5 vertical divsion lines will line up with 4a, 8a, 12p, 4p, 8p.
        lv_chart_set_div_line_count(temperature_chart, 3, 5);

        // So the x-axis has an equal number of minor tick marks between major tick marks we set the point count to 25.
        // The points we will be updating will be from 0-23 (hours of the 24 hour clock) so we will never have a point
        // displayed a x-axis position 25.  In the labeling of the x-axis the 25th point is labeled "hr".
        lv_chart_set_point_count(temperature_chart, 25);

        // The y-axis range
        lv_chart_set_range(temperature_chart, 60, 80);
        temperature_ser = lv_chart_add_series(temperature_chart, lv_color_hex3(0xC00));

        // The count of 4 will actually place 3 minor tick marks between two major tick marks
        lv_chart_set_x_tick_texts(temperature_chart, TICKS_X_AXIS, 4, LV_CHART_AXIS_DRAW_LAST_TICK);

        // The count of 5 will actually place 4 minor tick marks between two major tick marks
        lv_chart_set_y_tick_texts(temperature_chart, TICKS_Y_AXIS, 5, LV_CHART_AXIS_DRAW_LAST_TICK | LV_CHART_AXIS_INVERSE_LABELS_ORDER);
    }

    // A published EnvirValue event
    void CPHourlyLog::event(const EnvirValue& event)
    {
        temperature = event.get_temperture_degree_F();
    }

    // A published TimeValue event
    void CPHourlyLog::event(const TimeValue& event)
    {
        int16_t hour = event.get_hour();
        int16_t minute = event.get_minute();
        int16_t second = event.get_second();

        if ( (hour == 23) && (minute == 59) && (second >= 55))
        {
            clear_chart();
        }

        if ( (current_hour != hour) && (minute == 0) )
        {
            current_hour = hour;
            update_chart(); 
        }
    }

    // Update the chart
    void CPHourlyLog::update_chart()
    {
        int16_t temp = static_cast<int16_t>(temperature + 0.5);  // round up
        temperature_ser->points[static_cast<lv_coord_t>(current_hour)] = temp;
        lv_chart_refresh(temperature_chart);
    }

    // Clear the chart
    void CPHourlyLog::clear_chart()
    {
        lv_chart_clear_serie(temperature_chart, temperature_ser);
    }

    // Show the content pane
    void CPHourlyLog::show()
    {
        lv_obj_set_hidden(content_container, false);
    }

    // Hide the content pane
    void CPHourlyLog::hide()
    {
        lv_obj_set_hidden(content_container, true);
    }
}
