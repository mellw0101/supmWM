#include "keykodes.cpp"

// #include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <xcb/xcb.h>
#include <xcb/xproto.h>
// #include <array>

#include "window.cpp"

using namespace std;

int main()
{
    xcb_connection_t *connection = xcb_connect(NULL, NULL);
    if (xcb_connection_has_error(connection))
    {
        std::cerr << "Error: Can't open display" << std::endl;
        return -1;
    }

    const xcb_setup_t *setup = xcb_get_setup(connection);
    xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);
    xcb_screen_t *screen = iter.data;

    // Create the window
    xcb_window_t window = xcb_generate_id(connection);
    uint32_t values[2] = {screen->white_pixel, XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_EXPOSURE};
    xcb_create_window
    (
        connection,
        XCB_COPY_FROM_PARENT,
        window,
        screen->root,
        0,
        0,
        screen->width_in_pixels,
        screen->height_in_pixels,
        0,
        XCB_WINDOW_CLASS_INPUT_OUTPUT,
        screen->root_visual,
        XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK,
        values
    );

    // Map the window (make it visible)
    xcb_map_window(connection, window);

    // Flush commands to X server
    xcb_flush(connection);

    // Event loop
    xcb_generic_event_t *event;
    bool finished = false;
    while (!finished && (event = xcb_wait_for_event(connection)))
    {
        switch (event->response_type & ~0x80)
        {
            case XCB_EXPOSE:
                // Window exposed, redraw (not necessary here since it's just white)
                break;
            case XCB_KEY_PRESS:
            {
                xcb_key_press_event_t *kp = (xcb_key_press_event_t *)event;
                if (kp->detail == 24) // 'q' key on a QWERTY keyboard
                {
                    finished = true;
                }
                break;
            }
        }
        free(event);
    }

    // Close the connection to the X server
    xcb_destroy_window(connection, window);
    xcb_disconnect(connection);
    return 0;
}