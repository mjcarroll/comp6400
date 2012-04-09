cmake_minimum_required(VERSION 2.6)
project(homework1)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

set(CMAKE_BUILD_TYPE Debug)

find_library(GLUT_LIBRARY GLUT)
find_library(OpenGL_LIBRARY OpenGL)

add_executable(homework1 src/homework1.c)
target_link_libraries(homework1 ${OpenGL_LIBRARY} ${GLUT_LIBRARY})
