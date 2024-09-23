cmake_minimum_required(VERSION 3.20.0)

project(STM32_APP_SHELL VERSION 1.0.0)

enable_language(C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(LSM_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE FILEPATH "Project source directory" FORCE)
set(LSM_BINARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE FILEPATH "Project source directory" FORCE)

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
if(EXISTS ${CMAKE_SOURCE_DIR}/ProjectConfig/devconf.h)
    add_compile_definitions(DEVCONF)
endif()

################################################################################
#   Add subdirectories
################################################################################

##  Device

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
    Devices/${DEVICE}/startup.S
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


