#include "keykodes.cpp"

// #include <cstddef>
// #include <cstdint>
#include <cstdlib>
#include <xcb/xcb.h>
#include <xcb/xproto.h>
// #include <array>

#include "tools/globals.h"
#include "window.cpp"
#include "event.h"

using namespace std;

xcb_connection_t* conn;

int main()
{
    conn = xcb_connect(NULL, NULL);
    if (xcb_connection_has_error(conn))
    {
        cerr << "Error: Can't open display" << endl;
        return -1;
    }

    const xcb_setup_t* setup = xcb_get_setup(conn);
    xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);
    xcb_screen_t* screen = iter.data;

    window window;

    window.create_simple(screen->root, 0, 0, screen->width_in_pixels, screen->height_in_pixels);
    window.set_back_color(screen->white_pixel);
    window.apply_event_mask(XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_EXPOSURE);
    window.map();

    handle_event();

    // Close the connection to the X server
    xcb_destroy_window(conn, window);
    xcb_disconnect(conn);
    return 0;
}