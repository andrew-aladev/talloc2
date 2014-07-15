// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/debug/callbacks.h>

#if defined ( TRALLOC_THREADS )
#   include <tralloc/threads/lock/debug/callbacks.h>
#endif

#if defined ( TRALLOC_DEBUG_LOG )
#   include <stdio.h>
#endif


#if defined ( TRALLOC_THREADS )

#if TRALLOC_DEBUG_STATS_LOCK_TYPE == TRALLOC_THREADS_RWLOCK
static pthread_rwlock_t _tralloc_add_lock    = PTHREAD_RWLOCK_INITIALIZER;
static pthread_rwlock_t _tralloc_move_lock   = PTHREAD_RWLOCK_INITIALIZER;
static pthread_rwlock_t _tralloc_resize_lock = PTHREAD_RWLOCK_INITIALIZER;
static pthread_rwlock_t _tralloc_free_lock   = PTHREAD_RWLOCK_INITIALIZER;
#elif TRALLOC_DEBUG_STATS_LOCK_TYPE == TRALLOC_THREADS_MUTEX
static pthread_mutex_t _tralloc_add_lock    = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t _tralloc_move_lock   = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t _tralloc_resize_lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t _tralloc_free_lock   = PTHREAD_MUTEX_INITIALIZER;
#elif TRALLOC_DEBUG_STATS_LOCK_TYPE == TRALLOC_THREADS_SPINLOCK
static pthread_spinlock_t _tralloc_add_lock;
static pthread_spinlock_t _tralloc_move_lock;
static pthread_spinlock_t _tralloc_resize_lock;
static pthread_spinlock_t _tralloc_free_lock;

__attribute__ ( ( constructor ) )
void lock_constructor ()
{
    if (
        pthread_spin_init ( &_tralloc_add_lock, 0 )    != 0 ||
        pthread_spin_init ( &_tralloc_move_lock, 0 )   != 0 ||
        pthread_spin_init ( &_tralloc_resize_lock, 0 ) != 0 ||
        pthread_spin_init ( &_tralloc_free_lock, 0 )   != 0
    ) {
#       if defined ( TRALLOC_DEBUG_LOG )
        fprintf (
            stderr,
            "%s:%zu error: %s\n",
            __FILE__,
            __LINE__,
            "static spinlock was failed to be initialized"
        );
#       endif
        exit ( TRALLOC_ERROR_SPINLOCK_FAILED );
    }
}
__attribute__ ( ( destructor ) )
void lock_destructor ()
{
    if (
        pthread_spin_destroy ( &_tralloc_add_lock )    != 0 ||
        pthread_spin_destroy ( &_tralloc_move_lock )   != 0 ||
        pthread_spin_destroy ( &_tralloc_resize_lock ) != 0 ||
        pthread_spin_destroy ( &_tralloc_free_lock )   != 0
    ) {
#       if defined ( TRALLOC_DEBUG_LOG )
        fprintf (
            stderr,
            "%s:%zu error: %s\n",
            __FILE__,
            __LINE__,
            "static spinlock was failed to be destroyed"
        );
#       endif
        exit ( TRALLOC_ERROR_SPINLOCK_FAILED );
    }
}
#endif

#endif


static void * _tralloc_before_add_data = NULL;
static void * _tralloc_after_add_data  = NULL;
static tralloc_debug_callback_before_add _tralloc_before_add_callback = NULL;
static tralloc_debug_callback_after_add  _tralloc_after_add_callback  = NULL;

tralloc_error tralloc_debug_callback_set_add_data ( void * before_add, void * after_add )
{

#   if defined ( TRALLOC_THREADS )
    tralloc_error result = _tralloc_wrlock_debug_callbacks ( &_tralloc_add_lock );
    if ( result != 0 ) {
        return result;
    }
#   endif

    _tralloc_before_add_data = before_add;
    _tralloc_after_add_data  = after_add;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_unlock_debug_callbacks ( &_tralloc_add_lock );
    if ( result != 0 ) {
        return result;
    }
#   endif

    return 0;
}

