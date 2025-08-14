#include "pipe.h"
#include "ev.h"
#include "../ob/ob.h"


uintptr_t pipe_create(uintptr_t owner_id, uintptr_t listener_id){
    object_t pipe = {
        .type = OBJ_TYPE_UNIDIR_PIPE,
        .data.unidir_pipe.owner = owner_id,
        .data.unidir_pipe.buffer = 0, //TODO: buffer
        .data.unidir_pipe.event_listener = listener_id
    };
    return ob_store(&pipe);
}
uintptr_t pipe_write(uintptr_t pipe_id, uintptr_t data, uintptr_t emitter_id){
    object_t* pipe = ob_get(pipe_id);
    
    if (pipe->type != OBJ_TYPE_UNIDIR_PIPE){
        return 0;
    }
    
    ev_subscribe(emitter_id, pipe->data.unidir_pipe.event_listener);
    return ev_emit(emitter_id, data);
}