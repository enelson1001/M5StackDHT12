/****************************************************************************************
 * LvglTask.cpp - A dedicated task for running LittlevGL
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
#include "gui/LvglTask.h"

using namespace std::chrono;
using namespace smooth::core;

namespace redstone
{
    // Class constants
    static const char* TAG = "LvglTask";

    // Constructor
    LvglTask::LvglTask()
            : Task("LvglTask", 4096, 10, milliseconds(100)),

              // The Task Name = "LvglTask"
              // The stack size is 4096 bytes
              // The priority is set to 10
              // The tick interval is 100ms

              view_controller(*this)
    {
    }

    // Intialize the Task
    void LvglTask::init()
    {
        Log::info(TAG, "initializing LvglTask");
        view_controller.init();
    }

    // The task tick event that happens every 100ms
    void LvglTask::tick()
    {
        // Let LittlevGL do some work
        lv_task_handler();
    }
}
