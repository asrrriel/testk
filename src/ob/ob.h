#ifndef __OB_H__
#define __OB_H__

#include <stdint.h>
#include <stdbool.h>

#define OB_SIZE 512

#define OBJ_TYPE_SIMPLE_BUFFER 1
#define OBJ_TYPE_SIMPLE_TYPE   2
#define OBJ_TYPE_FILE          3
#define OBJ_TYPE_OBTREE_NODE   4

#define OB_SIMPLE_BUFFER_SIZE (OB_SIZE - (8 + sizeof(uintptr_t)))

typedef struct object {
    uint8_t type;
    union {
        struct {
            uint8_t data[OBJ_TYPE_SIMPLE_BUFFER];
            uintptr_t next_page;
        } simp_buffer;
        struct {
            uintptr_t events[(OB_SIZE - 8) / sizeof(uintptr_t)];
        } simp_objtype;
        struct {
            uintptr_t name; // OBJ id
            uintptr_t data;
            uint64_t size;
            uint64_t last_access;
            uint64_t last_modified;
            uint8_t flags;
        } fs_file;
        struct {
            uintptr_t key;
            uint8_t color;
            uintptr_t obj;
            uintptr_t parent;
            uintptr_t left;
            uintptr_t right;
        } obtree_node;
    } data;
} object_t;

_Static_assert(sizeof(object_t) == OB_SIZE, "object_t size");

bool ob_init();
uintptr_t ob_store(object_t* obj);
object_t* ob_get(uintptr_t id);
bool ob_free(uintptr_t id);
bool ob_print();

#endif // __OB_H__