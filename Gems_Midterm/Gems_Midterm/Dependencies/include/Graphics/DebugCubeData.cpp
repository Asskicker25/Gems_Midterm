#include "DebugCubeData.h"

DebugCubeData::DebugCubeData()
{
    vertices =
    {
        // Vertices for the corners of the cube
        {glm::vec3(-1.0f, -1.0f, -1.0f)},  // 0
        {glm::vec3(1.0f, -1.0f, -1.0f)},  // 1
        {glm::vec3(1.0f,  1.0f, -1.0f)},  // 2
        {glm::vec3(-1.0f,  1.0f, -1.0f)},  // 3
        {glm::vec3(-1.0f, -1.0f,  1.0f)},  // 4
        {glm::vec3(1.0f, -1.0f,  1.0f)},  // 5
        {glm::vec3(1.0f,  1.0f,  1.0f)},  // 6
        {glm::vec3(-1.0f,  1.0f,  1.0f)}   // 7
    };

    indices = 
    {
        //Front face
        0, 1, 1,
        1, 2, 2,
        2, 3, 3,
        3, 0, 0,
        
        //Back Face
        4, 5, 5,
        5, 6, 6,
        6, 7, 7,
        7, 7, 4,

        0, 4, 4,
        1, 5, 5,
        2, 6, 6,
        3, 7, 7


    };
}
