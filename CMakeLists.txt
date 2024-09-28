cmake_minimum_required(VERSION 3.20.0)

project(STM32_APP_SHELL VERSION 1.0.0)

enable_language(C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(LSM_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE FILEPATH "Project source directory" FORCE)
set(LSM_BINARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build" CACHE FILEPATH "Project binary directory" FORCE)

set(CMAKE_MODULE_PATH ${LSM_SOURCE_DIR}/CMake)

include(lsm_add_common_flags)
include(lsm_add_linker_flags)

message("Current toolchain location: " ${CMAKE_TOOLCHAIN_FILE})

################################################################################
#   Add common flags
################################################################################
lsm_add_common_flags()

################################################################################
#   Device config
################################################################################
# if(EXISTS ${CMAKE_SOURCE_DIR}/ProjectConfig/devconf.h)
#     add_compile_definitions(DEVCONF)
# endif()

# if(EXISTS ${CMAKE_SOURCE_DIR}/ProjectConfig/devconf.h)
#     file(READ ${CMAKE_SOURCE_DIR}/ProjectConfig/devconf.h DEVCONF_CONTENT)
# 
#     if(DEVCONF_CONTENT MATCHES "#define USE_FULL_LL_DRIVER")
#         add_compile_definitions(USE_FULL_LL_DRIVER)
#     endif()
#     
#     # Add more checks for other defines if needed
# endif()

# Automatically adds every define as a add_compile_definition variable from 
# devconf.h user-configurable file
if(EXISTS ${CMAKE_SOURCE_DIR}/ProjectConfig/devconf.h)
    # Read the content of devconf.h
    file(READ ${CMAKE_SOURCE_DIR}/ProjectConfig/devconf.h DEVCONF_CONTENT)

    # Use regex to find all #define statements and extract the macro names
    string(REGEX MATCHALL "#define ([A-Za-z_][A-Za-z0-9_]*)" DEFINE_MATCHES "${DEVCONF_CONTENT}")

    # Iterate over all matches and extract just the macro names
    foreach(DEFINE IN LISTS DEFINE_MATCHES)
        string(REGEX REPLACE "#define ([A-Za-z_][A-Za-z0-9_]*)" "\\1" DEFINE_NAME "${DEFINE}")
        list(APPEND COMPILE_DEFINITIONS_LIST ${DEFINE_NAME})
    endforeach()

    # Add all found defines as global compile definitions
    if(COMPILE_DEFINITIONS_LIST)
        add_compile_definitions(${COMPILE_DEFINITIONS_LIST})
    endif()
endif()


################################################################################
#   Add subdirectories
################################################################################

##  Device
add_subdirectory(Devices/${DEVICE} ${LSM_BINARY_DIR}/Devices/${DEVICE})
##  Libs

##  Drivers

##  Middleware

##  Messages


################################################################################
#   FreeRTOS Config
################################################################################
# Note: Add needed configuration bellow when porting FreeRTOS



################################################################################
#   Set app sources
################################################################################
set(PROJECT_SOURCES
    Devices/${DEVICE}/startup.c
    ${MAIN_REL_PATH}/main.c
)

################################################################################
#   Set executable
################################################################################
set(EXECUTABLE image.elf)

add_executable(${EXECUTABLE} ${PROJECT_SOURCES})

target_include_directories(${EXECUTABLE}
PUBLIC
    Core/App
)


################################################################################
#   Add linker flags
################################################################################
lsm_add_linker_flags()

################################################################################
#   Link libraries
################################################################################
target_link_libraries(${EXECUTABLE}
    ${DEVICE}

    # Libs

    # Drivers

    # Middleware

    # Messages

)

################################################################################
#   Creating .hex and .bin files from .elf image file
################################################################################
add_custom_command(TARGET ${EXECUTABLE}
    POST_BUILD
    COMMAND arm-none-eabi-objcopy -O ihex image.elf image.hex
    COMMAND arm-none-eabi-objcopy -O binary image.elf imaege.bin
)


