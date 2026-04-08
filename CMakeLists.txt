cmake_minimum_required(VERSION 3.10)

project(RSP_OpenCV_Classification LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(OpenCV_DIR "C:/opencv/build/x64/vc16/lib")

find_package(OpenCV REQUIRED)

set(SOURCES
    main.cpp
    img_crop.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${OpenCV_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/CLI11/include
)

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

target_compile_definitions(${PROJECT_NAME} PRIVATE
    PROJECT_ROOT_DIR="${CMAKE_SOURCE_DIR}"
)