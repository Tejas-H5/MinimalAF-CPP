# CMakeList.txt : CMake project for MinimalAFCPP, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (MinimalAFCPP "main.cpp" "helpers.h")

target_sources(MinimalAFCPP PUBLIC "Window.cpp" "Window.h")

# TODO: Add tests and install targets if needed.
target_link_libraries(MinimalAFCPP
	libglew_static
	glfw
)