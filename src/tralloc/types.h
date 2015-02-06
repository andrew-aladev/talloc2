// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#if !defined ( TRALLOC_TYPES_H )
#define TRALLOC_TYPES_H

#include <stdint.h>
#include <stddef.h>

#include "config.h"

#if defined ( TRALLOC_THREADS )
#   include <pthread.h>
#endif


#if defined ( TRALLOC_HAVE_BOOL )
#    include <stdbool.h>
typedef bool tralloc_bool;
#    define TRALLOC_TRUE true
#    define TRALLOC_FALSE false
#else
typedef char tralloc_bool;
#    define TRALLOC_TRUE 1
#    define TRALLOC_FALSE 0
#endif

typedef void tralloc_context;

enum {

    TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_UNDEFINED = 1,

    TRALLOC_ERROR_MALLOC_FAILED,
    TRALLOC_ERROR_CALLOC_FAILED,
    TRALLOC_ERROR_REALLOC_FAILED,
    TRALLOC_ERROR_OPEN_DESCRIPTOR_FAILED,
    TRALLOC_ERROR_CLOSE_DESCRIPTOR_FAILED,
    TRALLOC_ERROR_PRINTF_FAILED,

    TRALLOC_ERROR_CHILD_EQUALS_PARENT,
    TRALLOC_ERROR_CHILD_HAS_SAME_PARENT,

#   if defined ( TRALLOC_EXTENSIONS )
    TRALLOC_ERROR_NO_SUCH_EXTENSION,
#   endif

#   if defined ( TRALLOC_POOL )
    TRALLOC_ERROR_POOL_CANT_BE_REALLOCATED,
#   endif

#   if defined ( TRALLOC_UTILS_BUFFER )
    TRALLOC_ERROR_UTILS_BUFFER_OVERFLOW,
#   endif

#   if defined ( TRALLOC_THREADS )
    TRALLOC_ERROR_RWLOCK_FAILED,
    TRALLOC_ERROR_MUTEX_FAILED,
    TRALLOC_ERROR_SPINLOCK_FAILED,

    TRALLOC_ERROR_NO_CHILDREN_LOCK,
    TRALLOC_ERROR_NO_SUBTREE_LOCK,

#   if defined ( TRALLOC_POOL )
    TRALLOC_ERROR_NO_POOL_LOCK,
#   endif

#   endif

};
typedef uint8_t tralloc_error;

#if defined ( TRALLOC_THREADS )

typedef struct _tralloc_debug_threads_lock_type {

#if TRALLOC_DEBUG_THREADS_LOCK_TYPE == TRALLOC_THREADS_RWLOCK
    pthread_rwlock_t data;
#elif TRALLOC_DEBUG_THREADS_LOCK_TYPE == TRALLOC_THREADS_MUTEX
    pthread_mutex_t data;
#elif TRALLOC_DEBUG_THREADS_LOCK_TYPE == TRALLOC_THREADS_SPINLOCK
    pthread_spinlock_t data;
#endif

} _tralloc_debug_threads_lock;

#endif

#if defined ( TRALLOC_DEBUG_THREADS )
enum {
    _TRALLOC_DEBUG_NOT_USED_BY_THREADS = 0,
    _TRALLOC_DEBUG_USED_BY_SINGLE_THREAD,
    _TRALLOC_DEBUG_USED_BY_MULTIPLE_THREADS
};

typedef uint8_t _tralloc_thread_usage_status;
#endif

#if defined ( TRALLOC_EXTENSIONS )
enum {

#   if defined ( TRALLOC_LENGTH )
    TRALLOC_EXTENSION_LENGTH = 1,
#   endif

#   if defined ( TRALLOC_DESTRUCTORS )
    TRALLOC_EXTENSION_DESTRUCTORS = 1 << 1,
#   endif

#   if defined ( TRALLOC_REFERENCES )
    TRALLOC_EXTENSION_REFERENCE  = 1 << 2,
    TRALLOC_EXTENSION_REFERENCES = 1 << 3,
#   endif

#   if defined ( TRALLOC_POOL )
    TRALLOC_EXTENSION_POOL       = 1 << 4,
    TRALLOC_EXTENSION_POOL_CHILD = 1 << 5,
#   endif

#   if defined ( TRALLOC_THREADS )
    TRALLOC_EXTENSION_LOCK_SUBTREE  = 1 << 6,
    TRALLOC_EXTENSION_LOCK_CHILDREN = 1 << 7,

#   if defined ( TRALLOC_POOL )
    TRALLOC_EXTENSION_LOCK_POOL = 1 << 8,
#   endif

#   endif

};
typedef uint16_t _tralloc_extension;
typedef uint16_t tralloc_extensions;
#endif

typedef struct _tralloc_chunk_type {
    // "parent", "prev", "next", "first_child" should be locked for thread safety.
    struct _tralloc_chunk_type * parent;
    struct _tralloc_chunk_type * prev;
    struct _tralloc_chunk_type * next;
    struct _tralloc_chunk_type * first_child;

#   if defined ( TRALLOC_EXTENSIONS )
    // "extensions" and "forced_extensions" should not be locked for thread safety.
    // It will be written only in alloc function. Other functions will read it.
    tralloc_extensions extensions;
#   endif

#   if defined ( TRALLOC_DEBUG_EXTENSIONS )
    // Some extensions can be forced.
    // If bit is 1 - extension value is forced, otherwise extension equals original extension.
    // So original extensions can be found by "extensions" ^ "forced_extensions".

    // If parent is pool or pool child - pool child will be enabled, otherwise it will be disabled.
    // Pool child will be disabled if related pool can't alloc it.
    // Pool lock will be disabled if pool is not enabled.
    // Subtree, children and pool locks will be enabled if TRALLOC_DEBUG_THREADS is defined.
    tralloc_extensions forced_extensions;
#   endif

#   if defined ( TRALLOC_DEBUG_LOG )
    // "initialized_in_file" and "initialized_at_line" should not be locked for thread safety.
    // It will be written only in alloc function. Other functions will read it.

    char * initialized_in_file;
    size_t initialized_at_line;
#   endif

#   if defined ( TRALLOC_DEBUG_THREADS )
    // "subtree_used_by_thread",  "subtree_usage_status",
    // "children_used_by_thread", "children_usage_status",
    // "pool_used_by_thread",     "pool_usage_status"
    // will be locked for thread safety by "thread_usage_lock".

    pthread_t subtree_used_by_thread;
    _tralloc_thread_usage_status subtree_usage_status;

    pthread_t children_used_by_thread;
    _tralloc_thread_usage_status children_usage_status;

#   if defined ( TRALLOC_POOL )
    pthread_t pool_used_by_thread;
    _tralloc_thread_usage_status pool_usage_status;
#   endif

    _tralloc_debug_threads_lock thread_usage_lock;
#   endif

} _tralloc_chunk;

// It is important to provide all necessary information about chunk when it has been prepaired to be created.
typedef struct _tralloc_chunk_prototype_type {
    _tralloc_chunk * parent;
    size_t length;

#   if defined ( TRALLOC_EXTENSIONS )
    tralloc_extensions extensions;
#   endif

#   if defined ( TRALLOC_DEBUG_EXTENSIONS )
    tralloc_extensions forced_extensions;
#   endif

#   if defined ( TRALLOC_DEBUG_LOG )
    const char * initialized_in_file;
    size_t initialized_at_line;
#   endif

} _tralloc_chunk_prototype;


#endif
