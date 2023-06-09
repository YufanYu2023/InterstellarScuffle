cmake_minimum_required(VERSION 3.18)

# Name of project
set(PROJ_NAME GameEngine)
project(${PROJ_NAME})

# Specify project files: header files and source files
set(HDRS
    file_utils.h
    game.h
    shader.h
    geometry.h
    sprite.h
    GameData.h
    GameObject.h
    GameObject_Collidable.h
    GameObject_Collidable_Collectable.h
    GameObject_Collidable_Mortal.h
    GameObject_Collidable_Mortal_Enemy.h
    GameObject_Collidable_Mortal_Player.h
    GameObject_Collidable_Projectile.h
    GameObject_Collidable_SubCollider.h
    GameObject_Temporary.h
    VectorMath.h	
    InstantiationManager.h
    particle_system.h
    particles.h
    BackgroundPane.h
)
 
set(SRCS
    file_utils.cpp
    game.cpp
    main.cpp
    shader.cpp
    sprite.cpp
    sprite_vertex_shader.glsl
    sprite_fragment_shader.glsl
    GameData.cpp
    GameObject.cpp
    GameObject_Collidable.cpp
    GameObject_Collidable_Collectable.cpp
    GameObject_Collidable_Mortal.cpp
    GameObject_Collidable_Mortal_Enemy.cpp
    GameObject_Collidable_Mortal_Player.cpp
    GameObject_Collidable_Projectile.cpp
    GameObject_Collidable_SubCollider.cpp
    GameObject_Temporary.cpp
    VectorMath.cpp
    InstantiationManager.cpp
    particle_system.cpp
    particles.cpp
    BackgroundPane.cpp
    sprite_vertex_shader.glsl
    sprite_fragment_shader.glsl
    particle_vertex_shader.glsl
    particle_fragment_shader.glsl

)

# Add path name to configuration file
configure_file(path_config.h.in path_config.h)

# Add executable based on the source files
add_executable(${PROJ_NAME} ${HDRS} ${SRCS})

# Directories to include for header files, so that the compiler can find
# path_config.h
target_include_directories(${PROJ_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

# Require OpenGL library
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
target_link_libraries(${PROJ_NAME} ${OPENGL_gl_LIBRARY})

# Other libraries needed
set(LIBRARY_PATH "" CACHE PATH "Folder with GLEW, GLFW, GLM, and SOIL libraries")
include_directories(${LIBRARY_PATH}/include)
if(NOT WIN32)
    find_library(GLEW_LIBRARY GLEW)
    find_library(GLFW_LIBRARY glfw)
    find_library(SOIL_LIBRARY SOIL)
elseif(WIN32)
    find_library(GLEW_LIBRARY glew32s HINTS ${LIBRARY_PATH}/lib)
    find_library(GLFW_LIBRARY glfw3 HINTS ${LIBRARY_PATH}/lib)
    find_library(SOIL_LIBRARY SOIL HINTS ${LIBRARY_PATH}/lib)
endif(NOT WIN32)
target_link_libraries(${PROJ_NAME} ${GLEW_LIBRARY})
target_link_libraries(${PROJ_NAME} ${GLFW_LIBRARY})
target_link_libraries(${PROJ_NAME} ${SOIL_LIBRARY})

# The rules here are specific to Windows Systems
if(WIN32)
    # Avoid ZERO_CHECK target in Visual Studio
    set(CMAKE_SUPPRESS_REGENERATION TRUE)
 
    # This will use the proper libraries in debug mode in Visual Studio
    set_target_properties(${PROJ_NAME} PROPERTIES DEBUG_POSTFIX _d)
endif(WIN32)
