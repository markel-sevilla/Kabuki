include(GoogleTest)

find_package(GTest CONFIG REQUIRED)

add_executable(k-lib-tests
    OpenGLTest.cpp
    OpenGLTest.h

    KbkBufferTesting.cpp
    #KbkCameraTesting.cpp
    #KbkLightTesting.cpp
    #KbkMeshTesting.cpp
    KbkPassTesting.cpp
    KbkPipelineTesting.cpp
    #KbkRendererTesting.cpp
    KbkShaderTesting.cpp
    #KbkTextureTesting.cpp
    KbkVertexTesting.cpp
)

target_include_directories(k-lib-tests
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src/include
        ${CMAKE_CURRENT_SOURCE_DIR}
)

find_package(GTest CONFIG REQUIRED)

target_link_libraries(k-lib-tests
    PRIVATE
    k-lib
    GTest::gtest
    GTest::gtest_main
)
# GLM
find_package(glm CONFIG REQUIRED)
target_link_libraries(k-lib-tests PUBLIC glm::glm)

# GLAD
find_package(glad CONFIG REQUIRED)
target_link_libraries(k-lib-tests PUBLIC glad::glad)

# GLFW3
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(k-lib-tests PUBLIC glfw)

# Discover tests when ctest runs, not as a post-build step.
# This avoids running the GL tests during the build and avoids the *_NOT_BUILT placeholder.
gtest_discover_tests(k-lib-tests DISCOVERY_MODE PRE_TEST
    PROPERTIES
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)