tralloc_error tralloc_debug_callback_set_add_functions ( tralloc_debug_callback_before_add before_add, tralloc_debug_callback_after_add after_add )
{

#   if defined ( TRALLOC_THREADS )
    tralloc_error result = _tralloc_wrlock_debug_callbacks ( &_tralloc_add_lock );
    if ( result != 0 ) {
        return result;
    }
#   endif

    _tralloc_before_add_callback = before_add;
    _tralloc_after_add_callback  = after_add;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_unlock_debug_callbacks ( &_tralloc_add_lock );
    if ( result != 0 ) {
        return result;
    }
#   endif

    return 0;
}

tralloc_error _tralloc_debug_callback_before_add_chunk ( _tralloc_chunk * parent_chunk, tralloc_extensions extensions, size_t chunk_length, size_t length )
{

#   if defined ( TRALLOC_THREADS )
    tralloc_error result = _tralloc_rdlock_debug_callbacks ( &_tralloc_add_lock );
    if ( result != 0 ) {
        return result;
    }
#   endif

    void * data = _tralloc_before_add_data;
    tralloc_debug_callback_before_add function = _tralloc_before_add_callback;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_unlock_debug_callbacks ( &_tralloc_add_lock );
    if ( result != 0 ) {
        return result;
    }
#   endif

    if ( function != NULL ) {
        return function ( data, parent_chunk, extensions, chunk_length, length );
    }
    return 0;
}

tralloc_error _tralloc_debug_callback_after_add_chunk ( _tralloc_chunk * chunk, size_t chunk_length, size_t length )
{

#   if defined ( TRALLOC_THREADS )
    tralloc_error result = _tralloc_rdlock_debug_callbacks ( &_tralloc_add_lock );
    if ( result != 0 ) {
        return result;
    }
#   endif

    void * data = _tralloc_after_add_data;
    tralloc_debug_callback_after_add function = _tralloc_after_add_callback;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_unlock_debug_callbacks ( &_tralloc_add_lock );
    if ( result != 0 ) {
        return result;
    }
#   endif

    if ( function != NULL ) {
        return function ( data, chunk, chunk_length, length );
    }
    return 0;
}


static void * _tralloc_before_move_data = NULL;
static void * _tralloc_after_move_data  = NULL;
static tralloc_debug_callback_before_move _tralloc_before_move_callback = NULL;
static tralloc_debug_callback_after_move  _tralloc_after_move_callback  = NULL;

tralloc_error tralloc_debug_callback_set_move_data ( void * before_move, void * after_move )
{

#   if defined ( TRALLOC_THREADS )
    tralloc_error result = _tralloc_wrlock_debug_callbacks ( &_tralloc_move_lock );
    if ( result != 0 ) {
        return result;
    }
#   endif

    _tralloc_before_move_data = before_move;
    _tralloc_after_move_data  = after_move;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_unlock_debug_callbacks ( &_tralloc_move_lock );
    if ( result != 0 ) {
        return result;
    }
#   endif

    return 0;
}

tralloc_error tralloc_debug_callback_set_move_functions ( tralloc_debug_callback_before_move before_move, tralloc_debug_callback_after_move after_move )
{

#   if defined ( TRALLOC_THREADS )
    tralloc_error result = _tralloc_wrlock_debug_callbacks ( &_tralloc_move_lock );
    if ( result != 0 ) {
        return result;
    }
#   endif

    _tralloc_before_move_callback = before_move;
    _tralloc_after_move_callback  = after_move;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_unlock_debug_callbacks ( &_tralloc_move_lock );
    if ( result != 0 ) {
        return result;
    }
#   endif

    return 0;
}

tralloc_error _tralloc_debug_callback_before_move_chunk ( _tralloc_chunk * chunk )
{

#   if defined ( TRALLOC_THREADS )
    tralloc_error result = _tralloc_rdlock_debug_callbacks ( &_tralloc_move_lock );
    if ( result != 0 ) {
        return result;
    }
#   endif

    void * data = _tralloc_before_move_data;
    tralloc_debug_callback_before_move function = _tralloc_before_move_callback;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_unlock_debug_callbacks ( &_tralloc_move_lock );
    if ( result != 0 ) {
        return result;
    }
#   endif

    if ( function != NULL ) {
        return function ( data, chunk );
    }
    return 0;
}

