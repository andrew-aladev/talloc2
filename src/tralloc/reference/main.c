// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "main.h"
#include "../extensions/chunk.h"

typedef tralloc_reference * ( * reference_alloc_chunk ) ( const tralloc_context * parent_context, size_t length );

static inline
tralloc_context * add_reference ( const tralloc_context * child_context, const tralloc_context * parent_context, size_t length, reference_alloc_chunk allocator )
{
    if ( child_context == NULL || parent_context == child_context ) {
        return NULL;
    }

    tralloc_chunk * child_chunk = tralloc_chunk_from_context ( child_context );

    if ( parent_context != NULL ) {
        tralloc_chunk * parent_chunk = tralloc_chunk_from_context ( parent_context );
        if ( parent_chunk->mode == TRALLOC_MODE_REFERENCE ) {
            tralloc_reference * reference = tralloc_reference_from_chunk ( parent_chunk );
            parent_chunk                  = tralloc_chunk_from_extensions ( reference->parent_extensions );
            parent_context                = tralloc_context_from_chunk ( parent_chunk );
        }

        if ( child_chunk->parent == parent_chunk ) {
            return NULL;
        }
    }

    tralloc_reference * reference = allocator ( parent_context, length );
    if ( reference == NULL ) {
        return NULL;
    }

    tralloc_extensions * child_extensions = tralloc_extensions_from_chunk ( child_chunk );

    reference->parent_extensions = child_extensions;
    reference->prev              = NULL;

    tralloc_reference * first_reference = child_extensions->first_reference;
    if ( first_reference == NULL ) {
        reference->next = NULL;
    } else {
        reference->next       = first_reference;
        first_reference->prev = reference;
    }
    child_extensions->first_reference = reference;

    return tralloc_context_from_chunk ( tralloc_chunk_from_reference ( reference ) );
}

tralloc_context * tralloc_add_reference_with_data ( const tralloc_context * child_context, const tralloc_context * parent_context, size_t length )
{
    return add_reference ( child_context, parent_context, length, tralloc_reference_malloc_chunk );
}

tralloc_context * tralloc_add_reference_with_zero_data ( const tralloc_context * child_context, const tralloc_context * parent_context, size_t length )
{
    return add_reference ( child_context, parent_context, length, tralloc_reference_calloc_chunk );
}

uint8_t tralloc_clear_references ( const tralloc_context * chunk_context )
{
    tralloc_extensions * extensions = tralloc_extensions_from_chunk ( tralloc_chunk_from_context ( chunk_context ) );
    tralloc_reference * reference   = extensions->first_reference;
    tralloc_reference * next_reference;

    uint8_t result, error = 0;
    tralloc_chunk * chunk;
    while ( reference != NULL ) {
        next_reference = reference->next;
        chunk = tralloc_chunk_from_reference ( reference );
        tralloc_detach_chunk ( chunk );
        if ( ( result = tralloc_free_chunk ( chunk ) ) != 0 ) {
            error = result;
        }
        reference = next_reference;
    }
    extensions->first_reference = NULL;

    return error;
}

extern inline tralloc_context * tralloc_add_reference ( const tralloc_context * child_context, const tralloc_context * parent_context );