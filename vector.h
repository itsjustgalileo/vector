#ifndef VECTOR_H_
#define VECTOR_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct Vector Vector;
    
#define vector_foreach(type, ptr, vec)            \
    for (type *ptr = (type *)vector_begin(vec);   \
         (char *)ptr < (char *)vector_end(vec);   \
         ++ptr)

Vector *vector_init(size_t capacity, size_t elem_size, void (*destructor)(void *));
void vector_cleanup(Vector *vec);
void vector_reset(Vector *vec);
void vector_clear(Vector *vec);

void *vector_push_back(Vector *vec, void *elem);
bool vector_pop_back(Vector *vec, void **out_elem);
void *vector_at(const Vector *vec, uint32_t idx);
void *vector_front(const Vector *vec);
void *vector_back(const Vector *vec);

bool vector_reserve(Vector *vec, size_t capacity);
bool vector_shrink_to_fit(Vector *vec);
size_t vector_size(const Vector *vec);
size_t vector_capacity(const Vector *vec);
bool vector_is_empty(const Vector *vec);

bool vector_insert(Vector *vec, uint32_t at, const void *src);
bool vector_erase(Vector *vec, uint32_t at);

void *vector_begin(const Vector *vec);
void *vector_end(const Vector *vec);

#endif /* VECTOR_H_ */
