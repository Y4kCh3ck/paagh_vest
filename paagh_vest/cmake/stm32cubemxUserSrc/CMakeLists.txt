cmake_minimum_required(VERSION 3.22)

project(stm32cubemxUserSrc)
add_library(stm32cubemxUserSrc USER_INTERFACE)

# Enable CMake support for ASM and C languages
enable_language(C ASM)

target_compile_definitions(stm32cubemxUserSrc USER_INTERFACE 
	USE_HAL_DRIVER 
	STM32L071xx
    $<$<CONFIG:Debug>:DEBUG>
)

target_include_directories(stm32cubemxUserSrc USER_INTERFACE
    ../../Core/Inc
)

target_sources(stm32cubemxUserSrc USER_INTERFACE
    ../../Core/Src/AHT20.c
	../../Core/Src/NEO6.c
	../../Core/Src/nRF24.c
)

target_link_directories(stm32cubemxUserSrc USER_INTERFACE
)

target_link_libraries(stm32cubemxUserSrc USER_INTERFACE
)

# Validate that stm32cubemxUserSrc code is compatible with C standard
if(CMAKE_C_STANDARD LESS 11)
    message(ERROR "Generated code requires C11 or higher")
endif()


