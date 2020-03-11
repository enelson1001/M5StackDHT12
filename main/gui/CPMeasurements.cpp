/****************************************************************************************
 * CPMeasurements.cpp - A content pane that displays temperature, humidity and dew point
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
#include "gui/CPMeasurements.h"

#include <smooth/core/logging/log.h>
using namespace smooth::core::logging;

namespace redstone
{
    // Class constants
    static const char* TAG = "CPMeasurements";

    // Constructor
    CPMeasurements::CPMeasurements(smooth::core::Task& task_lvgl) :
            subr_queue_envir_value(SubQEnvirValue::create(2, task_lvgl, *this))

            // Create Subscriber Queue (SubQ) so this content pane can listen for
            // EnvirValue events
            // the queue will hold up to 2 items
            // the "task_lvgl" is this task which to signal when an event is available.
            // the "*this" is the class instance that will receive the events
    {
    }

    // Create the content pane
    void CPMeasurements::create(int width, int height)
    {
        Log::info(TAG, "Creating CPMeasurements");

        // create style for the content container
        lv_style_copy(&content_container_style, &lv_style_plain);
        content_container_style.body.main_color = lv_color_hex3(0xaaa);
        content_container_style.body.grad_color = lv_color_hex3(0xaaa);

        // create style for measurement containers
        lv_style_copy(&measurement_container_style, &lv_style_pretty);
        measurement_container_style.body.main_color = LV_COLOR_BLACK;
        measurement_container_style.body.grad_color = LV_COLOR_BLACK;

        // create style for measurement descriptors
        lv_style_copy(&descriptor_label_style, &lv_style_pretty);
        descriptor_label_style.text.font = &lv_font_roboto_22;
        descriptor_label_style.text.color = LV_COLOR_WHITE;
        descriptor_label_style.text.opa = 200;

        // create style for measurement values
        lv_style_copy(&measurement_label_style, &lv_style_pretty);
        measurement_label_style.text.font = &lv_font_roboto_22;
        measurement_label_style.text.color = LV_COLOR_YELLOW;

        // create a content container
        content_container = lv_cont_create(lv_scr_act(), NULL);
        lv_obj_set_size(content_container, width, height);
        lv_cont_set_layout(content_container, LV_LAYOUT_CENTER);
        lv_obj_align(content_container, NULL, LV_ALIGN_CENTER, 0, 0);
        lv_cont_set_style(content_container, LV_CONT_STYLE_MAIN, &content_container_style);
        lv_obj_set_hidden(content_container, true);

        // create a container for temperature measurements
        temperature_container = lv_cont_create(content_container, NULL);
        lv_cont_set_style(temperature_container, LV_CONT_STYLE_MAIN, &measurement_container_style);
        lv_obj_set_size(temperature_container, (LV_HOR_RES_MAX - 20), 40);
        lv_obj_align(temperature_container, NULL, LV_ALIGN_CENTER, 0, -60);

        // create a descriptor for the temperature measurement
        lv_obj_t* label = lv_label_create(temperature_container, NULL);
        lv_obj_set_style(label, &descriptor_label_style);
        lv_label_set_text(label, "Temperature: ");
        lv_obj_align(label, NULL, LV_ALIGN_IN_LEFT_MID, 30, 0);

        // create a dynamic label for temperature measurement value
        temperature_value_label = lv_label_create(temperature_container, NULL);
        lv_obj_set_style(temperature_value_label, &measurement_label_style);
        lv_label_set_text(temperature_value_label, "--");
        lv_obj_align(temperature_value_label, label, LV_ALIGN_OUT_RIGHT_MID, 5, 0);

        // create a container for humidity measurements
        humdity_container = lv_cont_create(content_container, NULL);
        lv_cont_set_style(humdity_container, LV_CONT_STYLE_MAIN, &measurement_container_style);
        lv_obj_set_size(humdity_container, (LV_HOR_RES_MAX - 20), 40);
        lv_obj_align(humdity_container, NULL, LV_ALIGN_CENTER, 0, -10);

        // create a descriptor for the humidity measurement
        label = lv_label_create(humdity_container, NULL);
        lv_obj_set_style(label, &descriptor_label_style);
        lv_label_set_text(label, "       Humidity: ");
        lv_obj_align(label, NULL, LV_ALIGN_IN_LEFT_MID, 30, 0);

        // create a dynamic label for humidity measurement value
        humidity_value_label = lv_label_create(humdity_container, NULL);
        lv_obj_set_style(humidity_value_label, &measurement_label_style);
        lv_label_set_text(humidity_value_label, "--");
        lv_obj_align(humidity_value_label, label, LV_ALIGN_OUT_RIGHT_MID, 5, 0);

        // create a container for dew point measurements
        dew_point_container = lv_cont_create(content_container, NULL);
        lv_cont_set_style(dew_point_container, LV_CONT_STYLE_MAIN, &measurement_container_style);
        lv_obj_set_size(dew_point_container, (LV_HOR_RES_MAX - 20), 40);
        lv_obj_align(dew_point_container, NULL, LV_ALIGN_CENTER, 0, 60);

        // create a descriptor for the dew point measurement
        label = lv_label_create(dew_point_container, NULL);
        lv_obj_set_style(label, &descriptor_label_style);
        lv_label_set_text(label, "     Dew Point: ");
        lv_obj_align(label, NULL, LV_ALIGN_IN_LEFT_MID, 30, 0);

        // create a dynamic label for dew point measurement value
        dew_point_value_label = lv_label_create(dew_point_container, NULL);
        lv_obj_set_style(dew_point_value_label, &measurement_label_style);
        lv_label_set_text(dew_point_value_label, "--");
        lv_obj_align(dew_point_value_label, label, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
    }

    // The published EnvirValue event
    void CPMeasurements::event(const EnvirValue& event)
    {
        temperature = event.get_temperture_degree_F();
        humidty = event.get_relative_humidity();
        dew_point = event.get_dew_point_fahrenheit();
        update_measurements_text();
    }

    // Update the measurement value labels
    void CPMeasurements::update_measurements_text()
    {
        std::ostringstream stream;

        stream << std::fixed << std::setprecision(1) << temperature;
        std::string temp_text = stream.str() + " F";
        lv_label_set_text(temperature_value_label, temp_text.c_str());

        stream.str("");
        stream.clear();
        stream << std::fixed << std::setprecision(1) << humidty;
        std::string humd_text = stream.str() + " %RH";
        lv_label_set_text(humidity_value_label, humd_text.c_str());

        stream.str("");
        stream.clear();
        stream << std::fixed << std::setprecision(1) << dew_point;
        std::string pres_text = stream.str() + " F";
        lv_label_set_text(dew_point_value_label, pres_text.c_str());
    }

    // Show the content pane
    void CPMeasurements::show()
    {
        lv_obj_set_hidden(content_container, false);
    }

    // Hide the content pane
    void CPMeasurements::hide()
    {
        lv_obj_set_hidden(content_container, true);
    }
}
