#include "keykodes.cpp"

// #include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <xcb/xcb.h>
#include <xcb/xproto.h>
// #include <array>

#include "tools/globals.h"
#include "window.cpp"
#include "event.h"

using namespace std;

xcb_connection_t* conn;
xcb_screen_t *screen;

int setup_wm()
{
    conn = xcb_connect(NULL, NULL);
    if (xcb_connection_has_error(conn))
    {
        cerr << "Error: Can't open display" << endl;
        return -1;
    }

    const xcb_setup_t *setup = xcb_get_setup(conn);
    xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);
    screen = iter.data;

    return 0;
}

int main()
{
    if (setup_wm() != 0)
    {
        loutE << "Failed to init wm" << '\n';
    }
    
    // uint32_t window = xcb_generate_id(conn);
    // uint32_t values[2] = {screen->white_pixel, XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_EXPOSURE};
    // xcb_create_window
    // (
    //     conn,
    //     XCB_COPY_FROM_PARENT,
    //     window,
    //     screen->root,
    //     0,
    //     0,
    //     screen->width_in_pixels,
    //     screen->height_in_pixels,
    //     0,
    //     XCB_WINDOW_CLASS_INPUT_OUTPUT,
    //     screen->root_visual,
    //     XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK,
    //     values
    // );

    // // Map the window (make it visible)
    // xcb_map_window(conn, window);

    // // Flush commands to X server
    // xcb_flush(conn);

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