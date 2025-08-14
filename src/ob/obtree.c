#include "obtree.h"
#include "ob.h"
#include "oh.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>

uintptr_t root_id;
object_t* root;

bool obtree_init(){
    uintptr_t key = oh_store_buf("/",1);

    object_t newroot = {
        .type = OBJ_TYPE_OBTREE_NODE,
        .data.obtree_node.color = 1,
        .data.obtree_node.left = 0,
        .data.obtree_node.right = 0,
        .data.obtree_node.parent = 0,
        .data.obtree_node.key = key,
        .data.obtree_node.obj = 0
    };

    root_id = ob_store(&newroot);
    root = ob_get(root_id);

    return root != NULL;
}

uintptr_t __obtree_new(char* key, uintptr_t obj,uintptr_t parent){
    uintptr_t key_obj = oh_store_buf(key, strlen(key));

    object_t newnode = {
        .type = OBJ_TYPE_OBTREE_NODE,
        .data.obtree_node.color = 1,
        .data.obtree_node.left = 0,
        .data.obtree_node.right = 0,
        .data.obtree_node.parent = parent,
        .data.obtree_node.key = key_obj,
        .data.obtree_node.obj = obj
    };

    return ob_store(&newnode);
}

bool obtree_append(uintptr_t obj_id, char* path){
    uintptr_t current_id = root_id;
    object_t* current = root;
    while(true){
        if (current->type != OBJ_TYPE_OBTREE_NODE){
            return false;
        }
        int verdict = 0;
        char* path_part = path;
        object_t* c2 = ob_get(current->data.obtree_node.key);
        while(verdict == 0 && c2 != NULL){
            if (c2->type != OBJ_TYPE_SIMPLE_BUFFER){
                return false;
            }

            verdict = strncmp(path_part,(char*)c2->data.simp_buffer.data,OB_SIMPLE_BUFFER_SIZE);

            c2 = ob_get(c2->data.obtree_node.key);
            path_part += OB_SIMPLE_BUFFER_SIZE;
        }
        if (verdict == 0){
            if (*path_part == '\0' && c2 == NULL) {
                return false; // key already exists
            } else if (*path_part == '\0') {
                verdict = -1;
            } else {
                verdict = 1;
            }
        } 
        
        if(verdict > 0){
            if(current->data.obtree_node.right == 0){
                current->data.obtree_node.right = __obtree_new(path, obj_id,current_id);
                current_id = current->data.obtree_node.right;
                current = ob_get(current->data.obtree_node.right);
                break;
            }
            current_id = current->data.obtree_node.right;
            current = ob_get(current->data.obtree_node.right);
        } else {
            if(current->data.obtree_node.left == 0){
                current->data.obtree_node.left = __obtree_new(path, obj_id,current_id);
                current_id = current->data.obtree_node.left;
                current = ob_get(current->data.obtree_node.left);
                break;
            }
            current_id = current->data.obtree_node.left;
            current = ob_get(current->data.obtree_node.left);
        }
    }

    //TODO: balancing
    //if (ob_get(current->data.obtree_node.parent)->data.obtree_node.color == 1){
    //    current->data.obtree_node.color = 0;
    //}

    return true;
}

uintptr_t obtree_get(char* path){
    uintptr_t current_id = root_id;
    object_t* current = root;
    while(true){
        if (current->type != OBJ_TYPE_OBTREE_NODE){
            return false;
        }
        int verdict = 0;
        char* path_part = path;
        object_t* c2 = ob_get(current->data.obtree_node.key);
        while(verdict == 0 && c2 != NULL){
            if (c2->type != OBJ_TYPE_SIMPLE_BUFFER){
                return false;
            }

            verdict = strncmp(path_part,(char*)c2->data.simp_buffer.data,OB_SIMPLE_BUFFER_SIZE);

            c2 = ob_get(c2->data.obtree_node.key);
            path_part += OB_SIMPLE_BUFFER_SIZE;
        }
        if (verdict == 0){
            if (*path_part == '\0' && c2 == NULL) {
                return current->data.obtree_node.obj;
            } else if (*path_part == '\0') {
                verdict = -1;
            } else {
                verdict = 1;
            }
        } 

        if(verdict > 0){
            if(current->data.obtree_node.right == 0){
                return 0;
            }
            current_id = current->data.obtree_node.right;
            current = ob_get(current->data.obtree_node.right);
        } else {
            if(current->data.obtree_node.left == 0){
                return 0;
            }
            current_id = current->data.obtree_node.left;
            current = ob_get(current->data.obtree_node.left);
        }
    }
    return false;
}

bool obtree_delete(char* path){
    uintptr_t current_id = root_id;
    object_t* current = root;
    while(true){
        if (current->type != OBJ_TYPE_OBTREE_NODE){
            return false;
        }
        int verdict = 0;
        char* path_part = path;
        object_t* c2 = ob_get(current->data.obtree_node.key);
        while(verdict == 0 && c2 != NULL){
            if (c2->type != OBJ_TYPE_SIMPLE_BUFFER){
                return false;
            }

            verdict = strncmp(path_part,(char*)c2->data.simp_buffer.data,OB_SIMPLE_BUFFER_SIZE);

            c2 = ob_get(c2->data.obtree_node.key);
            path_part += OB_SIMPLE_BUFFER_SIZE;
        }
        if (verdict == 0){
            if (*path_part == '\0' && c2 == NULL) {
                ob_free(current->data.obtree_node.obj);
                ob_free(current_id);
                return true;
            } else if (*path_part == '\0') {
                verdict = -1;
            } else {
                verdict = 1;
            }
        } 

        if(verdict > 0){
            if(current->data.obtree_node.right == 0){
                return false;
            }
            current_id = current->data.obtree_node.right;
            current = ob_get(current->data.obtree_node.right);
        } else {
            if(current->data.obtree_node.left == 0){
                return false;
            }
            current_id = current->data.obtree_node.left;
            current = ob_get(current->data.obtree_node.left);
        }
    }
    return false;
}

bool __obtree_print_dir(uintptr_t ob_id){
    object_t* ob = ob_get(ob_id);
    size_t nests = 0;

    if(ob_id != root_id){
        object_t* c2 = ob_get(ob->data.obtree_node.key);
        while(c2 != NULL){
            if (c2->type != OBJ_TYPE_SIMPLE_BUFFER){
                return false;
            }
    
            for(int i = 0; i < OB_SIMPLE_BUFFER_SIZE; i++){
                if (c2->data.simp_buffer.data[i] == '/'){
                    nests++;
                }
            }
    
            c2 = ob_get(c2->data.obtree_node.key);
        }
    }

    if (ob->type != OBJ_TYPE_OBTREE_NODE){
        return false;
    }
    
    if (ob->data.obtree_node.left != 0){
        __obtree_print_dir(ob->data.obtree_node.left);
    }

    for (size_t i = 0; i < nests; i++){
        printf("    ");
    }

    printf("|-\"");

    object_t* c2 = ob_get(ob->data.obtree_node.key);
    while(c2 != NULL){
        if (c2->type != OBJ_TYPE_SIMPLE_BUFFER){
            return false;
        }

        printf("%s",c2->data.simp_buffer.data);

        c2 = ob_get(c2->data.obtree_node.key);
    }
    printf("\"\n");
    if (ob->data.obtree_node.right != 0){
        __obtree_print_dir(ob->data.obtree_node.right);
    }
    return true;
}

bool obtree_print(){
    return __obtree_print_dir(root_id);
}