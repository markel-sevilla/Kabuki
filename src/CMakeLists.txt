cmake_minimum_required(VERSION 3.8)
project(rendering-playground)

# Engine library
#add_library(${PROJECT_NAME}   
        
 #       )
 
add_executable(${PROJECT_NAME}
		)

target_include_directories(${PROJECT_NAME} PUBLIC 
    .
    include        # relativo al CMakeLists.txt
    ${CMAKE_SOURCE_DIR}/include 
)

# GLM
find_package(glm CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC glm::glm)

# GLAD
find_package(glad CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC glad::glad)

# GLFW3
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)

# IMGUI
find_package(imgui CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC imgui::imgui)


