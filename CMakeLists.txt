cmake_minimum_required(VERSION 3.23)
project(Kiero_DX9_ImGui_Base)

set(CMAKE_CXX_STANDARD 20)


include_directories(
        "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include"
)

link_directories(
        "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x86"
)

file(GLOB IMGUI
        ${PROJECT_SOURCE_DIR}/external/imgui/*.cpp
        ${PROJECT_SOURCE_DIR}/external/imgui/*.h
        )

file(GLOB KIERO
        ${PROJECT_SOURCE_DIR}/external/kiero/*.cpp
        ${PROJECT_SOURCE_DIR}/external/kiero/*.h
        ${PROJECT_SOURCE_DIR}/external/kiero/minhook/*.h
        ${PROJECT_SOURCE_DIR}/external/kiero/minhook/*.c
        ${PROJECT_SOURCE_DIR}/external/kiero/minhook/*.h
        ${PROJECT_SOURCE_DIR}/external/kiero/minhook/hde/*.c
        ${PROJECT_SOURCE_DIR}/external/kiero/minhook/hde/*.h
        )

file(GLOB SRC_FILES
        ${PROJECT_SOURCE_DIR}/src/*.cpp
        ${PROJECT_SOURCE_DIR}/src/*.h
        ${PROJECT_SOURCE_DIR}/src/style/*.cpp
        ${PROJECT_SOURCE_DIR}/src/style/*.h)


add_library(Kiero_DX9_ImGui_Base SHARED ${SRC_FILES} ${IMGUI} ${KIERO})
