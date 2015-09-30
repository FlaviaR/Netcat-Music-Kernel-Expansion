#include "../netcat.h"
#define NETCAT_CPI_TRK3_LEN 4327635
struct netcat_track netcat_cpi_trk3 = {
	.name = "DW_Amys_Theme",
	.len  = NETCAT_CPI_TRK3_LEN,
	.data = {
#include "DW_Amys_Theme.h"
	}
};
