#pragma once

#include "defines.h"

enum { DARRAY_CAPACITY, DARRAY_LENGTH, DARRAY_STRIDE, DARRAY_FIELD_LENGTH };

/**
 * Make a new darray.
 * @param length Number of elements.
 * @param stride Width (size) of elements.
 * @return Created darray.
 */
PAPI void *_darray_create(u64 length, u64 stride);

/**
 * Delete a darray.
 */
PAPI void _darray_destroy(void *array);

/**
 * Get field data of a darray.
 * @param array darray to get from.
 * @param field Field to get. One of the above
 * @return Field's value.
 */
PAPI u64 _darray_field_get(void *array, u64 field);

/**
 * Set field data of a darray.
 * @param array darray to set.
 * @param field Field to set. One of the above.
 * @param value New value.
 */
PAPI void _darray_field_set(void *array, u64 field, u64 value);

/**
 * Automatically resize provided darray.
 */
PAPI void *_darray_resize(void *array);

/**
 * Push to end of array.
 * @param array darray to push to.
 * @param value_ptr Ptr to new element.
 */
PAPI void *_darray_push(void *array, const void *value_ptr);

/**
 * Pop from end of darray.
 * @param array Array to pop off of.
 * @param dest Destination for popped value.
 */
PAPI void _darray_pop(void *array, void *dest);

/**
 * Push to specific value.
 * @param array darray to push to.
 * @param index Index of new element.
 * @param value_ptr Ptr to new element.
 */
PAPI void _darray_push_at(void *array, u64 index, void *value_ptr);

/**
 * Pop a specific value.
 * @param array darray to pop from.
 * @param index Index to pop from
 * @param dest Destination for popped value.
 */
PAPI void *_darray_pop_at(void *array, u64 index, void *dest);

#define DARRAY_DEFAULT_CAPACITY 1
#define DARRAY_RESIZE_FACTOR 2

// Create a dynamic array.
#define darray_create(type)                                                    \
    _darray_create(DARRAY_DEFAULT_CAPACITY, sizeof(type))

// Reserve a dynamic array with determined capacity.
#define darray_reserve(type, capacity) _darray_create(capacity, sizeof(type))

// Destroy a dynamic array.
#define darray_destroy(array) _darray_destroy(array);

// Push to a dynamic array.
#define darray_push(array, value)                                              \
    {                                                                          \
        typeof(value) temp = value;                                            \
        array = _darray_push(array, &temp);                                    \
    }

// Pop from a dynamic array.
#define darray_pop(array, value_ptr) _darray_pop(array, value_ptr)

#define darray_insert_at(array, index, value)                                  \
    {                                                                          \
        typeof(value) temp = value;                                            \
        array = _darray_insert_at(array, index, &temp);                        \
    }

#define darray_pop_at(array, index, value_ptr)                                 \
    _darray_pop_at(array, index, value_ptr)

#define darray_clear(array) _darray_field_set((void *)&array, DARRAY_LENGTH, 0)

#define darray_capacity(array) _darray_field_get(array, DARRAY_CAPACITY)

#define darray_length(array) _darray_field_get(array, DARRAY_LENGTH)

#define darray_stride(array) _darray_field_get(array, DARRAY_STRIDE)

#define darray_length_set(array, value)                                        \
    _darray_field_set(array, DARRAY_LENGTH, value)
