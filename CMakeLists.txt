cmake_minimum_required(VERSION 3.29)

SET (TEMPLATE_NAME "Centauri-Engine")
project(${TEMPLATE_NAME})

#FILE LIST

add_executable(${TEMPLATE_NAME} 
    TestApp/main.cpp 
    Centauri/src/Application/application.cpp
    Centauri/src/Application/scene.cpp
    Centauri/src/Events/eventSystem.cpp
    Centauri/src/Components/entity.cpp
    Centauri/src/Platform/vulkanGLFWWindow.cpp
    Centauri/src/Platform/vulkanPlatform.cpp
    Centauri/src/Render/vulkanRenderer.cpp
    Centauri/src/Resources/renderAttachment.cpp
    Centauri/src/Resources/swapchain.cpp
    Centauri/src/Resources/pipeline.cpp
    Centauri/src/Render/vulkanUtils.cpp
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

find_program(SLANGC_EXECUTABLE slangc HINTS $ENV{VULKAN_SDK}/bin REQUIRED)

# Slang shader
set (SHADER_TARGET ${TEMPLATE_NAME}_Shader)
set (SHADERS_DIR ${CMAKE_SOURCE_DIR}/$<CONFIG>/shaders)
set (SHADER_SOURCES "${CMAKE_SOURCE_DIR}/Centauri/src/Shaders/shader.slang")
set (ENTRY_POINTS -entry vertMain -entry fragMain)
add_custom_command(OUTPUT ${SHADERS_DIR} COMMAND ${CMAKE_COMMAND} -E make_directory ${SHADERS_DIR})
add_custom_command(
  OUTPUT  ${SHADERS_DIR}/slang.spv
  COMMAND ${SLANGC_EXECUTABLE} ${SHADER_SOURCES} -target spirv -profile spirv_1_4 -emit-spirv-directly -fvk-use-entrypoint-name ${ENTRY_POINTS} -o ${SHADERS_DIR}/slang.spv
  WORKING_DIRECTORY ${SHADERS_DIR}
  DEPENDS ${SHADERS_DIR} ${SHADER_SOURCES}
  COMMENT "Compiling Slang Shaders"
  VERBATIM
)
add_custom_target(${SHADER_TARGET} DEPENDS ${SHADERS_DIR}/slang.spv)
add_dependencies(${TEMPLATE_NAME} ${SHADER_TARGET})