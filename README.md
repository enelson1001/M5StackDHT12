# M5StackDHT12
M5Stack using Smooth C++ library and LittlevGL Graphics library with 3 different views of DHT12 environment data and 1 view of date and time.

## Overview
This application shows how to use the Smooth C++ library and the LittlevGL Graphichs library
to display DHT12 values in three different views and a view to display the current date and time. 
The app has three tasks running besides the main Application Task.  The details about the
application memory usage, bin file size and versions of software used on this project are
listed at the top of the App.cpp file.

## Views
The following views are dsiplayed by this application.
- Measurements View - display numerical values of temp, humidity and dew point
- Envir Table View - display temperature, humidity, dew point and heat index in a table
- Hourly Temp Log - display an hourly log of temperature over a 24 hour period
- Time and Date - display the current time (MST) and current date

## A view
A view consists of a title pane, a content pane and a menu pane.  The title pane is at the top of the screen
and the title changes depending upon which view is selected.  The content pane varies depending upon the view 
selected and is positioned in the middle of the screen.  And lastly the menu pane is at the bottom of the screen
where the 3 hardware buttons on the M5Stack are located.  Two menu buttons are supported "NEXT" and "PREV".
The app uses LittlevGL input device driver to debounce the buttons.  The LittlevGL input device driver creates an
on-clicked event when the hw button is pressed and the released.  It also shows the gui button pressed or released
when the hardware button is pressed or released.

## Tasks
The app has 3 additional tasks running besides the Application Task. 
- Dht12Task - A task that periodically collects measurements(temperature, humidity) from the DHT12.
- LvglTask - A tasks that runs LittlevGL.  All files in gui folder are running under this task.
- SntpTask - A task that runs SNTP.  Updates the local system time on Sntp TimeSyncEvents and publishes TimeValue.

## Pictures of the various views
The Measurements View
![Measurement view](photos/measurements_view.jpg)

The Envir Table View
![Envir Table View](photos/envir_table_view.jpg)

The Temperature Hourly Log View
![Temperature Hourly Log](photos/temp_hourly_log_view.jpg)

The Time and Date View
![Time and Date View](photos/time_and_date_view.jpg)

# Versions
### v1.0 : 
- initial release
### v2.0 : 
- update to lvgl v7.11.0 commit ec9de51 - March 16, 2021
- update to Smooth master commit 5578b8b - April 15, 2021 
- update to esp-idf v4.3-beta3 commit e9cf9e2 - April 14, 2021
- added SDCard, read wifi credentials from sdcard
