#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct Vertex {
    float x, y, z;  // Position
    float r, g, b;  // Color (if available)
};

class ModelLoader {
public:
    static std::vector<Vertex> loadPLY(const std::string& filename);
};

#endif // MODEL_LOADER_H
