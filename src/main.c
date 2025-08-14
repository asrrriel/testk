#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "ob/ob.h"
#include "ob/oh.h"
#include "ob/obtree.h"
#include "ev/ev.h"
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>

bool make_root_dir(){
    if(!obtree_init()){
        return false;
    }
    
    if (!obtree_append(0, "/DEV")){
        printf("DEV mounting failed!\n");
        return false;
    }

    if (!obtree_append(0, "/IRD")){
        printf("IRD mounting failed!\n");
        return false;
    }

    if (!obtree_append(0, "/PRC")){
        printf("PRC mounting failed!\n");
        return false;
    }

    if (!obtree_append(0, "/REG")){
        printf("REG mounting failed!\n");
        return false;
    }

    return true;
}


//CHATGPT CODE

void process_file(const char *fullpath, const char *treepath) {
    FILE *f = fopen(fullpath, "rb");
    if (!f) {
        perror("fopen");
        return;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *buf = malloc(size);
    if (!buf) {
        perror("malloc");
        fclose(f);
        return;
    }

    if (fread(buf, 1, size, f) != (size_t)size) {
        perror("fread");
        free(buf);
        fclose(f);
        return;
    }
    fclose(f);

    uintptr_t string_id = oh_store_buf(buf, size);
    free(buf);

    if (!obtree_append(string_id, treepath)) {
        printf("String mounting failed for %s!\n", treepath);
    }
}

void walk_dir(const char *basepath, const char *treeprefix) {
    DIR *dir = opendir(basepath);
    if (!dir) {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char fullpath[1024];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", basepath, entry->d_name);

        char treepath[1024];
        snprintf(treepath, sizeof(treepath), "%s/%s", treeprefix, entry->d_name);

        struct stat st;
        if (stat(fullpath, &st) == -1) {
            perror("stat");
            continue;
        }

        if (S_ISDIR(st.st_mode)) {
            if (!obtree_append(0, treepath)) {
                printf("Dir failed for %s!\n", treepath);
            }
            walk_dir(fullpath, treepath); // recurse into subdirectory
        } else if (S_ISREG(st.st_mode)) {
            process_file(fullpath, treepath);
        }
    }
    closedir(dir);
}
//END OF CHATGPT CODE

bool event_handler1(uintptr_t emitter_id, uintptr_t passed_value){
    printf("event_handler1 called with emitter_id: %d, passed_value: %d\n", emitter_id, passed_value);
    return true;
}

bool event_handler2(uintptr_t emitter_id, uintptr_t passed_value){
    printf("event_handler2 called with emitter_id: %d, passed_value: %d\n", emitter_id, passed_value);
    return true;
}

int main() {
    if (!ob_init()){
        printf("Ob initialization failed!\n");
        return 1;
    }
    if(!make_root_dir()){
        printf("Root directory creation failed!\n");
        return 1;
    }
    
    if(!obtree_append(0 /*TODO: streams*/,"/DEV/UART0")){
        printf("UART0 mounting failed!\n");
        return 1;
    }

    uintptr_t emitter_id = ev_register_emitter(0);

    uintptr_t listener_id1 = ev_register_listener(emitter_id, event_handler1);
    uintptr_t listener_id2 = ev_register_listener(emitter_id, event_handler2);

    ev_subscribe(emitter_id, listener_id1);
    ev_subscribe(emitter_id, listener_id2);

    ev_emit(emitter_id, 42);
    ev_emit(emitter_id, 69);
    ev_emit(emitter_id, 420);

    walk_dir("test", "/IRD");

    obtree_print();
    ob_print();
    return 0;
}