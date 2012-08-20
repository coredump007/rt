#include <stdlib.h>
#include <string.h>

#include "rt.h"

void show_link(struct rt_link *link, int b_show_inherit)
{
	struct rt_node *node;
	struct list_head *pos;

	fprintf(stderr, "link: ");

	list_for_each(pos, &link->node_head) {
		node = container_of(pos, struct rt_node, list);
		
		if (b_show_inherit) {
			fprintf(stderr, "[ %s ] %s [ %s ] -> ",
					node->parent ? node->parent->name : NULL,
					node->name, node->child ? node->child->name : NULL);
		}else{
			fprintf(stderr, "%s -> ", node->name);
		}
	}

	fprintf(stderr, "END\n");

	return;
}

void link_group_show(struct rt_link_group *link_group)
{
	struct rt_link *link;
	struct list_head *pos;
	
	D("Show group:");

	list_for_each(pos, &link_group->link_head) {
		link = container_of(pos, struct rt_link, list);
		show_link(link, 0);
	}

	return;
}

struct rt_node *build_node(struct rt_node *temp)
{
	struct rt_node *node;

	D("name: %s.", temp->name);

	node = (struct rt_node *)calloc(1, sizeof(struct rt_node));
	if (!node) {
		return NULL;
	}

	memcpy(node, temp, sizeof(*node));

	INIT_LIST_HEAD(&node->list);

	return node;
}

void free_node(struct rt_node *node)
{
	free(node);

	return;
}

void link_group_init(struct rt_link_group *link_group)
{
	INIT_LIST_HEAD(&link_group->link_head);

	return;
}

struct rt_link *link_group_new_link(struct rt_link_group *link_group)
{
	struct rt_link *link;

	link = (struct rt_link *)calloc(1, sizeof(*link));
	if (!link) {
		return NULL;
	}

	INIT_LIST_HEAD(&link->list);
	INIT_LIST_HEAD(&link->node_head);

	list_add_tail(&link->list, &link_group->link_head);

	return link;
}

void link_group_move_links(struct rt_link_group *from, struct rt_link_group *to)
{
	struct list_head *pos, *n;

	list_for_each_safe(pos, n, &from->link_head) {
		list_del(pos);
		list_add_tail(pos, &to->link_head);
	}

	return;
}

int link_group_build_node_to_each(struct rt_node *temp, struct rt_link_group *lg)
{
	struct rt_node *parent;
	struct rt_link *link;
	struct rt_node *n;
	struct list_head *pos;

	list_for_each(pos, &lg->link_head) {
		link = container_of(pos, struct rt_link, list);

		n = build_node(temp);
		if (!n) {
			return -1;
		}

		n->parent = NULL;
		n->child = NULL;			

		if (!list_empty(&link->node_head)) {
			parent = container_of(link->node_head.prev, 
				struct rt_node, list);

			if (parent->child) {
				E("parent node have already have a child ?");
			}

			parent->child = n;
			n->parent = parent;
		}

		list_add_tail(&n->list, &link->node_head);
	}

	return 0;
}

int link_group_build(struct rt_node *temp, struct rt_link_group *link_group)
{
	struct rt_class *class;
	struct rt_node *node;
	struct rt_link *link;

	struct rt_link_group group;

	int i, j;

	link_group_init(&group);

	if (temp->dep_class[0]) {
		for (i = 0; i < N_MAX_DEP_CLASS; i++) {
			class = temp->dep_class[i];
			if (!class)
				break;

			for (j = 0; j < N_MAX_CLASS_NODE; j++) {
				node = class->node[j];
				if (!node)
					break;

				link_group_build(node, &group);

//				D("collect group:");
//				link_group_show(&group);

				link_group_build_node_to_each(temp, &group);

//				D("after build node group:");
//				link_group_show(&group);

				link_group_move_links(&group, link_group);

//				D("group without links:");
//				link_group_show(&group);

//				D("final link_group:");
//				link_group_show(link_group);
			}
		}
	}else{
		D("Single node with single link.");

		link_group_new_link(link_group);
		link_group_build_node_to_each(temp, link_group);
		
//		D("final link_group.");

//		link_group_show(link_group);
	}

	return 0;
}
