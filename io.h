#pragma once
#include "global.h"
#include "image.h"
#include "string.h"

//this belongs more in window.h, but everything that uses it is based more on io.h than window.h
#ifdef WNDPROT_X11
//Returns the display and screen we should use.
//The concept of screens only exists on X11, so this should not be used elsewhere.
//Only I/O drivers should have any reason to use this.
struct _XDisplay;
typedef struct _XDisplay Display;
struct window_x11_info {
	Display* display;
	int screen;
	unsigned long root; //The real type is Window aka XID.
};
extern struct window_x11_info window_x11;
#endif

//needed features:
//                    2d input
//       3d input   thread move?          2d input
//             shaders                  thread move?
//   thread move?    direct output      direct output
//separate backend
//thread moves are optional

struct retro_hw_render_callback;//libretro.h

//The first function called on this object must be initialize(), except if it should chain, in which case set_chain() must come before that.
//Any thread is allowed to call the above two (but not simultaneously).
//The owner thread becomes the one calling initialize().
//set_chain may be called multiple times. However, the target must be initialize()d first.
class video : nocopy {
public:
	struct driver {
		const char * name;
		
		//The returned objects can only use their corresponding 2d or 3d functions; calling the other is undefined behaviour.
		//If an object shall chain, the window handle is 0.
		video* (*create2d)(uintptr_t windowhandle);
		//The caller will fill in get_current_framebuffer and get_proc_address. The created object cannot
		// set it up, as a function pointer cannot point to a C++ member.
		//On failure, the descriptor is guaranteed to remain sufficiently untouched that creation can be retried
		// with another driver, and the results will be the same as if this other driver was the first tried.
		//Can be NULL if features&f_3d == 0.
		video* (*create3d)(uintptr_t windowhandle, struct retro_hw_render_callback * desc);
		
		uint32_t features;
	};
	
private:
	static const driver driver_d3d9;
	static const driver driver_ddraw;
	static const driver driver_opengl;
	static const driver driver_gdi;
	static const driver driver_xshm;
	static const driver driver_none;
	static const driver driver_opengl_old;
	
public:
	static const driver* const drivers[];
	
	//This driver cannot draw anything; instead, it copies the input data and calls the next driver on
	// another thread, while allowing the caller to do something else in the meanwhile (assuming vsync is off).
	//This means that the chain creator will not own the subsequent items in the chain, and can therefore not
	// ask for either initialization, vsync, shaders, nor screenshots.
	// Instead, they must be called on this object; the calls will be passed on to the real driver.
	//Due to its special properties, it is not included in the list of drivers.
	static video* create_thread();
	
	enum {
		//TODO: Reassign those bits once the feature set quits changing.
		f_sshot_ptr=0x00200000,//get_screenshot will return a pointer, and not allocate or copy stuff. This is faster.
		f_sshot   = 0x00100000,//get_screenshot is implemented. If f_shaders is set, get_screenshot_after is also implemented.
		f_chain_out=0x00020000,//Can both chain and emit output at the same time. Rarely set unless f_shaders is; if it isn't, use the video cloner.
		f_chain   = 0x00010000,//Chaining is allowed. Implies that at least one in f_shaders is set.
		f_v_vsync = 0x00002000,//Can vsync with a framerate different from 60.
		f_vsync   = 0x00001000,//Can vsync. The given framerate cares only about whether it's different from zero; it's locked to 60.
		//f_a_vsync=0x000000,//[Windows] Can almost vsync - framerate is 60, but it stutters due to DWM being stupid.
		//TODO: examine https://github.com/libretro/fba-libretro/blob/master/svn-current/trunk/fbacores/cps1/src/burner/win32/dwmapi_core.cpp and DwmEnableMMCSS
		f_shaders = 0x00000F00,//Each of these bits correspond to f_sh_base<<shader::lang.
		f_sh_base = 0x00000100,//Base value for shaders. This specific bit must not be checked.
		f_3d      = 0x000000FF,//Each of these bits correspond to f_3d_base<<retro_hw_context_type.
		f_3d_base = 0x00000001,//Base value for 3d. This specific bit is never set, because it corresponds to RETRO_HW_CONTEXT_NONE.
	};
	//Returns the features this driver supports. Can be called on any thread, both before and after initialize().
	//Since video drivers can be chained, you can combine them however you want and effectively get everything; therefore, the flags are in no particular order.
	//Some features may claim to be implemented in the specification array, but depend on better runtime libraries than what's actually present.
	//If the driver is chained, truthfully reporting vsync capabilities is still recommended. If not detectable, report the theoretical maximum.
	virtual uint32_t features() = 0;
	
	//Sets the device to render to. NULL is allowed if f_chain_out is set.
	virtual void set_chain(video* next) {}
	
	//Finishes initialization. Used to move the object to another thread.
	virtual void initialize() {}
	
