
# 👈用户头文件路径 
set(USER_Include_Dirs
    ${CMAKE_CURRENT_SOURCE_DIR}/Application/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Peripherals/inc
    $<TARGET_PROPERTY:stm32cubemx,INTERFACE_INCLUDE_DIRECTORIES>  # ✅ 自动从 CubeMX 获取头文件路径
)

# 👈自动收集 Application/Src 下的所有 .c 源文件
file(GLOB_RECURSE USER_Application_Src
    "${CMAKE_CURRENT_SOURCE_DIR}/Application/src/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Application/src/*.cpp"
)

# 👈自动收集 Peripherals/Src 下的所有 .c 源文件
file(GLOB_RECURSE USER_Peripherals_Src
    "${CMAKE_CURRENT_SOURCE_DIR}/Peripherals/src/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Peripherals/src/*.cpp"
)

# 👈合并所有用户源文件
set(USER_Sources
    ${USER_Application_Src}
    ${USER_Peripherals_Src}
)

# 👈接口库：用于共享头文件和宏定义
add_library(user_interface INTERFACE)
target_include_directories(user_interface INTERFACE
    ${USER_Include_Dirs}
)


# 👈对象库：包含实际编译的用户代码
add_library(user_objects OBJECT)
target_sources(user_objects PRIVATE
    ${USER_Sources}
)

target_link_libraries(user_objects PUBLIC user_interface stm32cubemx STM32_Drivers)

# 👈将变量暴露给父级 CMakeLists.txt
set(USER_Sources ${USER_Sources} PARENT_SCOPE)
set(USER_Include_Dirs ${USER_Include_Dirs} PARENT_SCOPE)
