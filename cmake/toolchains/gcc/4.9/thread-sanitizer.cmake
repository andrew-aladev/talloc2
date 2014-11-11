set (CMAKE_SYSTEM_NAME "Linux")
set (CMAKE_C_COMPILER "gcc-4.9.2")
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIE -Werror -Wno-maybe-uninitialized -fsanitize=thread -fno-omit-frame-pointer" CACHE STRING "gcc-4.9.2 thread cflags")
set (CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS}    -pie" CACHE STRING "gcc-4.9.2 thread exe ldflags")
set (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pie" CACHE STRING "gcc-4.9.2 thread shared ldflags")
set (TRALLOC_HAVE_LTO false CACHE STRING "Status of LTO support")
