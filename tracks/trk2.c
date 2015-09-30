#include "../netcat.h"
#define NETCAT_CPI_TRK2_LEN 2648127
struct netcat_track netcat_cpi_trk2 = {
	.name = "Wandering_Flame_Guitar_FFX",
	.len  = NETCAT_CPI_TRK2_LEN,
	.data = {
#include "Wandering_Flame_Guitar_FFX.h"
	}
};
