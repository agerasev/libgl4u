cmake_minimum_required(VERSION 2.6)
project(gl4u)

if(${WIN32})
	include_directories(${GLEW_INCLUDE_DIR})
	link_directories(${GLEW_LINK_DIR})
endif()

include_directories(includes)
include_directories(c-includer)

set(SOURCE 
	sources/exception.cpp
	sources/filereader.cpp
	sources/shader.cpp
	sources/vertexbuffer.cpp
	sources/texture.cpp
	sources/framebuffer.cpp
	sources/program.cpp
)

if(${WIN32})
	set(LIBS opengl32 glew32)
else()
	set(LIBS GL GLEW)
endif()

add_library(${PROJECT_NAME} STATIC ${SOURCE})
target_link_libraries(${PROJECT_NAME} ${LIBS})
