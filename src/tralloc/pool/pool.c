// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#define _TRALLOC_INCLUDED_FROM_POOL_POOL_C
#include <tralloc/pool/pool.h>
#include <tralloc/pool/fragment.h>

#include <string.h>


void _tralloc_new_pool ( _tralloc_pool * pool, void * memory, tralloc_extensions extensions, size_t length )
{
    pool->first_child  = NULL;
    pool->extensions   = extensions;
    pool->memory       = memory;
    pool->max_fragment = _tralloc_new_pool_fragment ( pool->memory, length );
    pool->length       = length;
    pool->autofree     = TRALLOC_FALSE;
}

void _tralloc_alloc_from_pool ( _tralloc_pool * pool, void ** memory, size_t length, tralloc_bool zero, _tralloc_pool_child ** prev_pool_child, _tralloc_pool_child ** next_pool_child )
{
    _tralloc_pool_fragment * fragment = pool->max_fragment;
    * prev_pool_child = fragment->prev_child;
    * next_pool_child = fragment->next_child;

    * memory = ( void * ) _tralloc_alloc_from_pool_fragment ( pool, fragment, length );

    if ( zero ) {
        memset ( * memory, 0, length );
    }
}