#include "../netcat.h"
#define NETCAT_CPI_TRK9_LEN 2543927
struct netcat_track netcat_cpi_trk9 = {
	.name = "Portal Still Alive",
	.len  = NETCAT_CPI_TRK9_LEN,
	.data = {
#include "Portal_Still_Alive.h"
	}
};
