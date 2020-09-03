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
			double theta_x = c.theta * 3.14159/180;
			double theta_y = theta_x * (double) win_h / win_w;
			
			// Shorthands used for the endpoints of an edge, their difference, and the intersection between this edge and the
			// clipping plane where appropriate.
			vecd3 a; // Projected endpoints of an edge
			vecd3 b;
			vecd3 A; // Unprojected endpoints of an edge
			vecd3 B;
			vecd3 dif;
			double t;
			vecd3 clip_pt;
			
			// Holds the start and end points of the edge prior to render.
			veci2 s;
			veci2 e;
			
			// Cache trig value
			double slope_x = tan(theta_x/2);
			double slope_y = tan(theta_y/2);
			
			// Project each point within the viewing frustum to the cube (-1, -1, -1) (1, 1, 1)
			// Points behind the near clipping plane are skipped,
			// edges including this point are clipped against the near plane while iterating over edges
			darray<vecd3> p_pts(m.pn);
			for (int j = 0; j < m.pn; j++) {
				if (m.p[j].x < c.clip_near) {
					continue;
				}
				
				p_pts[j].x =  (m.p[j].x - c.clip_near) / (c.clip_far - c.clip_near) * 2 - 1;
				p_pts[j].y =  m.p[j].y / (m.p[j].x * slope_x);
				p_pts[j].z = -m.p[j].z / (m.p[j].x * slope_y);
			}
			
			// Iterate over and render each edge.
			for (int j = 0; j < m.en; j++) {
				a = p_pts[m.e[j].x];
				b = p_pts[m.e[j].y];
				
				A = m.p[m.e[j].x];
				B = m.p[m.e[j].y];
				
				// printf("%d: A(%.2f, %.2f, %.2f) B(%.2f, %.2f, %.2f) ", j, A.x, A.y, A.z, B.x, B.y, B.z);
				
				// If both points are behind the near clipping plane or ahead of the far clipping plane, skip this edge.
				if (A.x < c.clip_near && B.x < c.clip_near) {continue;}
				if (A.x > c.clip_far  && B.x > c.clip_far)  {continue;}
				
				// Near-plane clipping. This must occur prior to projection to the cube, as projection prior to near-plane
				// clipping causes errors. Thus, A/B are used instead of a/b
				if (A.x < c.clip_near) {
					dif = B - A;
					// printf("dif(%.2f, %.2f, %.2f) ", dif.x, dif.y, dif.z);
					dif = dif / dif.x * (c.clip_near - A.x) + A;
					// printf("ac(%.2f, %.2f, %.2f) ", dif.x, dif.y, dif.z);
					a.x =  (dif.x - c.clip_near) / (c.clip_far - c.clip_near) * 2 - 1;
					a.y =  dif.y / (dif.x * slope_x);
					a.z = -dif.z / (dif.x * slope_y);
					// printf("ap(%.2f, %.2f, %.2f) bp(%.2f, %.2f, %.2f) ", a.x, a.y, a.z, b.x, b.y, b.z);
				}
				else if (B.x < c.clip_near) {
					dif = A - B;
					// printf("%d: A(%.2f, %.2f, %.2f) B(%.2f, %.2f, %.2f) dif(%.2f, %.2f, %.2f) ", j, A.x, A.y, A.z, B.x, B.y, B.z, dif.x, dif.y, dif.z);
					dif = dif / dif.x * (c.clip_near - B.x) + B;
					// printf("bc(%.2f, %.2f, %.2f) ", dif.x, dif.y, dif.z);
					b.x =  (dif.x - c.clip_near) / (c.clip_far - c.clip_near) * 2 - 1;
					b.y =  dif.y / (dif.x * slope_x);
					b.z = -dif.z / (dif.x * slope_y);
					// printf("bp(%.2f, %.2f, %.2f) ", b.x, b.y, b.z);
				}
				
				// If both points are too far off one side, skip them. Clipping off +/-x is handled above
				if (a.y > 1 && b.y > 1) {
					// printf("clipped off +y\n");
					continue;
				}
				if (a.y < -1 && b.y < -1) {
					// printf("clipped off -y\n");
					continue;
				}
				if (a.z > 1 && b.z > 1) {
					// printf("clipped off +z\n");
					continue;
				}
				if (a.z < -1 && b.z < -1) {
					// printf("clipped off -z\n");
					continue;
				}
				
				// If both points are in view, display this edge as expected.
				if (a.x >= -1 && a.x <= 1 && a.y >= -1 && a.y <= 1 && a.z >= -1 && a.z <= 1 &&
				    b.x >= -1 && b.x <= 1 && b.y >= -1 && b.y <= 1 && b.z >= -1 && b.z <= 1) {
					s.x = (int) ((a.y+1) / 2.0 * this->win_w);
					s.y = (int) ((a.z+1) / 2.0 * this->win_h);
					e.x = (int) ((b.y+1) / 2.0 * this->win_w);
					e.y = (int) ((b.z+1) / 2.0 * this->win_h);
					this->draw_line(s.x, s.y, e.x, e.y);
					// printf("in bounds\n");
					continue;
				}
				// printf("\n");
				
				// Clip the edge against the planes.
				dif = b - a;
				// printf("%d: (%.2f, %.2f, %.2f), (%.2f, %.2f, %.2f)\n", j, a.x, a.y, a.z, b.x, b.y, b.z);
				if (dif.y != 0) {
					// Clip against +y
					t = 1 / dif.y * (1 - a.y);
					// printf("  +yt: %.2f\n", t);
					if (t > 0 && t < 1) {
						if (a.y > 1) {
							a = vecd3(a.x + t * dif.x, a.y + t * dif.y, a.z + t * dif.z);
							// printf("    a -> (%.2f, %.2f, %.2f)\n", a.x, a.y, a.z);
						} else {
							b = vecd3(a.x + t * dif.x, a.y + t * dif.y, a.z + t * dif.z);
							// printf("    b -> (%.2f, %.2f, %.2f)\n", b.x, b.y, b.z);
						}
						dif = b-a;
					}
					// Clip against -y
					t = 1 / dif.y * (-1 - a.y);
					// printf("  -yt: %.2f\n", t);
					if (t > 0 && t < 1) {
						if (a.y < -1) {
							a = vecd3(a.x + t * dif.x, a.y + t * dif.y, a.z + t * dif.z);
							// printf("    a -> (%.2f, %.2f, %.2f)\n", a.x, a.y, a.z);
						} else {
							b = vecd3(a.x + t * dif.x, a.y + t * dif.y, a.z + t * dif.z);
							// printf("    b -> (%.2f, %.2f, %.2f)\n", b.x, b.y, b.z);
						}
						dif = b-a;
					}
				}
				if (dif.z != 0) {
					// Clip against +z
					t = 1 / dif.z * (1 - a.z);
					// printf("  +zt: %.2f\n", t);
					if (t > 0 && t < 1) {
						if (a.z > 1) {
							a = vecd3(a.x + t * dif.x, a.y + t * dif.y, a.z + t * dif.z);
							// printf("    a -> (%.2f, %.2f, %.2f)\n", a.x, a.y, a.z);
						} else {
							b = vecd3(a.x + t * dif.x, a.y + t * dif.y, a.z + t * dif.z);
							// printf("    b -> (%.2f, %.2f, %.2f)\n", b.x, b.y, b.z);
						}
						dif = b-a;
					}
					// Clip against -z
					t = 1 / dif.z * (-1 - a.z);
					// printf("  -zt: %.2f\n", t);
					if (t > 0 && t < 1) {
						if (a.z < -1) {
							a = vecd3(a.x + t * dif.x, a.y + t * dif.y, a.z + t * dif.z);
							// printf("    a -> (%.2f, %.2f, %.2f)\n", a.x, a.y, a.z);
						} else {
							b = vecd3(a.x + t * dif.x, a.y + t * dif.y, a.z + t * dif.z);
							// printf("    b -> (%.2f, %.2f, %.2f)\n", b.x, b.y, b.z);
						}
					}
				}
				
				s.x = (int) ((a.y+1) / 2.0 * this->win_w);
				s.y = (int) ((a.z+1) / 2.0 * this->win_h);
				e.x = (int) ((b.y+1) / 2.0 * this->win_w);
				e.y = (int) ((b.z+1) / 2.0 * this->win_h);
				
				this->draw_line(s.x, s.y, e.x, e.y);
			}
		}
	}
}
































