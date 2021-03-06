// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/tree.h>

#if defined(TRALLOC_DEBUG)
#   include <tralloc/events.h>
#endif

int main ()
{
    size_t data_1_length = tralloc_predict_chunk_length ( TRALLOC_EXTENSION_POOL_CHILD ) + sizeof ( uint8_t ) * 200;
    size_t data_2_length = tralloc_predict_chunk_length ( TRALLOC_EXTENSION_POOL_CHILD ) + sizeof ( uint8_t ) * 100;

    tralloc_context * pool_ctx;
    uint8_t * data_1, * data_2;
    if (
        tralloc_with_extensions ( NULL, &pool_ctx, TRALLOC_EXTENSION_POOL, data_1_length + data_2_length ) != 0 ||
        tralloc      ( pool_ctx, ( tralloc_context ** ) & data_1, sizeof ( uint8_t ) * 200 ) != 0 ||
        tralloc_zero ( data_1,   ( tralloc_context ** ) & data_2, sizeof ( uint8_t ) * 100 ) != 0
    ) {
        return 1;
    }

    if ( tralloc_free ( data_1 ) != 0 ) {
        return 2;
    }

    if (
        tralloc      ( pool_ctx, ( tralloc_context ** ) & data_1, sizeof ( uint8_t ) * 100 ) != 0 ||
        tralloc_zero ( data_1,   ( tralloc_context ** ) & data_2, sizeof ( uint8_t ) * 200 ) != 0
    ) {
        return 3;
    }

    if ( tralloc_free ( pool_ctx ) != 0 ) {
        return 4;
    }
    // there were only 1 alloc and 1 free calls
    // data_1 and data_2 are pool children

#   if defined(TRALLOC_DEBUG)
    if ( tralloc_get_chunks_count() != 0 ) {
        return 5;
    }
#   endif

    return 0;
}
