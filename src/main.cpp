#include "keykodes.cpp"
#include "tools/globals.h"
#include "event.h"

#include <cstdlib>
#include <xcb/xcb.h>
#include <xcb/xproto.h>

using namespace std;

xcb_connection_t* conn;

class window
{
    private:
        uint32_t _window;

    public:

};

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

    // Create the window
    xcb_window_t window = xcb_generate_id(conn);
    uint32_t values[2] = {screen->white_pixel, XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_EXPOSURE};
    xcb_create_window(conn, XCB_COPY_FROM_PARENT, window, screen->root,
                      0, 0, 800, 600, 0, XCB_WINDOW_CLASS_INPUT_OUTPUT,
                      screen->root_visual, XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK, values);

    // Map the window (make it visible)
    xcb_map_window(conn, window);

    // Flush commands to X server
    xcb_flush(conn);

    handle_event();

    // Close the connection to the X server
    xcb_destroy_window(conn, window);
    xcb_disconnect(conn);
    return 0;
}