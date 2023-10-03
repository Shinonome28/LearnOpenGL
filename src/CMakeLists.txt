
include_directories(${LearnOpenGLCN_SOURCE_DIR}/includes)
include_directories(${Stb_INCLUDE_DIR})

add_library(stb_implementation OBJECT stb_implementation.cpp)

add_executable(SimpleTriangle SimpleTriangle.cpp)
target_link_libraries(SimpleTriangle PRIVATE glad::glad glfw)

add_executable(TextureExample TextureExample.cpp)
target_link_libraries(TextureExample PRIVATE glad::glad glfw $<TARGET_OBJECTS:stb_implementation>)

add_executable(TransformBasis TransformBasis.cpp)
target_link_libraries(TransformBasis PRIVATE glad::glad glfw glm::glm $<TARGET_OBJECTS:stb_implementation>)

add_executable(CoordinateSystem CoordinateSystem.cpp)
target_link_libraries(CoordinateSystem PRIVATE glad::glad glfw glm::glm $<TARGET_OBJECTS:stb_implementation>)

add_executable(UniformExample UniformExample.cpp)
target_link_libraries(UniformExample PRIVATE glad::glad glfw glm::glm $<TARGET_OBJECTS:stb_implementation>)

add_executable(BasicColor BasicColor.cpp)
target_link_libraries(BasicColor PRIVATE glad::glad glfw glm::glm $<TARGET_OBJECTS:stb_implementation>)

add_executable(BlingPhone BlingPhone.cpp)
target_link_libraries(BlingPhone PRIVATE glad::glad glfw glm::glm $<TARGET_OBJECTS:stb_implementation>)