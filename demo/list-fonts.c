#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

int main() {
	int p;
		
	Display* d = XOpenDisplay(NULL);
	char** fonts;
	
	fonts = XListFonts(d, (char*) "*--0-0-0-0-*", 256, &p);
	
	printf("%d\n", p);
	for (int i = 0; i < p; i++) {
		printf("%s\n", fonts[i]);
	}
	
}
