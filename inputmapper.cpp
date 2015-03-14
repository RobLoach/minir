#include "minir.h"
#include "containers.h"
#include <ctype.h>

namespace {
class devmgr_inputmapper_impl : public devmgr::inputmapper {
	//function<void(size_t id, bool down)> callback; // held in the parent class
	
	//each button maps to two uint32, known as 'descriptor', which uniquely describes it
	//format:
	//trigger: ttttxxxx xxxxxxxx xxxxxxxx xxxxxxxx
	//extra:   yyyyyyyy yyyyyyyy yyyyyyyy yyyyyyyy
	//tttt=type
	// 0000=keyboard
	// others=TODO
	//x=meaning varies between different device types
	//y=meaning varies between different device types
	//
	//TODO: various input sources are not buttons, the following exist and may require changing this one:
	//analog/discrete
	//centering/positioned/unpositioned
	// button (keyboard)
	//  already solved
	// self-centering analog (joystick)
	//  maybe I should define some of the bits to be some kind of minimum value? The data structures are not prepared for that...
	//  how will I solve issues with rounding errors? I don't want the device to fire events if it jitters across the limit.
	//  do I use the padding in the keydata? That should be a good solution. There's no padding on 32bit, but 32bit is dying anyways.
	//  events must contain the positions, I can't just turn them into booleans
	// unpositioned analog (mouse, when grabbed)
	//  fire an event each time it moves
	//  merge events? probably not
	// positioned analog (mouse pointer)
	//  fire an event for each change
	//  do I send delta or absolute position? probably absolute
	// unpositioned discrete (mouse wheel)
	//  make it fire press events, but never release and never be held? Fire release instantly?
	//  wheels can be analog too; none are in practice, but MS docs say they can be
	//
	//for keyboard:
	//0000---- -------- kkkkksnn nnnnnnnn
	//-------- -------- -------- --------
	//-=unused, always 0
	//k=keyboard ID, 1-31, or 0 for any; if more than 31, loops back to 16
	//s=is scancode flag
	//n=if s is set, scancode, otherwise libretro code
	//
	//00000000 00000000 00000000 00000000 (RETROK_NONE on any keyboard) can never fire and may be used as special case
	//
	//none of those descriptors may leave this file
	
	class keydata {
	public:
		uint32_t trigger; // 0 if the slot is unused
		uint32_t extra; // used by some event sources
		multiint<uint32_t> mods;//must be held for this to fire
	};
	
	array<keydata> mappings;//the key is a slot ID
	uint16_t firstempty;//any slot such that all previous slots are used
	
	intmap<uint32_t, multiint<uint32_t> > keylist;//returns which keys are modifiers for this key
	
	size_t register_group(size_t len)
	{
		while (true)
		{
			size_t n=0;
			while (true)
			{
				if (mappings[firstempty+n].trigger != 0)
				{
					firstempty += n+1;
					break;
				}
				n++;
				if (n == len) return firstempty;
			}
		}
	}
	
	/*private*/ bool parse_descriptor_kb(keydata& key, const char * desc)
	{
return false;
	}
	
	/*private*/ bool parse_descriptor(keydata& key, const char * desc)
	{
		if (desc[0]=='K' && desc[1]=='B' && !isalpha(desc[2])) return parse_descriptor_kb(key, desc);
		return false;
	}
	
	/*private*/ template<typename K, typename V, typename K2, typename V2> void multimap_remove(intmap<K, multiint<V> >& map, K2 key, V2 val)
	{
		multiint<K>* items = map.get_ptr(key);
		if (!items) return;
		items->remove(val);
		if (items->count() == 0) map.remove(key);
	}
	
	bool register_button(size_t id, const char * desc)
	{
		keydata& key = mappings[id];
		//multimap_remove(keylist, key.trigger, id);
		if (!desc) goto fail;//this isn't really a failure, but the results are the same.
		
		if (!parse_descriptor(key, desc)) goto fail;
		//keylist.get(key.trigger).add(id);
		return true;
		
	fail:
		key.trigger=0;
		firstempty=id;
		return false;
	}
	
	//enum dev_t {
	//	dev_kb,
	//	dev_mouse,
	//	dev_gamepad,
	//};
	//enum { mb_left, mb_right, mb_middle, mb_x4, mb_x5 };
	//type is an entry in the dev_ enum.
	//device is which item of this type is relevant. If you have two keyboards, pressing A on both
	// would give different values here.
	//button is the 'common' ID for that device.
	// For keyboard, it's a RETROK_*. For mouse, it's the mb_* enum. For gamepads, [TODO]
	//scancode is a hardware-dependent unique ID for that key. If a keyboard has two As, they will
	// have different scancodes. If a key that doesn't map to any RETROK (Mute, for example), the common
	// ID will be some NULL value (RETROK_NONE, for example), and scancode will be something valid.
	//down is the new state of the button. Duplicate events are fine and will be ignored.
	void event(dev_t type, unsigned int device, unsigned int button, unsigned int scancode, bool down)
	{
		
	}
	
	bool query(dev_t type, unsigned int device, unsigned int button, unsigned int scancode)
	{
		
		return false;
	}
	
	void reset(dev_t type)
	{
		
	}
	
	//~devmgr_inputmapper_impl()
	//{
	//	
	//}
};
}

devmgr::inputmapper* devmgr::inputmapper::create()
{
	return new devmgr_inputmapper_impl;
}
