#ifndef util_util
#define util_util

int next(char* in, int s, char* buf) {
	bool char_found = false;
	int offset = 0;
	
	for (int i = s; true; i++) {
		if (in[i] == '\0') {
			buf[i-offset-s] = '\0';
			return i;
		}
		
		if (in[i] != ' ' && in[i] != '\t' && in[i] != '\n') {
			if (char_found) {
				buf[i-offset-s] = in[i];
			} else {
				char_found = true;
				buf[i-offset-s] = in[i];
			}
		} else {
			if (char_found) {
				buf[i-offset-s] = '\0';
				return i;
			} else {
				offset++;
			}
		}
	}
}

#endif
