cmake_minimum_required(VERSION 3.10)

get_filename_component(PROJECT_FOLDER_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
string(REPLACE " " "_" PROJECT_FOLDER_NAME ${PROJECT_FOLDER_NAME})
project(${PROJECT_FOLDER_NAME} VERSION 0.1.0)
message("Project Name: ${PROJECT_NAME}")

find_package(SDL2 REQUIRED)
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)

set(CMAKE_C_STANDARD 11) # Requires C11 standard
set(CMAKE_CXX_STANDARD 17) # Requires C++20 standard

file(GLOB_RECURSE SRC_C_CORE ${CMAKE_SOURCE_DIR}/core/src/*.c)
file(GLOB_RECURSE SRC_CPP_CORE ${CMAKE_SOURCE_DIR}/core/src/*.cpp)

add_executable(${PROJECT_NAME} ${SRC_C_CORE} ${SRC_CPP_CORE})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/core/src)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/core/include)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL2)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2main)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_image)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_gfx)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)

set(ARCTIC_STATIC_LIB ON CACHE INTERNAL "" FORCE)
add_subdirectory(lib/arctic)
target_link_libraries(${PROJECT_NAME} PRIVATE arctic)
