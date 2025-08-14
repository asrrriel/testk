#ifndef __PIPE_H__
#define __PIPE_H__

#include <stdint.h>

uintptr_t pipe_create(uintptr_t owner_id, uintptr_t listener_id);
uintptr_t pipe_write(uintptr_t pipe_id, uintptr_t data, uintptr_t emitter_id);


#endif // __PIPE_H__