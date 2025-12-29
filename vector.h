/**
 * @file vector.h
 * @author itsjustgalileo
 * @version 1.2
 * @brief Dynamic array.
 */
#ifndef VECTOR_H_
#define VECTOR_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @brief Opaque handle to a dynamically-sized contiguous array.
     *
     * The Vector owns its storage. Elements are stored densely and
     * addressed by index. Reallocation may invalidate pointers
     * previously obtained via vector_at, vector_begin, etc.
     */
    typedef struct Vector Vector;

    /**
     * @brief Helper macro to iterate over all elements of a vector.
     *
     * @param type Element type
     * @param ptr  Iterator variable name
     * @param vec  Vector pointer
     *
     * Example:
     * @code
     * vector_foreach(int, it, vec) {
     *     printf("%d\n", *it);
     * }
     * @endcode
     *
     * The vector must not be modified during iteration.
     */
#define vector_foreach(type, ptr, vec)          \
    for (type *ptr = (type *)vector_begin(vec); \
         (char *)ptr < (char *)vector_end(vec); \
         ++ptr)

    /**
     * @brief Create a new vector.
     *
     * @param capacity   Initial number of elements to reserve.
     *                   If zero, a minimum capacity is allocated.
     * @param elem_size  Size in bytes of a single element.
     * @param destructor Optional per-element destructor.
     *                   Called on each element during erase, clear,
     *                   and cleanup. May be NULL.
     *
     * @return Pointer to a new Vector, or NULL on allocation failure.
     */
    extern Vector *vector_create(size_t capacity,
                               size_t elem_size,
                               void (*destructor)(void *));

    /**
     * @brief Destroy a vector and release all resources.
     *
     * Calls the destructor on all stored elements (if provided),
     * then frees internal storage and the vector itself.
     *
     * @param vec Vector to destroy.
     */
    extern void vector_destroy(Vector *vec);

    /**
     * @brief Reset the vector size to zero.
     *
     * Does NOT call destructors.
     * Capacity is preserved.
     *
     * @param vec Vector to reset.
     */
    extern void vector_reset(Vector *vec);

    /**
     * @brief Remove all elements from the vector.
     *
     * Calls the destructor on each element (if provided),
     * then resets the size to zero.
     *
     * Capacity is preserved.
     *
     * @param vec Vector to clear.
     */
    extern void vector_clear(Vector *vec);

    /**
     * @brief Append an element to the end of the vector.
     *
     * Reallocates storage if capacity is exceeded.
     *
     * @param vec  Vector to append to.
     * @param elem Pointer to element data to copy into the vector.
     *
     * @return Pointer to the newly inserted element,
     *         or NULL on allocation failure.
     */
    extern void *vector_push_back(Vector *vec, const void *elem);

    /**
     * @brief Remove the last element of the vector.
     *
     * The element is copied into out_elem.
     * The destructor is NOT called.
     *
     * @param vec      Vector to pop from.
     * @param out_elem Destination buffer (must be at least elem_size bytes).
     *
     * @return true on success, false otherwise.
     */
    extern bool vector_pop_back(Vector *vec, void **out_elem);

    /**
     * @brief Access an element by index.
     *
     * @param vec Vector to access.
     * @param idx Index of element (0 <= idx < size).
     *
     * @return Pointer to the element.
     */
    extern void *vector_at(const Vector *vec, size_t idx);

    /**
     * @brief Access the first element.
     *
     * @param vec Vector to access.
     *
     * @return Pointer to the first element.
     */
    extern void *vector_front(const Vector *vec);

    /**
     * @brief Access the last element.
     *
     * @param vec Vector to access.
     *
     * @return Pointer to the last element.
     */
    extern void *vector_back(const Vector *vec);

    /**
     * @brief Ensure the vector has at least the given capacity.
     *
     * If the requested capacity is less than or equal to the
     * current capacity, no action is taken.
     *
     * @param vec      Vector to reserve storage for.
     * @param capacity Desired minimum capacity.
     *
     * @return true if reallocation occurred, false otherwise.
     */
    extern bool vector_reserve(Vector *vec, size_t capacity);

    /**
     * @brief Shrink capacity to fit the current number of elements.
     *
     * @param vec Vector to shrink.
     *
     * @return true if reallocation occurred, false otherwise.
     */
    extern bool vector_shrink_to_fit(Vector *vec);

    /**
     * @brief Get the number of stored elements.
     *
     * @param vec Vector to query.
     *
     * @return Number of elements.
     */
    extern size_t vector_size(const Vector *vec);

    /**
     * @brief Get the current storage capacity.
     *
     * @param vec Vector to query.
     *
     * @return Capacity in elements.
     */
    extern size_t vector_capacity(const Vector *vec);

    /**
     * @brief Test whether the vector is empty.
     *
     * @param vec Vector to query.
     *
     * @return true if empty, false otherwise.
     */
    extern bool vector_is_empty(const Vector *vec);

    /**
     * @brief Insert an element at a given index.
     *
     * Elements at and after the index are shifted right.
     *
     * @param vec Vector to modify.
     * @param at  Insertion index (0 <= at <= size).
     * @param src Pointer to element data to insert.
     *
     * @return true on success, false on allocation failure.
     */
    extern bool vector_insert(Vector *vec, size_t at, const void *src);

    /**
     * @brief Erase an element at a given index.
     *
     * Calls the destructor on the erased element (if provided).
     * Remaining elements are shifted left.
     *
     * @param vec Vector to modify.
     * @param at  Index of element to erase.
     *
     * @return true on success.
     */
    extern bool vector_erase(Vector *vec, size_t at);

    /**
     * @brief Get a pointer to the first element.
     *
     * @param vec Vector to access.
     *
     * @return Pointer to first element.
     */
    extern void *vector_begin(const Vector *vec);

    /**
     * @brief Get a pointer one past the last element.
     *
     * @param vec Vector to access.
     *
     * @return Pointer past the last element.
     */
    extern void *vector_end(const Vector *vec);

    /**
     * @brief Get a pointer to the underlying storage.
     *
     * @param vec Vector to access.
     *
     * @return Pointer to contiguous element storage.
     */
    extern void *vector_data(const Vector *vec);

    /**
     * @brief Create a shallow clone of a vector.
     *
     * Elements are bitwise-copied.
     * The destructor pointer is copied as-is.
     *
     * @param vec Vector to clone.
     *
     * @return New vector with copied contents, or NULL on failure.
     */
    extern Vector *vector_clone(const Vector *vec);

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif

#endif /* VECTOR_H_ */
