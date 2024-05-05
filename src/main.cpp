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

static void event_handler(xcb_generic_event_t* ev)
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

static void setup_wm()
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
    setup_wm();

    xcb_generic_event_t* ev = nullptr;

    while (true)
    {
        ev = xcb_wait_for_event(conn);
        if (!ev) continue;

        event_handler(ev);
        free(ev);
    }
}
