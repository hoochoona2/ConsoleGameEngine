#ifndef _LIST_H_
#define _LIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#ifdef __GNUC__

#define _list_iterate_at(v, head, start) \
	for (v = (typeof(v))(start)->n; v != (typeof(v))head; v = v->n)
#define _list_iterate(v, head) \
	_list_iterate_at(v, head, head)

#define _list_uniterate_at(v, head, start) \
    for (v = (typeof(v))(start)->p; v != (typeof(v))head; v = v->p)
#define _list_uniterate(v, head) \
	_list_uniterate_at(v, head, head)

#define _list_iterate_safe_at(v, t, head, start) \
    for (v = (typeof(v))(start)->n, t = (typeof(v))v->n; v != (typeof(v))head; v = t, t = v->n)
#define _list_iterate_safe(v, t, head) \
	_list_iterate_safe_at(v, t, head, head)

#define _list_uniterate_safe_at(v, t, head, start) \
    for (v = (typeof(v))(start)->p, t = (typeof(v))v->p; v != (typeof(v))head; v = t, t = v->p)
#define _list_uniterate_safe(v, t, head) \
	_list_uniterate_safe_at(v, t, head, head)

#define list_iterate_at_t(t, v, head, start)			_list_iterate_at(v, head, start)
#define list_iterate_at									_list_iterate_at
#define list_iterate_t(t, v, head)						_list_iterate(v, head)
#define list_iterate									_list_iterate

#define list_uniterate_at_t(t, v, head, start)			_list_uniterate_at(v, head, start)
#define list_uniterate_at								_list_uniterate_at
#define list_uniterate_t(t, v, head)					_list_uniterate(v, head)
#define list_uniterate									_list_uniterate

#define list_iterate_safe_at_t(type, v, t, head, start)		_list_iterate_safe_at(v, t, head, start)
#define list_iterate_safe_at							_list_iterate_safe_at
#define list_iterate_safe_t(type, v, t, head)			_list_iterate_safe(v, t, head)
#define list_iterate_safe								_list_iterate_safe

#define list_uniterate_safe_at_t(type, v, t, head, start)		_list_uniterate_safe_at(v, t, head, start)
#define list_uniterate_safe_at							_list_uniterate_safe_at
#define list_uniterate_safe_t(type, v, t, head)				_list_uniterate_safe(v, t, head)
#define list_uniterate_safe								_list_uniterate_safe

#else // __GNUC__

#define _list_iterate_at_t(t, v, head, start) \
	for (v = (t*)(start)->n; v != (t*)head; v = v->n)
#define _list_iterate_at(v, head, start) \
	_list_iterate_at_t(struct list, v, head, start)
#define _list_iterate_t(t, v, head) \
	_list_iterate_at_t(t, v, head, head)
#define _list_iterate(v, head) \
	_list_iterate_at_t(struct list, v, head, head)

#define _list_uniterate_at_t(t, v, head, start) \
    for (v = (t*)(start)->p; v != (t*)head; v = v->p)
#define _list_uniiterate_at(v, head, start) \
	_list_uniterate_at_t(struct list, v, head, start)
#define _list_uniterate_t(t, v, head) \
    _list_uniterate_at_t(t, v, head, head)
#define _list_uniterate(v, head) \
    _list_uniterate_at_t(struct list, v, head, head)

#define _list_iterate_safe_at_t(type, v, t, head, start) \
    for (v = (type*)(start)->n, t = (type*)v->n; v != (type*)head; v = t, t = v->n)
#define _list_iterate_safe_at(v, t, head, start) \
	_list_iterate_safe_at_t(struct list, v, t, head, start)
#define _list_iterate_safe_t(type, v, t, head) \
	_list_iterate_safe_at_t(type, v, t, head, head)
#define _list_iterate_safe(v, t, head) \
	_list_iterate_safe_at_t(struct list, v, t, head, head)

#define _list_uniterate_safe_at_t(type, v, t, head, start) \
    for (v = (type*)(start)->p, t = (type*)v->p; v != (type*)head; v = t, t = v->p)
#define _list_uniterate_safe_at(v, t, head, start) \
	_list_uniterate_safe_at_t(struct list, v, t, head, start)
