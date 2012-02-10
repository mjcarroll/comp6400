cmake_minimum_required(VERSION 2.6)
project(homework2)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

set(CMAKE_BUILD_TYPE Debug)

find_library(GLUT_LIBRARY GLUT)
find_library(OpenGL_LIBRARY OpenGL)

include_directories(include)
add_executable(homework2 src/shelby.cc src/person.cc)
target_link_libraries(homework1 ${OpenGL_LIBRARY} ${GLUT_LIBRARY})
