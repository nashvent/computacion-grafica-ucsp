CMAKE_MINIMUM_REQUIRED(VERSION 3.7)
get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
PROJECT(${ProjectId}.o)

SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_BUILD_TYPE DEBUG)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory($ENV{HOME}/glfw $ENV{HOME}/glfw/src)

FIND_PACKAGE(OpenGL REQUIRED)

SET(SOURCE_FILES classes/Sphere.cpp classes/Icosphere.cpp classes/Cubesphere.cpp main.cpp)

ADD_EXECUTABLE(${ProjectId}.o ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(${ProjectId}.o glfw glad)
TARGET_LINK_LIBRARIES(${ProjectId}.o OpenGL::GL)