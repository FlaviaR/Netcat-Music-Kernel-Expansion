#include "../netcat.h"
#define NETCAT_CPI_TRK6_LEN 3475236
struct netcat_track netcat_cpi_trk6 = {
	.name = "Zelda_Main_Theme",
	.len  = NETCAT_CPI_TRK6_LEN,
	.data = {
#include "Zelda_Main_Theme.h"
	}
};
