#include <stdio.h>
#include <stdlib.h>

#include <X11/Xlib.h>

#include "client.h"

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	Display *dpy = XOpenDisplay(NULL);
	if (dpy == NULL) {
		printf("Failed to a connection to the display server\n");
		return EXIT_FAILURE;
	}

	printf("Open connection to the X server\n");

	struct Client c = client_default();

	printf("Client Point: (x: %d, y: %d)\n", c.curr_geom.pt.x,
	       c.curr_geom.pt.y);

	printf("Client Dimensions: (w: %d, h: %u)\n", c.curr_geom.dim.w,
	       c.curr_geom.dim.h);

	XCloseDisplay(dpy);
	return EXIT_SUCCESS;
}