	//Sets the maximum input size of the object, as well as depth. Can be called multiple times, and all values can vary.
	//Must be called at least once before the first draw_*.
	//For 2D drivers, the format has only one correct value, and the rest will give garbage.
	//For 3D drivers, the format can be chosen arbitrarily. It is recommended to use high precision.
	//The libretro-compatible values are guaranteed to work; the others will probably not (in particular, the ones with alpha are unlikely to work).
	virtual void set_source(unsigned int max_width, unsigned int max_height, videoformat depth) = 0;
	
	//Asks where to put the video data for best performance. Returning data=NULL means 'I have no opinion, give me whatever'.
	//If this returns data!=NULL, the next call to this object must be draw_2d, with the same arguments as draw_2d_where; otherwise, the object is unchanged.
	//However, it is allowed to call draw_2d without draw_2d_where.
	virtual void draw_2d_where(unsigned int width, unsigned int height, void * * data, unsigned int * pitch) { *data=NULL; *pitch=0; }
	virtual void draw_2d(unsigned int width, unsigned int height, const void * data, unsigned int pitch) = 0;
	
	virtual uintptr_t draw_3d_get_current_framebuffer() { return 0; }
	virtual funcptr draw_3d_get_proc_address(const char * sym) { return NULL; }
	virtual void draw_3d(unsigned int width, unsigned int height) {}
	
	//This repeats the last drawn frame until the next vblank. Also affects time-dependent shaders. If the device does not vsync, it does nothing.
	//It can be called whether this one is configured as 2d or 3d.
	//If there is no last frame (for example if set_source_size was called after the last draw_* cycle), it is undefined what will be
	// shown and whether vsync will apply, but any resulting anomalies are guaranteed to go away once the next draw_* cycle is finished.
	virtual void draw_repeat() {}
	
	//This only has effect if the device is enabled. 0 means off, and is the default.
	virtual void set_vsync(double fps) {}
	
	//Shaders can only be set after initialize().
	class shader : nocopy {
	public:
		enum lang_t { la_none, la_glsl, la_cg, la_last };
		#define VIDEO_SHADER_LANG_NAMES "glsl", "cg"
		static const char * langtable[];
		
		static const char * lang_to_str(lang_t lang) { return langtable[lang]; }
		static lang_t str_to_lang(const char * str)
		{
			for (size_t i=1;i<la_last;i++)
			{
				if (!strcmp(str, langtable[i])) return (lang_t)i;
			}
			return la_none;
		}
		
		enum interp_t { in_nearest, in_linear };
		enum wrap_t { wr_border, wr_edge, wr_repeat, wr_mir_repeat };
		enum scale_t { sc_source, sc_absolute, sc_viewport };
		enum fbo_t { fb_int, fb_float, fb_srgb };
		
		struct pass_t {
			lang_t lang;
			const char * source;
			interp_t interpolate;
			wrap_t wrap;
			unsigned int frame_max; // 0 for UINT_MAX.
			fbo_t fboformat;
			bool mipmap_input;
			scale_t scale_type_x;
			scale_t scale_type_y;
			float scale_x;
			float scale_y;
		};
		unsigned int n_pass;
		//NULL is possible if translate() can't handle this, if the file is not found, or similar.
		//Each pass must be freed before asking for another one.
		virtual const struct pass_t * pass(unsigned int n, lang_t language) = 0;
		virtual void pass_free(const struct pass_t * pass) = 0;
		
		//Returns NULL if it doesn't know how to do this translation, or if the syntax is invalid. Send
		// it to free() once you're done with it.
		//Translating from a language to itself will work. It is implementation defined whether this
		// resolves #include/#ifdef SHADER_PARAMETER/etc, rejects invalid code, and various other stuff.
		//Translating a shader is not guaranteed lossless. Do not store translated text.
		static char * translate(lang_t from, lang_t to, const char * text, function<char*(const char * filename)> get_include=NULL);
		
	private:
		static char * translate_cgc(lang_t from, lang_t to, const char * text, function<char*(const char * filename)> get_include);
	public:
		
		struct tex_t {
			const char * name;
			struct image data;
			bool linear;
			bool mipmap;
			wrap_t wrap;
		};
		unsigned int n_tex;
		//Each texture must be freed before asking for another one.
		virtual const struct tex_t * texture(unsigned int n) = 0;
		virtual void texture_free(const struct tex_t * texture) = 0;
		
		
		class var : nocopy {
		public:
			//Must be called before the first auto_frame. May be called multiple times; will call auto_reset if called.
			void auto_set_wram(const uint8_t * data, size_t size);
			//Call this every frame, or whenever it changes.
			void auto_set_input(uint16_t player1, uint16_t player2) { this->au_input[0]=player1; this->au_input[1]=player2; }
			//Resets the state of all autos.
			void auto_reset();
			//This reads WRAM and input and updates all relevant parameters.
			void auto_frame();
			
