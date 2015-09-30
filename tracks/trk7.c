#include "../netcat.h"
#define NETCAT_CPI_TRK7_LEN 5256175
struct netcat_track netcat_cpi_trk7 = {
	.name = "Isnt_It_Wonderfule_FFX",
	.len  = NETCAT_CPI_TRK7_LEN,
	.data = {
#include "Isnt_It_Wonderful_FFX.h"
	}
};
