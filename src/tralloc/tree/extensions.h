// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_TREE_EXTENSIONS_H
#define TRALLOC_TREE_EXTENSIONS_H

#include "../common.h"


tralloc_error tralloc_with_extensions ( tralloc_context * parent_context, tralloc_context ** child_context, tralloc_extensions extensions, size_t length );

inline
tralloc_error tralloc ( tralloc_context * parent_context, tralloc_context ** child_context, size_t length )
{
    return tralloc_with_extensions ( parent_context, child_context, 0, length );
}

tralloc_error tralloc_zero_with_extensions ( tralloc_context * parent_context, tralloc_context ** child_context, tralloc_extensions extensions, size_t length );

inline
tralloc_error tralloc_zero ( tralloc_context * parent_context, tralloc_context ** child_context, size_t length )
{
    return tralloc_zero_with_extensions ( parent_context, child_context, 0, length );
}


inline
tralloc_error tralloc_new ( tralloc_context * parent_context, tralloc_context ** child_context )
{
    return tralloc ( parent_context, child_context, 0 );
}

inline
tralloc_error tralloc_with_extensions_new ( tralloc_context * parent_context, tralloc_context ** child_context, tralloc_extensions extensions )
{
    return tralloc_with_extensions ( parent_context, child_context, extensions, 0 );
}


#endif