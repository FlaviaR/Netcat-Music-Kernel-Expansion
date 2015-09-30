#include "../netcat.h"
#define NETCAT_CPI_TRK1_LEN 2822653
struct netcat_track netcat_cpi_trk1 = {
	.name = "Chocobos_Chasing_Dreams_FFXIII",
	.len  = NETCAT_CPI_TRK1_LEN,
	.data = {
#include "Chocobos_Chasing_Dreams_FFXIII.h"
	}
};
