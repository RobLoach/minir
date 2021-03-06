#ifdef CONFIG_HEADER
union { struct {
char* input_joy[128];
char* input_joy_auto[128];
char* input_savestate_load[10];
char* input_savestate_save[10];
char* input_savestate_slot_load;
char* input_savestate_slot_save;
char* input_savestate_slot_next;
char* input_savestate_slot_prev;
char* input_savestate_slot_next_10;
char* input_savestate_slot_prev_10;
char* input_savestate_manager;
char* input_screenshot;
char* input_turbo;
char* input_turbo_toggle;
char* input_pause;
char* input_frame_adv;
char* input_frame_adv_hold;
char* input_slowdown;
char* input_speedup;
char* input_rewind;
char* input_sram_write_manual;
}; char* inputs[293]; };
union { struct {
char* driver_video;
char* driver_audio;
char* driver_inputkb;
char* driver_inputmouse;
char* driver_inputjoy;
char* datefmt;
}; char* _strings[6]; };
union { struct {
}; int _ints[1]; };
union { struct {
unsigned int verbosity;
unsigned int video_scale;
unsigned int audio_latency;
unsigned int input_frame_adv_hold_delay;
unsigned int rewind_granularity;
unsigned int rewind_granularity_turbo;
unsigned int rewind_speedup;
unsigned int rewind_mem;
unsigned int rewind_per_push;
unsigned int cheat_mem;
}; unsigned int _uints[10]; };
union { struct {
}; unsigned int _enums[1]; };
union { struct {
bool readonly;
bool video_sync;
bool video_frameskip;
bool video_thread;
bool audio_sync;
bool auto_locate_cores;
bool joypad_block_opposing;
bool savestate_auto;
bool savestate_disable;
bool rewind_enable;
bool rewind_stats;
bool sram_write_unload;
bool sram_write_auto;
bool cursor_hide;
bool defocus_pause;
}; bool _bools[15]; };
union { struct {
}; char** _strlists[1]; };
union { struct {
}; char** _strmaps[1]; };
union { struct {
union { struct {
unsigned char input_joy_scope[128];
unsigned char input_joy_auto_scope[128];
unsigned char input_savestate_load_scope[10];
unsigned char input_savestate_save_scope[10];
unsigned char input_savestate_slot_load_scope;
unsigned char input_savestate_slot_save_scope;
unsigned char input_savestate_slot_next_scope;
unsigned char input_savestate_slot_prev_scope;
unsigned char input_savestate_slot_next_10_scope;
unsigned char input_savestate_slot_prev_10_scope;
unsigned char input_savestate_manager_scope;
unsigned char input_screenshot_scope;
unsigned char input_turbo_scope;
unsigned char input_turbo_toggle_scope;
unsigned char input_pause_scope;
unsigned char input_frame_adv_scope;
unsigned char input_frame_adv_hold_scope;
unsigned char input_slowdown_scope;
unsigned char input_speedup_scope;
unsigned char input_rewind_scope;
unsigned char input_sram_write_manual_scope;
}; unsigned char _scopes_input[293]; };
union { struct {
unsigned char driver_video_scope;
unsigned char driver_audio_scope;
unsigned char driver_inputkb_scope;
unsigned char driver_inputmouse_scope;
unsigned char driver_inputjoy_scope;
unsigned char datefmt_scope;
}; unsigned char _scopes_str[6]; };
union { struct {
}; unsigned char _scopes_int[1]; };
union { struct {
unsigned char verbosity_scope;
unsigned char video_scale_scope;
unsigned char audio_latency_scope;
unsigned char input_frame_adv_hold_delay_scope;
unsigned char rewind_granularity_scope;
unsigned char rewind_granularity_turbo_scope;
unsigned char rewind_speedup_scope;
unsigned char rewind_mem_scope;
unsigned char rewind_per_push_scope;
unsigned char cheat_mem_scope;
}; unsigned char _scopes_uint[10]; };
union { struct {
}; unsigned char _scopes_enum[1]; };
union { struct {
unsigned char readonly_scope;
unsigned char video_sync_scope;
unsigned char video_frameskip_scope;
unsigned char video_thread_scope;
unsigned char audio_sync_scope;
unsigned char auto_locate_cores_scope;
unsigned char joypad_block_opposing_scope;
unsigned char savestate_auto_scope;
unsigned char savestate_disable_scope;
unsigned char rewind_enable_scope;
unsigned char rewind_stats_scope;
unsigned char sram_write_unload_scope;
unsigned char sram_write_auto_scope;
unsigned char cursor_hide_scope;
unsigned char defocus_pause_scope;
}; unsigned char _scopes_bool[15]; };
union { struct {
}; unsigned char _scopes_strlist[1]; };
union { struct {
}; unsigned char _scopes_strmap[1]; };
}; unsigned char _scopes[324]; };
#define CONFIG_ENUM_INPUT \
	input_joy=0, \
	input_joy_auto=128, \
	input_savestate_load=256, \
	input_savestate_save=266, \
	input_savestate_slot_load=276, \
	input_savestate_slot_save=277, \
	input_savestate_slot_next=278, \
	input_savestate_slot_prev=279, \
	input_savestate_slot_next_10=280, \
	input_savestate_slot_prev_10=281, \
	input_savestate_manager=282, \
	input_screenshot=283, \
	input_turbo=284, \
	input_turbo_toggle=285, \
	input_pause=286, \
	input_frame_adv=287, \
	input_frame_adv_hold=288, \
	input_slowdown=289, \
	input_speedup=290, \
	input_rewind=291, \
	input_sram_write_manual=292, \
	input_count=293
#endif

#ifdef CONFIG_CLEAR_DEFAULTS
for (unsigned int i=0;i<128;i++)
{
this->inputs[0+i]=NULL;
}
this->inputs[0+0]=strdup("KB1::Z");
this->inputs[0+1]=strdup("KB1::A");
this->inputs[0+2]=strdup("KB1::Space");
this->inputs[0+3]=strdup("KB1::Return");
this->inputs[0+4]=strdup("KB1::Up");
this->inputs[0+5]=strdup("KB1::Down");
this->inputs[0+6]=strdup("KB1::Left");
this->inputs[0+7]=strdup("KB1::Right");
this->inputs[0+8]=strdup("KB1::X");
this->inputs[0+9]=strdup("KB1::S");
this->inputs[0+10]=strdup("KB1::Q");
this->inputs[0+11]=strdup("KB1::W");
for (unsigned int i=0;i<128;i++)
{
this->inputs[128+i]=NULL;
}
for (unsigned int i=0;i<10;i++)
{
this->inputs[256+i]=NULL;
}
this->inputs[256+0]=strdup("KB1::F1");
this->inputs[256+1]=strdup("KB1::F2");
this->inputs[256+2]=strdup("KB1::F3");
this->inputs[256+3]=strdup("KB1::F4");
this->inputs[256+4]=strdup("KB1::F5");
this->inputs[256+5]=strdup("KB1::F6");
this->inputs[256+6]=strdup("KB1::F7");
this->inputs[256+7]=strdup("KB1::F8");
this->inputs[256+8]=strdup("KB1::F9");
this->inputs[256+9]=strdup("KB1::F10");
for (unsigned int i=0;i<10;i++)
{
this->inputs[266+i]=NULL;
}
this->inputs[266+0]=strdup("KB1::ShiftL+F1, KB1::ShiftR+F1");
this->inputs[266+1]=strdup("KB1::ShiftL+F2, KB1::ShiftR+F2");
this->inputs[266+2]=strdup("KB1::ShiftL+F3, KB1::ShiftR+F3");
this->inputs[266+3]=strdup("KB1::ShiftL+F4, KB1::ShiftR+F4");
this->inputs[266+4]=strdup("KB1::ShiftL+F5, KB1::ShiftR+F5");
this->inputs[266+5]=strdup("KB1::ShiftL+F6, KB1::ShiftR+F6");
this->inputs[266+6]=strdup("KB1::ShiftL+F7, KB1::ShiftR+F7");
this->inputs[266+7]=strdup("KB1::ShiftL+F8, KB1::ShiftR+F8");
this->inputs[266+8]=strdup("KB1::ShiftL+F9, KB1::ShiftR+F9");
this->inputs[266+9]=strdup("KB1::ShiftL+F10, KB1::ShiftR+F10");
this->inputs[276]=NULL;
this->inputs[277]=NULL;
this->inputs[278]=NULL;
this->inputs[279]=NULL;
this->inputs[280]=NULL;
this->inputs[281]=NULL;
this->inputs[282]=NULL;
this->inputs[283]=strdup("KB1::F12");
this->inputs[284]=strdup("KB1::Tab");
this->inputs[285]=NULL;
this->inputs[286]=NULL;
this->inputs[287]=NULL;
this->inputs[288]=NULL;
this->inputs[289]=strdup("KB1::KP_Minus");
this->inputs[290]=strdup("KB1::KP_Plus");
this->inputs[291]=strdup("KB1::Backspace");
this->inputs[292]=NULL;
this->_strings[0]=NULL;
this->_strings[1]=NULL;
this->_strings[2]=NULL;
this->_strings[3]=NULL;
this->_strings[4]=NULL;
this->_strings[5]=strdup("%c");
this->_uints[0]=2;
this->_uints[1]=2;
this->_uints[2]=120;
this->_uints[3]=500;
this->_uints[4]=1;
this->_uints[5]=1;
this->_uints[6]=1;
this->_uints[7]=64;
this->_uints[8]=0;
this->_uints[9]=128;
this->_bools[0]=false;
this->_bools[1]=true;
this->_bools[2]=false;
this->_bools[3]=true;
this->_bools[4]=true;
this->_bools[5]=true;
this->_bools[6]=true;
this->_bools[7]=false;
this->_bools[8]=false;
this->_bools[9]=true;
this->_bools[10]=false;
this->_bools[11]=true;
this->_bools[12]=false;
this->_bools[13]=true;
this->_bools[14]=true;
#endif

