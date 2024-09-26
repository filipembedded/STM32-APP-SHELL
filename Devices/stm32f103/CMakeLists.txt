cmake_minimum_required(VERSION 3.0)

project(stm32f103 VERSION 1.0.0)

file(GLOB LL_DRIVER_SOURCES ll_drivers/*.c)
file(GLOB HL_SOURCES HL_drivers/*c)
file(GLOB CORE_SOURCES core/*.c)


set(SOURCES
    ${LL_DRIVER_SOURCES}
    ${HL_SOURCES}
    ${CORE_SOURCES}
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_include_directories(${PROJECT_NAME}
SYSTEM PUBLIC
    core
    ll_drivers
    HL_drivers
)
