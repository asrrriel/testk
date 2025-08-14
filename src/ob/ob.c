#include "ob.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

void* object_space = NULL;
uintptr_t object_space_size = 0;
object_t* lowest_free = NULL;

bool ob_init() {
    object_space_size = 1024;
    object_space = calloc(OB_SIZE, object_space_size);
    if(object_space == NULL) {
        return false;
    }
    lowest_free = object_space;
    return true;
}

uintptr_t ob_store(object_t* obj) {
    *lowest_free = *obj;
    uintptr_t toret = ((uintptr_t)lowest_free - (uintptr_t)object_space) / OB_SIZE;

    for (object_t* i = lowest_free; i < (object_t*)object_space + object_space_size; i++) {
        lowest_free = i;
        if (i->type == 0) { // the lowest free is fine as is
            return toret;
        }
    }

    //we stepped out of bounds, we need to expand
    object_space_size += object_space_size / 2;
    object_space = realloc(object_space, object_space_size * OB_SIZE);
    if (object_space == NULL) {
        return 0; // TODO: panic
    }
    lowest_free = object_space;

    return toret;
}

object_t* ob_get(uintptr_t id) {
    if (id >= object_space_size) {
        return NULL;
    }

    object_t* toret = (object_t*)object_space + id;
    
    if (toret->type != 0) {
        return toret;
    }

    return NULL;
}

bool ob_free(uintptr_t id) {
    if (id >= object_space_size) {
        return false;
    }

    object_t* toret = (object_t*)object_space + id;

    if (toret->type == 0) {
        return false; //already free
    }

    if (lowest_free > toret) {
        lowest_free = toret;
    }

    toret->type = 0;
    return true;
}

bool ob_print()
{
    printf("Object space size: %lu\n", object_space_size);
    printf("Lowest free ID: %lu\n", ((uintptr_t)lowest_free - (uintptr_t)object_space) / OB_SIZE);
    for (object_t* i = object_space; i < (object_t*)object_space + object_space_size; i++) {
        switch (i->type) {
            case 0:
                break; // say nothing
            case OBJ_TYPE_SIMPLE_BUFFER:
                printf("[%4lu] Simple buffer: %s\n", ((uintptr_t)i - (uintptr_t)object_space) / OB_SIZE, i->data.simp_buffer.data);
                break;
            case OBJ_TYPE_OBTREE_NODE:
                printf("[%4lu] Obtree node\n", ((uintptr_t)i - (uintptr_t)object_space) / OB_SIZE);
                printf("   |-- Color: %s\n", i->data.obtree_node.color ? "red" : "black");
                printf("   |-- Left: %lu\n", i->data.obtree_node.left);
                printf("   |-- Right: %lu\n", i->data.obtree_node.right);
                printf("   |-- Parent: %lu\n", i->data.obtree_node.parent);
                printf("   |-- Key: %lu\n", i->data.obtree_node.key);
                printf("   \\-- Object: %lu\n", i->data.obtree_node.obj);
                break;
            case OBJ_TYPE_FILE:
                printf("[%4lu] File\n", ((uintptr_t)i - (uintptr_t)object_space) / OB_SIZE);
                break;
            
            case OBJ_TYPE_EVENT_EMITTER:
                printf("[%4lu] Event emitter\n", ((uintptr_t)i - (uintptr_t)object_space) / OB_SIZE);
                break;
            case OBJ_TYPE_EVENT_LISTENER:
                printf("[%4lu] Event listener\n", ((uintptr_t)i - (uintptr_t)object_space) / OB_SIZE);
                break;
            case OBJ_TYPE_UNIDIR_PIPE:
                printf("[%4lu] Unidirectional pipe\n", ((uintptr_t)i - (uintptr_t)object_space) / OB_SIZE);
                break;

            default:
                printf("[%3lu] Object type %d\n", ((uintptr_t)i - (uintptr_t)object_space) / OB_SIZE, i->type);
                break;
        }
    }
    return true;
}