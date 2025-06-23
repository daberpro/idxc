cmake_minimum_required(VERSION 4.0)
project(idx_testing)

file(GLOB LIB_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp" "${CMAKE_SOURCE_DIR}/src/*.c")
file(GLOB TEST_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
add_executable(
    ${PROJECT_NAME}
    ${TEST_FILES}
    ${LIB_FILES}
)

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/include" "${CMAKE_SOURCE_DIR}/msvc-x64/include")
target_link_libraries(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/msvc-x64/lib/CppUTest.lib" winmm)