#define _list_uniterate_safe_t(type, v, t, head) \
	_list_uniterate_safe_at_t(type, v, t, head, head)
#define _list_uniterate_safe(v, t, head) \
	_list_uniterate_safe_at_t(struct list, v, t, head, head)

#define list_iterate_at_t				_list_iterate_at_t
#define list_iterate_at					_list_iterate_at
#define list_iterate_t					_list_iterate_t
#define list_iterate					_list_iterate

#define list_uniterate_at_t				_list_uniterate_at_t
#define list_uniiterate_at				_list_uniiterate_at
#define list_uniterate_t				_list_uniterate_t
#define list_uniterate					_list_uniterate

#define list_iterate_safe_at_t			_list_iterate_safe_at_t
#define list_iterate_safe_at			_list_iterate_safe_at
#define list_iterate_safe_t				_list_iterate_safe_t
#define list_iterate_safe				_list_iterate_safe

#define list_uniterate_safe_at_t		_list_uniterate_safe_at_t
#define list_uniterate_safe_at			_list_uniterate_safe_at
#define list_uniterate_safe_t			_list_uniterate_safe_t
#define list_uniterate_safe				_list_uniterate_safe

#endif // __GNUC__

struct list
{
	struct list *n, *p;
};

void _list_init(struct list *head);
#define list_init(head) _list_init((struct list*)head)

/*
 * Insert an element before 'head'.
 * If 'head' is the list head, this adds an element to the end of the list.
 */
void _list_add(struct list *head, struct list *elem);
#define list_add(head, elem) _list_add((struct list*)head, (struct list*)elem)

/*
 * Insert an element after 'head'.
 * If 'head' is the list head, this adds an element to the front of the list.
 */
void _list_add_h(struct list *head, struct list *elem);
#define list_add_h(head, elem) _list_add_h((struct list*)head, (struct list*)elem)

/*
 * Delete an element from its list.
 * Note that this doesn't change the element itself - it may still be safe
 * to follow its pointers.
 */
void _list_del(struct list *elem);
#define list_del(elem) _list_del((struct list*)elem)

/*
 * Remove an element from existing list and insert before 'head'.
 */
void _list_move(struct list *head, struct list *elem);
#define list_move(head, elem) _list_move((struct list*)head, (struct list*)elem)

/*
 * Remove an element from existing list and insert after 'head'.
 */
void _list_move_h(struct list *head, struct list *elem);
#define list_move_h(head, elem) _list_move_h((struct list*)head, (struct list*)elem)

/*
 * Is the list empty?
 */
int _list_empty(const struct list *head);
#define list_empty(head) _list_empty((const struct list*)head)

/*
 * Is this the first element of the list?
 */
int _list_start(const struct list *head, const struct list *elem);
#define list_stat(head, elem) _list_start((const struct list*)head, (const struct list*) elem)

/*
 * Is this the last element of the list?
 */
int _list_end(const struct list *head, const struct list *elem);
#define list_end(head, elem) _list_end((const struct list*)head, (const struct list*) elem)

/*
 * Return first element of the list or NULL if empty
 */
struct list *_list_first(const struct list *head);
#define list_first(head) _list_first((const struct list*)head)

/*
 * Return last element of the list or NULL if empty
 */
struct list *_list_last(const struct list *head);
#define list_last(head) _list_last((const struct list*) head)

/*
 * Return the previous element of the list, or NULL if we've reached the start.
 */
struct list *_list_prev(const struct list *head, const struct list *elem);
#define list_prev(head, elem) _list_prev((const struct list*)head, (const struct list*)elem)

/*
 * Return the next element of the list, or NULL if we've reached the end.
 */
struct list *_list_next(const struct list *head, const struct list *elem);
#define list_next(head, elem) _list_next((const struct list*)head, (const struct list*)elem)

/*
 * Return the number of elements in a list by walking it.
 */
unsigned int _list_size(const struct list *head);
#define list_size(head) _list_size((const struct list*)head)

/*
 * Join two lists together.
 * This moves all the elements of the list 'head1' to the end of the list
 * 'head', leaving 'head1' empty.
 */
void _list_splice(struct list *head, struct list *head1);
#define list_splice(head, head1) _list_splice((struct list*)head, (struct list*)head1)

#endif // _LIST_H_
