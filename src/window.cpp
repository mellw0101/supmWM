#include "tools/globals.h"

#include <cstdint>
#include <xcb/xcb.h>
#include <xcb/xproto.h>

class window
{
    private:
        uint32_t _window;

    public:
    /* Constructor */
        window() {}

    /* Operators   */
        operator uint32_t()
        {
            return _window;
        }
    
        // Overload the assignment operator for uint32_t
        window& operator=(uint32_t new_window)
        {
            _window = new_window;
            return *this;
        }

        void create_simple(
            uint32_t parent,
            int16_t  x,
            int16_t  y,
            uint16_t width,
            uint16_t height)
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
                0,
                XCB_WINDOW_CLASS_INPUT_OUTPUT,
                screen->root_visual,
                0,
                nullptr
            );
            xcb_flush(conn);
        }

        void map()
        {
            xcb_map_window(conn, _window);
            xcb_flush(conn);
        }

        void set_back_color(uint32_t color)
        {
            uint32_t values[1] = {color};
            xcb_configure_window(conn, _window, XCB_CW_BACK_PIXEL, values);
            xcb_flush(conn);
        }

        void apply_event_mask(uint32_t mask)
        {
            uint32_t values[1] = {mask};
            xcb_configure_window(conn, _window, XCB_CW_EVENT_MASK, values);
            xcb_flush(conn);
        }
};