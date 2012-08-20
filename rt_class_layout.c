#include "layout/layout0.c"
#include "layout/layout1.c"
#include "layout/layout2.c"

struct rt_class layout_class = {
	.name = "layout class",
	.node = {
		&layout0_temp,
		&layout1_temp,
		&layout2_temp,
	},
};
