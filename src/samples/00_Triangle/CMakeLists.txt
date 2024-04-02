get_filename_component(PROJECT_NAME "${CMAKE_CURRENT_LIST_DIR}" NAME)
message(STATUS "Building ${PROJECT_NAME}")

find_package(glfw3 CONFIG REQUIRED)

project(${PROJECT_NAME})
file(GLOB SOURCES "*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE vulk)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

compile_shaders(${CMAKE_CURRENT_LIST_DIR})


