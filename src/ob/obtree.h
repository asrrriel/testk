#ifndef __OBTREE_H__
#define __OBTREE_H__

#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>
#include "ob.h"

extern object_t* root;

bool obtree_init();
bool obtree_append(uintptr_t obj_id, char* path);
bool obtree_delete(char* path);
uintptr_t obtree_get(char* path);
bool obtree_print();

#endif // __OBTREE_H__