#pragma once

#include "tools/globals.h"

#include <cstdint>
// #include <iostream>
#include <xcb/xcb.h>
#include <xcb/xproto.h>
#include <xcb/xcb_ewmh.h>

namespace XCB
{
    inline void change_back_pixel(uint32_t window, uint32_t pixel)
    {
        uint32_t values[1] = {pixel};
        xcb_change_window_attributes(conn, window, XCB_CW_BACK_PIXEL, values);
    }

    inline void flush()
    {
        xcb_flush(conn);
    }
}