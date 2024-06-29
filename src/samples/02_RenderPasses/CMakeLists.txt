get_filename_component(PROJECT_NAME "${CMAKE_CURRENT_LIST_DIR}" NAME)
project(${PROJECT_NAME})

find_package(glfw3 CONFIG REQUIRED)
find_package(Vulkan)
find_package(VulkanHeaders CONFIG)
find_package(VulkanMemoryAllocator CONFIG REQUIRED)

add_compile_definitions(VK_USE_PLATFORM_WIN32_KHR) # so we can make a win32 window
add_compile_definitions(NOMINMAX) # so we can use std::min and std::max

file(GLOB SOURCES "*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE vulk)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE Vulkan::Vulkan GPUOpen::VulkanMemoryAllocator Vulkan::Headers)

# add_shader_compilation(${CMAKE_CURRENT_LIST_DIR} ${PROJECT_NAME})


