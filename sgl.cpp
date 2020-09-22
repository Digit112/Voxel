namespace sgl {
	// Event_map constructor
	event_map::event_map() {
		state = NULL;
		
		key_press   = NULL;
		key_repeat  = NULL;
		key_release = NULL;
		
		button_press   = NULL;
		button_release = NULL;
		
		motion = NULL;
		
		focus_in = NULL;
		focus_out = NULL;
		
		modify = NULL;
		
		init = NULL;
		draw_init = NULL;
		draw = NULL;
		update = NULL;
		exit = NULL;
	}
	
	// app_handle constructor
	app_handle::app_handle() : do_destroy(false), is_focused(true), is_hovered(false) {
		for (int i = 0; i < 198; i++) {
			key_states[i] = false;
		}
		for (int i = 0; i < 9; i++) {
			button_states[i] = false;
		}
	};
	
	// Creates a display
	int app_handle::mainloop(init_data id, event_map em) {
		is_focused = true;
		
		d = XOpenDisplay(NULL);
		if (d == NULL) {
			return 1;
		}
		
		s = DefaultScreen(d);
		w = XCreateSimpleWindow(d, DefaultRootWindow(d), 0, 0, id.width, id.height, id.border, id.foreground, id.background);
		
		win_w = id.width;
		win_h = id.height;
		win_x = 0;
		win_y = 0;
		
		gc = XCreateGC(d, w, 0, NULL);
		XGetGCValues(d, gc, 0xffffffff, &gcv);
		gcv.foreground = id.foreground;
		gcv.background = id.background;
		
//		XSetLineAttributes(d, gc, 4, LineSolid, CapButt, JoinMiter);
		
		XVisualInfo vi;
		XMatchVisualInfo(d, s, 24, TrueColor, &vi);
		v = vi.visual;
		
		sp = DefaultScreenOfDisplay(d);
		sw = WidthOfScreen(sp);
		sh = HeightOfScreen(sp);
		sd = DefaultDepthOfScreen(sp);
		
		pm = XCreatePixmap(d, w, sw, sh, sd);
		XSetForeground(d, gc, gcv.background);
		XFillRectangle(d, pm, gc, 0, 0, sw, sh);
		XSetForeground(d, gc, gcv.foreground);
		
		XStoreName(d, w, id.title);
				
		XSelectInput(d, w, KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | EnterWindowMask | LeaveWindowMask | FocusChangeMask | StructureNotifyMask);
		XMapWindow(d, w);
		
		Atom window_delete = XInternAtom(d, "WM_DELETE_WINDOW", False); 
		XSetWMProtocols(d, w, &window_delete, 1);
		
		int min_kc;
		int max_kc;
		int ksprkc;
		XDisplayKeycodes(d, &min_kc, &max_kc);
		KeySym* ks = XGetKeyboardMapping(d, min_kc, max_kc - min_kc, &ksprkc);
		
		if (em.init != NULL) {
			em.init(*this, em.state);
		}
		
		double loop_start = 0;
		double draw_timer = 0;
		int queued;
		XEvent xe;
		XEvent nxe;
		event e;
		while (1) {
			while (clock() / (double) CLOCKS_PER_SEC - loop_start < 0.002) {}
			loop_start = clock() / (double) CLOCKS_PER_SEC;
			
			queued = XEventsQueued(d, QueuedAfterFlush);
			for (int i = 0; i < queued; i++) {
				while (clock() / (double) CLOCKS_PER_SEC - loop_start < 0.001) {}
				loop_start = clock() / (double) CLOCKS_PER_SEC;
				
				XNextEvent(d, &xe);
				
				// Handle window resize and movement
				if (xe.type == ConfigureNotify) {
					if (win_w != xe.xconfigure.width || win_h != xe.xconfigure.height) {
						win_w = xe.xconfigure.width;
						win_h = xe.xconfigure.height;
					} else {
						win_x = xe.xconfigure.x;
						win_y = xe.xconfigure.y;
					}
					if (em.modify != NULL) {
						e = {0, 0, 0, 0, win_w, win_h, win_x, win_y};
						em.modify(e, *this, em.state);
					}
				}
				// Handle MapNotify event
				else if (xe.type == MapNotify) {
					if (em.draw_init != NULL) {
						em.draw_init(*this, em.state);
					}
				}
				else if (xe.type == FocusIn) {
					is_focused = true;
					if (em.focus_in != NULL) {
						em.focus_in(*this, em.state);
					}
				}
				else if (xe.type == FocusOut) {
					is_focused = false;
					if (em.focus_out != NULL) {
						em.focus_out(*this, em.state);
					}
				}
				else if (xe.type == EnterNotify) {
					is_hovered = true;
				}
				else if (xe.type == LeaveNotify) {
					is_hovered = false;
				}
				// Check for key press events
				else if (xe.type == KeyPress) {
					key_states[xe.xkey.keycode - 8] = true;
					if (em.key_press != NULL) {
						e = {xe.xkey.x, xe.xkey.y, xe.xkey.keycode, translate_key(ks, min_kc, ksprkc, xe.xkey.keycode, xe.xkey.state)};
						em.key_press(e, *this, em.state);
					}
				}
				// Check for key release events
				else if (xe.type == KeyRelease) {
					e = {xe.xkey.x, xe.xkey.y, xe.xkey.keycode, translate_key(ks, min_kc, ksprkc, xe.xkey.keycode, xe.xkey.state)};
					// Check if this is an AutoRepeat release. If so, call key_repeat rather than key_release
					if (XPending(d) > 0) {
						XPeekEvent(d, &nxe);
						if (nxe.type == KeyPress && nxe.xkey.time == xe.xkey.time) {
							XNextEvent(d, &xe);
							if (em.key_repeat != NULL) {
								em.key_repeat(e, *this, em.state);
							}
						} else {
							key_states[xe.xkey.keycode - 8] = false;
							if (em.key_release != NULL) {
								em.key_release(e, *this, em.state);
							}
						}
					} else {
						key_states[xe.xkey.keycode - 8] = false;
						if (em.key_release != NULL) {
							em.key_release(e, *this, em.state);
						}
					}
				}
				else if (xe.type == ButtonPress) {
					button_states[xe.xbutton.button - 1] = true;
					if (em.button_press != NULL) {
						e = {xe.xbutton.x, xe.xbutton.y, xe.xbutton.button};
						em.button_press(e, *this, em.state);
					}
				}
				else if (xe.type == ButtonRelease) {
					button_states[xe.xbutton.button - 1] = false;
					if (em.button_release != NULL) {
						e = {xe.xbutton.x, xe.xbutton.y, xe.xbutton.button};
						em.button_release(e, *this, em.state);
					}
				}
				else if (xe.type == MotionNotify) {
					if (em.motion != NULL) {
						e = {xe.xmotion.x, xe.xmotion.y};
						em.motion(e, *this, em.state);
					}
				}
				else if ((xe.type == ClientMessage) && (static_cast<unsigned int>(xe.xclient.data.l[0]) == window_delete)) {
					if (em.exit == NULL || em.exit(*this, em.state)) {
						exit_display();
					}
				}
				
				// Check if the destroy flag is set
				if (do_destroy) {
					XDestroyWindow(d, w);
					XFree((void*) ks);
					XFreeGC(d, gc);
					XFreePixmap(d, pm);
					XCloseDisplay(d);
					break;
				}
			}
				
			if (do_destroy) {
				break;
			}
			
			if (em.update != NULL) {
				em.update(*this, em.state);
			}
			
			if (em.draw != NULL && clock() / (double) CLOCKS_PER_SEC - draw_timer > 0.016) {
				em.draw(*this, em.state, clock() / (double) CLOCKS_PER_SEC - draw_timer);
				draw_timer = clock() / (double) CLOCKS_PER_SEC;
			}
		}
		
		return 0;
	}

	// Destroy the display on the next event cycle.
	void app_handle::exit_display() {
		do_destroy = true;
	}
	
	bool app_handle::get_key(unsigned int key) {
		return key_states[key - 8];
	}
	
	bool app_handle::get_button(unsigned int button) {
		return button_states[button - 1];
	}
	
	bool app_handle::get_focus() {
		return is_focused;
	}
	
	bool app_handle::get_hover() {
		return is_hovered;
	}
	
	veci2 app_handle::get_pointer() {
		veci2 out;
		Window a;
		int b;
		unsigned int c;
		XQueryPointer(d, w, &a, &a, &b, &b, &out.x, &out.y, &c);
		return out;
	}
	
	void app_handle::set_pointer(int x, int y) {
		if (is_focused) {
			XWarpPointer(d, w, w, 0, 0, 0, 0, x, y);
		}
	}
	
	void app_handle::hide_pointer() {
		static char data[1] = {0};
		Cursor cur;
		XColor a;
		Pixmap blank;
		blank = XCreateBitmapFromData(d, w, data, 1, 1);
		cur = XCreatePixmapCursor(d, blank, blank, &a, &a, 0, 0);
		XFreePixmap(d, blank);
		XDefineCursor(d, w, cur);
	}
	
	void app_handle::show_pointer() {
		XDefineCursor(d, w, XCreateFontCursor(d, 68));
	}
	
	void app_handle::draw_pixel(int x, int y) {
		XDrawPoint(d, pm, gc, x, y);
	}
	
	void app_handle::draw_line(int x1, int y1, int x2, int y2) {
		XDrawLine(d, pm, gc, x1, y1, x2, y2);
	}
	
	void app_handle::draw_rect(int x1, int y1, int x2, int y2) {
		int t;
		if (x1 > x2) {
			t = x1;
			x1 = x2;
			x2 = t;
		}
		if (y1 > y2) {
			t = y1;
			y1 = y2;
			y2 = t;
		}
		XDrawRectangle(d, pm, gc, x1, y1, abs(x1 - x2), abs(y1 - y2));
	}
	
	void app_handle::fill_rect(int x1, int y1, int x2, int y2) {
		int t;
		if (x1 > x2) {
			t = x1;
			x1 = x2;
			x2 = t;
		}
		if (y1 > y2) {
			t = y1;
			y1 = y2;
			y2 = t;
		}
		XFillRectangle(d, pm, gc, x1, y1, abs(x1 - x2), abs(y1 - y2));
	}
	
	void app_handle::draw_text(char* t, int size, int x, int y) {
		char* str = (char*) malloc(128 * sizeof(char*));
		sprintf(str, "-bitstream-bitstream charter-medium-i-normal--%d-0-0-0-p-0-iso10646-1", size);
		Font f = XLoadFont(d, str);
		XTextItem i = {t, (int) strlen(t), 0, f};
		XDrawText(d, pm, gc, x, y, &i, 1);
		free(str);
	}
	
	void app_handle::set_foreground(unsigned long c) {
		gcv.foreground = c;
		XSetForeground(d, gc, gcv.foreground);
	}
	
	void app_handle::set_background(unsigned long c) {
		gcv.background = c;
		XSetBackground(d, gc, gcv.background);
	}
	
	void app_handle::clear_display() {
		XSetForeground(d, gc, gcv.background);
		XFillRectangle(d, pm, gc, 0, 0, sw, sh);
		XSetForeground(d, gc, gcv.foreground);
	}
	
	void app_handle::update_display() {
		XCopyArea(d, pm, w, gc, 0, 0, win_w, win_h, 0, 0);
	}
	
	unsigned long app_handle::translate_key(KeySym* ks, unsigned int min_kc, unsigned int ksprkc, unsigned int keycode, unsigned int state) {
		int offset;
		KeySym t;
		
//		int shift = state & 1;
//		int caps  = state & 2;
//		int ctrl  = state & 4;
//		int alt   = state & 8;
//		int num   = state & 16;
		
		// If no modifiers are active use the first keysym
		if (! (state & 2) && ! (state & 1)) {
			return ks[(keycode - min_kc) * ksprkc];
		}
		
		// If shift is off and capslock is on, convert lowercase letters to uppercase letters
		if (state & 2 && ! (state & 1)) {
			t = (keycode - min_kc) * ksprkc;
			if (ks[t] >= 97 && ks[t] <= 122) {
				return ks[t] - 32;
			} else {
				return ks[t];
			}
		}
		
		// If shift and capslock are on, convert uppercase letters to lowercase letters
		if (state & 2 && state & 1) {
			t = (keycode - min_kc) * ksprkc + 1;
			if (ks[t] == NoSymbol) {
				return ks[t-1];
			}
			
			if (ks[t] >= 65 && ks[t] <= 90) {
				return ks[t] + 32;
			} else {
				return ks[t];
			}
		}
		
		// If shift is on and capslock is off, use the second keysym if available
		if (state & 1 && ! (state & 2)) {
			t = (keycode - min_kc) * ksprkc + 1;
			if (ks[t] == NoSymbol) {
				return ks[t-1];
			}
			return ks[t];
		}
	}
	
	// Render the passed object and all its children
	void app_handle::render(cam c, object& o, void* s, double dt) {
		// Recursively call this function to render this object's children
		for (int i = 0; i < o.children.size; i++) {
			this->render(c, *o.children[i], s, dt);
		}
		
		// Render this object
		unsigned short rf = o.render_function;
		if (rf > 3) {
			printf("Error: Invalid render_function of %u.\n", rf);
			exit(1);
		}
		if (rf == 0) {
			o.raster_method(this, &c, &o, s, dt);
		}
		else if (rf == 1) {
			printf("Logic not yet implemented for kernels.\n");
		}
		else if (rf == 2) {
			o.raytrace_method(this, &c, &o, s, dt);
		}
		else if (rf == 3) {
			printf("Logic not yet implemented for kernels.\n");
		}
	}
}
































