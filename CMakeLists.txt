cmake_minimum_required(VERSION 3.15)
project(ecsgame)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

add_executable(ecsgame Util/glad.c Util/OBJ_Loader.h Rendering/Shader.cpp Rendering/Shader.h main.cpp ECS/Components/Transform.h ECS/Components/Movement.h ECS/Events/InputEvent.h ECS/Systems/GravitySystem.h ECS/Systems/PositionDebugSystem.h ECS/Systems/KeyboardMovementSystem.h ECS/Components/Camera.h)

include_directories(${OPENGL_INCLUDE_DIRS})

target_link_libraries(ecsgame ${OPENGL_LIBRARY} glfw glm ${CMAKE_DL_LIBS})