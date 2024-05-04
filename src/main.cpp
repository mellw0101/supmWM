#include "tools/globals.h"
#include "tools/defines.h"

#include <xcb/xcb.h>
#include <xcb/xproto.h>

#include "window.cpp"
#include "tools/logger.h"

using namespace std;

xcb_connection_t* conn = nullptr;
xcb_screen_t* screen = nullptr;
xcb_screen_iterator_t iter;
const xcb_setup_t* setup = nullptr;

static void event_handler(xcb_generic_event_t* ev)
{
    uint8_t evType = ev->response_type & ~0x80;
    switch (evType)
    {
        case XCB_EXPOSE:
        {

        }
    }
}

static void setup_wm()
{
    conn = xcb_connect(nullptr, nullptr);
    setup = xcb_get_setup(conn);
    iter = xcb_setup_roots_iterator(setup);
    screen = iter.data;

    xcb_void_cookie_t cookie = xcb_change_window_attributes_checked
    (
        conn,
        screen->root,
        XCB_CW_EVENT_MASK,
        (const uint32_t[1])
        {
            XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY
        }
    );

    xcb_generic_error_t *error = xcb_request_check(conn, cookie);
    if (error)
    {
        loutE << "Error: Another window manager is already running or failed to set SubstructureRedirect mask" << loutEND;
        free(error);
    }
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
    }
}