#ifdef CONFIG_BYTECODE
#define CONFIG_BYTECODE_LEN 10679
0xED,0x5A,0xDB,0x96,0x1B,0xC7,0x75,0x45,0x83,0xA4,0x86,0x84,0x86,0xA4,0x24,0x4B,
0xD6,0xD5,0x76,0x59,0x23,0x85,0x64,0x84,0x01,0x09,0x60,0x6E,0xE6,0x38,0x51,0x28,
0x51,0x52,0x18,0x91,0xA2,0x4C,0x4A,0xA2,0x6C,0x45,0x41,0x0A,0xDD,0x05,0xA0,0x35,
0x7D,0x53,0x5F,0x06,0x03,0xE5,0xE6,0x95,0x97,0xE4,0x2F,0xF2,0x0D,0xC9,0xF2,0x53,
0x7E,0x20,0x0F,0x59,0xFE,0x85,0x3C,0xE7,0x2B,0x2C,0xEF,0x7D,0xAA,0x1A,0x68,0x0C,
0x49,0xBF,0xFA,0xC5,0xB3,0x56,0x2F,0x0C,0xBA,0xEA,0x9C,0x3A,0x75,0xEE,0x17,0x78,
0xDF,0x6F,0x7D,0xF5,0xD9,0xFD,0xDB,0xF7,0xBF,0x56,0xB7,0x4D,0x64,0x4A,0xA3,0xCA,
0x59,0x58,0xA8,0xC2,0xF8,0x65,0x98,0x26,0x9D,0xAD,0x38,0x4C,0xC2,0x5C,0xE1,0x4D,
0x92,0x96,0x2A,0x48,0x13,0xD3,0x53,0x0F,0xD3,0x58,0x76,0x25,0xD3,0x42,0x85,0x89,
0xDD,0xEF,0xA7,0xC9,0x24,0x9C,0xAA,0x49,0x18,0x19,0x65,0x4E,0xC2,0xA2,0xEC,0xAA,
0x71,0x45,0x00,0xC2,0x71,0xEB,0x21,0xF6,0x99,0x85,0xD2,0xB9,0x51,0xB1,0xCE,0x8F,
0x4C,0xA0,0xE6,0x61,0x39,0x53,0xF6,0xE8,0x5E,0x67,0x4B,0x7D,0xA0,0xFD,0x99,0xE2,
0x37,0x55,0xEA,0xA9,0xD2,0x59,0x16,0x85,0xA6,0x50,0x65,0xAA,0xD2,0x24,0x5A,0x10,
0x58,0xA5,0x19,0x49,0x52,0x41,0x98,0x83,0x38,0xBC,0xD3,0x93,0xD2,0x80,0x34,0x1C,
0xA5,0x93,0x40,0x76,0xF8,0x69,0x1C,0x9B,0xA4,0x2C,0x78,0xAE,0xF6,0xFD,0x2A,0xD7,
0xA5,0xC1,0xC6,0xC0,0x14,0x7E,0x1E,0x8E,0x0D,0xF6,0x16,0x2A,0x8B,0x74,0x92,0xE0,
0xF8,0x49,0x95,0xC8,0x15,0x7B,0xDE,0xF7,0x9D,0xAD,0xCF,0x9A,0xC0,0x3A,0x2A,0x70,
0x2A,0x90,0x27,0x2A,0x37,0x13,0x1C,0x01,0x22,0x74,0x7D,0xC3,0x4C,0x27,0x26,0xC2,
0x61,0x9A,0x97,0x33,0x45,0x72,0xA5,0xB4,0xF7,0xC5,0x15,0xEE,0xA5,0xB9,0xA9,0xB7,
0x59,0x5A,0x0B,0xDC,0x75,0xC1,0xBB,0x18,0x9D,0x5B,0x56,0x19,0x9C,0x5B,0x56,0x79,
0xCD,0xC5,0x74,0xC2,0x77,0x71,0x57,0x99,0x22,0x33,0x7E,0xA8,0xA3,0xFA,0xAA,0x89,
0x29,0x6A,0x3E,0x91,0x27,0x5D,0x41,0x64,0x8E,0x0D,0xAF,0x5F,0x58,0x84,0x38,0xB1,
0xB3,0x75,0xAB,0xE0,0x7E,0x9C,0x2B,0x12,0x5A,0x27,0x72,0x61,0xC8,0x1A,0x15,0x81,
0x3A,0x1E,0x94,0xA5,0x45,0x11,0x8E,0x21,0x9F,0x20,0x0F,0x8F,0x4D,0x5E,0xA8,0x49,
0x1A,0x45,0xE9,0xBC,0xB8,0x49,0x06,0x7C,0x11,0x06,0x26,0xBD,0xA9,0x6E,0x0B,0x6F,
0x87,0xB7,0xBF,0x7A,0xF4,0x75,0x57,0xDD,0xCF,0x4C,0xF2,0xD1,0x5D,0xFC,0xFB,0xD5,
0x5D,0x7C,0xFB,0xF2,0xE1,0x2C,0x96,0x7F,0x3E,0xBA,0x7D,0x07,0xEF,0x70,0x74,0x15,
0x84,0x00,0xF9,0xB4,0x8A,0x0A,0x23,0xFF,0xCB,0xAA,0xC5,0xF0,0x30,0xAD,0x92,0x40,
0x76,0xDD,0x49,0xB2,0xAA,0xBC,0xA9,0x1E,0xE8,0xB9,0xFC,0x27,0x88,0xED,0x9E,0xD5,
0xF7,0x2A,0x30,0xC7,0x0E,0xF5,0xC7,0xF6,0x2C,0x59,0x1B,0xD8,0xFF,0xFB,0x7D,0x7C,
0x76,0xB6,0xB0,0x53,0xC5,0x46,0x83,0xA7,0xFA,0x58,0x87,0x91,0xE6,0x4D,0xA0,0x0D,
0x8F,0xC2,0x24,0xC0,0x25,0x0E,0x15,0x36,0x3D,0x69,0xFD,0x6E,0x98,0x54,0x27,0x3D,
0x91,0xB0,0xD5,0x3E,0x88,0x21,0xCD,0x03,0x48,0x15,0x2C,0xA1,0x62,0x98,0x24,0xF0,
0xBE,0x87,0x82,0xF2,0x4B,0x98,0xEB,0x71,0x18,0x85,0xE5,0xE2,0xD0,0x8A,0x2A,0xCC,
0xC9,0xBA,0x44,0xB8,0x3B,0x36,0x54,0x6A,0x73,0xE2,0x9B,0xAC,0xB4,0xF2,0xCF,0x20,
0xC7,0x6D,0xC1,0xBF,0x64,0xA9,0x3B,0x00,0x92,0xC8,0x74,0x5E,0x86,0x7E,0x15,0x41,
0xF0,0xF6,0xB8,0xDC,0x44,0xBA,0xC4,0x2E,0x6A,0x93,0x40,0x3A,0xCA,0x41,0xDB,0x27,
0xA9,0x98,0x1D,0x50,0x56,0x05,0x29,0x21,0x3F,0xB0,0xFF,0xDB,0x0A,0x7C,0x2A,0xA8,
0xC8,0xA6,0x10,0x4B,0x38,0xD6,0x79,0x98,0x56,0x30,0xBA,0xD2,0xC4,0x62,0x7A,0xD7,
0xB1,0xF1,0x7A,0x48,0x5E,0x75,0xD5,0x7C,0x16,0xC2,0x80,0x96,0x50,0x26,0xA4,0x5E,
0x28,0x7F,0x16,0xA7,0x41,0x40,0xA4,0xD4,0x33,0x90,0xA2,0xF2,0x2A,0x49,0xF8,0xDD,
0x1A,0xB6,0x18,0xA1,0xF1,0x5E,0x2A,0x7C,0x0D,0xF2,0xA0,0x6D,0x19,0x6E,0x02,0x23,
0x9E,0x1A,0x10,0xD6,0xF6,0xBE,0xDF,0xBA,0x53,0x5E,0x81,0xA6,0x84,0x89,0xD0,0x6D,
0x82,0xB0,0xB4,0xF6,0x4E,0x43,0x3F,0x24,0x09,0x13,0xED,0xE3,0xF0,0x45,0x5A,0xA9,
0xC4,0x00,0xFC,0xB1,0x4D,0x2A,0x9C,0xC8,0xEA,0x5C,0x27,0x25,0xD4,0x13,0xF7,0x26,
0x11,0x53,0x1D,0x9B,0x6D,0x51,0xF9,0x49,0xE8,0xC3,0xD9,0x94,0x25,0xBD,0x09,0x58,
0x71,0xC7,0xEE,0xA6,0xFD,0x26,0x0B,0x71,0x1C,0x6A,0x9E,0xA7,0xC9,0xB4,0xBB,0xA4,
0x37,0x8A,0x70,0xC9,0x38,0x25,0x23,0x69,0x28,0x93,0x09,0x04,0xC8,0x6D,0x11,0x69,
0x9C,0xE4,0x69,0xDC,0x38,0x9B,0x7E,0xA1,0x80,0x18,0x8E,0x6A,0xE3,0x0B,0xCC,0x44,
0x57,0x51,0x09,0x4E,0x46,0x95,0x11,0xAD,0xC0,0x56,0xAB,0x35,0xC2,0xFF,0x50,0xEC,
0xD1,0xE2,0xC2,0x29,0x9A,0x9A,0x51,0x38,0x47,0x33,0x36,0x72,0x4C,0xAA,0x03,0xDC,
0x53,0x30,0xEB,0x63,0x13,0xD4,0x84,0x41,0xDF,0xA1,0x00,0x65,0x9A,0x06,0x10,0x0F,
0x30,0xFF,0x32,0xAD,0x20,0x79,0x91,0x41,0x7D,0x3D,0xD1,0x8E,0x42,0x4F,0x96,0x3C,
0x09,0x74,0xAC,0xA7,0x46,0x4C,0x34,0x31,0x4B,0xDD,0x3A,0xBD,0x48,0xA6,0xD5,0x3C,
0x83,0x5A,0x95,0xB3,0x2E,0xBF,0xE9,0xDA,0x45,0xAB,0x99,0x01,0x45,0xB9,0xF8,0x1F,
0x68,0x6B,0x52,0xC5,0xE3,0x5A,0x13,0xFD,0x48,0xC7,0x99,0xF3,0xB4,0x60,0x80,0xC6,
0x95,0x74,0x91,0x26,0x62,0x1A,0x61,0x82,0x3B,0x81,0x0D,0x87,0xF5,0x79,0xB8,0xA8,
0xC9,0x4B,0x55,0xC0,0x2D,0x59,0xBE,0x57,0x09,0xB6,0x7B,0x43,0x07,0xB0,0x26,0x00,
0x41,0xAC,0x28,0xE6,0x54,0xF8,0x95,0xC0,0x1D,0xC1,0x3C,0x70,0x8F,0x98,0x97,0x6F,
0x7B,0xFF,0x61,0x39,0xEB,0x4C,0x67,0x3E,0x33,0xC2,0x08,0xEC,0xAE,0xB2,0x40,0xD7,
0x31,0x46,0x44,0x94,0x26,0xBE,0x71,0xA8,0xE1,0x47,0xCB,0xA2,0xA7,0x56,0xD6,0x00,
0xCA,0xE6,0x79,0x48,0xDE,0x11,0x0B,0x1D,0xDF,0x18,0x9C,0x82,0x2E,0x84,0x09,0x28,
0x0F,0x03,0x17,0x0A,0x60,0x54,0x38,0x23,0xA5,0x2B,0x86,0x8F,0x97,0x15,0xAA,0x32,
0x38,0xF2,0xBE,0x76,0x2F,0x21,0x02,0x18,0x65,0xB2,0x3D,0x8D,0xD2,0xB1,0x8E,0x7A,
0x9D,0x33,0x9B,0xAD,0xD6,0x79,0x5C,0x2F,0x60,0x70,0xA1,0x9A,0x7F,0x61,0xF2,0x71,
0x5A,0xC0,0xF2,0x79,0xD2,0x2C,0x9D,0xAB,0xB8,0x82,0x35,0xB9,0x20,0x47,0x5E,0xC2,
0xC6,0x96,0xC1,0x8E,0x84,0x03,0xFB,0xF0,0xA6,0xBA,0x93,0xF8,0x11,0xAC,0x95,0x9E,
0x39,0xB7,0xEA,0x8A,0xF7,0x83,0x9B,0xF0,0x3A,0x47,0x46,0x0D,0x6D,0x0C,0x74,0xDA,
0x8A,0x70,0x33,0xD3,0xC9,0x14,0xE2,0x40,0x90,0xC9,0x19,0xDB,0x9C,0xB2,0x1A,0xF5,
0x4D,0xBA,0xC8,0x74,0x00,0x17,0x9F,0x65,0x62,0x06,0x4A,0x98,0x17,0x16,0x4D,0x95,
0x05,0xDE,0xBE,0xC3,0x3B,0x58,0xC3,0xBB,0x0C,0x5B,0xD0,0x3D,0x6C,0xBA,0xE1,0x36,
0xF5,0xD7,0x36,0x41,0xB8,0x65,0x5E,0xF9,0x36,0x26,0x41,0x0C,0x96,0x14,0xDA,0x98,
0x98,0x82,0xF7,0x5B,0x31,0x6F,0x86,0xC7,0xDA,0x42,0xAE,0x32,0xDE,0x7F,0x53,0x51,
0xA8,0x3A,0xE7,0xD6,0x3C,0xD7,0x8B,0xE2,0x5A,0xCF,0x45,0x6C,0x75,0xCB,0x2F,0x2B,
0x09,0x57,0x2B,0x63,0x2C,0xCC,0xF2,0x6A,0xD6,0x8E,0x16,0x57,0xC8,0x38,0x68,0x06,
0x5E,0xF7,0xEA,0x20,0xDB,0x08,0x50,0x0C,0xA0,0x92,0x57,0x84,0x8E,0x8D,0xA5,0x53,
0x9A,0xAE,0x82,0x1F,0xF4,0x19,0xAF,0xD5,0x4C,0x4B,0x60,0x33,0xB0,0x73,0x9F,0xBE,
0x58,0x78,0xF2,0xD1,0xE7,0x77,0xFE,0xA0,0x74,0x3B,0xAD,0xFA,0xEF,0xCC,0x85,0xE3,
0x5A,0xB2,0x6D,0xEF,0xBF,0xE9,0x69,0x11,0xDE,0xA3,0xA5,0xBF,0x2E,0xAA,0x2C,0x4B,
0xA9,0xF5,0x0B,0x48,0x07,0x7E,0x26,0xFC,0x8E,0x22,0x54,0x12,0x0D,0xAF,0xDB,0xF8,
0xD7,0x08,0x32,0xD6,0x5B,0xDE,0x7E,0x74,0x0F,0xF7,0xA4,0x4D,0xC0,0xF8,0x27,0x29,
0xF5,0x16,0xD6,0x40,0x0B,0x71,0x09,0x01,0x48,0x7B,0x24,0xCE,0xB8,0x19,0x15,0xDC,
0x49,0x10,0xBF,0xCB,0xA5,0x18,0x96,0xF1,0x8D,0xC1,0xFB,0x90,0x17,0xA5,0x73,0x87,
0xD3,0x55,0x9F,0x17,0xC6,0xE5,0x32,0x2B,0x4E,0xC1,0xA4,0xCE,0xB7,0x5A,0x9B,0x16,
0xDF,0xE8,0x98,0xD4,0x75,0x5A,0x1E,0xAE,0xC7,0xA7,0xF3,0xAC,0xBC,0x18,0xD1,0x85,
0x9B,0xCD,0x96,0xD7,0x71,0x5F,0x71,0x25,0x2F,0x6F,0xD8,0x61,0xE2,0xD3,0xFC,0x45,
0xEF,0xE0,0x49,0x98,0x1A,0x29,0xC4,0x8A,0x92,0x2E,0x66,0x1C,0xBA,0x7C,0x2D,0xD6,
0x47,0x94,0x1E,0x18,0x52,0x56,0x25,0x9D,0x9E,0xB8,0x35,0x71,0x0A,0xCA,0x0F,0x73,
0xBF,0x8A,0x8B,0x52,0x43,0x32,0x74,0xD5,0x4E,0x11,0xAE,0x06,0x36,0x6D,0x44,0x82,
0x32,0x36,0xEF,0x5E,0xEB,0x6C,0xB6,0xDA,0x97,0x2D,0x05,0x72,0x4E,0x71,0x14,0x66,
0xDE,0xFF,0xAE,0xC8,0xA0,0xB2,0x58,0xC5,0x96,0x4D,0x8E,0x47,0xB5,0xF6,0xA5,0x73,
0x0A,0x98,0x86,0xD9,0x55,0x53,0x04,0x22,0xC6,0x29,0x8D,0xB4,0xE5,0xD8,0x34,0xA8,
0x1E,0xA7,0x70,0x74,0xC5,0x1F,0x22,0xED,0x4E,0x49,0x2E,0x4B,0x1E,0x17,0xC3,0x53,
0xEB,0x32,0xCD,0x17,0x22,0x2A,0xE4,0x81,0xC6,0x24,0xC5,0x2C,0x15,0x7B,0x81,0xD3,
0xC8,0x8F,0x7A,0xEA,0xBE,0x24,0x98,0x55,0x9E,0xF0,0xE2,0x08,0x28,0xCE,0x23,0x52,
0x79,0xCD,0x49,0x66,0x20,0x96,0xC4,0x27,0x25,0xE3,0x8A,0x21,0x6A,0xB3,0x75,0x66,
0xD3,0xDE,0xCF,0x12,0x0A,0xC9,0x78,0xB5,0x64,0x34,0xD3,0xA0,0x4E,0xAB,0x4D,0xA9,
0x40,0xF7,0xFE,0xFF,0xA2,0xBC,0x18,0x31,0xB2,0x26,0xFE,0x62,0xB3,0x75,0xB6,0x63,
0x5F,0x50,0x36,0x6D,0xEF,0x0B,0x06,0x09,0xE5,0xEB,0x44,0x14,0xD8,0x1A,0x50,0x61,
0x90,0x38,0x48,0xD6,0x6A,0x05,0x12,0x32,0xFC,0x8E,0x8D,0xC8,0x82,0xEE,0xD3,0x29,
0x47,0xC1,0x58,0xE0,0x58,0x27,0x2A,0xD8,0xF0,0x3F,0xE7,0x5B,0xED,0x4B,0x8E,0x1E,
0x49,0x0D,0x8E,0xC6,0xDE,0x79,0x27,0x2C,0xAC,0x9D,0x79,0xBE,0xB9,0x16,0x23,0x9D,
0x30,0xCD,0xE5,0xB3,0x97,0x9B,0xCB,0x70,0x4C,0x74,0x8E,0x8F,0x56,0xFE,0x5B,0x57,
0x65,0x1A,0x23,0x8F,0xF1,0xC5,0xF6,0xA3,0x34,0x3D,0x92,0xF3,0x19,0xA3,0x8A,0x3A,
0xC8,0xC6,0x8C,0x43,0x48,0xB3,0x2B,0x9B,0x6E,0xE2,0x3F,0xDF,0xB9,0x84,0x34,0x92,
0x0C,0x21,0x85,0x61,0xC1,0xF7,0x40,0x82,0x47,0x09,0x25,0xFE,0xE0,0xFE,0x3D,0x1B,
0xFA,0x4B,0xE7,0xF7,0x26,0x10,0x9D,0x91,0xAC,0xE2,0x0A,0x8C,0x75,0xA6,0x6D,0xD6,
0x54,0x88,0x5F,0x96,0x50,0x61,0x0D,0x84,0x67,0x8E,0x17,0xDC,0x96,0x17,0x26,0x9A,
0xD8,0xB8,0xE8,0x4B,0x10,0xBD,0x6A,0x99,0x92,0x48,0xDE,0x71,0xAD,0x86,0x4A,0x81,
0x39,0x8C,0x24,0x86,0xDB,0x08,0x37,0xD1,0x31,0xF2,0x3D,0xE4,0x68,0x20,0xA1,0x78,
0x8A,0x4B,0xF1,0xCE,0xD5,0x21,0xE3,0xDC,0xF3,0xBC,0xFE,0x28,0x4A,0x7D,0x48,0x68,
0x24,0xE7,0xB7,0xCF,0xFD,0xFA,0xEC,0x73,0x67,0xCF,0x3D,0xB3,0x71,0xBE,0x75,0xC1,
0x3B,0xD3,0xEE,0x3C,0xBB,0x79,0xF1,0xD2,0xE5,0x17,0x5F,0xFA,0xE1,0xCB,0xAF,0x3C,
0xF7,0xEA,0xF3,0x3F,0x78,0xE1,0xB5,0xD7,0xDF,0xF8,0xD1,0x8F,0x7F,0xF2,0xD6,0xDB,
0x7F,0x76,0xE5,0xAA,0xBA,0xF6,0xD3,0xAD,0x37,0xFF,0xFC,0x9D,0xEE,0x76,0xEF,0xFA,
0xCE,0xEE,0xDE,0xFE,0xC1,0x8D,0x9F,0xF5,0x87,0x83,0x9B,0x87,0x3F,0xFF,0x8B,0xBF,
0x7C,0xF7,0xF6,0x07,0x1F,0x7E,0xF4,0xD7,0x7F,0x75,0xE7,0xD6,0xFB,0xEF,0xFD,0xCD,
0xC7,0x77,0xEF,0x7D,0x72,0xFF,0xB3,0xCF,0xBF,0x78,0xF4,0xE5,0xA7,0xBF,0xFC,0xC5,
0xC3,0x07,0xBF,0xFA,0xEA,0x6F,0xBF,0xFE,0xBB,0x11,0x7C,0xF2,0x74,0xF6,0xF7,0xA1,
0xF6,0xC7,0xDF,0x1C,0x45,0x31,0x9C,0x47,0x75,0x3C,0x3F,0xC9,0x16,0xDF,0x16,0xF9,
0x77,0xFF,0xF0,0x8F,0xFF,0xF4,0xCF,0xFF,0xD2,0x1F,0x55,0x59,0x7F,0x14,0x24,0xFD,
0x51,0x34,0xE9,0x8F,0xF2,0xB2,0x3F,0xD2,0xBA,0x3F,0x1A,0x8F,0xFB,0xA3,0x93,0x93,
0xFE,0x68,0xB1,0xE8,0x8F,0x0A,0xBC,0x2B,0x0C,0xD6,0xFB,0x58,0xC7,0x13,0x0D,0xF0,
0x89,0x27,0x1A,0xE2,0x73,0x38,0x00,0xFC,0x00,0xF0,0x03,0xC0,0x0F,0x00,0x3F,0x00,
0xFC,0x00,0xF0,0x03,0xC0,0x0F,0x00,0x3F,0x00,0x3C,0x1E,0x83,0xF5,0x3E,0xD6,0xF1,
0x44,0x03,0x7C,0xE2,0x89,0x00,0x9B,0x0F,0x87,0x80,0x1F,0x02,0x7E,0x08,0xF8,0x21,
0xE0,0x87,0x80,0x1F,0x02,0x7E,0x08,0xF8,0x21,0xE0,0x87,0x80,0xC7,0x63,0xB0,0xDE,
0xC7,0x3A,0x9E,0x68,0x80,0x4F,0x3C,0x11,0x60,0xF3,0xE1,0x0E,0xE0,0x77,0x00,0xBF,
0x03,0xF8,0x1D,0xC0,0xEF,0x00,0x7E,0x07,0xF0,0x3B,0x80,0xDF,0x01,0xFC,0x0E,0xE0,
0xF1,0x18,0xAC,0xF7,0xB1,0x8E,0x27,0x1A,0xE0,0x13,0x4F,0x04,0xD8,0x7C,0xB8,0x0B,
0xF8,0x5D,0xC0,0xEF,0x02,0x7E,0x17,0xF0,0xBB,0x80,0xDF,0x05,0xFC,0x2E,0xE0,0x77,
0x01,0xBF,0x0B,0x78,0x3C,0x06,0xEB,0x7D,0xAC,0xE3,0x89,0x06,0xF8,0xC4,0x13,0x01,
0x36,0x1F,0xEE,0x01,0x7E,0x0F,0xF0,0x7B,0x80,0xDF,0x03,0xFC,0x1E,0xE0,0xF7,0x00,
0xBF,0x07,0xF8,0x3D,0xC0,0xEF,0x01,0x1E,0x8F,0xC1,0x7A,0x1F,0xEB,0x78,0xA2,0x01,
0x3E,0xF1,0x44,0x80,0xCD,0x87,0xFB,0x80,0xDF,0x07,0xFC,0x3E,0xE0,0xF7,0x01,0xBF,
0x0F,0xF8,0x7D,0xC0,0xEF,0x03,0x7E,0x1F,0xF0,0xFB,0x80,0xC7,0x63,0xB0,0xDE,0xC7,
0x3A,0x9E,0x68,0x80,0x4F,0x3C,0x11,0x60,0xF3,0xE1,0x01,0xE0,0x0F,0x00,0x7F,0x00,
0xF8,0x03,0xC0,0x1F,0x00,0xFE,0x00,0xF0,0x07,0x80,0x3F,0x00,0xFC,0x01,0xE0,0xF1,
0x18,0xAC,0xF7,0xB1,0x8E,0x27,0x1A,0xE0,0x13,0x4F,0x04,0xD8,0x7C,0xB8,0xD1,0x6A,
0x3D,0x2B,0xF6,0x3A,0x82,0xC1,0x8E,0xB2,0xF6,0xCA,0xA0,0xFF,0xA4,0x9D,0x7F,0xD2,
0xCE,0x3F,0xBE,0x76,0xFE,0xFA,0xB9,0x95,0x76,0x8A,0x07,0xCD,0x18,0x55,0x9E,0x52,
0x21,0x8C,0xE1,0x5E,0x8F,0x54,0x64,0x26,0xE5,0x3B,0x79,0x38,0x9D,0x95,0x52,0x6A,
0x55,0xD9,0x3B,0x48,0xC0,0x92,0x9E,0x42,0x90,0x90,0x0D,0xCC,0xBD,0xC6,0x28,0xAF,
0x6C,0xC5,0x3D,0x4D,0xE0,0x8B,0x03,0x96,0xC0,0x91,0x2B,0x0B,0x05,0x1B,0x53,0x79,
0x13,0xB1,0x10,0xB8,0x7A,0xE7,0x4A,0x6C,0xC3,0x0E,0xE3,0x48,0x35,0x45,0x79,0x6B,
0x73,0x61,0x06,0x2F,0x86,0x15,0x83,0x80,0xC3,0xF0,0xEF,0xA7,0x15,0x62,0x14,0x6A,
0xDA,0xDE,0xB5,0x66,0x5D,0x6E,0x8B,0xC2,0x71,0x6E,0xCA,0x3C,0xB5,0x05,0x99,0x34,
0x76,0x98,0x60,0x58,0x7A,0xEF,0xBE,0xF3,0x40,0x88,0x0B,0x6D,0x5B,0xA8,0xCB,0x17,
0xB6,0x56,0x92,0xF6,0xD5,0x94,0x75,0x52,0x9E,0x56,0xD3,0x19,0xE3,0x20,0x09,0xF7,
0xDE,0x72,0x41,0x69,0xD5,0xBB,0x92,0xEA,0x70,0x61,0xD3,0xAF,0x19,0xE9,0x60,0xB2,
0xF1,0xCC,0x4B,0xB6,0x3A,0x18,0xC9,0x39,0xA3,0x14,0x59,0x24,0x7B,0x04,0xED,0xB3,
0x9D,0x76,0xA7,0xDF,0x1A,0xB4,0x86,0xAD,0x9D,0xD6,0x6E,0x6B,0xAF,0xB5,0xDF,0x3A,
0x68,0xFD,0xAC,0xD5,0xBF,0xB1,0xE1,0xB5,0x5E,0xB4,0xDC,0x66,0x79,0x8A,0x94,0x08,
0x91,0x8A,0xF1,0xEE,0xE9,0xFB,0x3B,0x8F,0xED,0xE7,0x7F,0x6D,0xEF,0x17,0x4C,0xD8,
0x73,0x23,0x1C,0xEE,0xDF,0xB8,0xA1,0x96,0xEB,0xAA,0x88,0x90,0x3F,0xD9,0x16,0x89,
0x1E,0x33,0xF9,0xAF,0x2B,0x97,0x55,0xBF,0x6C,0xD5,0x3E,0x61,0x1B,0x4D,0x00,0x7A,
0x9D,0x0D,0xEF,0xC5,0x57,0x1F,0x3B,0x0B,0x4B,0x42,0xE0,0x86,0xF7,0xD2,0x93,0x17,
0xF9,0x75,0xC3,0xFB,0xE1,0x93,0x17,0x13,0x73,0x52,0x6E,0x78,0x2F,0x3F,0x79,0x11,
0xA5,0xC7,0xB1,0x77,0x85,0xD7,0x60,0xDA,0x34,0x4F,0x95,0xAD,0x54,0x6A,0x82,0x94,
0x13,0x6C,0x19,0xC6,0x86,0xC4,0xBD,0xF2,0xC6,0x53,0x8F,0x18,0x91,0x51,0xAF,0xBE,
0xF1,0xD4,0x53,0xB0,0xEE,0x5D,0xA8,0x53,0x62,0xA0,0x7A,0xED,0xE5,0xD3,0x5B,0x91,
0x87,0xA2,0x9C,0xCF,0xDB,0xDE,0xBF,0x6E,0x3D,0x9C,0x31,0xE9,0x71,0xC9,0xD2,0x8A,
0xAB,0x6E,0x07,0xB4,0xF5,0xA1,0x31,0x6A,0x56,0x96,0xD9,0xCD,0xEB,0xD7,0xE7,0xF3,
0x79,0xCF,0xCF,0xA2,0xAA,0xE0,0xD3,0x43,0xAD,0x77,0x5D,0x58,0x8C,0x74,0xD2,0x5C,
0xF7,0x49,0xF8,0x75,0x14,0x76,0x13,0xF9,0x47,0x74,0x39,0x48,0x91,0x08,0xA3,0xDC,
0xD2,0xD2,0x1A,0x70,0xE5,0x12,0x16,0x90,0xB3,0x2D,0x53,0x76,0xA4,0x79,0xE7,0x36,
0x58,0x22,0x4F,0xE2,0xB2,0xED,0x45,0x4F,0x4F,0xEF,0x48,0x1B,0x1B,0x0D,0x4B,0x12,
0x61,0xA6,0x09,0x8A,0x7F,0xD1,0x40,0xC1,0x8C,0xB4,0xC9,0x76,0x5F,0xD1,0xDD,0x82,
0x0C,0xA9,0xBA,0xB2,0xC7,0x7E,0xAD,0x77,0x49,0xB6,0x2A,0x49,0xDE,0x66,0x6B,0xE3,
0xD2,0x8A,0x27,0x3C,0xCD,0xF3,0x3B,0x2B,0x4F,0x20,0x06,0x63,0xEB,0xFC,0x06,0x67,
0x60,0x4D,0x28,0x02,0xE7,0x06,0x6B,0x9A,0x2A,0x36,0x47,0x35,0xA6,0xAE,0x16,0x60,
0xD2,0x18,0xC7,0xCC,0x51,0x93,0xDE,0xAE,0x0B,0x49,0x6D,0xCB,0xC4,0xF5,0x23,0x78,
0xEC,0xF9,0xE7,0x57,0xEF,0xDC,0x01,0x6D,0xEF,0xDF,0xA4,0x24,0x5D,0xD5,0x04,0x2A,
0x21,0xA1,0x96,0x5B,0x74,0x1E,0xA8,0xCF,0xF9,0x66,0x7B,0xB1,0x88,0xE3,0x20,0xD8,
0x9E,0xCD,0xE2,0xB8,0x28,0x7A,0x19,0x9B,0xD9,0x3C,0xCC,0x47,0x75,0x9C,0xC6,0xE1,
0x77,0x44,0x06,0xE3,0x07,0x67,0x50,0x5E,0xB1,0xAD,0x08,0x2D,0xCB,0x23,0x9D,0x81,
0xEE,0xED,0xAD,0x2D,0xA9,0x44,0xD0,0xB0,0x4D,0xD8,0x36,0x1A,0x9B,0x09,0xCD,0x9C,
0x4C,0x81,0x56,0xE1,0x54,0x36,0xA0,0xA1,0x2F,0xAF,0x3B,0x0F,0xB9,0xA2,0xA5,0xED,
0xFD,0xCF,0xD6,0x67,0x15,0x4A,0xD7,0x9A,0x1F,0xC5,0x7A,0x91,0xAA,0xA4,0x92,0x10,
0xDE,0x4B,0x35,0x02,0x02,0xCA,0x65,0x41,0xED,0xFA,0x37,0x11,0xFB,0x86,0xAE,0x11,
0x68,0x3B,0xCD,0x0D,0x1C,0x56,0x43,0x26,0x91,0x76,0x0D,0x0E,0x57,0xD2,0x42,0x4B,
0x3E,0x48,0x6C,0x9F,0x05,0x98,0xC0,0x89,0x88,0xA5,0x11,0xC8,0x58,0x93,0x4D,0x99,
0x4E,0xA7,0xFC,0x90,0x15,0x69,0xE1,0xAC,0xED,0x25,0xEF,0x50,0x15,0x82,0x1D,0x01,
0x6F,0xF7,0x86,0xCB,0x4E,0x64,0x71,0xC3,0xFB,0xD1,0x0B,0x8D,0xAF,0x23,0x8B,0xAA,
0xED,0xBD,0xBE,0x52,0x83,0x0C,0xA5,0x83,0x2B,0x12,0xD9,0xF4,0x22,0x8A,0x1F,0x3B,
0x14,0xB2,0xE4,0xDD,0xDD,0xFA,0x90,0xD5,0xA0,0xD2,0xC1,0x31,0x4B,0xBE,0x1E,0x9A,
0xD0,0xC5,0xCC,0xF5,0xA3,0xC4,0xC6,0xC1,0xE5,0xB9,0xCE,0x03,0xC1,0x26,0x85,0x63,
0x4F,0x3D,0x42,0xAD,0x57,0x58,0x6F,0x6C,0xE3,0x84,0xA0,0x12,0xFA,0x7E,0x72,0xD9,
0x22,0x97,0x9D,0x23,0x20,0xF5,0x4E,0x3A,0x0D,0xAF,0xA1,0xD9,0x26,0xB7,0x86,0xB4,
0xAC,0x41,0xEB,0x83,0xAE,0xB2,0xF0,0xA2,0x3B,0x99,0x47,0x8B,0x6B,0x6B,0x88,0x0F,
0x57,0x87,0x63,0xA2,0x71,0x64,0x96,0xDD,0x9C,0x90,0xD3,0x07,0x14,0xCD,0x7A,0xC1,
0x46,0x16,0xA2,0x40,0x84,0x56,0x80,0x41,0xB9,0x1F,0x88,0x8B,0x54,0x2F,0x9E,0x22,
0x66,0x34,0x43,0xD9,0xD4,0x69,0x9D,0x41,0x51,0x19,0xB4,0x5A,0x3F,0xF8,0xF7,0xD7,
0x9E,0xB4,0x3E,0x12,0x84,0x36,0xAC,0x92,0x6E,0x4B,0x33,0xFB,0x7A,0xA4,0x14,0x0D,
0x9D,0x30,0xC3,0xC5,0xE9,0x20,0x42,0xAA,0x8B,0xD5,0x40,0xBC,0xB7,0x5A,0x80,0x4E,
0x2C,0x95,0x73,0x21,0x0D,0x21,0x76,0x06,0xAD,0x1F,0x6A,0x74,0xE2,0x4A,0x2B,0x7F,
0x44,0x4D,0x5A,0x21,0xCA,0xD1,0x38,0x13,0x6E,0xAF,0xF4,0xC9,0xD8,0xEE,0x88,0x2D,
0x00,0x93,0x10,0x25,0x37,0xBA,0x38,0x69,0xDD,0x5D,0x64,0x3D,0xB5,0xE2,0xDE,0x0C,
0x81,0x9D,0x0E,0x07,0x5D,0x28,0xF0,0x4C,0x97,0x18,0xFF,0x10,0xEE,0xFD,0x4F,0x3F,
0x97,0x72,0xF8,0xC8,0x98,0x0C,0x21,0x5F,0x10,0x76,0x4F,0x31,0x5E,0xC8,0xD0,0x75,
0xF3,0x29,0xC8,0xD3,0xCC,0xB6,0x4C,0x26,0xBA,0x28,0x21,0x75,0xEF,0x0D,0x91,0xBB,
0x3B,0x15,0x6E,0x12,0x83,0x08,0xA9,0x9D,0xA1,0x5B,0xE4,0xEE,0x4F,0x2F,0x39,0x43,
0x03,0x7B,0x98,0x51,0x6C,0x78,0x6F,0x5E,0x74,0x6F,0xC8,0x83,0x0A,0x99,0xC9,0x6F,
0xED,0xCC,0xEB,0xCA,0xD2,0xD5,0x14,0x0B,0x74,0x6A,0x62,0x2A,0xF5,0x7B,0xB9,0x0E,
0x83,0xED,0x08,0xFD,0xB5,0x43,0x45,0xAE,0xB3,0x75,0x82,0x3E,0x49,0x9A,0xD8,0x28,
0x1E,0xD2,0x7B,0x70,0xA4,0xA2,0xC6,0xDA,0x3F,0x22,0x21,0x74,0x01,0xFD,0x93,0xDA,
0x38,0x2C,0x9B,0xAF,0xCE,0x25,0xAC,0xBE,0x69,0x5F,0xBE,0x09,0xBC,0xB8,0x7E,0xAD,
0x17,0x74,0x74,0x60,0x08,0xEB,0x79,0xB5,0x4E,0xA9,0x1C,0xB1,0x46,0x2A,0xB3,0x85,
0x6E,0x2D,0xDC,0xD0,0x0A,0xD0,0x92,0x5C,0xEF,0xB8,0x46,0xD7,0x77,0xE1,0xA2,0x7B,
0x69,0x2D,0xDC,0xAA,0x88,0x9A,0xE6,0x3A,0xE1,0x74,0xA2,0xD9,0xF2,0xA4,0xF4,0x6D,
0xD3,0x46,0x9A,0x23,0x3A,0x64,0x29,0x5C,0xCE,0x0D,0x03,0xA6,0x3E,0x5E,0x7A,0x73,
0xF1,0xC9,0x3D,0xD5,0xAF,0x3B,0x94,0x45,0x9C,0x32,0xDF,0xAA,0x67,0x7E,0x92,0x15,
0x2D,0xFB,0x00,0x94,0x29,0x35,0x2A,0x61,0x13,0xC7,0x35,0x01,0x57,0xD2,0x84,0x5B,
0xD4,0x11,0x28,0xB0,0xF9,0x0E,0x0B,0x6F,0x9D,0x73,0x74,0x84,0x84,0x63,0xBB,0xBF,
0xFB,0xB6,0xC4,0xAB,0x9B,0xD6,0xAD,0x49,0x6F,0xC0,0x76,0xB7,0x6B,0x35,0xC4,0x8C,
0x70,0xF1,0x2E,0x75,0x93,0x7D,0x08,0xD8,0x76,0x14,0x60,0x5E,0x14,0x98,0xCC,0x4D,
0x04,0xD2,0xD3,0x11,0xB6,0xA0,0x96,0xA2,0xA7,0x0F,0xFC,0xCB,0x16,0x2F,0xE9,0x5B,
0xA6,0x5F,0x1C,0x61,0xB0,0x0D,0x70,0x2B,0x82,0xCE,0x24,0x32,0xAE,0x61,0x23,0xC6,
0x69,0x53,0x66,0xB3,0x9B,0x5F,0x3D,0xFC,0xE4,0x83,0x87,0x20,0x61,0x11,0x19,0xC7,
0xEE,0xC2,0x0E,0x40,0x1A,0x9D,0x9C,0x35,0xF6,0xD2,0x4C,0x96,0x5D,0x75,0xB4,0x8C,
0xA8,0xB8,0xB4,0x00,0x1B,0x39,0x1B,0x6D,0xF5,0x69,0x0A,0x4D,0xA0,0xEA,0xD4,0x19,
0x14,0x53,0x0B,0x99,0xF8,0xD4,0x9A,0x88,0x7C,0xC3,0xF6,0x7A,0xAC,0xB2,0x39,0xAA,
0x0A,0x2B,0x07,0xAB,0x8A,0xEC,0x1E,0x5B,0x53,0x5A,0x67,0x6E,0x77,0xD9,0xBE,0xF6,
0x7E,0xC7,0x40,0xD5,0x25,0xB7,0x13,0x33,0x8D,0xD8,0xA4,0x61,0x8A,0xFC,0xE6,0x83,
0x15,0x69,0xD4,0x4A,0x76,0x42,0x9C,0x5A,0x1E,0xAE,0x53,0xF9,0x64,0x1A,0x85,0x38,
0xF0,0xDC,0x12,0x24,0xCD,0x17,0x37,0x4E,0x21,0xAD,0x92,0x8D,0x43,0x95,0xED,0xA5,
0x4F,0x5D,0x57,0x27,0x6E,0x52,0x42,0x14,0x8E,0xDB,0x62,0x5F,0xAB,0x6B,0x01,0x16,
0xDC,0x33,0x09,0x73,0x68,0xC6,0x3A,0x98,0x27,0xFA,0xAE,0x32,0x07,0x5E,0xA6,0x18,
0xD6,0x50,0x0F,0x57,0x13,0xE3,0xAA,0xE0,0x08,0x25,0xA3,0xBF,0xCA,0x91,0xEC,0xA3,
0xC5,0xBA,0x40,0x5E,0xDF,0x69,0x9D,0xB5,0xAD,0xD3,0xF6,0x97,0x2F,0x38,0xD3,0x68,
0x88,0xCB,0xFB,0x3F,0xD1,0x51,0x1B,0xC8,0x70,0x95,0x34,0xAA,0xC4,0x47,0x8A,0x47,
0x2D,0x6A,0x97,0x5A,0x77,0x53,0x9D,0x53,0x87,0xEF,0x5D,0x9A,0x8A,0x93,0x55,0x96,
0x86,0xEC,0xCA,0xBB,0x9A,0xA4,0x0E,0x8B,0xCB,0xC6,0x31,0x70,0x04,0xC6,0xE2,0x70,
0xDE,0xAF,0xE9,0x1E,0xCB,0x9E,0xBA,0x21,0xAD,0x61,0xCB,0x29,0xB6,0x65,0xA9,0xE7,
0xCC,0x3A,0x6C,0x52,0x66,0xDB,0xD8,0x0D,0xE4,0xD2,0xB7,0x7E,0xE0,0xD4,0x44,0xFC,
0x0C,0xA7,0xF8,0x62,0x27,0x70,0x40,0xD4,0x3A,0x97,0xE7,0x76,0x5A,0xE7,0x6C,0x6B,
0xBC,0xFD,0xE5,0x2B,0x8F,0xDF,0xDE,0x06,0x65,0xEF,0x3F,0x1B,0x0D,0x5B,0x89,0x5D,
0x76,0x27,0x15,0x18,0xE2,0xFF,0x44,0x50,0x15,0xE2,0x73,0x6B,0x37,0xCE,0x13,0xE4,
0x00,0x2B,0x1B,0x6C,0x16,0x3B,0x44,0x2A,0xCA,0xD4,0xE3,0xF1,0x83,0xBA,0x72,0x6B,
0xA9,0x88,0x24,0x3E,0x58,0x07,0x90,0xE5,0xE8,0xE0,0x8F,0x99,0x7A,0x1A,0x37,0x43,
0x04,0xD3,0xEB,0x23,0xD8,0xD2,0xFB,0xB6,0xE2,0x45,0xD2,0xD3,0x36,0xC6,0x6B,0x3D,
0xE3,0xFA,0xE1,0x2F,0x5E,0x5A,0x77,0x82,0x75,0xA9,0x89,0x08,0x8B,0xEA,0x2D,0x6A,
0x8C,0x77,0x62,0x8C,0x2E,0xF2,0xA6,0xF3,0x61,0xDC,0xA6,0x17,0x25,0x76,0x27,0xC6,
0x71,0x85,0x94,0x32,0xB7,0x91,0xDA,0x4C,0xF5,0x78,0x51,0x4A,0x87,0xF9,0x7E,0x62,
0xD6,0x04,0x0D,0x50,0x1B,0x91,0x6C,0xA8,0x47,0x59,0x04,0x4F,0x78,0xA3,0xB7,0xBB,
0x3D,0x78,0x7B,0x99,0x7A,0xD1,0xFB,0xE0,0xFF,0x66,0x42,0x4D,0xBD,0x82,0x39,0x43,
0x4B,0xEB,0x76,0xAF,0x9B,0x03,0xD0,0x32,0x9C,0x3E,0x49,0x9F,0xB1,0xBB,0xFC,0xCA,
0x8C,0xA8,0xB0,0x56,0x54,0xBF,0xE2,0xAC,0xB9,0xA8,0x8F,0xE1,0xFA,0xE1,0xDA,0x5C,
0xC2,0xFB,0x2F,0xC5,0x31,0x37,0x14,0x05,0x0C,0xC3,0x24,0x44,0x0A,0x5F,0xC0,0xE3,
0x8C,0x3A,0x4B,0x77,0x79,0xA7,0x92,0xBC,0x0B,0x4C,0x81,0x18,0xAF,0xBE,0x07,0x7A,
0x56,0xA2,0xA1,0x37,0xA8,0x27,0xBC,0xEE,0xE2,0xA7,0x1D,0x40,0x04,0x9B,0x8F,0x48,
0xD9,0x62,0x8E,0x32,0x96,0x05,0xF4,0x9D,0x89,0xB3,0x60,0xF2,0x50,0xD9,0xC9,0x19,
0xB4,0x1F,0xB1,0xDD,0x86,0x75,0x98,0x38,0xC7,0x86,0xA8,0x45,0xB4,0x9B,0xF0,0xF9,
0x08,0x9A,0x4C,0xCA,0x3A,0xAD,0x0D,0x2B,0xCE,0xB3,0xAD,0x8E,0x13,0x27,0xA4,0xE5,
0x7D,0x5E,0x4F,0x1D,0x44,0xA2,0xEB,0x26,0xE9,0xA8,0xD2,0x6A,0x62,0xE6,0x72,0x87,
0x3A,0x96,0x09,0x2F,0xF8,0xEB,0x92,0x8C,0x59,0xA2,0x63,0x93,0xDB,0x7D,0x64,0x44,
0x77,0xCE,0xBB,0x71,0xD1,0xCF,0x2F,0xBB,0xC3,0xD0,0xF6,0x1F,0x71,0x7B,0xB3,0x4F,
0x61,0x2D,0xC1,0x7A,0x42,0x8C,0xC9,0x21,0xF4,0xC6,0xA0,0xF0,0x94,0xC2,0x88,0xB6,
0x69,0x8C,0xDD,0xD6,0x6A,0x17,0xAD,0xA6,0x15,0x9D,0x23,0xA7,0xB6,0x35,0xA8,0x18,
0xF8,0x44,0x42,0xAD,0xD4,0x06,0xF8,0x8F,0x5C,0x2A,0x84,0x49,0x75,0x08,0x83,0xB2,
0x54,0xCC,0x2A,0x4E,0x65,0x65,0x4B,0xC3,0xB2,0x1E,0x68,0x39,0x4F,0xB3,0x66,0xD2,
0x5C,0xB3,0x76,0x2D,0x5A,0xE3,0x32,0x25,0xC9,0x93,0x38,0x5D,0x1B,0x53,0x85,0xA8,
0xF9,0x5D,0x77,0x0D,0x37,0xA7,0x92,0x19,0xB9,0xF2,0xBE,0xAF,0xB1,0x42,0x5D,0x10,
0x00,0x23,0xD6,0x57,0x4B,0xE2,0x27,0x1C,0xDA,0xB8,0x4A,0x76,0xED,0xFE,0x87,0x32,
0x72,0x94,0x39,0x4E,0x58,0xD6,0x70,0x0D,0xD3,0x9B,0x8B,0xC9,0x4D,0xE4,0x97,0x02,
0xF2,0xF3,0x15,0xE7,0xF4,0x51,0x43,0xF2,0x02,0x62,0x88,0x0D,0x42,0x25,0x26,0x17,
0xF2,0x63,0x22,0x77,0xF7,0x27,0x6A,0xF4,0x63,0xFA,0x4C,0x6B,0xB4,0xBA,0x50,0x64,
0xDA,0x37,0x4F,0x24,0xD5,0x3A,0x17,0x97,0xA2,0xD1,0x7A,0x30,0xB0,0x80,0x52,0x6C,
0x7D,0x18,0x26,0xB4,0x68,0xEF,0x37,0xDD,0xC6,0xB1,0x61,0x59,0xCF,0xEF,0xA6,0x15,
0x06,0x35,0x48,0x6C,0x98,0xB4,0xB9,0x9F,0x1A,0x71,0x23,0xE6,0x31,0x95,0xD4,0xCE,
0xA5,0x2D,0x0B,0x25,0xD7,0x12,0xA1,0xC7,0x12,0x23,0xDC,0xCF,0x19,0xC0,0x4C,0xD0,
0x22,0x1D,0x2C,0x37,0xC4,0x4E,0x93,0x62,0xF5,0x43,0x26,0x1D,0x33,0x6F,0x95,0x9F,
0xA8,0xD0,0x36,0xE4,0x68,0xE1,0x35,0x91,0xB8,0xE1,0x2D,0xB3,0x63,0x7A,0x99,0x74,
0x6C,0xCD,0x0F,0xBE,0xF3,0x98,0x93,0x09,0x29,0x29,0x37,0x5B,0x9D,0xCD,0xDA,0x0D,
0x42,0x79,0x0A,0x6F,0x7F,0xEB,0x16,0x65,0x6F,0xEF,0xD4,0x5D,0x4E,0x5A,0x6D,0xF3,
0x4D,0x4B,0x2E,0xEF,0x32,0x09,0x58,0x35,0x1A,0x6F,0x53,0xF9,0x69,0x15,0x53,0xE6,
0xAD,0x4D,0x9B,0x75,0x5A,0x74,0x6D,0xEF,0x37,0x5B,0xF7,0xF4,0x49,0x18,0x57,0x71,
0x83,0x4A,0xE7,0x49,0x9B,0xFE,0xD3,0x9F,0x19,0x8A,0xC9,0xE8,0xDC,0x9F,0x3D,0xE6,
0x3E,0xE1,0x18,0x44,0x2A,0x0E,0x4E,0x24,0x20,0x24,0xCD,0x6B,0x57,0x4C,0x5F,0x43,
0x95,0xB4,0x08,0x28,0xD3,0x89,0xFC,0xEA,0xC1,0xFE,0x8C,0x06,0xCC,0x9A,0x85,0xD9,
0xAA,0xE2,0x58,0xFA,0x20,0xAB,0xB4,0x62,0x53,0x40,0xE2,0xCA,0x55,0xF1,0x27,0x17,
0xC4,0xC0,0xE1,0x51,0x2E,0x08,0x65,0x74,0x27,0xA7,0x66,0x5B,0x8C,0xC6,0x1C,0x87,
0x26,0xEA,0xE1,0x83,0x5B,0xF7,0xEC,0xCF,0x13,0xAA,0x2C,0xE5,0x9C,0xAA,0xD9,0xCD,
0x40,0x23,0x43,0x5D,0xAD,0x9B,0x20,0xD6,0x1B,0x48,0x95,0x51,0xD0,0xC2,0xB9,0x68,
0xB7,0x5F,0xA3,0x8C,0x25,0xD7,0x61,0x43,0x24,0xC0,0x88,0xD4,0x2F,0x2D,0x0A,0xB0,
0x45,0x7E,0xCB,0xB1,0x34,0x3E,0xF9,0x21,0x09,0x58,0xB4,0xCD,0xD7,0x5D,0x26,0x1D,
0x99,0xFD,0x59,0x43,0xB4,0xE8,0x9D,0xEE,0x42,0xBA,0x5D,0x75,0x42,0xEE,0xF0,0xA6,
0x64,0x2F,0xF5,0x22,0x2C,0x62,0x97,0xBA,0xC1,0x83,0xB1,0x17,0xE2,0x92,0x4C,0xA8,
0x4D,0x2D,0x55,0xC9,0xA5,0xEC,0x75,0xB4,0xC7,0x9F,0xED,0xCC,0xAD,0xE7,0xA6,0x8C,
0x64,0x9A,0x2A,0x11,0xA6,0x31,0x90,0xD3,0x49,0x92,0x2E,0xC4,0xCD,0x76,0xD7,0xC6,
0xA4,0xAE,0xEF,0xB3,0x20,0xC6,0x02,0xAE,0x76,0xC4,0x1F,0x6B,0x98,0x91,0xBD,0x3E,
0xA3,0xB5,0xAB,0x56,0x56,0x4B,0xA8,0x30,0x2A,0xCE,0xD2,0x36,0x5B,0xCF,0x3E,0xFF,
0x18,0xC4,0x6A,0x6A,0xB1,0xD9,0xDA,0xBC,0xDC,0x58,0x96,0x36,0xD1,0xF9,0x55,0x73,
0xED,0xAD,0x97,0x9F,0x82,0xB8,0xED,0x7D,0xDC,0x94,0xE7,0xAC,0x2E,0x74,0x61,0x9D,
0x05,0xC9,0x99,0xD8,0x71,0xAA,0xED,0x59,0x4A,0x85,0xE2,0xB2,0x5A,0x4E,0xF7,0xE5,
0x3A,0x34,0x6C,0xD4,0xF5,0x32,0x47,0xDD,0x6C,0x5D,0x7C,0xD6,0x42,0x8E,0x88,0xC9,
0xFB,0xAE,0xD3,0x44,0x2E,0x45,0xFE,0x32,0xE0,0x12,0xB7,0x45,0x06,0xBD,0x90,0x90,
0x83,0xE6,0x50,0x4F,0x7D,0xC8,0x8F,0x46,0x29,0x82,0xCE,0x1C,0x74,0x34,0xAD,0x7F,
0x18,0xD4,0x3C,0xBE,0x68,0x16,0xD8,0xEB,0xC8,0x49,0xCA,0xA5,0x8B,0x48,0xC5,0x89,
0xCD,0x76,0x40,0xDA,0xAD,0xDF,0x03,
#endif

