#include "tools/defines.h"

class window
{
    private:
        u32 window_;

    operator u32()
    {
        return window_;
    }
};