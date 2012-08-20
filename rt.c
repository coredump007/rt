#include "rt.h"

#include "rt_link_group.c"

int run_temp(struct rt_node *temp)
{
	struct rt_link_group group;
	struct rt_link *link;
	struct list_head *pos;

	int r;

	I("Run template: %s.", temp->name);

	link_group_init(&group);

	r = link_group_build(temp, &group);
	if (r == -1) {
		E("Fail to build link group.");
		return -1;
	}
	
	I("Final links in group:");

	link_group_show(&group);

	list_for_each(pos, &group.link_head) {
		link = container_of(pos, struct rt_link, list);

		/*
		setup_link(link);
		test_link(link);
		clean_link(link);
		*/
	}

	return 0;
}