tralloc_error _tralloc_debug_callback_after_move_chunk ( _tralloc_chunk * chunk, _tralloc_chunk * old_parent_chunk )
{

#   if defined ( TRALLOC_THREADS )
    tralloc_error result = _tralloc_rdlock_debug_callbacks ( &_tralloc_move_lock );
    if ( result != 0 ) {
        return result;
    }
#   endif

    void * data = _tralloc_after_move_data;
    tralloc_debug_callback_after_move function = _tralloc_after_move_callback;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_unlock_debug_callbacks ( &_tralloc_move_lock );
    if ( result != 0 ) {
        return result;
    }
#   endif

    if ( function != NULL ) {
        return function ( data, chunk, old_parent_chunk );
    }
    return 0;
}


static void * _tralloc_before_resize_data = NULL;
static void * _tralloc_after_resize_data  = NULL;
static tralloc_debug_callback_before_resize _tralloc_before_resize_callback = NULL;
static tralloc_debug_callback_after_resize  _tralloc_after_resize_callback  = NULL;

tralloc_error tralloc_debug_callback_set_resize_data ( void * before_resize, void * after_resize )
{

#   if defined ( TRALLOC_THREADS )
    tralloc_error result = _tralloc_wrlock_debug_callbacks ( &_tralloc_resize_lock );
    if ( result != 0 ) {
        return result;
    }
#   endif

    _tralloc_before_resize_data = before_resize;
    _tralloc_after_resize_data  = after_resize;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_unlock_debug_callbacks ( &_tralloc_resize_lock );
    if ( result != 0 ) {
        return result;
    }
#   endif

    return 0;
}

tralloc_error tralloc_debug_callback_set_resize_functions ( tralloc_debug_callback_before_resize before_resize, tralloc_debug_callback_after_resize after_resize )
{

#   if defined ( TRALLOC_THREADS )
    tralloc_error result = _tralloc_wrlock_debug_callbacks ( &_tralloc_resize_lock );
    if ( result != 0 ) {
        return result;
    }
#   endif

    _tralloc_before_resize_callback = before_resize;
    _tralloc_after_resize_callback  = after_resize;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_unlock_debug_callbacks ( &_tralloc_resize_lock );
    if ( result != 0 ) {
        return result;
    }
#   endif

    return 0;
}

tralloc_error _tralloc_debug_callback_before_resize_chunk ( _tralloc_chunk * chunk )
{

#   if defined ( TRALLOC_THREADS )
    tralloc_error result = _tralloc_rdlock_debug_callbacks ( &_tralloc_resize_lock );
    if ( result != 0 ) {
        return result;
    }
#   endif

    void * data = _tralloc_before_resize_data;
    tralloc_debug_callback_before_resize function = _tralloc_before_resize_callback;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_unlock_debug_callbacks ( &_tralloc_resize_lock );
    if ( result != 0 ) {
        return result;
    }
#   endif

    if ( function != NULL ) {
        return function ( data, chunk );
    }
    return 0;
}

tralloc_error _tralloc_debug_callback_after_resize_chunk ( _tralloc_chunk * chunk, size_t old_length )
{

#   if defined ( TRALLOC_THREADS )
    tralloc_error result = _tralloc_rdlock_debug_callbacks ( &_tralloc_resize_lock );
    if ( result != 0 ) {
        return result;
    }
#   endif

    void * data = _tralloc_after_resize_data;
    tralloc_debug_callback_after_resize function = _tralloc_after_resize_callback;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_unlock_debug_callbacks ( &_tralloc_resize_lock );
    if ( result != 0 ) {
        return result;
    }
#   endif

    if ( function != NULL ) {
        return function ( data, chunk, old_length );
    }
    return 0;
}


static void * _tralloc_before_free_subtree_data           = NULL;
static void * _tralloc_before_free_chunk_data             = NULL;
static void * _tralloc_before_refuse_to_free_subtree_data = NULL;
static void * _tralloc_after_refuse_to_free_subtree_data  = NULL;
static void * _tralloc_before_refuse_to_free_chunk_data   = NULL;
static void * _tralloc_after_refuse_to_free_chunk_data    = NULL;

