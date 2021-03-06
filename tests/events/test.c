// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "common.h"
#include <tralloc/tree.h>


int test_events ( tralloc_context * ctx )
{
    tralloc_context * root;
    if ( tralloc_new ( ctx, &root ) != 0 ) {
        return 1;
    }
    if ( !test_events_add ( root ) ) {
        tralloc_free ( root );
        return 2;
    }
    if ( !test_events_resize ( root ) ) {
        tralloc_free ( root );
        return 3;
    }
    if ( !test_events_move ( root ) ) {
        tralloc_free ( root );
        return 4;
    }
    if ( !test_events_free ( root ) ) {
        tralloc_free ( root );
        return 5;
    }
    if ( tralloc_free ( root ) != 0 ) {
        return 6;
    }
    return 0;
}
