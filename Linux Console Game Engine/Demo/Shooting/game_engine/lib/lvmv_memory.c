#include "lvmv_memory.h"
#include <game_engine/data_struct/list.h>

void* __lvmv_malloc(size_t size)
{
    return malloc(size);
}

void* __lvmv_realloc(void* memory, size_t size)
{
    return realloc(memory, size);
}

void __lvmv_free(void* memory)
{
    free(memory);
}

struct lvmv_memory_node* _lvmv_create_node()
{
    struct lvmv_memory_node* ret = NULL;

    ret = (struct lvmv_memory_node*)__lvmv_malloc(sizeof(struct  lvmv_memory_node));

    if(ret)
    {
        ret->filename = ret->memory = NULL;
        ret->line = -1;
        ret->size = 0;
    }

    return ret;
}

int _lvmv_memory_append(struct lvmv_memory* lvmv_memory_info, struct lvmv_memory_node* node)
{
    struct lvmv_memory* head = lvmv_memory_info;

    if(!head)
        return 0;

	list_add_h(&lvmv_memory_info->memorys, node);

    head->allocate_count++;
    head->allocate_size += node->size;

    return 1;
}

struct lvmv_memory_node* _lvmv_find_node(char* filename, int line, struct lvmv_memory* lvmv_memory_info, void* memory)
{
    struct lvmv_memory_node* v = NULL;

    if(!lvmv_memory_info)
        return NULL;

    if(!memory)
        return NULL;

	list_iterate_t(struct lvmv_memory_node, v, &lvmv_memory_info->memorys)
	{
		if(v->memory == memory)
		{
			return v;
		}
	}

    return NULL;
}

void _lvmv_memory_init(char* filename, int line, struct lvmv_memory* lvmv_memory_info)
{
	lvmv_memory_info->allocate_count = 0;
	lvmv_memory_info->allocate_size = 0;
	lvmv_memory_info->memorys.size = 0;
	lvmv_memory_info->memorys.line = -1;
	lvmv_memory_info->memorys.filename = NULL;
	lvmv_memory_info->memorys.memory = NULL;
	lvmv_memory_info->memorys.n = NULL;
	lvmv_memory_info->memorys.p = NULL;

	list_init(&lvmv_memory_info->memorys);
}

/*
struct lvmv_memory* _lvmv_memory_create(char* filename, int line)
{
    struct lvmv_memory* ret = NULL;

    ret = (struct lvmv_memory*)__lvmv_malloc(sizeof(struct lvmv_memory));

    if(ret)
    {
        ret->allocate_size = ret->allocate_count = 0;

		ret->memorys = (struct lvmv_memory_node*)__lvmv_malloc(sizeof(struct lvmv_memory));
		if(!ret->memorys)
		{
			__lvmv_free(ret);
			lvmv_memory_error_print("lvmv memory node didn't create");
			goto bad;
		}
		list_init(ret->memorys);
    }
    else
    {
bad:
        lvmv_memory_error_print("lvmv memory didn't create");
    }

    return ret;
}
*/

void* _lvmv_malloc_final(char* filename, int line, struct lvmv_memory* lvmv_memory_info, size_t size)
{
    struct lvmv_memory_node* node = NULL;
    void* memory = NULL;

    if(!lvmv_memory_info)
    {
        return NULL;
    }

    node = _lvmv_create_node();

    if(!node)
    {
        return NULL;
    }

    node->memory = __lvmv_malloc(size);
    node->size = size;
    node->filename = filename;
    node->line = line;

    memory = node->memory;
    memset(memory, 0x00, size);

    if(!_lvmv_memory_append(lvmv_memory_info, node))
    {
        __lvmv_free(node->memory);
        __lvmv_free(node);

        lvmv_memory_error_print("lvmv memory node didn't append");

        return NULL;
    }

    return node->memory;
}

void* _lvmv_malloc(char* filename, int line, struct lvmv_memory* lvmv_memory_info, size_t size)
{
    if(!lvmv_memory_info)
    {
        lvmv_memory_error_print("lvmv memory is null");

        return NULL;
    }

    if(!size)
    {
        lvmv_memory_error_print("lvmv memory allocate size is 0 byte");

        return NULL;
    }

    return _lvmv_malloc_final(filename, line, lvmv_memory_info, size);
}

