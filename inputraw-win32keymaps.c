#include "minir.h"
#ifdef WNDPROT_WINDOWS
#include <windows.h>
#include "libretro.h"

static bool initialized=false;
static unsigned int scancode_to_libretro_g[256];
static unsigned int libretro_to_scancode_g[RETROK_LAST];
//GetKeyboardLayout(GetCurrentThreadId())
//guess whether the mingw headers include these
//I don't use all of them, but no reason not to include them.
#ifndef MAPVK_VK_TO_VSC
#define MAPVK_VK_TO_VSC 0
#endif
#ifndef MAPVK_VSC_TO_VK
#define MAPVK_VSC_TO_VK 1
#endif
#ifndef MAPVK_VK_TO_CHAR
#define MAPVK_VK_TO_CHAR 2
#endif
#ifndef MAPVK_VSC_TO_VK_EX
#define MAPVK_VSC_TO_VK_EX 3
#endif
#ifndef MAPVK_VK_TO_VSC_EX
#define MAPVK_VK_TO_VSC_EX 4
#endif

#ifndef VK_OEM_PLUS
#define VK_OEM_PLUS 0xBB
#endif
#ifndef VK_OEM_COMMA
#define VK_OEM_COMMA 0xBC
#endif
#ifndef VK_OEM_MINUS
#define VK_OEM_MINUS 0xBD
#endif
#ifndef VK_OEM_PERIOD
#define VK_OEM_PERIOD 0xBE
#endif

struct {
	uint16_t libretro;
	uint8_t virtual;
} const map_raw[]={
	//I have a feeling half of those are bogus. A whole pile of keys don't map to virtual keys for
	//me, and some of them are mapped to idiocy - numpad numbers are mapped to arrows and stuff.
	{ RETROK_BACKSPACE, VK_BACK }, { RETROK_TAB, VK_TAB }, { RETROK_CLEAR, VK_CLEAR },
	{ RETROK_RETURN, VK_RETURN }, { RETROK_PAUSE, VK_PAUSE }, { RETROK_ESCAPE, VK_ESCAPE },
	{ RETROK_SPACE, VK_SPACE }, /*{ RETROK_EXCLAIM,  },*/ /*{ RETROK_QUOTEDBL,  },*/
	/*{ RETROK_HASH,  },*/ /*{ RETROK_DOLLAR,  },*/ /*{ RETROK_AMPERSAND,  },*/
	/*{ RETROK_QUOTE,  },*/ /*{ RETROK_LEFTPAREN,  },*/ /*{ RETROK_RIGHTPAREN,  },*/
	/*{ RETROK_ASTERISK,  },*/ { RETROK_PLUS, VK_OEM_PLUS }, { RETROK_COMMA, VK_OEM_COMMA },
	{ RETROK_MINUS, VK_OEM_MINUS }, { RETROK_PERIOD, VK_OEM_PERIOD }, /*{ RETROK_SLASH,  },*/
	{ RETROK_0, '0' }, { RETROK_1, '1' }, { RETROK_2, '2' },
	{ RETROK_3, '3' }, { RETROK_4, '4' }, { RETROK_5, '5' },
	{ RETROK_6, '6' }, { RETROK_7, '7' }, { RETROK_8, '8' },
	{ RETROK_9, '9' },
	/*{ RETROK_COLON,  },*/ /*{ RETROK_SEMICOLON,  },*/ /*{ RETROK_LESS,  },*/
	/*{ RETROK_EQUALS,  },*/ /*{ RETROK_GREATER,  },*/ /*{ RETROK_QUESTION,  },*/
	/*{ RETROK_AT,  },*/ /*{ RETROK_LEFTBRACKET,  },*/ /*{ RETROK_BACKSLASH,  },*/
	/*{ RETROK_RIGHTBRACKET,  },*/ /*{ RETROK_CARET,  },*/ /*{ RETROK_UNDERSCORE,  },*/
	/*{ RETROK_BACKQUOTE,  },*/
	{ RETROK_a, 'A' }, { RETROK_b, 'B' }, { RETROK_c, 'C' }, { RETROK_d, 'D' },
	{ RETROK_e, 'E' }, { RETROK_f, 'F' }, { RETROK_g, 'G' }, { RETROK_h, 'H' },
	{ RETROK_i, 'I' }, { RETROK_j, 'J' }, { RETROK_k, 'K' }, { RETROK_l, 'L' },
	{ RETROK_m, 'M' }, { RETROK_n, 'N' }, { RETROK_o, 'O' }, { RETROK_p, 'P' },
	{ RETROK_q, 'Q' }, { RETROK_r, 'R' }, { RETROK_s, 'S' }, { RETROK_t, 'T' },
	{ RETROK_u, 'U' }, { RETROK_v, 'V' }, { RETROK_w, 'W' }, { RETROK_x, 'X' },
	{ RETROK_y, 'Y' }, { RETROK_z, 'Z' },
	{ RETROK_DELETE, VK_DELETE },
	
	{ RETROK_KP0, VK_NUMPAD0 }, { RETROK_KP1, VK_NUMPAD1 }, { RETROK_KP2, VK_NUMPAD2 },
	{ RETROK_KP3, VK_NUMPAD3 }, { RETROK_KP4, VK_NUMPAD4 }, { RETROK_KP5, VK_NUMPAD5 },
	{ RETROK_KP6, VK_NUMPAD6 }, { RETROK_KP7, VK_NUMPAD7 }, { RETROK_KP8, VK_NUMPAD8 },
	{ RETROK_KP9, VK_NUMPAD9 },
	{ RETROK_KP_PERIOD, VK_DECIMAL }, { RETROK_KP_DIVIDE, VK_DIVIDE },
	{ RETROK_KP_MULTIPLY, VK_MULTIPLY }, { RETROK_KP_MINUS, VK_SUBTRACT },
	{ RETROK_KP_PLUS, VK_ADD }, /*{ RETROK_KP_ENTER,  },*/
	/*{ RETROK_KP_EQUALS,  },*/
	
	{ RETROK_UP, VK_UP }, { RETROK_DOWN, VK_DOWN }, { RETROK_RIGHT, VK_RIGHT }, { RETROK_LEFT, VK_LEFT },
	{ RETROK_INSERT, VK_INSERT },
	{ RETROK_HOME, VK_HOME }, { RETROK_END, VK_END },
	{ RETROK_PAGEUP, VK_PRIOR }, { RETROK_PAGEDOWN, VK_NEXT },
	
	{ RETROK_F1, VK_F1 },   { RETROK_F2, VK_F2 },   { RETROK_F3, VK_F3 },   { RETROK_F4, VK_F4 },  
	{ RETROK_F5, VK_F5 },   { RETROK_F6, VK_F6 },   { RETROK_F7, VK_F7 },   { RETROK_F8, VK_F8 },  
	{ RETROK_F9, VK_F9 },   { RETROK_F10, VK_F10 }, { RETROK_F11, VK_F11 }, { RETROK_F12, VK_F12 },
	{ RETROK_F13, VK_F13 }, { RETROK_F14, VK_F14 }, { RETROK_F15, VK_F15 },
	
	{ RETROK_NUMLOCK, VK_NUMLOCK }, { RETROK_CAPSLOCK, VK_CAPITAL }, { RETROK_SCROLLOCK, VK_SCROLL },
	{ RETROK_RSHIFT, VK_RSHIFT }, { RETROK_LSHIFT, VK_LSHIFT },
	{ RETROK_RCTRL, VK_RCONTROL }, { RETROK_LCTRL, VK_LCONTROL },
	{ RETROK_RALT, VK_RMENU }, { RETROK_LALT, VK_LMENU },
	/*{ RETROK_RMETA,  },*/ /*{ RETROK_LMETA,  },*/
	/*{ RETROK_LSUPER,  },*/ /*{ RETROK_RSUPER,  },*/
	/*{ RETROK_MODE,  },*/ /*{ RETROK_COMPOSE,  },*/
	
	/*{ RETROK_HELP,  },*/ { RETROK_PRINT, VK_SNAPSHOT }, /*{ RETROK_SYSREQ,  },*/
	/*{ RETROK_BREAK,  },*/ /*{ RETROK_MENU,  },*/ { RETROK_POWER, VK_SLEEP },
	/*{ RETROK_EURO,  },*/ /*{ RETROK_UNDO,  },*/
};

