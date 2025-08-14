#include "../ob/ob.h"
#include "../ob/oh.h"
#include <stdint.h>
#include <string.h>
#include "ev.h"

uintptr_t ev_register_emitter(uintptr_t starter_id){
    object_t emitter = {
        .type = OBJ_TYPE_EVENT_EMITTER,
        .data = {
            .event_emitter = {
                .starter_id = starter_id,
            }
        } 
    };

    memset(emitter.data.event_emitter.listeners, 0, sizeof(emitter.data.event_emitter.listeners));

    return ob_store(&emitter);
}

uintptr_t ev_register_listener(uintptr_t emitter_id, ev_callback callback){
    object_t listener = {
        .type = OBJ_TYPE_EVENT_LISTENER,
        .data = {
            .event_listener = {
                .emitter_id = emitter_id,
                .callback = (uintptr_t)callback,
            }
        }
    };

    return ob_store(&listener);
}

bool ev_subscribe(uintptr_t emitter_id, uintptr_t listener_id){
    object_t* emitter = ob_get(emitter_id);
    object_t* listener = ob_get(listener_id);

    uintptr_t* listeners = emitter->data.event_emitter.listeners;

    while(*listeners != 0 && (uintptr_t)listeners < (uintptr_t)emitter->data.event_emitter.listeners + sizeof(emitter->data.event_emitter.listeners)){
        if (*listeners == listener_id){
            return false;
        }
        listeners++;
    }

    //TODO: scheduler integration
    *listeners = listener_id;
    return true;
}

bool ev_emit(uintptr_t emitter_id, uintptr_t passed_value){
    object_t* emitter = ob_get(emitter_id);
    uintptr_t* listeners = emitter->data.event_emitter.listeners;

    while(*listeners != 0 && (uintptr_t)listeners < (uintptr_t)emitter->data.event_emitter.listeners + sizeof(emitter->data.event_emitter.listeners)){
        object_t* listener = ob_get(*listeners);

        if (listener->type != OBJ_TYPE_EVENT_LISTENER){
            return false;
        }

        //TODO: scheduler integration
        if (!((ev_callback)listener->data.event_listener.callback)(emitter_id, passed_value)){
            return false;
        }

        listeners++;
    }

    return true;
}