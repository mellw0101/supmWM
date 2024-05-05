/* GLOBALS_H */
#pragma once

#include <iostream>
#include <xcb/xcb.h>
#include <xcb/xproto.h>

#include "logger.h"

extern xcb_connection_t* conn;
extern const xcb_setup_t* setup;
extern xcb_screen_iterator_t iter;
extern xcb_screen_t* screen;