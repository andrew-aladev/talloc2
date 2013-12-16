// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "dynarr.h"

#include <stdbool.h>
#include <string.h>

static inline
uint8_t tralloc_dynarr_grow ( tralloc_dynarr * arr, size_t new_length, bool null_on_init )
{
    size_t length   = arr->length;
    arr->length     = new_length;
    size_t capacity = arr->current_capacity;
    if ( capacity >= new_length ) {
        return 0;
    }

    size_t start_capacity = arr->start_capacity;
    capacity = ( new_length / start_capacity + 1 ) * start_capacity;

    tralloc_context ** data = tralloc_realloc ( arr->data, capacity * sizeof ( uintptr_t ) );
    if ( data == NULL ) {
        return 1;
    }
    if ( null_on_init ) {
        size_t index;
        for ( index = length; index < new_length; index ++ ) {
            data[index] = NULL;
        }
    }

    if ( arr->data != data ) {
        arr->data = data;
    }
    arr->current_capacity = capacity;
    return 0;
}

uint8_t tralloc_dynarr_push ( tralloc_dynarr * arr, void * data )
{
    size_t index = arr->length;
    if ( tralloc_dynarr_grow ( arr, index + 1, false ) != 0 ) {
        return 1;
    }
    arr->data[index] = data;
    return 0;
}

uint8_t tralloc_dynarr_insert_before ( tralloc_dynarr * arr, size_t index, void * data )
{
    size_t length      = arr->length;
    size_t tail_length = length - index;
    if ( tail_length == 0 ) {
        return tralloc_dynarr_push ( arr, data );
    }
    if ( tralloc_dynarr_grow ( arr, length + 1, false ) != 0 ) {
        return 1;
    }
    void ** tail = arr->data + index;
    memmove ( tail + 1, tail, sizeof ( uintptr_t ) * tail_length );
    arr->data[index] = data;
    return 0;
}

static inline
uint8_t tralloc_dynarr_reduction ( tralloc_dynarr * arr )
{
    size_t capacity       = arr->current_capacity;
    size_t start_capacity = arr->start_capacity;
    if ( capacity < arr->length + start_capacity ) {
        return 0;
    }

    capacity -= start_capacity;
    tralloc_context ** data = tralloc_realloc ( arr->data, capacity * sizeof ( uintptr_t ) );
    if ( data == NULL ) {
        return 1;
    }
    arr->data             = data;
    arr->current_capacity = capacity;
    return 0;
}

uint8_t tralloc_dynarr_pop ( tralloc_dynarr * arr )
{
    arr->length--;
    if ( tralloc_dynarr_reduction ( arr ) != 0 ) {
        return 1;
    }
    return 0;
}

uint8_t tralloc_dynarr_delete ( tralloc_dynarr * arr, size_t index )
{
    index ++;
    size_t tail_length = arr->length - index;
    if ( tail_length == 0 ) {
        return tralloc_dynarr_pop ( arr );
    }
    void ** tail = arr->data + index;
    memmove ( tail - 1, tail, sizeof ( uintptr_t ) * tail_length );

    return tralloc_dynarr_pop ( arr );
}

uint8_t tralloc_dynarr_grow_and_set ( tralloc_dynarr * arr, size_t index, void * data )
{
    size_t new_length = index + 1;
    if ( arr->length < new_length ) {
        if ( tralloc_dynarr_grow ( arr, new_length, true ) != 0 ) {
            return 1;
        }
    }
    arr->data[index] = data;
    return 0;
}

extern inline tralloc_dynarr * tralloc_dynarr_new          ( const tralloc_context * ctx, size_t capacity );
extern inline uint8_t          tralloc_dynarr_insert_after ( tralloc_dynarr * arr, size_t index, void * data );
extern inline void             tralloc_dynarr_set          ( tralloc_dynarr * arr, size_t index, void * data );
extern inline void *           tralloc_dynarr_get          ( const tralloc_dynarr * arr, size_t index );
extern inline size_t           tralloc_dynarr_get_length   ( const tralloc_dynarr * arr );