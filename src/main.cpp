#include "tools/globals.h"
#include "tools/logger.h"
#include "tools/defines.h"
#include "keykodes.cpp"

// #include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <xcb/xcb.h>
#include <xcb/xproto.h>
// #include <array>

#include "window.cpp"

using namespace std;

xcb_connection_t* conn = nullptr;
xcb_screen_t* screen = nullptr;
xcb_screen_iterator_t iter;
const xcb_setup_t* setup = nullptr;
__key_codes__ key_code;
static window root;

void event_handler(xcb_generic_event_t* ev)
{
    switch (ev->response_type & ~0x80)
    {
        case XCB_EXPOSE:
        {
            break;
        }

        case XCB_KEY_PRESS:
        {
            auto e = reinterpret_cast<xcb_key_press_event_t *>(ev);

            if (e->detail == key_code.q)
            {
                exit(EXIT_SUCCESS);
            }

            break;
        }
    }
}

void setup_wm()
{
    conn = xcb_connect(nullptr, nullptr);
    setup = xcb_get_setup(conn);
    iter = xcb_setup_roots_iterator(setup);
    screen = iter.data;

    root = screen->root;

    xcb_void_cookie_t cookie;
    xcb_generic_error_t *error = nullptr;

    uint32_t values[1] = {XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY};
    cookie = root.apply_event_mask(values);

    error = xcb_request_check(conn, cookie);
    if (error)
    {
        loutE << "Error: Another window manager is already running or failed to set SubstructureRedirect mask" << loutEND;
        free(error);
    }

    uint32_t values2[1] =
    {
        XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT |
        XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY |  
        XCB_EVENT_MASK_STRUCTURE_NOTIFY |
        XCB_EVENT_MASK_BUTTON_PRESS |          
        XCB_EVENT_MASK_FOCUS_CHANGE
    };
    cookie = root.apply_event_mask(values2);

    error = nullptr;
    error = xcb_request_check(conn, cookie);
    if (error)
    {
        loutE << "Failed to set event mask" << '\n';
        free(error);
    }

    key_code.init();
}

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
        800,
        600,
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