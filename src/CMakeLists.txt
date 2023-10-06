
include_directories(${LearnOpenGLCN_SOURCE_DIR}/includes)
include_directories(${Stb_INCLUDE_DIR})

add_library(stb_implementation OBJECT stb_implementation.cpp)

file(GLOB_RECURSE SOURCEFILES "*.cpp")
list(FILTER SOURCEFILES EXCLUDE REGEX  "stb_implementation.cpp$")
foreach(_source IN LISTS SOURCEFILES)
    get_filename_component(_basename ${_source} NAME_WE)
    add_executable(${_basename} ${_source})
    target_link_libraries(${_basename} PRIVATE glad::glad glfw glm::glm $<TARGET_OBJECTS:stb_implementation>)
endforeach(_source IN LISTS SOURCEFILES)