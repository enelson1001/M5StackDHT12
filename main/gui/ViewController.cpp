/****************************************************************************************
 * ViewController.cpp - Controls which view will be displayed on the M5Stack
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
#include "gui/ViewController.h"
#include "gui/GuiButtonPrev.h"
#include "gui/GuiButtonNext.h"
#include "gui/HwPushButton.h"
#include "gui/TitlePane.h"
#include "gui/CPMeasurements.h"
#include "gui/CPTable.h"
#include "gui/CPHourlyLog.h"
#include "gui/CPTimeAndDate.h"

#include <smooth/core/logging/log.h>

using namespace smooth::core::logging;

namespace redstone
{
    // Class constants
    static const char* TAG = "ViewController";

    // Constructor
    ViewController::ViewController(smooth::core::Task& task_lvgl) : task_lvgl(task_lvgl)
    {
    }

    // Initialize view controller
    void ViewController::init()
    {
        Log::info(TAG, "====== Initializing ViewController ======");

        // initialize the display driver
        display_driver.initialize();

        // create the title panes
        title_pane = std::make_unique<TitlePane>("DHT12 Measurements");
        title_pane->create(LV_HOR_RES, 40);
        title_panes[Measurements] = std::move(title_pane);

        title_pane = std::make_unique<TitlePane>("DHT12 Envir Table");
        title_pane->create(LV_HOR_RES, 40);
        title_panes[Table] = std::move(title_pane);

        title_pane = std::make_unique<TitlePane>("DHT12 Hourly Temp Log");
        title_pane->create(LV_HOR_RES, 40);
        title_panes[HourlyLog] = std::move(title_pane);

        title_pane = std::make_unique<TitlePane>("DHT12 Time & Date");
        title_pane->create(LV_HOR_RES, 40);
        title_panes[TimeDate] = std::move(title_pane);

        // create content panes
        content_pane = std::make_unique<CPMeasurements>(task_lvgl);
        content_pane->create(LV_HOR_RES, 158);
        content_panes[Measurements] = std::move(content_pane);

        content_pane = std::make_unique<CPTable>(task_lvgl);
        content_pane->create(LV_HOR_RES, 158);
        content_panes[Table] = std::move(content_pane);

        content_pane = std::make_unique<CPHourlyLog>(task_lvgl);
        content_pane->create(LV_HOR_RES, 158);
        content_panes[HourlyLog] = std::move(content_pane);

        content_pane = std::make_unique<CPTimeAndDate>(task_lvgl);
        content_pane->create(LV_HOR_RES, 158);
        content_panes[TimeDate] = std::move(content_pane);


        // create menu pane
        menu_pane.initialize();
        menu_pane.add_menu_button(MenuPane::LeftButtonID, std::make_unique<GuiButtonPrev>(*this), std::make_unique<HwPushButton>(GPIO_NUM_39, false, false));
        menu_pane.add_menu_button(MenuPane::RightButtonID, std::make_unique<GuiButtonNext>(*this), std::make_unique<HwPushButton>(GPIO_NUM_37, false, false));
        menu_pane.create(LV_HOR_RES, 40);
        menu_pane.show();  // only need to do this once since we never change the menu pane

        // show new view
        show_new_view();
    }

    void ViewController::show_new_view()
    {
        title_panes[new_view_id]->show();
        content_panes[new_view_id]->show();
        current_view_id = new_view_id;
    }

    void ViewController::hide_current_view()
    {
        title_panes[current_view_id]->hide();
        content_panes[current_view_id]->hide();
    }

    void ViewController::show_next_view()
    {
        hide_current_view();
        new_view_id = current_view_id == TimeDate ? Measurements : static_cast<ViewID>(static_cast<int>(current_view_id) + 1);
        show_new_view();
    }

    void ViewController::show_previous_view()
    {
        hide_current_view();
        new_view_id = current_view_id == Measurements ? TimeDate : static_cast<ViewID>(static_cast<int>(current_view_id) - 1);
        show_new_view();
    }
}
