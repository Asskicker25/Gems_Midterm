#pragma once

#include "Vertex.h"
#include <vector>

class MeshDataHolder
{
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};

