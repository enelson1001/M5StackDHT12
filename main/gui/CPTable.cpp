/****************************************************************************************
 * CPTable.cpp - A content pane that displays a temperature, humidity, dew point,
 * heat index in a table
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
#include "gui/CPTable.h"

#include <smooth/core/logging/log.h>
using namespace smooth::core::logging;

namespace redstone
{
    // Class constants
    static const char* TAG = "CPTable";

    // Constructor
    CPTable::CPTable(smooth::core::Task& task_lvgl) :
            subr_queue_envir_value(SubQEnvirValue::create(2, task_lvgl, *this))

            // Create Subscriber Queue (SubQ) so this content pane can listen for
            // EnvirValue events
            // the queue will hold up to 2 items
            // the "task_lvgl" is this task which to signal when an event is available.
            // the "*this" is the class instance that will receive the events
    {
    }

    // Create the content pane
    void CPTable::create(int width, int height)
    {
        Log::info(TAG, "Creating CPTable");

        // create style for the content container
        lv_style_copy(&content_container_style, &lv_style_plain);
        content_container_style.body.main_color = lv_color_hex3(0xaaa);
        content_container_style.body.grad_color = lv_color_hex3(0xaaa);

        // create style for cell
        lv_style_copy(&cell_style, &lv_style_plain);
        cell_style.body.border.width = 1;
        cell_style.body.border.color = LV_COLOR_BLACK;
        cell_style.text.color = LV_COLOR_BLACK;

        // create style for header cells
        lv_style_copy(&header_style, &lv_style_plain);
        header_style.body.border.width = 1;
        header_style.body.border.color = LV_COLOR_BLACK;
        header_style.body.main_color = lv_color_hex3(0xff9);
        header_style.body.grad_color = lv_color_hex3(0xff9);
        header_style.text.color = LV_COLOR_BLACK;

        // create a content container
        content_container = lv_cont_create(lv_scr_act(), NULL);
        lv_obj_set_size(content_container, width, height);
        lv_obj_align(content_container, NULL, LV_ALIGN_CENTER, 0, 0);
        lv_cont_set_style(content_container, LV_CONT_STYLE_MAIN, &content_container_style);
        lv_obj_set_hidden(content_container, true);

        // create table
        table = lv_table_create(content_container, NULL);
        lv_table_set_style(table, LV_TABLE_STYLE_CELL1, &cell_style);
        lv_table_set_style(table, LV_TABLE_STYLE_CELL2, &header_style);
        lv_table_set_style(table, LV_TABLE_STYLE_BG, &lv_style_transp_tight);
        lv_table_set_col_width(table, 0, 150);
        lv_table_set_col_cnt(table, 2);
        lv_table_set_row_cnt(table, 5);
        lv_obj_align(table, NULL, LV_ALIGN_CENTER, 0, 0);

        // Make the cells of the first row center aligned
        lv_table_set_cell_align(table, 0, 0, LV_LABEL_ALIGN_CENTER);
        lv_table_set_cell_align(table, 0, 1, LV_LABEL_ALIGN_CENTER);

        // Make the cells of the first row TYPE = 2 (use `header_style`)
        lv_table_set_cell_type(table, 0, 0, 2);
        lv_table_set_cell_type(table, 0, 1, 2);

        // Fill the first column
        lv_table_set_cell_value(table, 0, 0, "Name");
        lv_table_set_cell_value(table, 1, 0, "Temperature");
        lv_table_set_cell_value(table, 2, 0, "Humidity");
        lv_table_set_cell_value(table, 3, 0, "Dew Point");
        lv_table_set_cell_value(table, 4, 0, "Heat Index");

        // Fill in header column 2
        lv_table_set_cell_value(table, 0, 1, "Value");
    }

    // The published EnvirValue event
    void CPTable::event(const EnvirValue& event)
    {
        temperature = event.get_temperture_degree_F();
        humidty = event.get_relative_humidity();
        dew_point = event.get_dew_point_fahrenheit();
        heat_index = event.get_heat_index_fahrenheit();
        update_table();
    }

    // Update the values in table
    void CPTable::update_table()
    {
        std::ostringstream stream;

        stream << std::fixed << std::setprecision(1) << temperature;
        std::string temp_text = stream.str() + " F";
        lv_table_set_cell_value(table, 1, 1, temp_text.c_str());

        stream.str("");
        stream.clear();
        stream << std::fixed << std::setprecision(1) << humidty;
        std::string humd_text = stream.str() + " %RH";
        lv_table_set_cell_value(table, 2, 1, humd_text.c_str());

        stream.str("");
        stream.clear();
        stream << std::fixed << std::setprecision(1) << dew_point;
        std::string dew_point_text = stream.str() + " F";
        lv_table_set_cell_value(table, 3, 1, dew_point_text.c_str());

        stream.str("");
        stream.clear();
        stream << std::fixed << std::setprecision(1) << heat_index;
        std::string heat_index_text = stream.str() + " F";
        lv_table_set_cell_value(table, 4, 1, heat_index_text.c_str());
    }

    // Show the content pane
    void CPTable::show()
    {
        lv_obj_set_hidden(content_container, false);
    }

    // Hide the content pane
    void CPTable::hide()
    {
        lv_obj_set_hidden(content_container, true);
    }
}
