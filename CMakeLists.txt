cmake_minimum_required(VERSION 3.14)
project(RealTimeGaussianSplattingRenderer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Required Libraries
find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
include_directories(${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include)
set(GLAD_LIBRARY "C:/Users/axrx/vcpkg/installed/x64-mingw-dynamic/lib/libglad.a")

find_package(glm CONFIG REQUIRED)
set(ASSIMP_LIBRARY "C:/Users/axrx/vcpkg/installed/x64-mingw-dynamic/lib/libassimp.dll.a")
include_directories("C:/Users/axrx/vcpkg/installed/x64-mingw-dynamic/include")

find_package(Eigen3 CONFIG REQUIRED)

# Ensure this is a GUI application
set(CMAKE_EXE_LINKER_FLAGS "-Wl,-subsystem,windows")

# Add Source Files
add_executable(GaussianSplattingRenderer 
    src/main.cpp 
    src/renderer.cpp 
    src/model_loader.cpp
)

# Link Libraries Properly
target_link_libraries(GaussianSplattingRenderer 
    OpenGL::GL 
    glfw 
    ${GLAD_LIBRARY}  # Manually set the correct GLAD path
    glm::glm 
    ${ASSIMP_LIBRARY}  # Manually set the correct Assimp path
    Eigen3::Eigen
)

