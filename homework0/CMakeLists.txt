cmake_minimum_required(VERSION 2.6)
project(homework0)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

set(CMAKE_BUILD_TYPE Debug)

find_library(GLUT_LIBRARY GLUT)
find_library(OpenGL_LIBRARY OpenGL)

# contour.c
add_executable(contour contour.c)
target_link_libraries(contour ${OpenGL_LIBRARY} ${GLUT_LIBRARY})

# gasket.c
add_executable(gasket gasket.c)
target_link_libraries(gasket ${OpenGL_LIBRARY} ${GLUT_LIBRARY})

# gasket2.c
add_executable(gasket2 gasket2.c)
target_link_libraries(gasket2 ${OpenGL_LIBRARY} ${GLUT_LIBRARY})

# gasket3.c
add_executable(gasket3 gasket3.c)
target_link_libraries(gasket3 ${OpenGL_LIBRARY} ${GLUT_LIBRARY})

# tetra.c
add_executable(tetra tetra.c)
target_link_libraries(tetra ${OpenGL_LIBRARY} ${GLUT_LIBRARY})
