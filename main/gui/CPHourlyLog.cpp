/****************************************************************************************
 * CPHourlyLog.cpp - A content pane that displays a temperature chart
 *
 * Created on Jan. 04, 2020
 * Modified on March 02, 2021 - Updated to lvgl to v7.10
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

        // create a plain style
        lv_style_init(&plain_style);
        lv_style_set_pad_top(&plain_style, LV_STATE_DEFAULT, 0);
        lv_style_set_pad_bottom(&plain_style, LV_STATE_DEFAULT, 0);
        lv_style_set_pad_left(&plain_style, LV_STATE_DEFAULT, 0);
        lv_style_set_pad_right(&plain_style, LV_STATE_DEFAULT, 0);
        lv_style_set_line_opa(&plain_style, LV_STATE_DEFAULT, 0);
        lv_style_set_pad_inner(&plain_style, LV_STATE_DEFAULT, 0);
        lv_style_set_margin_all(&plain_style, LV_STATE_DEFAULT, 0);
        lv_style_set_border_width(&plain_style, LV_STATE_DEFAULT, 0);
        lv_style_set_radius(&plain_style, LV_STATE_DEFAULT, 0);

        // create a style for the content container
        lv_style_copy(&content_container_style, &plain_style);
        lv_style_set_bg_color(&content_container_style, LV_STATE_DEFAULT, lv_color_hex3(0xaaa));

        // create style for the chart background
        // set pad left so ticks and labels are displayed on left side of chart
        // set pad right so "hr" on bottom right side tick is displayed completely
        // set pad bottom so ticks and labels are displayed on bottom of chart
        // set text color to black
        // set text font to size 10
        // set border width to 0 to reduce size of chart
        // set background opa to transparent so ticks and labels are diplayed on content container background
        lv_style_init(&chart_bg_style);
        lv_style_set_pad_left(&chart_bg_style, LV_STATE_DEFAULT, 40);
        lv_style_set_pad_right(&chart_bg_style, LV_STATE_DEFAULT, 10);
        lv_style_set_pad_bottom(&chart_bg_style, LV_STATE_DEFAULT, 30);
        lv_style_set_text_color(&chart_bg_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
        lv_style_set_text_font(&chart_bg_style, LV_STATE_DEFAULT, &lv_font_montserrat_10);
        lv_style_set_border_width(&chart_bg_style, LV_STATE_DEFAULT, 0);
        lv_style_set_bg_opa(&chart_bg_style, LV_STATE_DEFAULT, LV_OPA_TRANSP);

        // create style for the series background
        // set background color to white
        // set background opa to cover so white background is displayed over content container background
        // set border width to 2 pixels
        // set border color to black
        lv_style_init(&series_bg_style);
        lv_style_set_bg_color(&series_bg_style, LV_STATE_DEFAULT, LV_COLOR_WHITE);
        lv_style_set_bg_opa(&series_bg_style, LV_STATE_DEFAULT, LV_OPA_COVER);
        lv_style_set_border_width(&series_bg_style, LV_STATE_DEFAULT, 2);
        lv_style_set_border_color(&series_bg_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);

        // create a style for the series
        // set radius of points on line chart
        lv_style_init(&series_style);
        lv_style_set_size(&series_style, LV_STATE_DEFAULT, 1);

        // create a content container
        content_container = lv_cont_create(lv_scr_act(), NULL);
        lv_obj_set_size(content_container, width, height);
        lv_obj_align(content_container, NULL, LV_ALIGN_CENTER, 0, 0);
        lv_obj_add_style(content_container, LV_CONT_PART_MAIN, &content_container_style);
        lv_obj_set_hidden(content_container, true);

        // create the temperature chart
        temperature_chart = lv_chart_create(content_container, NULL);
        lv_obj_add_style(temperature_chart, LV_CHART_PART_BG, &chart_bg_style);
        lv_obj_add_style(temperature_chart, LV_CHART_PART_SERIES_BG, &series_bg_style);
        lv_obj_add_style(temperature_chart, LV_CHART_PART_SERIES, &series_style);

        // set size of chart to fit content pane almost completely for best look
        lv_obj_set_size(temperature_chart, 260, 140);

        // Center chart in content pane
        lv_obj_align(temperature_chart, NULL, LV_ALIGN_CENTER, 0, 0);

        // Show lines and points
        lv_chart_set_type(temperature_chart, LV_CHART_TYPE_LINE);

        // The chart will use update mode circular
        lv_chart_set_update_mode(temperature_chart, LV_CHART_UPDATE_MODE_CIRCULAR);

        // The 3 horizontal division lines will line up with 65F, 70F, 75F and 80F.
        // The 5 vertical divsion lines will line up with 4a, 8a, 12p, 4p, 8p.
        lv_chart_set_div_line_count(temperature_chart, 3, 5);

        // So the x-axis has an equal number of minor tick marks between major tick marks we set the point count to 25.
        // The points we will be updating will be from 0-23 (hours of the 24 hour clock) so we will never have a point
        // displayed a x-axis position 25.  In the labeling of the x-axis the 25th point is labeled "hr".
        lv_chart_set_point_count(temperature_chart, 25);

        // The y-axis range
        lv_chart_set_range(temperature_chart, 60, 80);

        // Set the color of line on the chart
        temperature_ser = lv_chart_add_series(temperature_chart, lv_color_hex3(0xC00));

        // The count of 4 will actually place 3 minor tick marks between two major tick marks
        lv_chart_set_x_tick_texts(temperature_chart, TICKS_X_AXIS, 4, LV_CHART_AXIS_DRAW_LAST_TICK);

        // The count of 5 will actually place 4 minor tick marks between two major tick marks
        lv_chart_set_y_tick_texts(temperature_chart,
        TICKS_Y_AXIS,
        5,
        LV_CHART_AXIS_DRAW_LAST_TICK | LV_CHART_AXIS_INVERSE_LABELS_ORDER);
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

        if ((hour == 23) && (minute == 59) && (second >= 55))
        {
            clear_chart();
        }

        if ((current_hour != hour) && (minute == 0))
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
