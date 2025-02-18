#include "model_loader.h"
#include <iostream>

std::vector<Vertex> ModelLoader::loadPLY(const std::string& filename) {
    std::vector<Vertex> vertices;
    Assimp::Importer importer;
    
    const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate);
    if (!scene || !scene->mNumMeshes) {
        std::cerr << "Failed to load PLY file: " << filename << std::endl;
        return vertices;
    }

    aiMesh* mesh = scene->mMeshes[0];  // Only load the first mesh
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex v;
        v.x = mesh->mVertices[i].x;
        v.y = mesh->mVertices[i].y;
        v.z = mesh->mVertices[i].z;

        // If the model has colors, use them; otherwise, set default white
        if (mesh->HasVertexColors(0)) {
            v.r = mesh->mColors[0][i].r;
            v.g = mesh->mColors[0][i].g;
            v.b = mesh->mColors[0][i].b;
        } else {
            v.r = v.g = v.b = 1.0f;  // Default to white
        }

        vertices.push_back(v);
    }

    std::cout << "Loaded " << vertices.size() << " vertices from " << filename << std::endl;
    return vertices;
}
