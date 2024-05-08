#include <cstdint>

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
};