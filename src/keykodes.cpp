#include <cstdint>
#include "tools/globals.h"
#include "tools/defines.h"
#include <xcb/xcb_keysyms.h>
#include <map>

using namespace std;

class __key_codes__
{
    public:
    // constructor and destructor.
        __key_codes__() 
        : keysyms(nullptr) {}

        ~__key_codes__()
        {
            free(keysyms);
        }

    // methods.
        void init()
        {
            keysyms = xcb_key_symbols_alloc(conn);
            if (keysyms)
            {
                map<uint32_t, xcb_keycode_t *> key_map = {
                    { A,            &a         },
                    { B,            &b         },
                    { C,            &c         },
                    { D,            &d         },
                    { E,            &e         },
                    { F,            &f         },
                    { G,            &g         },
                    { H,            &h         },
                    { I,            &i         },
                    { J,            &j         },
                    { K,            &k         },
                    { L,            &l         },
                    { M,            &m         },
                    { _N,           &n         },
                    { O,            &o         },
                    { P,            &p         },
                    { Q,            &q         },
                    { R,            &r         },
                    { S,            &s         },
                    { T,            &t         },
                    { U,            &u         },
                    { V,            &v         },
                    { W,            &w         },
                    { _X,           &x         },
                    { _Y,           &y         },
                    { Z,            &z         },

                    { SPACE_BAR,    &space_bar },
                    { ENTER,        &enter     },
                    { DELETE,       &_delete   },

                    { F11,          &f11       },
                    { N_1,          &n_1       },
                    { N_2,          &n_2       },
                    { N_3,          &n_3       },
                    { N_4,          &n_4       },
                    { N_5,          &n_5       },
                    { R_ARROW,      &r_arrow   },
                    { L_ARROW,      &l_arrow   },
                    { U_ARROW,      &u_arrow   },
                    { D_ARROW,      &d_arrow   },
                    { TAB,          &tab       },
                    { SUPER_L,      &super_l   },
                    { MINUS,        &minus     },
                    { UNDERSCORE,   &underscore}
                };
                
                for (auto &pair : key_map)
                {
                    xcb_keycode_t * keycode = xcb_key_symbols_get_keycode(keysyms, pair.first);
                    if (keycode)
                    {
                        *(pair.second) = *keycode;
                        free(keycode);
                    }
                }
            }
        }

        constexpr uint8_t char_to_keycode__(int8_t c)
        {
            switch (c)
            {
                case 'a': return this->a;
                case 'b': return this->b;
                case 'c': return this->c;
                case 'd': return this->d;
                case 'e': return this->e;
                case 'f': return this->f;
                case 'g': return this->g;
                case 'h': return this->h;
                case 'i': return this->i;
                case 'j': return this->j;
                case 'k': return this->k;
                case 'l': return this->l;
                case 'm': return this->m;
                case 'n': return this->n;
                case 'o': return this->o;
                case 'p': return this->p;
                case 'q': return this->q;
                case 'r': return this->r;
                case 's': return this->s;
                case 't': return this->t;
                case 'u': return this->u;
                case 'v': return this->v;
                case 'w': return this->w;
                case 'x': return this->x;
                case 'y': return this->y;
                case 'z': return this->z;
                case '-': return this->minus;
                case ' ': return this->space_bar;
            }

            return (uint8_t)0;
        }

    // variabels.
        xcb_keycode_t
            a{}, b{}, c{}, d{}, e{}, f{}, g{}, h{}, i{}, j{}, k{}, l{}, m{},
            n{}, o{}, p{}, q{}, r{}, s{}, t{}, u{}, v{}, w{}, x{}, y{}, z{},
            
            space_bar{}, enter{},

            f11{}, n_1{}, n_2{}, n_3{}, n_4{}, n_5{}, r_arrow{},
            l_arrow{}, u_arrow{}, d_arrow{}, tab{}, _delete{},
            super_l{}, minus{}, underscore{};

    private:
    // variabels.
        xcb_key_symbols_t * keysyms;
};
