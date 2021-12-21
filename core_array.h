#ifndef CORE_ARRAY_H_
#define CORE_ARRAY_H_

#include "cheaders.h"

typedef struct array_list ArrayList;
typedef void (*array_free_fn)(void *ptr);

struct array_list {
    void ** data;
    size_t size, alloc;
    array_free_fn free_fn;
};

ArrayList * array_new(array_free_fn free_fn);
void array_add(ArrayList *array, void *ptr);
void *array_get(ArrayList *array, size_t at_index);
void *array_pop_back(ArrayList *array);
void array_free(ArrayList *array);
void array_dummy_free_fn(void *ptr);

#endif /* CORE_ARRAY_H_ */
