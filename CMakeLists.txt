cmake_minimum_required(VERSION 3.10.0)
project(SDLShooter VERSION 0.1.0 LANGUAGES C CXX)

#设置编译选项
if(MSCV)
    add_compile_options(/W4)
elseif()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()
if(MSVC)
    add_compile_options(/utf-8)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif()

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

#设置编译输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR})

set(TARGET ${PROJECT_NAME}-${CMAKE_SYSTEM_NAME})

#查找SDL2库
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)

#设置可执行文件
add_executable(${TARGET} 
                "src/main.cpp"
                "src/Game.cpp"
                "src/Scene.cpp"
                "src/SceneTitle.cpp"
                "src/SceneMain.cpp"
                "src/SceneEnd.cpp"
                )

#链接SDL2库
target_link_libraries(${TARGET} 
                        ${SDL2_LIBRARIES}
                        SDL2_image::SDL2_image
                        SDL2_ttf::SDL2_ttf
                        SDL2_mixer::SDL2_mixer
                        )

#不要弹出窗口
if(MSVC)
    target_link_options(${TARGET} PRIVATE "/SUBSYSTEM:WINDOWS")
endif()