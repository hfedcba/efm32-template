project(platform_common)

include_directories(${PROJECT_NAME} ../Device/SiliconLabs/${CPU_FAMILY_U}/Include ../CMSIS/Core/Include config inc)

# find . -name '*.c' | sort -u
add_library(${PROJECT_NAME}
        src/sl_slist.c)

target_include_directories(${PROJECT_NAME} INTERFACE
        config
        inc)