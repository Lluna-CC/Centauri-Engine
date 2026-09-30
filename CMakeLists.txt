cmake_minimum_required(VERSION 3.29)

SET (TEMPLATE_NAME "Centauri-Engine")
project(${TEMPLATE_NAME})

#FILE LIST

add_executable(${TEMPLATE_NAME} 
    TestApp/main.cpp 
    Centauri/src/Application/application.cpp
    Centauri/src/Events/eventSystem.cpp
    Centauri/src/Components/entity.cpp
)
set_target_properties (${TEMPLATE_NAME} PROPERTIES CXX_STANDARD 20)
target_link_libraries (${TEMPLATE_NAME} Vulkan::cppm glfw)

find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(Vulkan 1.4.335 REQUIRED)

add_library(VulkanCppModule INTERFACE)
add_library(Vulkan::cppm ALIAS VulkanCppModule)
target_link_libraries(VulkanCppModule INTERFACE Vulkan::Vulkan)
target_compile_definitions(VulkanCppModule INTERFACE VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1 VULKAN_HPP_NO_STRUCT_CONSTRUCTORS=1)

include_directories("src/Application")