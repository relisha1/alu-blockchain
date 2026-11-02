#include <stdlib.h>
#include "llist.h"

/**
 * struct lnode_s - list node
 * @data: user data
 * @next: next node
 */
typedef struct lnode_s
{
	llist_node_t data;
	struct lnode_s *next;
} lnode_t;

/**
 * struct llist_s - linked list
 * @head: first node
 * @tail: last node
 * @size: number of nodes
 * @flags: creation flags (thread support is accepted but not needed here)
 */
struct llist_s
{
	lnode_t *head;
	lnode_t *tail;
	int size;
	unsigned int flags;
};

/**
 * llist_create - create an empty list
 * @flags: MT_SUPPORT_TRUE or MT_SUPPORT_FALSE
 * Return: new list or NULL
 */
llist_t *llist_create(unsigned int flags)
{
	llist_t *list = calloc(1, sizeof(*list));

	if (list)
		list->flags = flags;
	return (list);
}

/**
 * llist_destroy - free a list
 * @list: list
 * @destroy_nodes: non-zero to also free node data
 * @destructor: function used to free node data (free() if NULL)
 * Return: 0 on success, -1 if list is NULL
 */
int llist_destroy(llist_t *list, int destroy_nodes, node_dtor_t destructor)
{
	lnode_t *node, *next;

	if (!list)
		return (-1);
	for (node = list->head; node; node = next)
	{
		next = node->next;
		if (destroy_nodes)
		{
			if (destructor)
				destructor(node->data);
			else
				free(node->data);
		}
		free(node);
	}
	free(list);
	return (0);
}

/**
 * llist_add_node - add a node at the front or rear
 * @list: list
 * @node: data
 * @where: ADD_NODE_FRONT or ADD_NODE_REAR
 * Return: 0 on success, -1 on failure
 */
int llist_add_node(llist_t *list, llist_node_t node, int where)
{
	lnode_t *n;

	if (!list)
		return (-1);
	n = calloc(1, sizeof(*n));
	if (!n)
		return (-1);
	n->data = node;
	if (!list->head)
		list->head = list->tail = n;
	else if (where == ADD_NODE_FRONT)
		n->next = list->head, list->head = n;
	else
		list->tail->next = n, list->tail = n;
	list->size++;
	return (0);
}

/**
 * llist_insert_node - insert a node in sorted position
 * @list: list
 * @node: data
 * @compar: comparison function
 * @arg: extra argument for compar
 * @where: ADD_NODE_FRONT or ADD_NODE_REAR for ties
 * Return: 0 on success, -1 on failure
 */
int llist_insert_node(llist_t *list, llist_node_t node, node_cmp_t compar,
		      void *arg, int where)
{
	lnode_t *n, *prev = NULL, *cur;
	int c;

	if (!list || !compar)
		return (-1);
	for (cur = list->head; cur; prev = cur, cur = cur->next)
	{
		c = compar(node, cur->data, arg);
		if (c < 0 || (c == 0 && where == ADD_NODE_FRONT))
			break;
	}
	n = calloc(1, sizeof(*n));
	if (!n)
		return (-1);
	n->data = node;
	n->next = cur;
	if (prev)
		prev->next = n;
	else
		list->head = n;
	if (!cur)
		list->tail = n;
	list->size++;
	return (0);
}

/**
 * llist_find_node - find the first node matching identifier
 * @list: list
 * @identifier: returns non-zero on match
 * @arg: extra argument for identifier
 * Return: matching data or NULL
 */
llist_node_t llist_find_node(llist_t *list, node_ident_t identifier,
			     void *arg)
{
	lnode_t *n;

	if (!list || !identifier)
		return (NULL);
	for (n = list->head; n; n = n->next)
		if (identifier(n->data, arg))
			return (n->data);
	return (NULL);
}

/**
 * llist_remove_node - remove the first node matching identifier
 * @list: list
 * @identifier: returns non-zero on match
 * @arg: extra argument for identifier
 * @destroy_node: non-zero to free node data
 * @destructor: function used to free node data (free() if NULL)
 * Return: 0 if a node was removed, -1 otherwise
 */
int llist_remove_node(llist_t *list, node_ident_t identifier, void *arg,
		      int destroy_node, node_dtor_t destructor)
{
	lnode_t *n, *prev = NULL;

	if (!list || !identifier)
		return (-1);
	for (n = list->head; n; prev = n, n = n->next)
	{
		if (!identifier(n->data, arg))
			continue;
		if (prev)
			prev->next = n->next;
		else
			list->head = n->next;
		if (list->tail == n)
			list->tail = prev;
		if (destroy_node)
		{
			if (destructor)
				destructor(n->data);
			else
				free(n->data);
		}
		free(n);
		list->size--;
		return (0);
	}
	return (-1);
}

/**
 * llist_for_each - call action on every node, stopping on non-zero return
 * @list: list
 * @action: function to call
 * @arg: extra argument for action
 * Return: 0 if every call returned 0, the non-zero value otherwise,
 *         -1 on bad input
 */
int llist_for_each(llist_t *list, node_func_t action, void *arg)
{
	lnode_t *n, *next;
	unsigned int i = 0;
	int ret;

	if (!list || !action)
		return (-1);
	for (n = list->head; n; n = next, i++)
	{
		next = n->next;
		ret = action(n->data, i, arg);
		if (ret)
			return (ret);
	}
	return (0);
}

/**
 * llist_get_head - first node's data
 * @list: list
 * Return: data or NULL
 */
llist_node_t llist_get_head(llist_t *list)
{
	return (list && list->head ? list->head->data : NULL);
}

/**
 * llist_get_tail - last node's data
 * @list: list
 * Return: data or NULL
 */
llist_node_t llist_get_tail(llist_t *list)
{
	return (list && list->tail ? list->tail->data : NULL);
}

/**
 * llist_get_node_at - data at index
 * @list: list
 * @index: position
 * Return: data or NULL
 */
llist_node_t llist_get_node_at(llist_t *list, unsigned int index)
{
	lnode_t *n;

	if (!list)
		return (NULL);
	for (n = list->head; n && index; n = n->next)
		index--;
	return (n ? n->data : NULL);
}

/**
 * llist_pop - remove the first node and return its data
 * @list: list
 * Return: data or NULL
 */
llist_node_t llist_pop(llist_t *list)
{
	lnode_t *n;
	llist_node_t data;

	if (!list || !list->head)
		return (NULL);
	n = list->head;
	data = n->data;
	list->head = n->next;
	if (!list->head)
		list->tail = NULL;
	free(n);
	list->size--;
	return (data);
}

/**
 * llist_size - number of nodes
 * @list: list
 * Return: size, or -1 if list is NULL
 */
int llist_size(llist_t const *list)
{
	return (list ? list->size : -1);
}

/**
 * llist_is_empty - check if list is empty
 * @list: list
 * Return: 1 if empty or NULL, 0 otherwise
 */
int llist_is_empty(llist_t const *list)
{
	return (!list || !list->size);
}
