// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "stats.h"
#include "chunk.h"


#if defined(TRALLOC_THREADS)
static pthread_mutex_t _chunks_count_mutex = PTHREAD_MUTEX_INITIALIZER;
#endif

static size_t _chunks_count = 0;

static inline
tralloc_error _add_chunks_count ( size_t length )
{

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_lock ( &_chunks_count_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    _chunks_count += length;

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_unlock ( &_chunks_count_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    return 0;
}

static inline
tralloc_error _subtract_chunks_count ( size_t length )
{

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_lock ( &_chunks_count_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    _chunks_count -= length;

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_unlock ( &_chunks_count_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    return 0;
}

tralloc_error tralloc_debug_stats_get_chunks_count ( size_t * length )
{

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_lock ( &_chunks_count_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    * length = _chunks_count;

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_unlock ( &_chunks_count_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    return 0;
}


#if defined(TRALLOC_THREADS)
static pthread_mutex_t _chunks_overhead_length_mutex = PTHREAD_MUTEX_INITIALIZER;
#endif

static size_t _chunks_overhead_length = 0;

tralloc_error _tralloc_debug_stats_add_chunks_overhead_length ( size_t length )
{

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_lock ( &_chunks_overhead_length_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    _chunks_overhead_length += length;

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_unlock ( &_chunks_overhead_length_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    return 0;
}

tralloc_error _tralloc_debug_stats_subtract_chunks_overhead_length ( size_t length )
{

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_lock ( &_chunks_overhead_length_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    _chunks_overhead_length -= length;

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_unlock ( &_chunks_overhead_length_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    return 0;
}

tralloc_error tralloc_debug_stats_get_chunks_overhead_length ( size_t * length )
{

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_lock ( &_chunks_overhead_length_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    * length = _chunks_overhead_length;

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_unlock ( &_chunks_overhead_length_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    return 0;
}


#if defined(TRALLOC_THREADS)
static pthread_mutex_t _chunks_length_mutex = PTHREAD_MUTEX_INITIALIZER;
#endif

static size_t _chunks_length = 0;

static inline
tralloc_error _add_chunks_length ( size_t length )
{

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_lock ( &_chunks_length_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    _chunks_length += length;

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_unlock ( &_chunks_length_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    return 0;
}

static inline
tralloc_error _subtract_chunks_length ( size_t length )
{

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_lock ( &_chunks_length_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    _chunks_length -= length;

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_unlock ( &_chunks_length_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    return 0;
}

tralloc_error tralloc_debug_stats_get_chunks_length ( size_t * length )
{

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_lock ( &_chunks_length_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    * length = _chunks_length;

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_unlock ( &_chunks_length_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    return 0;
}

tralloc_error _tralloc_debug_stats_after_add_chunk ( size_t chunk_length, size_t length )
{
    tralloc_error result;
    if (
        ( result = _add_chunks_count ( 1 ) ) != 0 ||
        ( result = _tralloc_debug_stats_add_chunks_overhead_length ( chunk_length ) ) != 0 ||
        ( result = _add_chunks_length ( length ) ) != 0
    ) {
        return result;
    }
    return 0;
}

tralloc_error _tralloc_debug_stats_after_resize_chunk ( size_t old_length, size_t length )
{
    if ( length > old_length ) {
        return _add_chunks_length ( length - old_length );
    } else if ( length < old_length ) {
        return _subtract_chunks_length ( old_length - length );
    }
    return 0;
}

tralloc_error _tralloc_debug_stats_before_free_chunk ( _tralloc_chunk * chunk )
{
    tralloc_error result;
    size_t length;
    if (
        ( result = _subtract_chunks_count ( 1 ) ) != 0 ||
        ( result = _tralloc_debug_stats_subtract_chunks_overhead_length ( chunk->chunk_length ) ) != 0 ||
        ( result = _tralloc_debug_get_length ( chunk, &length ) ) != 0 ||
        ( result = _subtract_chunks_length ( length ) ) != 0
    ) {
        return result;
    }
    return 0;
}