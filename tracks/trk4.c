#include "../netcat.h"
#define NETCAT_CPI_TRK4_LEN 2589164
struct netcat_track netcat_cpi_trk4 = {
	.name = "Fabula_Nova_Crystallis_FFXIII",
	.len  = NETCAT_CPI_TRK4_LEN,
	.data = {
#include "Fabula_Nova_Crystallis_FFXIII.h"
	}
};
