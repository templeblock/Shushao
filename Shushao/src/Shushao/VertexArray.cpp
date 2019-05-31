#include "sepch.h"

#include <glad/glad.h>

#include "Debug.h"
#include "VertexArray.h"

namespace se {

	VertexArray::VertexArray() {
		name = "Vao";
	}

	VertexArray::~VertexArray() {
		for (auto& it : buffers) {
			if (it.second != nullptr) {
				Debug::Log << "Cancello buffer" << it.second->name << std::endl;  //<< " (" + util::classtitle(typeid(*it.second).name()) +")"
				delete (it.second);
				it.second = nullptr;
			}
		}

		buffers.clear();

		glDeleteVertexArrays(1, &Id);
	}

	VertexBuffer* VertexArray::AddBuffer(const std::string name_, VboConfiguration config_) {
		VertexBuffer* buff = new VertexBuffer(config_);
		buff->name = name_;
		AddBuffer(buff);
		return buffers[name_];
	}

	VertexBuffer* VertexArray::AddBuffer(VertexBuffer* vbo_) {
		buffers[vbo_->name] = vbo_;
		return vbo_;
	}

	VertexBuffer* VertexArray::GetBuffer(const std::string name_) {
		auto it = buffers.find(name_);
		if (it == buffers.end()) return nullptr;
		return (VertexBuffer*)buffers[name_];
	}

	VertexArray* VertexArray::Use() {
		if (!Id) {
			Debug::Log(ERROR) << "VAO not initializated" << std::endl;
		}
		glBindVertexArray(Id);
		EnablePointers();
		inUse = true;
		return this;
	}

	VertexArray* VertexArray::Leave() {
		glBindVertexArray(0);
		DisablePointers();
		inUse = false;
		return this;
	}

	VertexArray* VertexArray::EnablePointers() {
		for (auto& it : buffers) {
			glEnableVertexAttribArray(it.second->config.location);
		}
		return this;
	}

	VertexArray* VertexArray::DisablePointers() {
		for (auto& it : buffers) {
			glDisableVertexAttribArray(it.second->config.location);
		}
		return this;
	}

	VertexArray* VertexArray::Init() {
		if (ready) return this;
		glGenVertexArrays(1, &Id);

		Use();
		for (auto& it : buffers) {
			if (it.second != nullptr && !it.second->ready) {
				(VertexBuffer*)it.second->Init();
			}
		}
		Leave();

		ready = (Id > 0);
		return this;
	}
}  // namespace se
