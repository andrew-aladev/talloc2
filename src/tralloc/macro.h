// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#if !defined ( TRALLOC_MACRO_H )
#define TRALLOC_MACRO_H

#include "config.h"


#if defined ( TRALLOC_HAVE_C99_INLINE )
#    define _TRALLOC_INLINE_IN_HEADER inline
#    define _TRALLOC_INLINE_IN_OBJECT
#else
#    define _TRALLOC_INLINE_IN_HEADER extern inline
#    define _TRALLOC_INLINE_IN_OBJECT
#endif

#if defined ( __GNUC__ )
#    define _TRALLOC_UNUSED(x) x __attribute__((__unused__))
#    define _TRALLOC_UNUSED_FUNCTION(x) __attribute__((__unused__)) x
#else
#    define _TRALLOC_UNUSED(x) x
#    define _TRALLOC_UNUSED_FUNCTION(x) x
#endif


#endif
