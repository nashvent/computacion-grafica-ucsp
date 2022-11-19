# Computación grafica - Laboratorio
## Dependencias
- Para instalar las dependencias seguir la siguiente [guia](https://stackoverflow.com/a/62905904).
- Despues seguir [guia](https://github.com/Dav1dde/glad/issues/186#issuecomment-436713397)
- Despues instalar los siguientes paquetes
```
sudo apt-get install libglm-dev
sudo apt-get install libsoil-dev
sudo apt-get install libsfml-dev
```
## Instrucciones para ejecutar
1. Crear una carpeta con los siguientes archivos `main.cpp`y `CMakeLists.txt`.
2. Dentro de la carpeta ejecutar los siguientes comandos.
```
cmake .
make
```
3. Luego se puede ejecutar con el siguiente comando
```
./{filename}.o
```

## Archivos
```
# CMakeLists.txt
CMAKE_MINIMUM_REQUIRED(VERSION 3.7)
get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
PROJECT(${ProjectId}.o)

SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_BUILD_TYPE DEBUG)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(/home/nash/glfw /home/nash/glfw/src)


FIND_PACKAGE(OpenGL REQUIRED)

SET(SOURCE_FILES main.cpp)

ADD_EXECUTABLE(${ProjectId}.o ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(${ProjectId}.o glfw glad)
TARGET_LINK_LIBRARIES(${ProjectId}.o OpenGL::GL)
```