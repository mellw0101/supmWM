#include "tools/globals.h"
#include "tools/defines.h"

#include <xcb/xcb.h>

#include "window.cpp"
#include "tools/logger.h"

using namespace std;

xcb_connection_t* conn = nullptr;
xcb_screen_t* screen = nullptr;
const xcb_setup_t* setup = nullptr;

static void quit()
{
    xcb_flush(conn);

    xcb_generic_event_t* ev;
    
    while (xcb_poll_for_event(conn));
}

int main()
{
    conn = xcb_connect(nullptr, nullptr);
    if (xcb_connection_has_error(conn) == 0)
    {
        cout << "SuccessBALLA\n";
    }

    setup = xcb_get_setup(conn);
    iter = xcb_setup_roots_iterator(setup);
    screen = iter.data;

    loutE << "screen->width_in_pixels" << screen->width_in_pixels << " screen->height_in_pixels" << screen->height_in_pixels << '\n';
}
