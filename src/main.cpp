#include <windows.h>
#include <iostream>
#include <vector>
#include <glad/glad.h>  // Make sure to include this first
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// Structure for storing vertex data
struct Vertex {
    float x, y, z;  // Position
    float r, g, b;  // Color
};

// Global variables for OpenGL
GLuint VAO, VBO;
std::vector<Vertex> vertices;

// GLFW Error Callback
void error_callback(int error, const char* description) {
    std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
}

// Function to load the PLY file using Assimp
std::vector<Vertex> loadPLY(const std::string& filename) {
    std::vector<Vertex> vertices;
    Assimp::Importer importer;

    std::cout << "Loading model: " << filename << std::endl;

    const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate);
    if (!scene || !scene->mNumMeshes) {
        std::cerr << "❌ Failed to load PLY file: " << filename << std::endl;
        return vertices;
    }

    aiMesh* mesh = scene->mMeshes[0];  // Load the first mesh
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex v;
        v.x = mesh->mVertices[i].x;
        v.y = mesh->mVertices[i].y;
        v.z = mesh->mVertices[i].z;

        // If the model has vertex colors, use them; otherwise, set white
        if (mesh->HasVertexColors(0)) {
            v.r = mesh->mColors[0][i].r;
            v.g = mesh->mColors[0][i].g;
            v.b = mesh->mColors[0][i].b;
        } else {
            v.r = v.g = v.b = 1.0f;  // Default white
        }

        vertices.push_back(v);
    }

    std::cout << "✅ Loaded " << vertices.size() << " vertices from " << filename << std::endl;

    if (vertices.empty()) {
        std::cerr << "⚠️ Warning: No vertices loaded! Check if the PLY file is valid." << std::endl;
    }

    return vertices;
}

// Function to upload point cloud data to OpenGL buffers
void setupPointCloud() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    std::cout << "✅ Point cloud uploaded to OpenGL buffers." << std::endl;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Open a console window for debugging (optional)
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);

    std::cout << "🚀 Starting Gaussian Splatting Renderer..." << std::endl;

    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "❌ Failed to initialize GLFW!" << std::endl;
        std::cout << "Press Enter to exit..." << std::endl;
        std::cin.get();
        return -1;
    }

    std::cout << "✅ GLFW initialized successfully." << std::endl;

    // Set GLFW error callback
    glfwSetErrorCallback(error_callback);

    // Create a window (800x600)
    GLFWwindow* window = glfwCreateWindow(800, 600, "Gaussian Splatting Renderer", NULL, NULL);
    if (!window) {
        std::cerr << "❌ Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        std::cout << "Press Enter to exit..." << std::endl;
        std::cin.get();
        return -1;
    }

    std::cout << "✅ Window created successfully." << std::endl;

    // Make the OpenGL context current
    glfwMakeContextCurrent(window);
    std::cout << "✅ OpenGL context set." << std::endl;

    // Initialize GLAD (this must be done after creating the OpenGL context)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "❌ Failed to initialize GLAD!" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        std::cout << "Press Enter to exit..." << std::endl;
        std::cin.get();
        return -1;
    }
    std::cout << "✅ GLAD initialized successfully." << std::endl;

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Load Stanford Bunny model (PLY format)
    std::string plyFile = "bunny/reconstruction/bun_zipper.ply";
    vertices = loadPLY(plyFile);

    if (vertices.empty()) {
        std::cerr << "❌ No vertices loaded. Check if the PLY file exists at: " << plyFile << std::endl;
        std::cout << "Press Enter to exit..." << std::endl;
        std::cin.get();
        return -1;
    }

    // Upload data to GPU
    setupPointCloud();

    // Set background color to dark gray
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    std::cout << "🎨 Rendering " << vertices.size() << " points..." << std::endl;

    // Main loop: Keep running until the user closes the window
    while (!glfwWindowShouldClose(window)) {
        std::cout << "🔄 Rendering frame..." << std::endl;
    
        // Clear color & depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
        // Set viewport
        glViewport(0, 0, 800, 600);
    
        // Set point size
        glPointSize(5.0f);
    
        // Draw the point cloud
        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, vertices.size());
    
        // Swap front and back buffers
        glfwSwapBuffers(window);
    
        // Poll for and process events
        glfwPollEvents();
    }

    std::cout << "🔄 Closing application..." << std::endl;

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwDestroyWindow(window);
    glfwTerminate();

    std::cout << "✅ Application closed successfully!" << std::endl;

    // Prevent console from closing immediately
    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.get();

    return 0;
}