			struct param_t {
				const char * pub_name;//To be shown to the user.
				const char * int_name;//To be stored in files.
				double min;
				double max;
				double initial;
				//shaders recommend a certain step size too, but minir prefers deciding that on its own.
			};
			const struct param_t * param_get(unsigned int * count) { if (count) *count=this->pa_count; return this->pa_items; }
			void param_set(unsigned int id, double val) { out_append(this->au_count + id, val); }
			
		/*protected:*/
			//For video_*.
			//Imports and parameters are combined; video_* has no reason to treat them differently, they're just uniform floats.
			//The IDs here shall be treated separately from all IDs elsewhere.
			const char * const * out_get_names(unsigned int * count) { if (count) *count=this->au_count+this->pa_count; return this->out_names; }
			struct change_t {
				unsigned int index;
				float value;
			};
			//The return value may contain duplicates; however, they will be very rare. count must be non-NULL.
			const change_t * out_get_changed(unsigned int * count) { *count=this->out_numchanges; this->out_numchanges=0; return this->out_changes; }
			//The return value here won't contain duplicates.
			const change_t * out_get_all(unsigned int * count) { if (count) *count=this->au_count+this->pa_count; return this->out_all; }
			
		/*protected:*/
			//For video::shader_*.
			enum semantic_t {
				se_constant, // For internal use only.
				se_capture,
				se_capture_previous,
				se_transition,
				se_transition_previous,
				se_transition_count,
				se_python, // Not implemented.
			};
			enum source_t { so_wram, so_input, so_python };
			struct auto_t {
				const char * name;
				
				semantic_t sem;
				source_t source;
				size_t index;//for so_wram, byte offset; for so_input, 0/1 for P1/P2
				uint16_t mask;
				uint16_t equal;
			};
			void setup(const struct auto_t * auto_items, unsigned int auto_count, const struct param_t * params, unsigned int param_count);
			
			var() { initialize(); }
			~var() { reset(); }
			
		private:
			void initialize()
			{
				this->au_count=0;
				this->out_names=NULL;
				this->out_all=NULL;
				this->out_changes=NULL;
				this->pa_count=0;
				this->pa_items=NULL;
			}
			void reset();
			
			//This is first the autos, then the params.
			const char ** out_names;
			struct change_t * out_all;
			
			struct change_t * out_changes;
			unsigned int out_numchanges;
			unsigned int out_changes_buflen;
			void out_append(unsigned int index, float value);
			
			const uint8_t * au_wram;
			size_t au_wramsize;
			
			uint16_t au_input[2];
			
			uint32_t au_framecount;
			
			struct au_item {
				uint8_t sem;
				uint8_t sem_org;
				uint8_t source;//can't optimize source+index into uint16* because that would violate alignment
				//char padding[1];
				uint16_t mask;
				uint16_t equal;
				//parent->values[i] is conceptually part of this too
				size_t index;
				
				uint32_t last;
				uint32_t transition;
			};
			struct au_item * au_items;
			unsigned int au_count;
			
			inline uint16_t au_fetch(au_item* item);
			
			unsigned int pa_count;
			struct param_t * pa_items;
		} variable;
		
		//data must be a shader preset.
		//All three functions may be called after create_from_data returns, until the shader object is
		// deallocated, so that potentially large objects aren't stored in memory multiple times.
		// However, path_join is guaranteed to have basepath equal to one of its prior return values if
		// the create_from_* call has returned.
		//All functions are allowed to return NULL, in which case the caller returns failure.
		//read_image's 'free' parameter tells whether to free() the returned image.pixels. If not, it is
		// assumed valid until the next call to the same function or until anything higher-level
		// returns. It will not be changed.
		static shader* create_from_data(const char * data,
		                                function<char*(const char * basepath, const char * path)> path_join,
		                                function<char*(const char * path)> read_text,
		                                function<struct image(const char * path, bool * free)> read_image);
		//Same as create_from_data, but changes 'data' into garbage. (create_from_data uses strdup on 'data', then calls this.)
		static shader* create_from_scratch_data(char * data,
		                                        function<char*(const char * basepath, const char * path)> path_join,
		                                        function<char*(const char * path)> read_text,
		                                        function<struct image(const char * path, bool * free)> read_image);
		//Another convenience shortcut to the above. The filename can be either a shader or a preset; if
		// the filename ends with 'p', it's a preset.
		static shader* create_from_file(const char * filename);
		
		virtual ~shader() = 0;
	};
	
	//NULL is valid and means nearest neighbor.
	//The shader object remains owned by the caller. Each shader object can only be used by one video driver simultaneously (except NULL).
	//If the given shader object is broken, incompatible, or otherwise unusable, a NULL will be assigned.
	//The shader can be changed multiple times, both with NULL and non-NULL arguments.
	virtual bool set_shader(shader * sh) { return (!sh); }
	
	//This sets the final size of the object output.
	virtual void set_dest_size(unsigned int width, unsigned int height) = 0;
	
