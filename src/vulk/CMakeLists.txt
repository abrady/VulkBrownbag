project(vulk)


find_package(glfw3 CONFIG REQUIRED)
find_package(Vulkan REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

file(GLOB SOURCES "*.cpp")
add_library(vulk ${SOURCES})
target_include_directories(vulk PUBLIC .)

target_link_libraries(vulk PRIVATE glfw)
target_link_libraries(vulk PRIVATE Vulkan::Vulkan)
target_link_libraries(vulk PRIVATE glm::glm-header-only)
target_link_libraries(vulk PRIVATE assimp::assimp)
target_link_libraries(vulk PRIVATE spdlog::spdlog)
