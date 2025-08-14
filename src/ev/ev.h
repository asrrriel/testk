#ifndef __EV_H__
#define __EV_H__

#include <stdint.h>

typedef bool (*ev_callback)(uintptr_t emitter_id, uintptr_t passed_value);

uintptr_t ev_register_emitter(uintptr_t starter_id);
uintptr_t ev_register_listener(uintptr_t emitter_id, ev_callback callback);

bool ev_subscribe(uintptr_t emitter_id, uintptr_t listener_id);
bool ev_emit(uintptr_t emitter_id, uintptr_t passed_value);


#endif // __EV_H__