static void create_map()
{
	if (initialized) return;
	for (int i=0;i<sizeof(map_raw)/sizeof(*map_raw);i++)
	{
		int scancode=MapVirtualKey(map_raw[i].virtual, MAPVK_VK_TO_VSC);
		//Ex gives me different answers for RETROK_PAUSE, RETROK_KP_DIVIDE, RETROK_RCTRL, RETROK_RALT, RETROK_POWER.
		//Of those, RETROK_PAUSE is the only one that doesn't just add 0xE0, and I am unsure on how to poll them.
		//It's not worth it.
		if (!scancode) continue;
		libretro_to_scancode_g[map_raw[i].libretro]=scancode;
		scancode_to_libretro_g[scancode]=map_raw[i].libretro;
	}
	initialized=true;
}

static unsigned int keyboard_num_keyboards(struct inputraw * this) { return 0; }
static unsigned int keyboard_num_keys(struct inputraw * this) { return 256; }

static void keyboard_get_map(struct inputraw * this_, const unsigned int ** keycode_to_libretro,
                                                      const unsigned int ** libretro_to_keycode)
{
	if (keycode_to_libretro) *keycode_to_libretro=scancode_to_libretro_g;
	if (libretro_to_keycode) *libretro_to_keycode=libretro_to_scancode_g;
}

void _inputraw_windows_keyboard_create_shared(struct inputraw * this)
{
	create_map();
	this->keyboard_num_keyboards=keyboard_num_keyboards;
	this->keyboard_num_keys=keyboard_num_keys;
	this->keyboard_get_map=keyboard_get_map;
}


unsigned int _inputraw_translate_key(unsigned int keycode)
{
	create_map();
	return scancode_to_libretro_g[keycode];
}
#endif
