# Base EFM32 CMake file
#
# This can be used as is as a project base, or by adding the efm32-base
# repository as a submodule to another project, copying this CMakeLists file
# to the top level directory, and updating the BASE_LOCATION variable to reflect this
# change
#
## Copyright (c) 2016 Ryan Kurte
# This file is covered under the MIT license available at: https://opensource.org/licenses/MIT

# Set minimum CMake version
cmake_minimum_required(VERSION 3.16)

# Optional verbose mode, uncomment or pass in -DCMAKE_VERBOSE_MAKEFILE=ON
set(CMAKE_VERBOSE_MAKEFILE ON)

set(BASE_LOCATION .)

# Set the toolchain path
if (WIN32 OR MINGW)
    set(PLATFORM_WINDOWS 1)
    set(COMPILER_PREFIX "C:/Program Files (x86)/Arm GNU Toolchain arm-none-eabi/11.3 rel1/bin/")
elseif (UNIX)
    set(PLATFORM_UNIX 1)
    set(COMPILER_PREFIX "/usr/src/arm-gnu-toolchain-11.3.rel1-x86_64-arm-none-eabi/bin/")
else ()
    message(FATAL_ERROR "Unsupported platform")
endif ()

# Set the compiler (must be prior to project setup)
include(${BASE_LOCATION}/cmake/arm-gcc.cmake)

##### Project Setup #####
# Configure project and languages
project(efm32-template C CXX ASM)

# Set build
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif ()

if (CMAKE_BUILD_TYPE MATCHES Debug)
    message("Build type: Debug")

    # ${DEVICE} sets the target specific model name
    if (NOT DEVICE)
        set(DEVICE EFM32PG22C200F512IM32)  # Sets device / used to locate HAL files
        set(FLASH_ORIGIN 0x00000000)       # Used to specify application start address
    endif ()

    # Set preprocessor definitions
    add_compile_definitions(
            DEBUG
    )
else ()
    message("Build type: Release")

    # ${DEVICE} sets the target specific model name
    if (NOT DEVICE)
        set(DEVICE EFM32PG22C200F512IM32)  # Sets device / used to locate HAL files
        set(FLASH_ORIGIN 0x00000000)       # Used to specify application start address
    endif ()

    # Set preprocessor definitions
    add_compile_definitions(
            RELEASE
    )
endif ()

# Set linker script
set(LINKER_SCRIPT ${CMAKE_CURRENT_LIST_DIR}/cmake/linkerscript.ld)

# Set preprocessor definitions
add_compile_definitions(
        SL_TRUSTZONE_SECURE=1
        __STACK_SIZE=0x3000
)

##### Modules #####

# Libraries can be added to the LIBS variable
# or manually included here.

# Add base libs (emlib, CMSIS, device files)
include(${BASE_LOCATION}/cmake/efm32-base.cmake)

# Enable protocols / BLE if desired
# Note this will overwrite the default linker files / may not behave in the manner you expect
#set(USE_PROTOCOL 1)

# Common Microcontroller Software Interface Standard (CMSIS): A vendor-independent abstraction
# layer for microcontrollers that are based on Arm Cortex processors
add_subdirectory(${BASE_LOCATION}/gecko-sdk/platform/CMSIS)
# EMLIB is a low-level peripheral support library that provides a unified API for all EFM32,
# EZR32 and EFR32 MCUs and SoCs from Silicon Laboratories.
# EMLIB modules are provided for all peripherals and core features. The library implements no
# interrupt handlers. Static data, critical sections and module interdependencies are kept at
# a minimum.
add_subdirectory(${BASE_LOCATION}/gecko-sdk/platform/emlib)

# This is normally set in efm32-base.cmake, but libraries may modify it so set
# it after libraries/subdirectories have been added
set(CMAKE_EXE_LINKER_FLAGS "${COMMON_DEFINITIONS} --specs=nosys.specs -Xlinker -T${LINKER_SCRIPT} -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.map -Wl,--gc-sections -Wl,-v")

##### Files #####

# Add project headers
include_directories(
        ${BASE_LOCATION}/gecko-sdk/platform/common/inc
        ${BASE_LOCATION}/source/Device/EFM32PG22/Include
)

# Generate executable and link
add_executable(${PROJECT_NAME}
        # {{{ Device startup and headers
        source/Device/EFM32PG22/Source/GCC/startup_efm32pg22.S
        #source/Device/EFM32PG22/Source/startup_efm32pg22.c
        source/Device/EFM32PG22/Source/system_efm32pg22.c
        # }}}

        source/stubs.c
        source/main.cpp)

target_link_libraries(${PROJECT_NAME} ${LIBS} emlib cmsis)

##### Post build #####

# Add post build commands
include(${BASE_LOCATION}/cmake/post-build.cmake)

# Add JLink commands
include(${BASE_LOCATION}/cmake/jlink.cmake)

##### CMake debug prints #####
if (CMAKE_VERBOSE_MAKEFILE)
    print_debug_info()
endif ()
