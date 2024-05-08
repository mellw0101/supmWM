/* GLOBALS_H */
#pragma once

#include <cstdint>
#include <iostream>
#include <xcb/xcb.h>
#include <xcb/xproto.h>
#include <xcb/xcb_ewmh.h>

#include "logger.h"

extern xcb_connection_t* conn;
extern xcb_ewmh_connection_t* ewmh;
extern const xcb_setup_t* setup;
extern xcb_screen_iterator_t iter;
extern xcb_screen_t* screen;
