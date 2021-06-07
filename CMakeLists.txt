cmake_minimum_required(VERSION 3.0)

cmake_policy(SET CMP0015 NEW)
cmake_policy(SET CMP0072 OLD)

project(BaseCode)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_EXE_LINKER_FLAGS "-v")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -std=c++17")
execute_process(COMMAND ln -s ../shaders)
execute_process(COMMAND ln -s ../resources)
execute_process(COMMAND ln -s ../LOD/models/wall.ply ../../models)
execute_process(COMMAND ln -s ../LOD/models/ground.ply ../../models)
execute_process(COMMAND ln -s ../../models)

set(appName BaseCode)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
find_package (Eigen3 3.3 REQUIRED NO_MODULE)
find_package(Freetype REQUIRED)
# ^ install it from https://www.freetype.org/download.html
find_package(tinyply REQUIRED)
# ^ follow this: https://github.com/markcox80/tinyply-cmake in libs/tinyply (using: mkdir ./tmp/build and not cd )

include_directories("${CMAKE_SOURCE_DIR}/headers")
include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${GLUT_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories("${CMAKE_SOURCE_DIR}/libs/glm")
include_directories(${FREETYPE_INCLUDE_DIRS})

link_libraries(${FREETYPE_LIBRARY})

link_directories(${OPENGL_LIBRARY_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
link_directories(${GLEW_LIBRARY_DIRS})
link_directories(${FREETYPE_LIBRARIES})

aux_source_directory("${CMAKE_SOURCE_DIR}/src" SUB_SOURCES)
set(SOURCES ${SUB_SOURCES})
add_executable(${appName} ${SOURCES} main.cpp)

target_link_libraries(${appName} tinyply ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${GLEW_LIBRARIES} Eigen3::Eigen)
