# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("MinimalAFCPP")

# Include sub-projects.
add_subdirectory ("lib/submodules/glfw")
add_subdirectory ("lib/submodules/glew")
add_subdirectory ("lib/submodules/glm")
add_subdirectory ("lib/submodules/freetype")
add_subdirectory ("MinimalAFCPP")

target_include_directories(MinimalAFCPP PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib/submodules/glfw/include)
target_include_directories(MinimalAFCPP PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib/submodules/glew/include)
target_include_directories(MinimalAFCPP PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib/submodules/glm/glm)
target_include_directories(MinimalAFCPP PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib/local/stb)
target_include_directories(MinimalAFCPP PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib/submodules/freetype/include)

file(COPY res DESTINATION ${CMAKE_BINARY_DIR}/MinimalAFCPP)