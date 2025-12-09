//Include GLEW
#include <GL/glew.h>


//Include GLFW  
#include <GLFW/glfw3.h>  

#include <vector>
#include <string>

#include "tiny_obj_loader.h"

#include "Model.h"
#include "Texture.h"

Model::Model(const std::vector<float>& points, int vertexCount)
{
	this->vertexCount = vertexCount;
	// VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(float), points.data(), GL_STATIC_DRAW);

	//VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);


    glEnableVertexAttribArray(0); //position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)0);

    glEnableVertexAttribArray(1);  //normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
}

Model::Model(const std::vector<float>& points, int vertexCount, bool hasUV)
{
    this->vertexCount = vertexCount;
    // VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(float), points.data(), GL_STATIC_DRAW);

    //VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);


    glEnableVertexAttribArray(0); //position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)0);

    glEnableVertexAttribArray(1); //normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)(3 * sizeof(float)));

    glEnableVertexAttribArray(2); //texcoord (UV)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
}

Model::Model(const std::string& objPath, const std::string& mtlPath)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, objPath.c_str(), mtlPath.c_str());
    if (!ret) 
    {
        printf("Error with load .obj: %s\n", err.c_str());
        return;
    }

    std::vector<float> points;

    for (auto& shape : shapes) {
        for (auto& idx : shape.mesh.indices) {
            points.push_back(attrib.vertices[3 * idx.vertex_index + 0]);
            points.push_back(attrib.vertices[3 * idx.vertex_index + 1]);
            points.push_back(attrib.vertices[3 * idx.vertex_index + 2]);

            points.push_back(attrib.normals[3 * idx.normal_index + 0]);
            points.push_back(attrib.normals[3 * idx.normal_index + 1]);
            points.push_back(attrib.normals[3 * idx.normal_index + 2]);

            points.push_back(attrib.texcoords[2 * idx.texcoord_index + 0]);
            points.push_back(attrib.texcoords[2 * idx.texcoord_index + 1]);
        }
    }

    vertexCount = points.size() / 8; // 3 pos + 3 normal + 2 UV

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(float), points.data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glEnableVertexAttribArray(0); //position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1); //normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2); //texcoord (UV)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
}

void Model::Draw()
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

Model::~Model()
{
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}
