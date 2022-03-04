# specify cmake version required
cmake_minimum_required(VERSION 3.0.0)
# project is called "OpenGL" and is not released yet so version is 0.1.0
project(OpenGLProject VERSION 0.1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "/home/jan/visual-studio-code-workspace/OpenGL")

# there's no tests but duh
include(CTest)
enable_testing()

SET(GCC_COVERAGE_COMPILE_FLAGS "-D _DEBUG")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

# get all source files of the project
file(GLOB SOURCES1 "src/*.cpp")
file(GLOB SOURCES2 "src/*/*.cpp")
file(GLOB SOURCES3 "src/*/*/*.cpp")
file(GLOB SOURCES4 "src/*/*/*/*.cpp")

set(SOURCES ${SOURCES1} ${SOURCES2} ${SOURCES3} ${SOURCES4})

# source files
add_executable(${PROJECT_NAME} ${SOURCES})

# get glew
find_package(GLEW REQUIRED)
target_link_libraries(${PROJECT_NAME} GLEW::GLEW)
# get OpenGL
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARY})
# get SDL
find_package(SDL2 REQUIRED)
target_link_libraries(${PROJECT_NAME} SDL2::SDL2)
# get assimp
find_package(ASSIMP REQUIRED)
target_link_libraries(${PROJECT_NAME} ${ASSIMP_LIBRARIES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
