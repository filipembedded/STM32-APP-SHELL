set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)



################################################################################
#   Setting UTIL_SEARCH_COMMAND to search for toolchain executables
################################################################################
if(MINGW OR CYGWIN OR WIN32)
    set(UTIL_SEARCH_CMD where)
elseif(UNIX OR APPLE)
    set(UTIL_SEARCH_CMD which)
endif()


################################################################################
#   Setting toolchain prefix
################################################################################
set(TOOLCHAIN_PREFIX arm-none-eabi-)


################################################################################
#   Searh for toolchain
################################################################################
execute_process(
    COMMAND ${UTIL_SEARCH_CMD} ${TOOLCHAIN_PREFIX}gcc
    OUTPUT_VARIABLE BINUTILS_PATH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

################################################################################
# NOTE: Check what this part is doing  
################################################################################
get_filename_component(ARM_TOOLCHAIN_DIR ${BINUTILS_PATH} DIRECTORY)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

################################################################################
#   Execute make
################################################################################
if(WIN32)
    set(CMAKE_MAKE_PROGRAM "mingw32-make.exe" CACHE string "Make program")
else()
    set(CMAKE_MAKE_PROGRAM "make" CACHE STRING "Make program")
endif()

################################################################################
#   Compiler settings
################################################################################
set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}g++)

################################################################################
#   Objcopy tool
################################################################################
set(CMAKE_OBJCOPY ${ARM_TOOLCHAIN_DIR}/${TOOLCHAIN_PREFIX}objcopy CACHE INTERNAL "objcopy tool")

################################################################################
#   Size calculation tool
################################################################################
set(CMAKE_SIZE_UTIL ${ARM_TOOLCHAIN_DIR}/${TOOLCHAIN_PREFIX}size CACHE INTERNAL "size tool")

################################################################################
#   NOTE: Check what this part does
################################################################################
set(CMAKE_FIND_ROOT_PATH ${BINUTILS_PATH})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)