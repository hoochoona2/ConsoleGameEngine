#ifndef _LVMV_MEMORY_H_
#define _LVMV_MEMORY_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
* struct lvmv_memory_node
* 한번의 메모리 할당에 대한 정보
**/
struct lvmv_memory_node
{
    struct lvmv_memory_node *n, *p;
    char* filename; //할당된 소스파일 이름
    int line; //할당된 줄 번호
    int size; //할당된 크기
    void* memory; //할당된 메모리
};

/**
* struct lvmv_memory
* 메모리 할당 정보 모음
**/
struct lvmv_memory
{
    int allocate_size; //이 구조체로 할당된 총 크기
    int allocate_count; //이 구조체로 할당된 총 횟수
    struct lvmv_memory_node memorys; //메모리 할당 정보 모음
};

//malloc, realloc, free를 직접 호출하는 함수들
void* __lvmv_malloc(size_t size);
void* __lvmv_realloc(void* memory, size_t size);
void __lvmv_free(void* memory);

//////////////
//직접 호출하지 않는 메모리 관련 함수들
struct lvmv_memory_node* _lvmv_create_node();
int _lvmv_memory_append(struct lvmv_memory* lvmv_memory_info, struct lvmv_memory_node* node);
struct lvmv_memory_node* _lvmv_find_node(char* filename, int line, struct lvmv_memory* lvmv_memory_info, void* memory);
struct lvmv_memory* _lvmv_memory_create(char* filename, int line);
void _lvmv_memory_init(char* filename, int line, struct lvmv_memory* lvmv_memory_info);
void* _lvmv_malloc_final(char* filename, int line, struct lvmv_memory* lvmv_memory_info, size_t size);
void* _lvmv_malloc(char* filename, int line, struct lvmv_memory* lvmv_memory_info, size_t size);
void* _lvmv_realloc_final(char* filename, int line, struct lvmv_memory* lvmv_memory_info, void* memory, size_t size);
void* _lvmv_realloc(char* filename, int line, struct lvmv_memory* lvmv_memory_info, void* memory, size_t size);
void _lvmv_free_final(char* filename, int line, struct lvmv_memory* lvmv_memory_info, struct lvmv_memory_node* node);
void _lvmv_free(char* filename, int line, struct lvmv_memory* lvmv_memory_info, void* memory);
int _lvmv_memory_destroy(char* filename, int line, struct lvmv_memory* lvmv_memory_info);
void _lvmv_memory_print(char* filename, int line, struct lvmv_memory* lvmv_memory_info);
char* _lvmv_memory2string(char* filename, int line, struct lvmv_memory* lvmv_memory_info);
////////////////

////////////////
//직접 호출하게 되는 메모리 관련 함수들
#define lvmv_malloc(h, size) _lvmv_malloc(__FILE__, __LINE__, h, size)
#define lvmv_free(h, memory) _lvmv_free(__FILE__, __LINE__, h, memory)
#define lvmv_realloc(h, memory, size) _lvmv_realloc(__FILE__, __LINE__, h, memory, size)
#define lvmv_memory_create() _lvmv_memory_create(__FILE__, __LINE__)
#define lvmv_memory_init(h) _lvmv_memory_init(__FILE__, __LINE__, h)
#define lvmv_memory_destroy(h) _lvmv_memory_destroy(__FILE__, __LINE__, h)
#define lvmv_memory_print(h) _lvmv_memory_print(__FILE__, __LINE__, h)
#define lvmv_memory2string(h) _lvmv_memory2string(__FILE__, __LINE__, h)
#define lvmv_memory_error_print(msg) fprintf(stderr, "%s(%d): %s\n", filename, line, msg)
/////////////////

#endif // _LVMV_MEMORY_H_

