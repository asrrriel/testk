#ifndef __MATH_H__
#define __MATH_H__

#include <stddef.h>

static inline int min(int a, int b) {
    return a < b ? a : b;
}

static inline int max(int a, int b) {
    return a > b ? a : b;
}

static inline int smin(size_t a, size_t b) {
    return a < b ? a : b;
}

static inline int smax(size_t a, size_t b) {
    return a > b ? a : b;
}

#endif // __MATH_H__