	//Returns the last input to this object. If the input is 3d, it's flattened to 2d before being returned.
	//The returned integer can only be compared with 0, or sent to release_screenshot(). It can vary depending on anything the object wants.
	//If get_screenshot can fail, release_screenshot with ret=0 is guaranteed to do nothing.
	//Whether it succeeds or fails, release_screenshot() must be the next called function. It is not allowed to ask for both screenshots then release both.
	virtual int get_screenshot(struct image * img) { img->pixels=NULL; return 0; }
	//Returns the last output of this object. This is different from the input if shaders are present.
	virtual int get_screenshot_out(struct image * img) { return get_screenshot(img); }
	//Frees the data returned from the above.
	virtual void release_screenshot(int ret, void* data) {}
	
	virtual ~video() = 0;
	
	//Used by various video drivers and other devices.
	static inline void copy_2d(void* dst, size_t dstpitch, const void* src, size_t srcpitch, size_t bytes_per_line, uint32_t height, bool full_write=false)
	{
		if (srcpitch==dstpitch) memcpy(dst, src, srcpitch*(height-1)+bytes_per_line);
		else
		{
			for (unsigned int i=0;i<height;i++)
			{
				memcpy((uint8_t*)dst + dstpitch*i, (uint8_t*)src + srcpitch*i, bytes_per_line);
				if (full_write && dstpitch>bytes_per_line) memset((uint8_t*)dst + dstpitch*i + bytes_per_line, 0, dstpitch-bytes_per_line);
			}
		}
	}
};
inline video::~video(){}
inline video::shader::~shader(){}



class audio : nocopy {
public:
	struct driver {
		const char * name;
		audio* (*create)(uintptr_t windowhandle, unsigned int device, unsigned int latency, double samplerate, bool exclusive);
		
		//The list is terminated by a 0, or you can get the length to the argument. They will be sorted ascending.
		//The list can be empty, in which case any sample rate is supported (within reasonable bounds).
		const unsigned int * (*samplerate)(unsigned int * count);
		
		uint32_t features;
	};
	
private:
	static const driver driver_pulseaudio;
	static const driver driver_wasapi;
	static const driver driver_directsound;
	static const driver driver_none;
	
public:
	static const driver* const drivers[];
	
	//Device 0 is always the default, named "(default)". The list is terminated by a NULL, or you can get the number into numdevices.
	static const char * const * get_devices(unsigned int * numdevices);
	
	class resampler {
	public:
		
		virtual ~resampler() = 0;
	};
	
	enum {
		f_sync     = 0x0800,//Can refuse to return from render() for a while, if it's called too fast.
		f_nosync   = 0x0400,//Can discard input to render() if it's called too fast.
		f_resample = 0x0200,//Can render audio at any sample rate. If not, call drivers->samplerate() and it will tell which sample rates it supports.
		f_anydevice= 0x0100,//Can use any audio device attached to the current machine.7
		f_pullmode = 0x0080,//Can poll the program for audio data, instead of waiting for the application to call render().
		f_selftest = 0x0040,//Can tell how much of the audio buffer is used. Required for DRC.
		f_automute = 0x0020,//If render() is not called for a while, the driver silences itself. The alternative is repeating something recent, or other garbled data.
		f_drc      = 0x0010,//Can change sample rate without resetting anything. Implies f_resample.
		f_exclusive= 0x0008,//Can take exclusive control of the system audio. If this is done, latency is lowered, but at the obvious cost.
		f_direct   = 0x0004,//Does not go through a separate process. Improves latency.
		f_remote   = 0x0002,//Compatible with X11 remoting, or equivalent. Implies !f_direct.
		f_public   = 0x0001,//Does not require elevated privileges to use. While this makes the device a lot more useful, insufficient privileges make it fail creation, and so it should have little if any effect on creation order.
	};
	virtual uint32_t features() = 0;
	
	//Only this audio format is supported because that's what libretro does.
	virtual void render(const int16_t* samples, size_t nsamples) = 0;
	//Empties the current audio buffer. Use if you're not going to call render() for a while.
	virtual void cancel() = 0;
	
	//Enables pull mode. The given function will be called as often as needed.
	//The function must render the requested number of samples, and must be prepared for any value in nsamples.
	virtual void enable_pullmode(function<void(int16_t* samples, size_t nsamples)>) {}
	
	//Sizes are in samples.
	virtual void get_bufuse(unsigned int * used, unsigned int * size) { if (used) *used=0; if (size) *size=0; }
	
	//Syncing is the act of sleeping in render() if too much data is pushed. Does not affect results if too little data is pushed.
	virtual void set_sync(bool sync) {}
	
	//Latency is in milliseconds. May reset the audio buffers.
	virtual void set_bufsize(unsigned int latency) {}
	
	//Sample rate is samples per second.
	virtual void set_samplerate(double samplerate) {}
	
	//TODO: multiple speakers
	//TODO: the-device-you're-rendering-to-has-become-invalid notifications
	
