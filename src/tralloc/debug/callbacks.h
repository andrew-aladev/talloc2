// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_DEBUG_CALLBACKS_H
#define TRALLOC_DEBUG_CALLBACKS_H

#include "../types.h"


typedef tralloc_error ( * tralloc_debug_callback_before_add ) ( void * data, _tralloc_chunk * parent_chunk, tralloc_extensions extensions, size_t chunk_length, size_t length );
typedef tralloc_error ( * tralloc_debug_callback_after_add )  ( void * data, _tralloc_chunk * chunk, size_t chunk_length, size_t length );

typedef tralloc_error ( * tralloc_debug_callback_before_resize ) ( void * data, _tralloc_chunk * chunk );
typedef tralloc_error ( * tralloc_debug_callback_after_resize )  ( void * data, _tralloc_chunk * chunk, size_t old_length );

typedef tralloc_error ( * tralloc_debug_callback_before_move ) ( void * data, _tralloc_chunk * chunk );
typedef tralloc_error ( * tralloc_debug_callback_after_move )  ( void * data, _tralloc_chunk * chunk, _tralloc_chunk * old_parent_chunk );

typedef tralloc_error ( * tralloc_debug_callback_before_free_subtree ) ( void * data, _tralloc_chunk * chunk );
typedef tralloc_error ( * tralloc_debug_callback_before_free_chunk )   ( void * data, _tralloc_chunk * chunk );

tralloc_error tralloc_debug_callback_set_add_data      ( void * before, void * after );
tralloc_error tralloc_debug_callback_set_add_functions ( tralloc_debug_callback_before_add before, tralloc_debug_callback_after_add after );
tralloc_error _tralloc_debug_callback_before_add_chunk ( _tralloc_chunk * parent_chunk, tralloc_extensions extensions, size_t chunk_length, size_t length );
tralloc_error _tralloc_debug_callback_after_add_chunk  ( _tralloc_chunk * chunk, size_t chunk_length, size_t length );

tralloc_error tralloc_debug_callback_set_resize_data      ( void * before, void * after );
tralloc_error tralloc_debug_callback_set_resize_functions ( tralloc_debug_callback_before_resize before, tralloc_debug_callback_after_resize after );
tralloc_error _tralloc_debug_callback_before_resize_chunk ( _tralloc_chunk * chunk );
tralloc_error _tralloc_debug_callback_after_resize_chunk  ( _tralloc_chunk * chunk, size_t old_length );

tralloc_error tralloc_debug_callback_set_move_data      ( void * before, void * after );
tralloc_error tralloc_debug_callback_set_move_functions ( tralloc_debug_callback_before_move before, tralloc_debug_callback_after_move after );
tralloc_error _tralloc_debug_callback_before_move_chunk ( _tralloc_chunk * chunk );
tralloc_error _tralloc_debug_callback_after_move_chunk  ( _tralloc_chunk * chunk, _tralloc_chunk * old_parent_chunk );

tralloc_error tralloc_debug_callback_set_free_data        ( void * before_subtree, void * before_chunk );
tralloc_error tralloc_debug_callback_set_free_functions   ( tralloc_debug_callback_before_free_subtree before_subtree, tralloc_debug_callback_before_free_chunk before_chunk );
tralloc_error _tralloc_debug_callback_before_free_subtree ( _tralloc_chunk * chunk );
tralloc_error _tralloc_debug_callback_before_free_chunk   ( _tralloc_chunk * chunk );


#endif