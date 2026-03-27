#include "client.h"

struct Client client_default(void)
{
	return (struct Client){ .curr_geom = rect_default(),
				.old_geom = rect_default() };
}