	virtual ~audio() = 0;
};
inline audio::~audio(){}
inline audio::resampler::~resampler(){}

struct caudio {
	//Plays the given samples. They're interleaved left then right then left then right; one pair is
	// one frame.
	void (*render)(struct caudio * This, unsigned int numframes, const int16_t * samples);
	
	//Clears out the sound buffer, silencing the buffer until the next call to render(). It is
	// implementation defined whether what's already in the buffer is played.
	void (*clear)(struct caudio * This);
	
	//It is implementation defined whether doing this will drop whatever is currently in the sound buffer.
	void (*set_samplerate)(struct caudio * This, double samplerate);
	void (*set_latency)(struct caudio * This, double latency);
	
	//Toggles synchronization, that is, whether render() should wait or drop some samples if there's
	// insufficient freespace in the internal sound buffer. Defaults to wait.
	void (*set_sync)(struct caudio * This, bool sync);
	
	bool (*has_sync)(struct caudio * This);
	
	//Deletes the structure.
	void (*free)(struct caudio * This);
};

const char * const * audio_supported_backends();
struct caudio * audio_create(const char * backend, uintptr_t windowhandle, double samplerate, double latency);

//Should be used only within audio_create.
#ifdef AUDIO_ALSA
struct caudio * audio_create_alsa(uintptr_t windowhandle, double samplerate, double latency);
#endif
#ifdef AUDIO_PULSEAUDIO
struct caudio * audio_create_pulseaudio(uintptr_t windowhandle, double samplerate, double latency);
#endif
#ifdef AUDIO_DIRECTSOUND
struct caudio * audio_create_directsound(uintptr_t windowhandle, double samplerate, double latency);
#endif
#ifdef AUDIO_WASAPI
struct caudio * audio_create_wasapi(uintptr_t windowhandle, double samplerate, double latency);
#endif
struct caudio * audio_create_none(uintptr_t windowhandle, double samplerate, double latency);



//inputkb is a quite low-level structure. You'll have to keep the state yourself.
class inputkb : nocopy {
public:
	struct driver {
		const char * name;
		inputkb* (*create)(uintptr_t windowhandle);
		uint32_t features;
	};
	
private:
	static const driver driver_rawinput;
	static const driver driver_udev;
	static const driver driver_gdk;
	static const driver driver_xinput2;
	static const driver driver_directinput;
	static const driver driver_x11;
	static const driver driver_none;
	
public:
	static const driver* const drivers[];
	
protected:
	function<void(unsigned int keyboard, unsigned int scancode, unsigned int libretrocode, bool down)> key_cb;
	
public:
	//It is safe to set this callback multiple times; the latest one applies. It is also safe to not set it at all, though that makes the structure quite useless.
	//If the driver can differ between multiple keyboards, 'keyboard' is unique for each keyboard, and
	// no greater than the number of connected keyboards. If it sees a merged view, 'keyboard' is 0.
	//scancode is in the range 0..1023. Scancodes may vary across drivers, but not across multiple keyboards.
	//The scancode always exists; if a driver truly lacks them, it should use the libretrocode instead.
	//libretrocode is in the RETROK_* enum (0..RETROK_LAST-1). However, it can be RETROK_UNKNOWN if the user hit an undefined key.
	//Recipents should be prepared for RETROK_LAST increasing up to 1024, and for previously unused values to be assigned a meaning.
	//The driver may repeat the current state. It may say 'not pressed' for keys that are impossible to hit on the current hardware.
	//Keep in mind that the callback may be called from window_run_*(), which may be on another thread than what created the object.
	void set_kb_cb(function<void(unsigned int keyboard, unsigned int scancode, unsigned int libretrocode, bool down)> key_cb) { this->key_cb = key_cb; }
	
	//Returns the features this driver supports. Numerically higher is better. Some flags contradict each other.
	enum {
		f_multi    = 0x0100,//Can differ between multiple keyboards.
		f_multiconst=0x0080,//Given unchanging hardware, the same keyboard will always have the same ID, even across reboots. The alternative is giving ID 1 to the first device that sends an event. [TODO: Check which of them does this.]
		f_delta    = 0x0040,//Does not call the callback for unchanged state. Improves processing time. Key repeat events may still happen.
		f_auto     = 0x0020,//poll() is empty, and the callback is called by window_run_*(). Implies f_delta.
		f_direct   = 0x0010,//Does not go through a separate process. Improves latency.
		f_background=0x0008,//Can view input events while the window is not focused. Implies f_auto.
		f_pollable = 0x0004,//refresh() is implemented.
		f_remote   = 0x0002,//Compatible with X11 remoting, or equivalent. Implies !f_direct.
		f_public   = 0x0001,//Does not require elevated privileges to use. While this makes the device a lot more useful, insufficient privileges make it fail creation, and so it should have little if any effect on creation order.
	};
	virtual uint32_t features() = 0;
	
