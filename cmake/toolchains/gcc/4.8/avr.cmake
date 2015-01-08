set (CMAKE_SYSTEM_NAME "Generic")
set (CMAKE_SYSTEM_PROCESSOR "avr")
set (CMAKE_C_COMPILER "avr-gcc-4.8.4")
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -Wno-maybe-uninitialized -mmcu=atmega328 -DF_CPU=16000000UL -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums" CACHE STRING "avr-gcc-4.8.4 cflags")
set (TRALLOC_SHARED false CACHE STRING "build shared libs")
