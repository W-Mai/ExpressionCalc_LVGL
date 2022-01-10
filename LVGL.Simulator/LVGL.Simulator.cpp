/*
 * PROJECT:   Expression Calc via LVGL.
 * FILE:      LVGL.Simulator.cpp
 * DEVELOPER: W-Mai
 */

#include <Windows.h>

#include "resource.h"

#if _MSC_VER >= 1200
 // Disable compilation warnings.
#pragma warning(push)
// nonstandard extension used : bit field types other than int
#pragma warning(disable:4214)
// 'conversion' conversion from 'type1' to 'type2', possible loss of data
#pragma warning(disable:4244)
#endif

#include "lvgl/lvgl.h"
#include "lvgl/examples/lv_examples.h"
#include "lvgl/demos/lv_demos.h"
#include "lv_drivers/win32drv/win32drv.h"

#if _MSC_VER >= 1200
// Restore compilation warnings.
#pragma warning(pop)
#endif

#include <stdio.h>

#include "../App.h"

int main() {
    lv_init();

    if (!lv_win32_init(
        GetModuleHandleW(NULL),
        SW_SHOW,
        520,
        360,
        LoadIconW(GetModuleHandleW(NULL), MAKEINTRESOURCE(IDI_LVGL)))) {
        return -1;
    }

    lv_win32_add_all_input_devices_to_group(NULL);

    App();

    // ----------------------------------
    // Task handler loop
    // ----------------------------------

    while (!lv_win32_quit_signal) {
        lv_task_handler();
        Sleep(1);
    }

    return 0;
}
