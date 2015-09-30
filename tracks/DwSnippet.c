#include "../netcat.h"
#define NETCAT_CPI_TRK9_LEN 3479329
struct netcat_track netcat_cpi_trk9 = {
	.name = "dwst",
	.len  = NETCAT_CPI_TRK9_LEN,
	.data = {
#include "dwst.h"
	}
};
