#include "sepch.h"

#include <glad/glad.h>

#include "Debug.h"
#include "Shaders/Shader.h"
#include "VertexBuffer.h"

namespace se {

	const std::string VertexBuffer::VERTICES = "vertices";
	const std::string VertexBuffer::NORMALS = "normals";
	const std::string VertexBuffer::INDEXES = "indexes";
	const std::string VertexBuffer::COLORS = "colors";
	const std::string VertexBuffer::SIZES = "sizes";
	const std::string VertexBuffer::UV = "uv";

	VboConfiguration VBO_CONFIG_VERTEX = {
		GL_ARRAY_BUFFER,
		ShaderLocation::LOCATION_POSITION,
		3, GL_FLOAT, GL_FALSE, 0, (void*)0,
		GL_STATIC_DRAW  //
	};

	VboConfiguration VBO_CONFIG_NORMAL = {
		GL_ARRAY_BUFFER,
		ShaderLocation::LOCATION_NORMAL,
		3, GL_FLOAT, GL_FALSE, 0, (void*)0,
		GL_STATIC_DRAW  //
	};

	VboConfiguration VBO_CONFIG_INDEX = {
		GL_ELEMENT_ARRAY_BUFFER,
		0  //
	};

	VboConfiguration VBO_CONFIG_COLOR{
		GL_ARRAY_BUFFER,
		ShaderLocation::LOCATION_COLOR,
		4, GL_FLOAT, GL_FALSE, 0, (void*)0,
		GL_STATIC_DRAW  //
	};

	VboConfiguration VBO_CONFIG_UV{
		GL_ARRAY_BUFFER,
		ShaderLocation::LOCATION_TEXCOORD,
		2, GL_FLOAT, GL_FALSE, 0, (void*)0,
		GL_STATIC_DRAW  //
	};

	VboConfiguration VBO_CONFIG_FONT{
		GL_ARRAY_BUFFER,
		ShaderLocation::LOCATION_POSITION,
		4, GL_FLOAT, GL_FALSE, 0, (void*)0,
		GL_DYNAMIC_DRAW  //
	};

	VertexBuffer::~VertexBuffer() {
		Delete();
	}

	VertexBuffer* VertexBuffer::Init() {
		if (ready) {
			Debug::Log(WARNING) << "VertexBuffer " << name << " already initializated" << std::endl;
			return this;
		}

		glGenBuffers(1, &Id);

		if (config.location > 0) {
			Bind();
			glEnableVertexAttribArray(config.location);
			glVertexAttribPointer(config.location, config.blocksize, config.type, config.normalized, config.stride, config.pointer);
			Unbind();
		}

		ready = true;
		return this;
	}

	VertexBuffer* VertexBuffer::Delete() {
		glDeleteBuffers(1, &Id);
		return this;
	}

	VertexBuffer* VertexBuffer::Bind() {
		if (!config.target) {
			Debug::Log(ERROR) << "Can't bind VertexBuffer " << name << ", no target array set" << std::endl;
			exit(1);
			return this;
		}
		glBindBuffer(config.target, Id);
		return this;
	}

	VertexBuffer* VertexBuffer::Unbind() {
		if (!config.target) {
			Debug::Log(WARNING) << "Can't unbind " << name << ", no target array set" << std::endl;
			return this;
		}
		glBindBuffer(config.target, 0);
		return this;
	}

	void VertexBuffer::BufferSubData(unsigned int target, signed long long int offset, signed long long int size, const void* data) {
		glBufferSubData(target, offset, size, data);
	}
	void VertexBuffer::BufferData(unsigned int target, signed long long int size, const void* data, int usage) {
		BufferData(target, size, data, usage);
	}

}  // namespace se
