#ifndef RADWM_CLIENT_H
#define RADWM_CLIENT_H

#include "geometry.h"

struct Client {
	struct Rect curr_geom;
	struct Rect old_geom;
};

/**
 * @brief Constructs a default client (x:0, y:0, w:1, h:1)
 * */
struct Client client_default(void);

#endif