void* _lvmv_realloc_final(char* filename, int line, struct lvmv_memory* lvmv_memory_info, void* memory, size_t size)
{
    struct lvmv_memory_node* node = NULL;
    void* ret = NULL;

    if(!lvmv_memory_info)
        return NULL;

    if(!size)
        return NULL;

    node = _lvmv_find_node(filename, line, lvmv_memory_info, memory);

    if(!node)
    {
        lvmv_memory_error_print("lvmv memory node not found");

        return NULL;
    }

    ret = __lvmv_realloc(memory, size);

    if(NULL == ret)
    {
        lvmv_memory_error_print("lvmv memory reallocate fail");

        return NULL;
    }

    node->memory = memory;
    lvmv_memory_info->allocate_size -= node->size;
    node->size = size;
    lvmv_memory_info->allocate_size += node->size;

    return memory;
}

void* _lvmv_realloc(char* filename, int line, struct lvmv_memory* lvmv_memory_info, void* memory, size_t size)
{
    if(!lvmv_memory_info)
        return NULL;

    if(!size)
        return NULL;

    return _lvmv_realloc_final(filename, line, lvmv_memory_info, memory, size);
}

void _lvmv_free_final(char* filename, int line, struct lvmv_memory* lvmv_memory_info, struct lvmv_memory_node* node)
{
    struct lvmv_memory_node* v = NULL;
    struct lvmv_memory_node* prev = NULL;

    if(!lvmv_memory_info)
        return;

    if(!node)
        return;

    lvmv_memory_info->allocate_size -= node->size;
    lvmv_memory_info->allocate_count--;

	list_del(node);
    __lvmv_free(node->memory);
    __lvmv_free(node);
}

void _lvmv_free(char* filename, int line, struct lvmv_memory* lvmv_memory_info, void* memory)
{
    struct lvmv_memory_node *node, *prev_target, *target, *temp;

    node = prev_target = target = temp = NULL;

    if(!lvmv_memory_info)
    {
        lvmv_memory_error_print("lvmv memory is null");

        return;
    }

    node = _lvmv_find_node(filename, line, lvmv_memory_info, memory);
   
    if(!node)
    {
        lvmv_memory_error_print("lvmv memory node not found");

        return;
    }

    _lvmv_free_final(filename, line, lvmv_memory_info, node);
}

int _lvmv_memory_destroy(char* filename, int line, struct lvmv_memory* lvmv_memory_info)
{
    struct lvmv_memory_node* v = NULL, *t = NULL;

    if(!lvmv_memory_info)
    {
        lvmv_memory_error_print("lvmv memory is null");

        return 0;
    }

	list_iterate_safe_t(struct lvmv_memory_node, v, t, &lvmv_memory_info->memorys)
	{
        lvmv_memory_info->allocate_count--;
        lvmv_memory_info->allocate_size -= v->size;
		
		list_del(v);

        __lvmv_free(v->memory);
        __lvmv_free(v);
	}

	list_init(&lvmv_memory_info->memorys);

    return 1;
}

void _lvmv_memory_print(char* filename, int line, struct lvmv_memory* lvmv_memory_info)
{
    struct lvmv_memory_node* v = NULL;

    if(!lvmv_memory_info)
    {
        lvmv_memory_error_print("lvmv memory is null");

        return;
    }

    printf( "---------------------\n"
            "allocate size : %d\n"
            "allocate count : %d\n"
            "---------------------\n",
            lvmv_memory_info->allocate_size,
            lvmv_memory_info->allocate_count);

	list_iterate_t(struct lvmv_memory_node, v, &lvmv_memory_info->memorys)
	{
        printf("%p %dbyte %s (%d)\n",
                v->memory,
                v->size,
                v->filename,
                v->line);
	}
}

char* _lvmv_memory2string(char* filename, int line, struct lvmv_memory* lvmv_memory_info)
{
    struct lvmv_memory_node* v = NULL;
    char* string = NULL;
    char* string_current = NULL;
    int string_len = 0;

    if(!lvmv_memory_info)
    {
        lvmv_memory_error_print("lvmv memory is null");

        return NULL;
    }

    string_len = (lvmv_memory_info->allocate_count+1)*(strlen(__FILE__) + 25) + 1;
    string = (char*)_lvmv_malloc(filename, line, lvmv_memory_info, string_len);
    if(!string)
    {
        lvmv_memory_error_print("lvmv memory can not allocate memory at tostring");

        return NULL;
    }

    string_current = string;

    string_current += sprintf(string_current, "---------------------\n"
            "allocate size : %d\n"
            "allocate count : %d\n"
            "---------------------\n",
            lvmv_memory_info->allocate_size,
            lvmv_memory_info->allocate_count);

	list_iterate_t(struct lvmv_memory_node, v, &lvmv_memory_info->memorys)
	{
        string_current += sprintf(string_current, "%p %dbyte %s (%d)\n",
                v->memory,
                v->size,
                v->filename,
                v->line);
	}

    return string;
}


