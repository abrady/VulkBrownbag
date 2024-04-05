get_filename_component(PROJECT_NAME "${CMAKE_CURRENT_LIST_DIR}" NAME)
message(STATUS "Building ${PROJECT_NAME}")
project(${PROJECT_NAME})

find_package(glfw3 CONFIG REQUIRED)

file(GLOB SOURCES "*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE vulk)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

add_shader_compilation(${CMAKE_CURRENT_LIST_DIR} ${PROJECT_NAME})


