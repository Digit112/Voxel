#ifndef sgl_lib
#define sgl_lib

#include <math.h>
#include <stdlib.h>
#include <time.h>

namespace sgl {	
	// Handle contains variables used to keep track of a specific window.
	class app_handle {
	public:
		// X11 hooks
		Display* d;
		int s;
		Window w;
		
		Visual* v;
		
		Screen* sp;
		
		GC gc;
		XGCValues gcv;
		Pixmap pm;
		
		// Window width, height, and position
		int win_w;
		int win_h;
		int win_x;
		int win_y;
		
		// Screen width, height, and depth
		int sw;
		int sh;
		int sd;
		
		// Set to true to destroy the window in the next event cycle
		bool do_destroy;
		
		// Whether the window has focus
		bool is_focused;
		
		// Whether the mouse is over the window
		bool is_hovered;
		
		// Keycode states for querying keys
		unsigned int key_states[198];
		
		// Button states for querying mouse button states
		unsigned int button_states[9];
		
		app_handle();

		// Enters the main loop
		int mainloop(init_data id, event_map em);

		// Destroy the display.
		void exit_display();
		
		bool get_key(unsigned int key);
		
		bool get_button(unsigned int button);
		
		// Get whether the window is focused
		bool get_focus();
		
		// Get whether the mouse is over this window
		bool get_hover();
		
		// Get the mouse location
		veci2 get_pointer();
		
		// Set the mouse location
		void set_pointer(int x, int y);
		
		// Make the pointer invisible
		void hide_pointer();
		
		// Make the pointer visible
		void show_pointer();
		
		// Draw functions
		void draw_pixel(int x, int y);
		void draw_line(int x1, int y1, int x2, int y2);
		void draw_rect(int x1, int y1, int x2, int y2);
		void fill_rect(int x1, int y1, int x2, int y2);
		
		void draw_text(char*, int size, int x, int y);
		
		// Modify GC
		void set_foreground(unsigned long c);
		void set_background(unsigned long c);
		
		// Wipe the buffer
		void clear_display();
		
		// Copy the buffer to the display
		void update_display();
		
		// Render the following object and all its children.
		void render(cam&, object&, void* state = NULL, double dt = 0);
	
	private:
		KeySym translate_key(KeySym*, unsigned int min_kc, unsigned int ksprkc, unsigned int keycode, unsigned int state);
	};
	
	// Holds data required for initializing a window
	class init_data {
	public:
		int width;  // Initial window width
		int height; // Initial window height
		
		int border; // Initial window border width
		
		char* title; // null-terminated title of the window
		
		unsigned long background; // 3-byte wide color representing the background color of the window
		unsigned long foreground; // 3-byte wide color representing the foreground color of the window
	};
	
	// Holds events
	struct event {
		// Cursor position relative to event window
		int cx;
		int cy;
		
		unsigned int code; // Key or button that has been pressed
		unsigned long symbol; // Symbol that was sent for key events
		
		// Position of a bounding box for exposure events
		int mx;
		int my;
		int Mx;
		int My;
	};
	
	// Holds pointers to functions that handle events
	// Each function takes an "event" struct and a void pointer which can be used to store information between events
	class event_map {
	public:
		void* state;
		
		void (*key_press)(event, app_handle&, void*);   // Called when a key is pressed
		void (*key_repeat)(event, app_handle&, void*);  // Called when AutoRepeat sends a key update
		void (*key_release)(event, app_handle&, void*); // Called when a key is released
		
		void (*button_press)(event, app_handle&, void*);   // Called when a mouse button is pressed
		void (*button_release)(event, app_handle&, void*); // Called when a mouse button is released
		
		void (*motion)(event, app_handle&, void*); // Called when the pointer is moved
		
		void (*focus_in)(app_handle&, void*);  // Called when the window gains focus
		void (*focus_out)(app_handle&, void*); // Called when the window loses focus
		
		void (*modify)(event, app_handle&, void*); // Called when the window is resized or moved
		
		void (*init)(app_handle&, void*);         // Runs once after all initialization steps but immediately prior to the event loop start.
		void (*draw_init)(app_handle&, void*);    // Runs once when the window appears on screen and can be drawn too.
		void (*draw)(app_handle&, void*, double); // Similar to update, but runs at most 60 times a second.
		void (*update)(app_handle&, void*);       // Runs immediately if no events need handling or alternates with event if events are available on the stack.
		bool (*exit)(app_handle&, void*);         // Runs when a request to close the window is recieved from the WM, returns whether to close the window.
		
		event_map();
	};
}

#include "sgl.cpp"

#endif






























