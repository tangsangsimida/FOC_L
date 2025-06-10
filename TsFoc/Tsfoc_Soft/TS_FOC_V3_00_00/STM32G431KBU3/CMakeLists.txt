cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME STM32G431KBU3)

# Include toolchain file
include("cmake/gcc-arm-none-eabi.cmake")

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C CXX ASM)          # 👈 开启C, C++和ASM语言支持

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)
add_subdirectory(user)              # 👈 添加user中的库文件

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx
    user_objects         # 👈 添加自己的cmakelists库文件

    # Add user defined libraries
)


# 👈添加printf_float重定向
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -u _printf_float")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -u _printf_float")

# 👈添加bin和hex文件生成规则

# 定义输出文件名
set(ELF_FILE ${CMAKE_PROJECT_NAME}.elf)
set(HEX_FILE ${CMAKE_PROJECT_NAME}.hex)
set(BIN_FILE ${CMAKE_PROJECT_NAME}.bin)

# 确保 objcopy 存在
if(NOT CMAKE_OBJCOPY)
    message(FATAL_ERROR "CMAKE_OBJCOPY not found. Please check your toolchain file.")
endif()

# 生成 HEX 文件
add_custom_command(
    TARGET ${CMAKE_PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${HEX_FILE}
    COMMENT "Generating ${HEX_FILE}"
)

# 生成 BIN 文件
add_custom_command(
    TARGET ${CMAKE_PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${BIN_FILE}
    COMMENT "Generating ${BIN_FILE}"
)

# 添加清理规则
set_directory_properties(PROPERTIES
    ADDITIONAL_MAKE_CLEAN_FILES "${HEX_FILE};${BIN_FILE}"
)
