Hierarchical memory allocator Tralloc
=====================================

Tralloc (tree alloc) is a hierarchical memory allocator with extensions. It is an alternative implementation of simplified `talloc`_ api.

The main goal of the project is to minimize memory overhead.
Size of chunk is 32-88 bytes on x86_64 system (depends on compiled and runtime features).
Debug features will increase size of each chunk by 16 bytes.


TODO
----
Thread safety.


Supported platforms
-------------------

GNU/Linux, GNU/Hurd and microcontrollers.


Debug build
-----------

    ::

     $ mkdir build && cd build
     $ cmake ..
     $ make VERBOSE=1 && make test


Documentation
-------------
Please read `man`_.
     
     
Extensions and features enable/disable
-------------------------
    
    ::
    
     $ cmake .. \
        -DTRALLOC_DEBUG=0        \
        -DTRALLOC_LENGTH=0       \
        -DTRALLOC_DESTRUCTOR=0   \
        -DTRALLOC_REFERENCE=0    \
        -DTRALLOC_POOL=0         \
        -DTRALLOC_UTILS_BUFFER=0 \
        -DTRALLOC_SHARED=0       \
        -DTRALLOC_STATIC=0       \
        -DTRALLOC_MAN=0          \
        -DTRALLOC_TEST=0         \
        -DTRALLOC_EXAMPLES=0

You can test all possible combinations of extensions and features : debug, length, destructor, reference, pool, utils_buffer

    ::
    
     $ ../combinations.sh


Multilib
--------

    ::
    
     $ CFLAGS="-m32" ../combinations.sh
     
     
Supported compilers
---------
    
    ::

     $ CC="gcc-3.4.6" ../combinations.sh
     $ CC="gcc-4.7.3" ../combinations.sh
     $ CC="gcc-4.8.2" ../combinations.sh
     $ NO_TESTS=1 ../combinations.sh -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/arduino.cmake -DTRALLOC_SHARED=0
     $ CC="tcc" ../combinations.sh -DTRALLOC_SHARED=0
     $ CC="clang" ../combinations.sh


Gentoo
------

Copy ebuilds from provided `gentoo`_ folder or add `overlay`_.


DEB and RPM
-----------

    ::
    
     $ cmake .. -DCMAKE_BUILD_TYPE=RELEASE
     $ make && make test && make package


LICENSE
-------
LGPL v3+ + SLE ( static link exception )


.. _talloc:  http://talloc.samba.org/talloc/doc/html/group__talloc.html
.. _man:     https://github.com/andrew-aladev/tralloc/blob/master/man/tralloc.txt
.. _overlay: https://github.com/andrew-aladev/puchuu-overlay
.. _gentoo:  https://github.com/andrew-aladev/tralloc/tree/master/gentoo
