#include "../netcat.h"
#define NETCAT_CPI_TRK8_LEN 2056854
struct netcat_track netcat_cpi_trk8 = {
	.name = "Tetris",
	.len  = NETCAT_CPI_TRK8_LEN,
	.data = {
#include "Tetris.h"
	}
};