static tralloc_debug_callback_before_free_subtree           _tralloc_before_free_subtree_callback           = NULL;
static tralloc_debug_callback_before_free_chunk             _tralloc_before_free_chunk_callback             = NULL;
static tralloc_debug_callback_before_refuse_to_free_subtree _tralloc_before_refuse_to_free_subtree_callback = NULL;
static tralloc_debug_callback_after_refuse_to_free_subtree  _tralloc_after_refuse_to_free_subtree_callback  = NULL;
static tralloc_debug_callback_before_refuse_to_free_chunk   _tralloc_before_refuse_to_free_chunk_callback   = NULL;
static tralloc_debug_callback_after_refuse_to_free_chunk    _tralloc_after_refuse_to_free_chunk_callback    = NULL;

tralloc_error tralloc_debug_callback_set_free_data (
    void * before_free_subtree,
    void * before_free_chunk,
    void * before_refuse_to_free_subtree,
    void * after_refuse_to_free_subtree,
    void * before_refuse_to_free_chunk,
    void * after_refuse_to_free_chunk
)
{
    tralloc_error error = 0;

#   if defined ( TRALLOC_THREADS )
    tralloc_error result = _tralloc_wrlock_debug_callbacks ( &_tralloc_free_lock );
    if ( result != 0 ) {
        error = result;
    }
#   endif

    _tralloc_before_free_subtree_data           = before_free_subtree;
    _tralloc_before_free_chunk_data             = before_free_chunk;
    _tralloc_before_refuse_to_free_subtree_data = before_refuse_to_free_subtree;
    _tralloc_after_refuse_to_free_subtree_data  = after_refuse_to_free_subtree;
    _tralloc_before_refuse_to_free_chunk_data   = before_refuse_to_free_chunk;
    _tralloc_after_refuse_to_free_chunk_data    = after_refuse_to_free_chunk;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_unlock_debug_callbacks ( &_tralloc_free_lock );
    if ( result != 0 ) {
        error = result;
    }
#   endif

    return error;
}

tralloc_error tralloc_debug_callback_set_free_functions (
    tralloc_debug_callback_before_free_subtree           before_free_subtree,
    tralloc_debug_callback_before_free_chunk             before_free_chunk,
    tralloc_debug_callback_before_refuse_to_free_subtree before_refuse_to_free_subtree,
    tralloc_debug_callback_after_refuse_to_free_subtree  after_refuse_to_free_subtree,
    tralloc_debug_callback_before_refuse_to_free_chunk   before_refuse_to_free_chunk,
    tralloc_debug_callback_after_refuse_to_free_chunk    after_refuse_to_free_chunk
)
{
    tralloc_error error = 0;

#   if defined ( TRALLOC_THREADS )
    tralloc_error result = _tralloc_wrlock_debug_callbacks ( &_tralloc_free_lock );
    if ( result != 0 ) {
        error = result;
    }
#   endif

    _tralloc_before_free_subtree_callback           = before_free_subtree;
    _tralloc_before_free_chunk_callback             = before_free_chunk;
    _tralloc_before_refuse_to_free_subtree_callback = before_refuse_to_free_subtree;
    _tralloc_after_refuse_to_free_subtree_callback  = after_refuse_to_free_subtree;
    _tralloc_before_refuse_to_free_chunk_callback   = before_refuse_to_free_chunk;
    _tralloc_after_refuse_to_free_chunk_callback    = after_refuse_to_free_chunk;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_unlock_debug_callbacks ( &_tralloc_free_lock );
    if ( result != 0 ) {
        error = result;
    }
#   endif

    return error;
}

tralloc_error _tralloc_debug_callback_before_free_subtree ( _tralloc_chunk * chunk )
{
    tralloc_error error = 0, result;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_rdlock_debug_callbacks ( &_tralloc_free_lock );
    if ( result != 0 ) {
        error = result;
    }
#   endif

    void * data = _tralloc_before_free_subtree_data;
    tralloc_debug_callback_before_free_subtree function = _tralloc_before_free_subtree_callback;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_unlock_debug_callbacks ( &_tralloc_free_lock );
    if ( result != 0 ) {
        error = result;
    }
#   endif

    if ( function != NULL ) {
        result = function ( data, chunk );
        if ( result != 0 ) {
            error = result;
        }
    }
    return error;
}