#ifdef KEYNAMES_COMP
#define KEYNAMES_DECOMP_LEN 869
#define NUM_COMP_KEYNAMES 323
0xC5,0x53,0xDB,0x76,0xDB,0x20,0x10,0x9C,0x4F,0xB2,0x7C,0xF7,0xA3,0x2D,0x4B,0x71,
0x1A,0x39,0x55,0xA4,0xB8,0xB7,0x97,0x1E,0x2A,0x93,0x98,0x63,0x04,0x0A,0xA0,0xA6,
0xF9,0xFB,0x8E,0x90,0x5F,0xFB,0x5C,0xCE,0x19,0x76,0xC5,0x0E,0x0B,0xCB,0x8E,0x80,
0x71,0xEC,0x44,0x73,0xF5,0x9D,0x68,0x24,0x9E,0xC5,0x2F,0x7E,0xA7,0x5A,0x0A,0x87,
0x4A,0x86,0xDE,0x99,0x91,0x51,0x8A,0xDE,0xCB,0x1B,0x1B,0x99,0x6F,0x44,0x37,0x7E,
0xD5,0x71,0x57,0xF6,0xA7,0xD1,0x42,0xB5,0x78,0xEA,0x6D,0x90,0x7B,0x1C,0x84,0xBF,
0x60,0x6F,0xB5,0x66,0x12,0x6C,0xDB,0x4E,0x3A,0x2F,0xCC,0x79,0x8C,0xD6,0x4C,0xE5,
0xA4,0x29,0x46,0x53,0x61,0xEB,0x83,0x74,0xCA,0x5F,0x51,0xEA,0xDE,0x23,0xB5,0x6D,
0x2B,0x70,0x54,0x86,0x7E,0xC9,0x75,0x7B,0x46,0xAD,0x87,0x6C,0x13,0x24,0x98,0x62,
0x86,0x39,0x16,0x58,0x62,0x85,0x35,0x36,0x24,0x6B,0x6B,0x50,0xCB,0x56,0x35,0xD1,
0x2B,0xA4,0xF7,0xC8,0xDE,0x7A,0xA1,0x3D,0xEE,0x9C,0x14,0x4C,0xCC,0x33,0xA5,0x0F,
0x8A,0xC1,0x6D,0xC0,0xBF,0xC7,0xCE,0xF1,0x01,0x64,0x28,0xC6,0x87,0x88,0x07,0xDE,
0x96,0x2A,0xA4,0xBC,0x67,0xC0,0xC9,0x9C,0x59,0x45,0x63,0x9D,0x8C,0x9C,0xA0,0x9A,
0x2B,0xB6,0xD8,0x21,0xC5,0x1E,0x19,0x72,0xDC,0xE1,0x80,0x7B,0x7C,0xC2,0x03,0x0A,
0x1C,0xF1,0x88,0xCF,0x28,0xF1,0x84,0x0A,0x35,0x9E,0x71,0xC2,0x17,0x7C,0xC5,0x37,
0x7C,0xC7,0x8F,0x78,0xDA,0x5E,0x6A,0x19,0x24,0xFE,0xF7,0x78,0x28,0x27,0x44,0x42,
0x4C,0x89,0x19,0x31,0x27,0x16,0xC4,0x92,0x58,0x11,0x6B,0x62,0x43,0xFC,0xBC,0xB5,
0x82,0xDE,0x5E,0xFD,0x56,0x67,0x39,0x78,0xC7,0x5E,0x07,0xD5,0xE9,0x8F,0xE8,0xC7,
0x86,0x0D,0x44,0x3D,0xDA,0xCC,0x0C,0x8F,0x3F,0x38,0x63,0x3B,0x4E,0x1D,0xF5,0xF0,
0x6E,0x50,0xA9,0xD7,0x4B,0x60,0xA3,0x5E,0x02,0xEE,0x8D,0x97,0x2E,0xE0,0x60,0x5B,
0xEA,0x87,0xEA,0x28,0xC5,0xAB,0x24,0x6D,0x30,0x91,0x9A,0x27,0xC8,0xA7,0xC8,0x67,
0xC8,0xE7,0xC8,0x17,0xC8,0x97,0xC8,0x57,0xC8,0xD7,0xC8,0x37,0x0C,0x4D,0x08,0xC6,
0x13,0x12,0x12,0x32,0x12,0x52,0x92,0xC5,0x50,0xD4,0x63,0xDF,0x16,0x96,0xBD,0x49,
0x45,0xE7,0xA3,0x53,0x37,0x8E,0x42,0x1C,0xDD,0x8B,0x7A,0x61,0x43,0xA3,0x29,0x90,
0x06,0xA7,0xAB,0x38,0x17,0xD8,0x6A,0xAE,0x73,0x62,0xEF,0x64,0x10,0x55,0x9C,0x0B,
0xD4,0x3D,0x95,0x7B,0x33,0x5C,0xB3,0xAC,0x9B,0xFA,0xEC,0x2C,0x7F,0x84,0x83,0xD4,
0xBC,0xAA,0x53,0x26,0xA0,0xFE,0xF0,0xD5,0x1B,0xE5,0x22,0xC5,0x95,0xFB,0x4C,0x8F,
0xD2,0xBE,0xB3,0xF6,0xAC,0x77,0x76,0x50,0x8D,0xC5,0x5F,
#endif
