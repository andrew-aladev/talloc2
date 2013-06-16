// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#include <stdbool.h>

#include <talloc2/tree.h>

#ifdef TALLOC_UTILS_BUFFER
#include <talloc2/utils/buffer.h>
#endif

#ifdef TALLOC_UTILS_DYNARR
#include <talloc2/utils/dynarr.h>
#endif

#ifdef TALLOC_UTILS_STACK
#include <talloc2/utils/stack.h>
#endif

#ifdef TALLOC_UTILS_BUFFER
bool test_buffer ( void * ctx )
{
    talloc_buffer * buffer = talloc_buffer_new ( ctx );
    if ( buffer == NULL ) {
        return false;
    }
    char * buf;

    if ( ( buf = talloc_buffer_get ( buffer, 10 ) ) == NULL ) {
        talloc_free ( buffer );
        return false;
    }
    buf[0] = '0';
    buf[1] = '1';
    buf[2] = '2';
    if ( talloc_buffer_cut ( buffer, 7 ) != 0 ) {
        talloc_free ( buffer );
        return false;
    }

    if ( ( buf = talloc_buffer_get ( buffer, 10 ) ) == NULL ) {
        talloc_free ( buffer );
        return false;
    }
    buf[0] = '3';
    buf[1] = '4';
    if ( talloc_buffer_cut ( buffer, 8 ) != 0 ) {
        talloc_free ( buffer );
        return false;
    }

    if ( ( buf = talloc_buffer_get ( buffer, 5 ) ) == NULL ) {
        talloc_free ( buffer );
        return false;
    }
    buf[0] = '5';
    if ( talloc_buffer_cut ( buffer, 4 ) != 0 ) {
        talloc_free ( buffer );
        return false;
    }

    if ( ( buf = talloc_buffer_get ( buffer, 10 ) ) == NULL ) {
        talloc_free ( buffer );
        return false;
    }
    buf[0] = '6';
    buf[1] = '7';
    buf[2] = '8';
    buf[3] = '9';
    if ( talloc_buffer_cut ( buffer, 6 ) != 0 ) {
        talloc_free ( buffer );
        return false;
    }

    if (
        buffer->length != 10 ||
        strncmp ( buffer->buf, "0123456789", 10 ) != 0
    ) {
        talloc_free ( buffer );
        return false;
    }
    return true;
}
#endif

#ifdef TALLOC_UTILS_DYNARR
bool test_dynarr ( void * ctx )
{
    size_t a, b;
    talloc_dynarr * arr = talloc_dynarr_new ( ctx, 0 );
    if ( arr != NULL ) {
        return false;
    }
    arr = talloc_dynarr_new ( ctx, 3 );
    if ( arr == NULL ) {
        return false;
    }
    if (
        talloc_dynarr_append ( arr, &a ) != 0 ||
        talloc_dynarr_append ( arr, &a ) != 0 ||
        talloc_dynarr_append ( arr, &b ) != 0 ||
        talloc_dynarr_append ( arr, &a ) != 0 ||
        talloc_dynarr_append ( arr, &a ) != 0 ||
        talloc_dynarr_append ( arr, &b ) != 0 ||
        talloc_dynarr_append ( arr, &a ) != 0 ||
        talloc_dynarr_append ( arr, &b ) != 0 ||
        talloc_dynarr_append ( arr, &b ) != 0
    ) {
        talloc_free ( arr );
        return false;
    }

    talloc_dynarr_set ( arr, 3, &b );
    talloc_dynarr_set ( arr, 2, &a );

    if (
        talloc_dynarr_get_length ( arr ) != 9 ||
        talloc_dynarr_get ( arr, 0 ) != &a ||
        talloc_dynarr_get ( arr, 1 ) != &a ||
        talloc_dynarr_get ( arr, 2 ) != &a ||
        talloc_dynarr_get ( arr, 3 ) != &b ||
        talloc_dynarr_get ( arr, 4 ) != &a ||
        talloc_dynarr_get ( arr, 5 ) != &b ||
        talloc_dynarr_get ( arr, 6 ) != &a ||
        talloc_dynarr_get ( arr, 7 ) != &b ||
        talloc_dynarr_get ( arr, 8 ) != &b
    ) {
        talloc_free ( arr );
        return false;
    }

    talloc_free ( arr );
    return true;
}
#endif

#ifdef TALLOC_UTILS_STACK
bool test_stack ( void * ctx )
{
    size_t a, b;
    talloc_stack * stack = talloc_stack_new ( ctx );
    if ( stack == NULL ) {
        return false;
    }

    if (
        talloc_stack_push ( stack, &a ) != 0 ||
        talloc_stack_push ( stack, &a ) != 0 ||
        talloc_stack_push ( stack, &b ) != 0 ||
        talloc_stack_push ( stack, &a ) != 0 ||
        talloc_stack_push ( stack, &a ) != 0 ||
        talloc_stack_push ( stack, &b ) != 0 ||
        talloc_stack_push ( stack, &a ) != 0 ||
        talloc_stack_push ( stack, &b ) != 0 ||
        talloc_stack_push ( stack, &b ) != 0
    ) {
        talloc_free ( stack );
        return false;
    }

    talloc_stack_item * item = stack->last_item;
    if (
        talloc_stack_get_length ( stack ) != 9 ||
        item == NULL ||

        item->data != &b || ( item = item->prev ) == NULL ||
        item->data != &b || ( item = item->prev ) == NULL ||
        item->data != &a || ( item = item->prev ) == NULL ||
        item->data != &b || ( item = item->prev ) == NULL ||
        item->data != &a || ( item = item->prev ) == NULL ||
        item->data != &a || ( item = item->prev ) == NULL ||
        item->data != &b || ( item = item->prev ) == NULL ||
        item->data != &a || ( item = item->prev ) == NULL ||
        item->data != &a
    ) {
        talloc_free ( stack );
        return false;
    }

    talloc_stack_pop ( stack );
    talloc_stack_pop ( stack );
    talloc_stack_pop ( stack );
    talloc_stack_pop ( stack );
    talloc_stack_pop ( stack );
    
    item = stack->last_item;
    
    if (
        talloc_stack_get_length ( stack ) != 4 ||
        item == NULL ||

        item->data != &a || ( item = item->prev ) == NULL ||
        item->data != &b || ( item = item->prev ) == NULL ||
        item->data != &a || ( item = item->prev ) == NULL ||
        item->data != &a
    ) {
        talloc_free ( stack );
        return false;
    }

    talloc_free ( stack );
    return true;
}
#endif

int main ()
{
    void * ctx = talloc_new ( NULL );
    if ( ctx == NULL ) {
        return 1;
    }

#ifdef TALLOC_UTILS_BUFFER
    if ( !test_buffer ( ctx ) ) {
        talloc_free ( ctx );
        return 2;
    }
#endif

#ifdef TALLOC_UTILS_DYNARR
    if ( !test_dynarr ( ctx ) ) {
        talloc_free ( ctx );
        return 3;
    }
#endif

#ifdef TALLOC_UTILS_STACK
    if ( !test_stack ( ctx ) ) {
        talloc_free ( ctx );
        return 4;
    }
#endif

    talloc_free ( ctx );
    return 0;
}
