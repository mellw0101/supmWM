#include "event.h"
#include "tools/globals.h"
#include "tools/defines.h"

#include <xcb/xcb.h>
#include <xcb/xproto.h>

void handle_event()
{
    xcb_generic_event_t* ev;
    bool finished = false;
    while (!finished && (ev = xcb_wait_for_event(conn)))
    {
        switch (ev->response_type & ~0x80)
        {
            case XCB_KEY_PRESS:
            {
                auto e = reinterpret_cast<xcb_key_press_event_t*>(ev);
                if (e->detail == 24
                &&  e->state & ALT
                &&  e->state & SHIFT)
                {
                    finished = true;
                }

                break;
            }
        }

        free(ev);
    }    
}