tralloc_error _tralloc_debug_callback_before_free_chunk ( _tralloc_chunk * chunk )
{
    tralloc_error error = 0, result;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_rdlock_debug_callbacks ( &_tralloc_free_lock );
    if ( result != 0 ) {
        error = result;
    }
#   endif

    void * data = _tralloc_before_free_chunk_data;
    tralloc_debug_callback_before_free_chunk function = _tralloc_before_free_chunk_callback;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_unlock_debug_callbacks ( &_tralloc_free_lock );
    if ( result != 0 ) {
        error = result;
    }
#   endif

    if ( function != NULL ) {
        result = function ( data, chunk );
        if ( result != 0 ) {
            error = result;
        }
    }
    return error;
}

tralloc_error _tralloc_debug_callback_before_refuse_to_free_subtree ( _tralloc_chunk * chunk )
{
    tralloc_error error = 0, result;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_rdlock_debug_callbacks ( &_tralloc_free_lock );
    if ( result != 0 ) {
        error = result;
    }
#   endif

    void * data = _tralloc_before_refuse_to_free_subtree_data;
    tralloc_debug_callback_before_refuse_to_free_subtree function = _tralloc_before_refuse_to_free_subtree_callback;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_unlock_debug_callbacks ( &_tralloc_free_lock );
    if ( result != 0 ) {
        error = result;
    }
#   endif

    if ( function != NULL ) {
        result = function ( data, chunk );
        if ( result != 0 ) {
            error = result;
        }
    }
    return error;
}

tralloc_error _tralloc_debug_callback_after_refuse_to_free_subtree ( _tralloc_chunk * chunk )
{
    tralloc_error error = 0, result;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_rdlock_debug_callbacks ( &_tralloc_free_lock );
    if ( result != 0 ) {
        error = result;
    }
#   endif

    void * data = _tralloc_after_refuse_to_free_subtree_data;
    tralloc_debug_callback_after_refuse_to_free_subtree function = _tralloc_after_refuse_to_free_subtree_callback;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_unlock_debug_callbacks ( &_tralloc_free_lock );
    if ( result != 0 ) {
        error = result;
    }
#   endif

    if ( function != NULL ) {
        result = function ( data, chunk );
        if ( result != 0 ) {
            error = result;
        }
    }
    return error;
}

tralloc_error _tralloc_debug_callback_before_refuse_to_free_chunk ( _tralloc_chunk * chunk )
{
    tralloc_error error = 0, result;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_rdlock_debug_callbacks ( &_tralloc_free_lock );
    if ( result != 0 ) {
        error = result;
    }
#   endif

    void * data = _tralloc_before_refuse_to_free_chunk_data;
    tralloc_debug_callback_before_refuse_to_free_chunk function = _tralloc_before_refuse_to_free_chunk_callback;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_unlock_debug_callbacks ( &_tralloc_free_lock );
    if ( result != 0 ) {
        error = result;
    }
#   endif

    if ( function != NULL ) {
        result = function ( data, chunk );
        if ( result != 0 ) {
            error = result;
        }
    }
    return error;
}

tralloc_error _tralloc_debug_callback_after_refuse_to_free_chunk ( _tralloc_chunk * chunk )
{
    tralloc_error error = 0, result;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_rdlock_debug_callbacks ( &_tralloc_free_lock );
    if ( result != 0 ) {
        error = result;
    }
#   endif

    void * data = _tralloc_after_refuse_to_free_chunk_data;
    tralloc_debug_callback_after_refuse_to_free_chunk function = _tralloc_after_refuse_to_free_chunk_callback;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_unlock_debug_callbacks ( &_tralloc_free_lock );
    if ( result != 0 ) {
        error = result;
    }
#   endif

    if ( function != NULL ) {
        result = function ( data, chunk );
        if ( result != 0 ) {
            error = result;
        }
    }
    return error;
}
