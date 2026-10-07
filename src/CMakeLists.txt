cmake_minimum_required(VERSION 3.8)
project(k-lib)

# Engine library
#add_library(${PROJECT_NAME}   
        
 #       )
 
add_executable(${PROJECT_NAME}
		    include/KbkCamera.h
            include/KbkBuffer.h
            include/DebuggingInstance.h
            include/KbkPipeline.h
            include/KbkPass.h
            include/Log.h
            include/KbkShader.h
            include/KbkTexture.h
            include/KbkLight.h
            include/KbkMesh.h
            include/KbkVertex.h
            include/KbkRenderer.h
            include/Sink.h
            src/KbkBuffer.cpp
            src/KbkPipeline.cpp
            src/KbkRenderer.cpp
            src/main.cpp
            src/KbkCamera.cpp
            src/DebuggingInstance.cpp
            src/KbkPass.cpp
            src/KbkMesh.cpp
            src/KbkTexture.cpp
            src/KbkLight.cpp
            src/KbkShader.cpp)

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

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/external/stb
)