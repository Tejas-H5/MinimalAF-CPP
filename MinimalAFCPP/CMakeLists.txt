# CMakeList.txt : CMake project for MinimalAFCPP, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (MinimalAFCPP "main.cpp" "helpers.h")

target_sources(MinimalAFCPP PUBLIC 
	"MinimalAF.h"
	"Window.cpp" "Window.h"
	"helpers.h"
	"Datatypes.h"
	"Element.cpp" "Element.h"
	 
	"IMDrawer.cpp" "IMDrawer.h"
	"Mesh.cpp" "Mesh.h"
	"Shader.cpp" "Shader.h"
	"Texture.cpp" "Texture.h"
	"Text.cpp" "Text.h"
	"Color.h"
	"BufferedMeshOutput.h" "BufferedMeshOutput.cpp" 
	"Datatypes/Vertex.h" "Datatypes/Rect.h"
)

# TODO: Add tests and install targets if needed.
target_link_libraries(MinimalAFCPP
	libglew_static
	glfw
	freetype
)