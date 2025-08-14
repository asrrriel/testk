#include "oh.h"
#include "ob.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>

uintptr_t oh_store_buf(char* str, size_t len) {
    size_t num_bufs = 1;
    if (len >= OB_SIMPLE_BUFFER_SIZE){
        num_bufs = len / OB_SIMPLE_BUFFER_SIZE;
        if(len % OB_SIMPLE_BUFFER_SIZE != 0){
            num_bufs++;
        }
    }
    uintptr_t next_page = 0;
    for(size_t i = num_bufs; i-- > 0;){
        object_t buf = {
            .type = OBJ_TYPE_SIMPLE_BUFFER,
            .data.simp_buffer.next_page = next_page
        };

        size_t offset = i * OB_SIMPLE_BUFFER_SIZE; 
        size_t chunk_size = (len > offset + OB_SIMPLE_BUFFER_SIZE)
            ? OB_SIMPLE_BUFFER_SIZE
            : len - offset;
        
        
        memcpy(buf.data.simp_buffer.data, str + offset, chunk_size);

        next_page = ob_store(&buf);
    }

    return next_page;
}