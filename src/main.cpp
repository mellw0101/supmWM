#include "keykodes.cpp"
#include "tools/globals.h"
#include "event.h"

#include <cstdint>
#include <cstdlib>
#include <xcb/xcb.h>
#include <xcb/xproto.h>

using namespace std;

xcb_connection_t* conn;
xcb_screen_t* screen;

class window
{
    private:
        uint32_t _window;

    public:
        window() {}

        operator uint32_t()
        {
            return _window;
        }
    
        window& operator=(uint32_t new_window)
        {
            _window = new_window;
            return *this;
        }

        void create_window(
            uint32_t parent,
            int16_t  x,
            int16_t  y,
            uint16_t width,
            uint16_t height,
            uint16_t border_width = 0,
            uint16_t _class = XCB_WINDOW_CLASS_INPUT_OUTPUT,
            uint32_t visual = 0L,
            uint32_t value_mask = 0,
            const void* value_list = nullptr)
        {
            _window = xcb_generate_id(conn);

            xcb_create_window
            (
                conn,
                XCB_COPY_FROM_PARENT,
                _window,
                parent,
                x,
                y,
                width,
                height,
                border_width,
                _class,
                visual,
                value_mask,
                value_list
            );
            xcb_flush(conn);
        }
};

void setup_wm()
{
    conn = xcb_connect(NULL, NULL);
    if (xcb_connection_has_error(conn))
    {
        cerr << "Error: Can't open display" << endl;
        return;
    }
    const xcb_setup_t* setup = xcb_get_setup(conn);
    xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);
    xcb_screen_t* screen = iter.data;
}

int main()
{
    setup_wm();

    // Create the window
    uint32_t values[2] = {screen->white_pixel, XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_EXPOSURE};
    xcb_window_t window = xcb_generate_id(conn);
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