	//Returns the number of keyboards.
	//virtual unsigned int numkb() { return 1; }
	
	//If f_pollable is set, this calls the callback for all pressed keys.
	//The implementation is allowed to call it for non-pressed keys.
	virtual void refresh() {}
	
	//If f_auto is not set, this calls the callback for all key states that changed since the last poll().
	//The implementation is allowed to call it for unchanged keys.
	virtual void poll() {}
	
	virtual ~inputkb() = 0;
	
	//These translate hardware scancodes or virtual keycodes to libretro cores. Can return RETROK_UNKNOWN.
	//Note that "virtual keycode" is platform dependent, and because they're huge on X11, they don't exist at all there.
	//void inputkb_translate_init();
	static unsigned translate_scan(unsigned int scancode);
	static unsigned translate_vkey(unsigned int vkey);
	
	//Returns an array containing the names of all keys, in retro_key order. ret[RETROK_BACKSPACE] is "Backspace"; nonexistent elements are NULL.
	//Guaranteed to contain only alphanumeric characters, and guaranteed to not start with a lowercase letter.
	static const char * const * keynames();
};
inline inputkb::~inputkb(){}


//In minir, mouse and cursor are two separate concepts. Mouse is the physical device; cursor is the pointer on the screen.
class inputmouse {
public:
	struct driver {
		const char * name;
		inputmouse* (*create)(uintptr_t windowhandle);
		uint32_t features;
	};
	
private:
	//TODO: do I want all of these? do they even make sense?
	static const driver driver_rawinput;
	static const driver driver_udev;
	static const driver driver_gdk;
	static const driver driver_xinput2;
	static const driver driver_directinput;
	static const driver driver_x11;
	static const driver driver_none;
	
public:
	static const driver* const drivers[];
	
protected:
	function<void(unsigned int mouse, signed int x, signed int y)> move_cb;
	function<void(unsigned int mouse, unsigned int button, bool down)> button_cb;
	
public:
	//this class is quite ugly, but it lets me import the enum to inputcursor too
	class button { public: enum { left, right, middle, x4, x5 }; };
	//x4 and higher are unsupported because I don't have any mice with extra buttons
	
	void set_listeners(function<void(unsigned int mouse, signed int x, signed int y)> move_cb,
	                   function<void(unsigned int mouse, unsigned int button, bool down)> button_cb)
	                   //TODO: mouse wheel?
	{
		this->move_cb = move_cb;
		this->button_cb = button_cb;
	}
	
	//Returns the features this driver supports. Numerically higher is better. Some flags contradict each other.
	enum {
		f_multi    = 0x0010,//Can differ between multiple mice.
		f_auto     = 0x0008,//poll() is empty, and the callback is called by window_run_*().
		f_direct   = 0x0004,//Does not go through a separate process. Improves latency.
		f_remote   = 0x0002,//Compatible with X11 remoting, or equivalent. Implies !f_direct.
		f_public   = 0x0001,//Does not require elevated privileges to use.
	};
	//virtual uint32_t features() = 0; // Features are constantly known at the start.
	
	//Calls the callbacks for all pressed buttons. May return mouse movement too.
	virtual void refresh() { poll(); }
	
	//If f_auto is not set, this calls the callback for all state that changed since the last poll().
	//The implementation is allowed to call the callbacks for unchanged state.
	virtual void poll() = 0;
	
	virtual ~inputmouse() = 0;
};
inline inputmouse::~inputmouse(){}


//TODO: merge inputmouse into this and have both cursor/mouse arguments to a bunch of functions?
class inputcursor {
public:
	struct driver {
		const char * name;
		inputcursor* (*create)(uintptr_t windowhandle);
		uint32_t features;
	};
	
private:
	static const driver driver_xrecord; //http://www.x.org/docs/Xext/recordlib.pdf
	static const driver driver_rawinput;
	static const driver driver_w32hook; //http://msdn.microsoft.com/en-us/library/windows/desktop/ms644990%28v=vs.85%29.aspx - not recommended
	static const driver driver_x11;
	static const driver driver_w32msg; //WM_MOUSE - hard to do with minir's architecture (especially since I'd be glad to have WM_NCMOUSEMOVE too)
	static const driver driver_none;
	
public:
	static const driver* const drivers[];
	
protected:
	function<void(unsigned int cursor, signed int x, signed int y)> move_cb;
	function<void(unsigned int cursor, signed int x, signed int y)> move_grab_cb;
	function<void(unsigned int cursor, unsigned int button, bool down)> button_cb;
	
public:
	class button : public inputmouse::button {};//import the button enum from inputmouse
	
	//move_cb contains position relative to the desktop. move_grab_cb contains the distance since the last call.
	void set_listeners(function<void(unsigned int cursor, signed int x, signed int y)> move_cb,
	                   function<void(unsigned int cursor, signed int x, signed int y)> move_grab_cb,
	                   function<void(unsigned int cursor, unsigned int button, bool down)> button_cb)
	                   //TODO: mouse wheel?
	{
		this->move_cb = move_cb;
		this->move_grab_cb = move_grab_cb;
		this->button_cb = button_cb;
	}
	
