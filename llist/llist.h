#ifndef _LLIST_H_
#define _LLIST_H_

/*
 * Minimal, API-compatible replacement for the libllist library that the
 * ALX/Holberton sandbox provides. It lets the project build on any machine.
 */

#include <stddef.h>

#define MT_SUPPORT_FALSE 0
#define MT_SUPPORT_TRUE 1

#define ADD_NODE_FRONT 0
#define ADD_NODE_REAR 1

typedef void *llist_node_t;
typedef struct llist_s llist_t;

typedef void (*node_dtor_t)(llist_node_t node);
typedef int (*node_ident_t)(llist_node_t node, void *arg);
typedef int (*node_func_t)(llist_node_t node, unsigned int idx, void *arg);
typedef int (*node_cmp_t)(llist_node_t first, llist_node_t second, void *arg);

llist_t *llist_create(unsigned int flags);
int llist_destroy(llist_t *list, int destroy_nodes, node_dtor_t destructor);
int llist_add_node(llist_t *list, llist_node_t node, int where);
int llist_insert_node(llist_t *list, llist_node_t node, node_cmp_t compar,
		      void *arg, int where);
llist_node_t llist_find_node(llist_t *list, node_ident_t identifier,
			     void *arg);
int llist_remove_node(llist_t *list, node_ident_t identifier, void *arg,
		      int destroy_node, node_dtor_t destructor);
int llist_for_each(llist_t *list, node_func_t action, void *arg);
llist_node_t llist_get_head(llist_t *list);
llist_node_t llist_get_tail(llist_t *list);
llist_node_t llist_get_node_at(llist_t *list, unsigned int index);
llist_node_t llist_pop(llist_t *list);
int llist_size(llist_t const *list);
int llist_is_empty(llist_t const *list);

#endif /* _LLIST_H_ */
