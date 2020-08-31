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
		
		modify = NULL;
		
		init = NULL;
		draw_init = NULL;
		draw = NULL;
		update = NULL;
		exit = NULL;
	}
	
	// app_handle constructor
	app_handle::app_handle() {
		do_destroy = false;
		
		for (int i = 0; i < 198; i++) {
			key_states[i] = false;
		}
		for (int i = 0; i < 9; i++) {
			button_states[i] = false;
		}
	};
	
	// Creates a display
	int app_handle::mainloop(init_data id, event_map em) {
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
		XMatchVisualInfo(d, s, sd, TrueColor, &vi);
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
				
		XSelectInput(d, w, KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | StructureNotifyMask);
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
					XCloseDisplay(d);
					XFree((void*) ks);
					XFreeGC(d, gc);
					XFreePixmap(d, pm);
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
				draw_timer = clock() / (double) CLOCKS_PER_SEC;
				em.draw(*this, em.state);
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
	
	void app_handle::render(cam c, const object* o, int o_n) {
		sgl::mesh_wire m;
		for (int i = 0; i < o_n; i++) {
			// Apply the inverse camera transform to all objects, then apply all object transforms to each verticy belonging to it.
			// This allows you to treat the camera as having a default position and rotation.
			c.apply(o[i].applied(m));
			
			// Convert theta to radians
			double theta = c.theta * 3.14159/180;
			
			// Shorthands used for the endpoints of an edge, their difference, and the intersection between this edge and the
			// clipping plane where appropriate.
			vecd3 a;
			vecd3 b;
			vecd3 dif;
			vecd3 clip_pt;
			
			// Holds the start and end points of the edge prior to render.
			veci2 s;
			veci2 e;
			
			// Cache trig value
			double slope = tan(theta/2);
			
			// Iterate over and render each edge.
			for (int j = 0; j < m.en; j++) {
				a = m.p[m.e[j].x];
				b = m.p[m.e[j].y];
				// If both points are behind the clipping plane, skip this edge.
				if (a.x < c.clipping && b.x < c.clipping) {
					continue;
				}
				// If one point is behind the clipping plane and the other is in front, find the intersection between the edge and
				// the clipping plane, and draw the line to that point instead.
				else if (a.x < c.clipping && b.x >= c.clipping) {
					dif = b - a;
					clip_pt = dif / dif.x * (c.clipping - a.x) + a;
					s.x = (int) ((clip_pt.y / (clip_pt.x * slope) + 1) / 2.0 * this->win_h);
					s.y = (int) ((clip_pt.z / (clip_pt.x * slope) + 1) / 2.0 * this->win_h);
					
					e.x = (int) ((b.y / (b.x * slope) + 1) / 2.0 * this->win_h);
					e.y = (int) ((b.z / (b.x * slope) + 1) / 2.0 * this->win_h);
				}
				else if (a.x >= c.clipping && b.x < c.clipping) {
					dif = a - b;
					clip_pt = dif / dif.x * (c.clipping - b.x) + b;
					s.x = (int) ((clip_pt.y / (clip_pt.x * slope) + 1) / 2.0 * this->win_h);
					s.y = (int) ((clip_pt.z / (clip_pt.x * slope) + 1) / 2.0 * this->win_h);
					
					e.x = (int) ((a.y / (a.x * slope) + 1) / 2.0 * this->win_h);
					e.y = (int) ((a.z / (a.x * slope) + 1) / 2.0 * this->win_h);
				}
				// If both points are in front of the clipping plane, project them both normally
				else {
					s.x = (int) ((a.y / (a.x * slope) + 1) / 2.0 * this->win_h);
					s.y = (int) ((a.z / (a.x * slope) + 1) / 2.0 * this->win_h);
					
					e.x = (int) ((b.y / (b.x * slope) + 1) / 2.0 * this->win_h);
					e.y = (int) ((b.z / (b.x * slope) + 1) / 2.0 * this->win_h);
				}
				
				this->draw_line(s.x, s.y, e.x, e.y);
			}
		}
	}
}
