	//Returns the features this driver supports. Numerically higher is better. Some flags contradict each other.
	enum {
		f_outside  = 0x0400,//Can show cursor position while it is not on top of the owner window.
		f_move     = 0x0200,//Can move the cursor to another position.
		f_grab     = 0x0100,//Can remove the cursor from the screen and send its movement to move_grab_cb instead. There may be ways for the user to get his mouse back.
		f_hide     = 0x0080,//Can remove the cursor from the screen. It's still there, it just won't show up. It will reappear if the mouse leaves the window.
		f_delta    = 0x0040,//Only fires events if the cursor state has changed. Improves processing time.
		f_multi    = 0x0020,//Can differ between multiple mice.
		f_background=0x0010,//Can show cursor position while the owner window is not the foreground window.
		f_auto     = 0x0008,//poll() is empty, and the callback is called by window_run_*().
		f_direct   = 0x0004,//Does not go through a separate process. Improves latency.
		f_remote   = 0x0002,//Compatible with X11 remoting, or equivalent. Implies !f_direct.
		f_public   = 0x0001,//Does not require elevated privileges to use.
	};
	
	//Calls the callbacks for all state.
	virtual void refresh() = 0;
	
	//If f_auto is not set, this calls the callback for all state that changed since the last poll().
	//The implementation is allowed to call the callbacks for unchanged state.
	virtual void poll() { refresh(); }
	
	//Moves the cursor so that the next refresh() (assuming no user input) will return x,y.
	virtual void move(unsigned int cursor, signed int x, signed int y) {}
	
	class mode { public: enum grabmode { none, hide, grab }; };
	virtual void grab(unsigned int cursor, mode::grabmode mode) {}
	
	virtual ~inputcursor() = 0;
};
inline inputcursor::~inputcursor(){}



struct inputjoy;



struct inputmapper {
	//Asks whether a button is pressed. If oneshot is set, the button is considered released after
	// being held for one frame. If the button is not mapped to anything, it's considered unheld.
	//It is safe to query the same button for both oneshot and non-oneshot.
	bool (*button)(struct inputmapper * This, unsigned int id, bool oneshot);
	
	//Tells which key was pressed (actually released) last. You're responsible for setting it free()
	// when you're done. Includes all relevant shift states.
	//It is undefined whether you'll miss anything if you call poll() twice without calling this. It
	// is undefined what you'll get if you call it twice without a poll() in between. It is undefined
	// which of them is returned if two keys are released at the same time.
	char * (*last)(struct inputmapper * This);
	
	//Sets where to get keyboard input. The inputmapper takes ownership of this item; it can not be used
	// anymore after calling this, not even to free it (the inputmapper takes care of that).
	void (*set_keyboard)(struct inputmapper * This, struct inputkb * in);
	
	//Maps an input descriptor (a string) to an input ID.
	//IDs should be assigned to low numbers (0 is fine); leaving big unused holes is wasteful, though
	// small ones don't matter too much.
	//Input descriptors are human readable and human editable text strings, for example KB1::ShiftR+F1,
	// if the input driver maps them to RetroKeyboard properly; if the driver behaves poorly, it ends
	// up as a mess (for example KB1::x4B+x54).
	//However, no matter how they look, they should not be parsed; they can be shown to the user,
	// stored in a text file, or whatever, but don't chop them apart.
	//Note that while A+B+C is the same as B+A+C, A+C+B is not the same. The last listed key is the
	// actual trigger; the others are treated as 'shift states'. If something is mapped to A+B+C, and
	// the keys are pressed in order A C B, A+B+C will be recorded as held, but it will never actually
	// fire if queried as oneshot.
	//Also note that mapping one input can affect the others; if H is mapped somewhere, L+H will
	// trigger it, but if L+H is mapped to something else, the plain H is no longer triggered by L+H.
	//Since all keys can be shift states in that way, Ctrl, Shift and Alt are not given special treatment.
	//Non-keyboard inputs are not supported right now.
	//The return value is whether it worked; if it didn't, the input slot will be unmapped.
	//A NULL or an empty string will return success and unmap the key.
	bool (*map_key)(struct inputmapper * This, const char * descriptor, unsigned int id);
	
	//Tells the structure to ask the inputraw for updates. Clears all prior oneshot flags.
	void (*poll)(struct inputmapper * This);
	
	void (*free)(struct inputmapper * This);
};
struct inputmapper * inputmapper_create();

//Takes an input descriptor and verifies that it's understandable; if it is, it's normalized into
// the mapper's favourite format, otherwise you get a NULL.
//Free it once you're done.
char * inputmapper_normalize(const char * descriptor);



