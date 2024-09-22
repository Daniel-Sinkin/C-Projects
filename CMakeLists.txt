# Define the minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Set the project name and specify that it's a C project
project(LearningC C)

# Add the source files
add_executable(learningC src/main.c)

# Optionally set compiler flags
# set(CMAKE_C_FLAGS "-Wall -Wextra")

# FetchContent module to download GLFW and GLAD
include(FetchContent)

# Define GLFW and specify the tag/branch to use (e.g., latest stable version)
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG        3.3.8  # Replace with the version you want
)

# Define GLAD repository
FetchContent_Declare(
        glad
        GIT_REPOSITORY https://github.com/Dav1dde/glad.git
        GIT_TAG        master  # Replace with the desired version or tag
)

# Fetch both GLFW and GLAD
FetchContent_MakeAvailable(glfw glad)

# Link the GLFW and GLAD libraries to your executable
target_link_libraries(learningC PRIVATE glfw glad)

# Platform-specific settings: only link X11 on non-macOS systems
if (NOT APPLE)
    find_package(X11 REQUIRED)
    target_link_libraries(learningC PRIVATE X11)
else()
    # On macOS, link the necessary Cocoa, IOKit, and CoreFoundation frameworks
    target_link_libraries(learningC PRIVATE "-framework Cocoa" "-framework IOKit" "-framework CoreFoundation")
endif()