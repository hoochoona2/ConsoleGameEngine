#include "list.h"

void _list_init(struct list *head)
{
	head->n = head->p = head;
}

/*
 * Insert an element before 'head'.
 * If 'head' is the list head, this adds an element to the end of the list.
 */
void _list_add(struct list *head, struct list *elem)
{
	assert(head->n);

	elem->n = head;
	elem->p = head->p;

	head->p->n = elem;
	head->p = elem;
}

/*
 * Insert an element after 'head'.
 * If 'head' is the list head, this adds an element to the front of the list.
 */
void _list_add_h(struct list *head, struct list *elem)
{
	assert(head->n);

	elem->n = head->n;
	elem->p = head;

	head->n->p = elem;
	head->n = elem;
}

/*
 * Delete an element from its list.
 * Note that this doesn't change the element itself - it may still be safe
 * to follow its pointers.
 */
void _list_del(struct list *elem)
{
	elem->n->p = elem->p;
	elem->p->n = elem->n;
}

/*
 * Remove an element from existing list and insert before 'head'.
 */
void _list_move(struct list *head, struct list *elem)
{
        _list_del(elem);
        _list_add(head, elem);
}

/*
 * Remove an element from existing list and insert after 'head'.
 */
void _list_move_h(struct list *head, struct list *elem)
{
        _list_del(elem);
        _list_add_h(head, elem);
}

/*
 * Is the list empty?
 */
int _list_empty(const struct list *head)
{
	return head->n == head;
}

/*
 * Is this the first element of the list?
 */
int _list_start(const struct list *head, const struct list *elem)
{
	return elem->p == head;
}

/*
 * Is this the last element of the list?
 */
int _list_end(const struct list *head, const struct list *elem)
{
	return elem->n == head;
}

/*
 * Return first element of the list or NULL if empty
 */
struct list *_list_first(const struct list *head)
{
	return (_list_empty(head) ? NULL : head->n);
}

/*
 * Return last element of the list or NULL if empty
 */
struct list *_list_last(const struct list *head)
{
	return (_list_empty(head) ? NULL : head->p);
}

/*
 * Return the previous element of the list, or NULL if we've reached the start.
 */
struct list *_list_prev(const struct list *head, const struct list *elem)
{
	return (_list_start(head, elem) ? NULL : elem->p);
}

/*
 * Return the next element of the list, or NULL if we've reached the end.
 */
struct list *_list_next(const struct list *head, const struct list *elem)
{
	return (_list_end(head, elem) ? NULL : elem->n);
}

/*
 * Return the number of elements in a list by walking it.
 */
unsigned int _list_size(const struct list *head)
{
	unsigned int s = 0;
	const struct list *v;

	list_iterate(v, head)
	    s++;

	return s;
}

/*
 * Join two lists together.
 * This moves all the elements of the list 'head1' to the end of the list
 * 'head', leaving 'head1' empty.
 */
void _list_splice(struct list *head, struct list *head1)
{
	assert(head->n);
	assert(head1->n);
	
	if (_list_empty(head1))
	    return;

	head1->p->n = head;
	head1->n->p = head->p;

	head->p->n = head1->n;
	head->p = head1->p;

	_list_init(head1);
}