class inputmapper2 : nocopy {
protected:
	function<void(unsigned int id, bool down)> callback;
public:
	void set_cb(function<void(unsigned int id, bool down)> callback) { this->callback=callback; }
	
	//void request_next(function<void(const char * desc)> callback) { this->req_callback=callback; }
	
	//Each input descriptor has an unique ID, known as its slot. 0 is valid.
	//It's fine to set a descriptor slot that's already used; this will remove the old one.
	//It's also fine to set a descriptor to NULL. This is the default for any slot which is not set, and will never fire.
	//If the descriptor is invalid, the slot will be set to NULL, and false will be returned.
	
	//The implementation may limit the maximum number of modifiers on any descriptor. At least 15 modifiers
	// must be supported, but more is allowed. If it goes above this limit, the descriptor is rejected.
	//'trigger' is whether it's a trigger-based or level-based event.
	//A level-triggered event fires the event whenever the combination changes between held and not,
	// while trigger-based fire when the relevant modifiers are held and one of the primaries is hit.
	//Examples of differences: If a slot is A+S and someone is holding A and repeatedly slapping S,
	// a level-triggered event will only fire when A is pressed, because the total state is still 'down'.
	// A trigger-based event will fire for each press of either key.
	//If a slot is A+S and S is hit before A, the trigger will never fire (the modifier, A,
	// had wrong state when S was pressed), but the level will fire (the combination is true).
	//They share their ID namespaces. A trigger event never fires with down=false.
	virtual bool register_button(unsigned int id, const char * desc, bool trigger) = 0;
	
	//Returns the lowest slot ID where the given number of descriptors can be sequentially added.
	//If called for len=4 and it returns 2, it means that slots 2, 3, 4 and 5 are currently unused.
	//It doesn't actually reserve anything, or otherwise change the state of the object; it just tells the current state.
	
	//The implementation may set an upper bound on the maximum valid slot. All values up to 4095 must work,
	// but if this is exceeded, behaviour is undefined.
	virtual unsigned int register_group(unsigned int len) = 0;
	//If you don't want to decide which slot to use, this one will pick an unused slot and tell which it used.
	//If the descriptor is invalid, -1 will be returned, and no slot will change.
	int register_button(const char * desc, bool trigger)
	{
		int slot = register_group(1);
		if (register_button(slot, desc, trigger)) return slot;
		else return -1;
	}
	
	enum dev_t {
		dev_unknown,
		dev_kb,
		dev_mouse,
		dev_gamepad,
	};
	enum { mb_left, mb_right, mb_middle, mb_x4, mb_x5 };
	
	//While you can emit events here directly, it's easier to bind an inputkb callback to event_kb, and similar.
	virtual void event(dev_t type, uint16_t device, uint16_t item, int16_t level) = 0;
	
	void event_kb(unsigned int keyboard, unsigned int scancode, unsigned int libretrocode, bool down)
	{
		event(dev_kb, keyboard, libretrocode?libretrocode:scancode|0x400, down);
	}
	
	//Returns the state of a button.
	virtual bool query(dev_t type, unsigned int device, unsigned int button, unsigned int scancode) = 0;
	
	//Returns the state of an input slot.
	virtual bool query_slot(unsigned int slot) = 0;
	
	//Releases all buttons held on the indicated device type. Can be dev_unknown to reset everything. The callback is not called.
	//This is likely paired with a refresh() on the relevant inputkb/etc. To avoid calling the callback for that, set it to NULL.
	virtual void reset(dev_t type) = 0;
	
	static inputmapper2* create();
	virtual ~inputmapper2(){}
};





struct cvideo {
	//Initializes the video system. It will draw on the windowhandle given during creation, at the given bit depth.
	//The user guarantees that the window is size screen_width*screen_height when This is called, and at
	// every subsequent call to draw(). If the window is resized, reinit() (or free()) must be called again.
	//The bit depths may be 32 (XRGB8888), 16 (RGB565), or 15 (0RGB1555).
	void (*reinit)(struct cvideo * This, unsigned int screen_width, unsigned int screen_height, videoformat format, double fps);
	
	//Draws the given data. Size doesn't need to be same as above; if it isn't, nearest neighbor scaling will be used.
	//pitch is how many bytes to go forward to reach the next scanline.
	//If data is NULL, the last frame is redrawn, and other arguments are ignored. It will still wait for vsync.
	void (*draw)(struct cvideo * This, unsigned int width, unsigned int height, const void * data, unsigned int pitch);
	
	//Toggles vsync; that is, whether draw() should wait for vblank before doing its stuff and
	// returning. Defaults to on; does not change on reinit().
	//Returns the previous state, if syncing is possible; otherwise, returns an undefined value.
	bool (*set_sync)(struct cvideo * This, bool sync);
	
	//Whether vsync can be enabled on This item.
	bool (*has_sync)(struct cvideo * This);
	
	//Deletes the structure.
	void (*free)(struct cvideo * This);
};

video* video_create_compat(cvideo* child);
