#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "./vector.h"

struct Vector {
    void *value;
    size_t elem_size;
    size_t elem_count;
    size_t capacity;
    void (*destructor)(void *);
};

Vector *vector_init(const size_t capacity, const size_t elem_size, void (*destructor)(void *)) {
    assert(elem_size > 0);
    
    Vector *vec = (Vector *)malloc(sizeof(struct Vector));
    if (NULL == vec) {
        return NULL;
    }

    vec->capacity = capacity == 0 ? 1 : capacity;
    vec->elem_size = elem_size;
    vec->elem_count = 0;
    vec->destructor = destructor == NULL ? NULL : destructor;

    vec->value = malloc(vec->capacity * vec->elem_size);
    if (NULL == vec->value) {
        free(vec);
        return NULL;
    }

    return vec;
}

void vector_cleanup(Vector *vec) {
    assert(vec);

    if (vec->destructor) {
        for (char *ptr = vec->value;
             ptr < (char *)vec->value + vec->elem_count * vec->elem_size;
             ptr += vec->elem_size) {
            vec->destructor(ptr);
        }
    }

    free(vec->value);
    free(vec);

    return;
}

void vector_reset(Vector *vec) {
    assert(vec);

    vec->elem_count = 0;

    return;
}

void vector_clear(Vector *vec) {
    assert(vec);

    if (vec->destructor) {
        for (char *ptr = vec->value;
             ptr < (char *)vec->value + vec->elem_count * vec->elem_size;
             ptr += vec->elem_size) {
            vec->destructor(ptr);
        }
    }

    vector_reset(vec);

    return;
}

void *vector_push_back(Vector *vec, const void *elem) {
    assert(vec && elem);

    if (vec->elem_count >= vec->capacity) {
        if (!vector_reserve(vec, (vec->capacity * 2))) {
            return NULL;
        }
    }

    memcpy((char *)vec->value + vec->elem_count * vec->elem_size, elem, vec->elem_size);

    ++vec->elem_count;

    return ((char *)vec->value + ((vec->elem_count - 1) * vec->elem_size));
}

bool vector_pop_back(Vector *vec, void **out) {
    assert(vec && out && vec->elem_count);

    --vec->elem_count;    

    memcpy(out, (char *)vec->value + (vec->elem_count * vec->elem_size), vec->elem_size);
    
    return true;
}

void *vector_at(const Vector *vec, const size_t idx) {
    assert(vec);
    assert(idx < vec->elem_count);
    
    return ((char *)vec->value + (idx * vec->elem_size));
}

bool vector_reserve(Vector *vec, const size_t capacity) {
    assert(vec);

    if (capacity <= vec->capacity) {
        return false;
    }

    void *res = realloc(vec->value, capacity * vec->elem_size);
    if (NULL == res) {
        
        return false;
    }
    
    vec->capacity = capacity;
    vec->value = res;

    return true;
}

bool vector_shrink_to_fit(Vector *vec) {
    assert(vec);

    if (vec->elem_count < vec->capacity) {
        size_t new_capacity = vec->elem_count;
        void *res = realloc(vec->value, new_capacity * vec->elem_size);
        if (NULL == res) {
            return false;
        }

        vec->value = res;
        vec->capacity = new_capacity;

        return true;
    }

    return false;
}

bool vector_insert(Vector *vec, const size_t at, const void *src) {
    assert(vec && src && at <= vec->elem_count);
    
    if (vec->elem_count >= vec->capacity) {
        if (!vector_reserve(vec, vec->capacity * 2)) {
            return false;
        }
    }

    memmove(
        (char *)vec->value + (at + 1) * vec->elem_size,
        (char *)vec->value + at * vec->elem_size,
        (vec->elem_count - at) * vec->elem_size);

    memcpy((char *)vec->value + at * vec->elem_size, src, vec->elem_size);

    ++vec->elem_count;

    return true;
}

bool vector_erase(Vector *vec, size_t at) {
    assert(vec && at < vec->elem_count);

    if (vec->destructor) {
        vec->destructor((char *)vec->value + at * vec->elem_size);
    }

    memmove(
        (char *)vec->value + at * vec->elem_size,
        (char *)vec->value + (at + 1) * vec->elem_size,
        (vec->elem_count - at - 1) * vec->elem_size);

    --vec->elem_count;
    
    return true;
}

void *vector_begin(const Vector *vec) {
    assert(vec);

    return (char *)vec->value;
}

void *vector_end(const Vector *vec) {
    assert(vec);

    return (char *)vec->value + (vec->elem_count * vec->elem_size);
}

size_t vector_size(const Vector *vec) {
    assert(vec);

    return vec->elem_count;
}

size_t vector_capacity(const Vector *vec) {
    assert(vec);

    return vec->capacity;
}

bool vector_is_empty(const Vector *vec) {
    assert(vec);

    return vec->elem_count == 0;
}

void *vector_front(const Vector *vec) {
    assert(vec && vec->elem_count > 0);

    return (char *)vec->value;
}

void *vector_back(const Vector *vec) {
    assert(vec && vec->elem_count > 0);

    return (char *)vec->value + ((vec->elem_count - 1) * vec->elem_size);
}

void *vector_data(const Vector *vec) {
    assert(vec);

    return (char *)vec->value;
}

Vector *vector_clone(const Vector *vec) {
    assert(vec);

    Vector *clone = (Vector *)calloc(1, sizeof(Vector));
    if (NULL == clone) {
        return NULL;
    }

    clone->value = malloc(vec->elem_count * vec->elem_size);
    if (NULL == clone->value) {
        free(clone);
        return NULL;
    }

    clone->capacity = vec->elem_count;
    clone->elem_count = vec->elem_count;
    clone->elem_size = vec->elem_size;
    clone->destructor = vec->destructor;

    memcpy(clone->value, vec->value, (vec->elem_count * vec->elem_size));

    return clone;
}
