#include "sepch.h"

#include <glad/glad.h>

#include "Core.h"
#include "Utility.h"
#include "Mesh.h"

namespace se {

	Mesh::Mesh() {
		name = "Mesh";
		Init();
	}

	Mesh::Mesh(std::string filename_) {
		name = util::basename(filename_);
		Init();
		Load(filename_);
	}

	Mesh::~Mesh() {
		delete (VAO);
		VAO = nullptr;
	}

	Mesh* Mesh::Init() {
		VAO = new VertexArray();
		VAO->AddBuffer(VertexBuffer::VERTICES, VBO_CONFIG_VERTEX);
		VAO->AddBuffer(VertexBuffer::UV, VBO_CONFIG_UV);
		VAO->AddBuffer(VertexBuffer::NORMALS, VBO_CONFIG_NORMAL);
		//VAO->AddBuffer(VertexBuffer::INDEXES, VBO_CONFIG_INDEX);
		VAO->Init();
		return this;
	}

	Mesh* Mesh::Load(std::string filename) {
		indexesData.clear();
		normalsData.clear();
		indexesData.clear();
		uvData.clear();

		std::vector<GLushort> vertexIndices, uvIndices, normalIndices;
		std::vector<glm::vec3> temp_vertices;
		std::vector<glm::vec2> temp_uvs;
		std::vector<glm::vec3> temp_normals;

		FILE* file = fopen(filename.c_str(), "r");
		if (file == NULL) {
			Debug::Log(ERROR) << "Impossible to open the file " << filename << std::endl;
			return this;
		}

		while (true) {
			char lineHeader[128];

			// read the first word of the line
			int res = fscanf(file, "%s", lineHeader);
			if (res == EOF) break;

			// Vertices
			if (strcmp(lineHeader, "v") == 0) {
				glm::vec3 vertex;
				fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				temp_vertices.push_back(vertex);

				// UV Map
			}
			else if (strcmp(lineHeader, "vt") == 0) {
				glm::vec2 uv;
				fscanf(file, "%f %f\n", &uv.x, &uv.y);
				temp_uvs.push_back(uv);

				// Normals
			}
			else if (strcmp(lineHeader, "vn") == 0) {
				glm::vec3 normal;
				fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
				temp_normals.push_back(normal);

				// Faces
			}
			else if (strcmp(lineHeader, "f") == 0) {
				unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
				int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
				if (matches != 9) {
					Debug::Log(ERROR) << "File can't be read by our simple parser: Try exporting with other options" << std::endl;
					return this;
				}
				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
				uvIndices.push_back(uvIndex[0]);
				uvIndices.push_back(uvIndex[1]);
				uvIndices.push_back(uvIndex[2]);
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);
			}
		}

		for (unsigned int v = 0; v < vertexIndices.size(); v += 3) {  // For each triangle
			for (unsigned int i = 0; i < 3; i += 1) {  // For each vertex of the triangle

				unsigned int vertexIndex = vertexIndices[v + i];
				glm::vec3 vertex = temp_vertices[vertexIndex - 1];

				unsigned int uvIndex = uvIndices[v + i];
				glm::vec2 uv = temp_uvs[uvIndex - 1];

				unsigned int normalIndex = normalIndices[v + i];
				glm::vec3 normal = temp_normals[normalIndex - 1];

				vertexData.push_back(vertex);
				uvData.push_back(uv);
				normalsData.push_back(normal);
				indexesData.push_back(vertexIndex);
			}
		}

		for (auto& v : vertexIndices)
			indexesData.push_back(v);

		// PrintData();

		VAO->Use();
		VAO->Load<glm::vec3>(VertexBuffer::VERTICES, vertexData);
		VAO->Load<glm::vec2>(VertexBuffer::UV, uvData);
		VAO->Load<glm::vec3>(VertexBuffer::NORMALS, normalsData);
		// VAO->Load<GLushort>(VertexBuffer::INDEXES, indexesData);
		VAO->Leave();

		ready = true;
		return this;
	}

	void Mesh::PrintData() {
		Debug::Log(WARNING) << "vertices:" << std::endl;
		for (auto& element : vertexData) {
			Debug::Log << element.x << ", " << element.y << ", " << element.z << std::endl;
		}

		Debug::Log(WARNING) << "uv:" << std::endl;
		for (auto& element : uvData) {
			Debug::Log << element.x << ", " << element.y << std::endl;
		}

		Debug::Log(WARNING) << "normals:" << std::endl;
		for (auto& element : normalsData) {
			Debug::Log << element.x << ", " << element.y << ", " << element.z << std::endl;
		}

		Debug::Log(WARNING) << "indexes:" << std::endl;
		for (auto& element : indexesData) {
			Debug::Log << element << std::endl;
		}
	}

}  // namespace se