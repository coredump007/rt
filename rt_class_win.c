#include "win/win0.c"
#include "win/win1.c"
#include "win/win2.c"

struct rt_class win_class = {
	.name = "win class",
	.node = {
		&win0_temp,
		&win1_temp,
		&win2_temp,
	},
};
