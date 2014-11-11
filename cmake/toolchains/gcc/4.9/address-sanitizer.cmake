set (CMAKE_SYSTEM_NAME "Linux")
set (CMAKE_C_COMPILER "gcc-4.9.2")
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -Wno-maybe-uninitialized -fsanitize=address -fno-omit-frame-pointer" CACHE STRING "gcc-4.9.2 address cflags")
set (TRALLOC_HAVE_LTO false CACHE STRING "Status of LTO support")
