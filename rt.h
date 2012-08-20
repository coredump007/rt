#ifndef RT_H
#define RT_H

#include "list.h"

#include <stdio.h>

#ifdef DEBUG
#define D(fmt, args...) \
	fprintf(stderr, "%s() - %d: "fmt"\n", __func__, __LINE__, ##args)
#else
#define D(fmt, args...)
#endif

#define I(fmt, args...) \
	fprintf(stderr, "%s() - %d: "fmt"\n", __func__, __LINE__, ##args)

#define E(fmt, args...) \
	fprintf(stderr, "%s() - %d: "fmt"\n", __func__, __LINE__, ##args)

#define N_MAX_DEP_CLASS		64

#define N_MAX_SETUP_FUNC	64
#define N_MAX_TEST_FUNC		64
#define N_MAX_CLEAN_FUNC	64
#define N_MAX_CLASS_NODE	64

struct rt_node;
struct rt_class;

typedef int (setup_func_t)(struct rt_node *node);
typedef int (test_func_t)(struct rt_node *node);
typedef int (clean_func_t)(struct rt_node *node);

struct rt_class {
	const char *name;

	struct rt_node *node[N_MAX_CLASS_NODE];
};

struct rt_attr {
	int x;
	int y;
	int w;
	int h;
};

struct rt_node {
	const char *name;

	struct rt_class *dep_class[N_MAX_DEP_CLASS];
	struct rt_attr attr;
	
	void *obj;
	void *data;

	setup_func_t *setup[N_MAX_SETUP_FUNC];
	test_func_t *test[N_MAX_TEST_FUNC];
	clean_func_t *clean[N_MAX_CLEAN_FUNC];

	struct rt_node *parent;
	struct rt_node *child;

	struct list_head list;
};

struct rt_link {
	struct list_head node_head;
	struct list_head list;
};

struct rt_link_group {
	struct list_head link_head;
};

#endif
