cmake_minimum_required(VERSION 3.10)

project(RSP_OpenCV_Classification LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(OpenCV_DIR "C:/opencv/build/x64/vc16/lib")

find_package(OpenCV REQUIRED)

file(GLOB_RECURSE SOURCES 
    main.cpp
    img_crop.h
    img_crop.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
target_compile_definitions(RSP_OpenCV_Classification PRIVATE
    PROJECT_ROOT_DIR="${CMAKE_SOURCE_DIR}"
)