#include <xcb/xcb.h>
#include <xcb/xproto.h>
#include "tools/globals.h"
#include "tools/defines.h"

class window
{
    private:
        u32 window_;

    public:
        window()
        {}

        operator uint32_t()
        {
            return window_;
        }
    
        window& operator=(uint32_t new_window) // Overload the assignment operator for uint32_t
        {
            window_ = new_window;
            return *this;
        }

        xcb_void_cookie_t apply_event_mask(const void* value_list)
        {
            xcb_void_cookie_t cookie = xcb_change_window_attributes_checked
            (
                conn,
                window_,
                XCB_CW_EVENT_MASK,
                value_list
            );

            return cookie;
        }
};