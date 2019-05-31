#include "sepch.h"

#include <glad/glad.h>

#include "Shushao/Core.h"
#include "Debug.h"
#include "Entity.h"
#include "Shaders/StandardShader.h"
#include "Sprite.h"
#include "Transform.h"
#include "Image.h"

namespace se {

	void Image::setup() {
		entity->transform->isRectTransform = true;
		material = new Material();
		material->SetShader(new StandardShader());
		transform->SetPivot(PivotPosition::CENTER);
	}

	Image::~Image() {
		if (material != nullptr) delete (material);
	}

	Image* Image::SetBorder(glm::vec4 border_) {
		border = border_;
		return this;
	}

	Image* Image::SetFillCenter(bool value_) {
		fillCenter = value_;
		return this;
	}

	Image* Image::SetSprite(Sprite* sprite_) {
		sprite = sprite_;
		return this;
	}

	Image* Image::SetImageType(Image::Type type_) {
		type = type_;
		return this;
	}

	Image* Image::SetPreserveAspect(bool value_) {
		preserveAspect = value_;
		return this;
	}

	Image* Image::SetMaterial(Material* material_) {
		material = material_;
		return this;
	}

	bool Image::isReady() {
		return (
			sprite != nullptr &&
			material != nullptr &&
			material->shader != nullptr &&
			transform != nullptr);
	}

	void Image::Awake() {
		if (sprite == nullptr) {
			Debug::Log(ERROR) << "Sprite undefined" << std::endl;
			return;
		}

		if (!sprite->ready) {
			sprite->Build();
		}

		refreshSprite();

		material->SetMainTexture(sprite->texture);

		if (type == Image::Type::SLICED || type == Image::Type::TILED) {
			material->shader->Use();

			material->shader->SetVector("sprite_border", sprite->border);  // in pixels
			material->shader->SetVector("image_border", border);  // in pixels
			material->shader->SetVector("texture_size", sprite->texture->size);  // in pixels
			material->shader->SetVector("image_size", transform->rectTransform->rect.size * (float)sprite->pixelPerUnit);  // in pixels
			material->shader->SetInteger("sliced_fill", fillCenter);  // bool

			if (type == Image::Type::SLICED) {
				material->shader->Enable("sliced");
				material->shader->Disable("tiled");
			}
			else if (type == Image::Type::TILED) {
				material->shader->Enable("tiled");
				material->shader->Disable("sliced");
			}

			material->shader->Leave();
		}
	}

	void Image::Update() {
		if (!isReady()) return;

		if (transform->rectTransform->rect.size != last_rectSize) {
			refreshSprite();
		}
	}

	void Image::refreshSprite() {
		if (!preserveAspect) {
			Rect newRect = {
				transform->rectTransform->rect.x * sprite->pixelPerUnit,
				transform->rectTransform->rect.y * sprite->pixelPerUnit,
				transform->rectTransform->rect.width * sprite->pixelPerUnit,
				transform->rectTransform->rect.height * sprite->pixelPerUnit };
			sprite->SetRect(newRect);
			sprite->SetPreserveAspect(false);
		}
		else {
			Rect newRect = {
				transform->rectTransform->rect.x * sprite->pixelPerUnit,
				transform->rectTransform->rect.y * sprite->pixelPerUnit,
				transform->rectTransform->rect.width * sprite->pixelPerUnit,
				transform->rectTransform->rect.height * sprite->pixelPerUnit };

			float spriteAspect = sprite->GetRect().GetAspect();
			float rectAspect = newRect.GetAspect();
			if (rectAspect > spriteAspect) {
				newRect.SetWidth(newRect.height * spriteAspect);
			}
			else if (rectAspect < spriteAspect) {
				newRect.SetHeight(newRect.width / spriteAspect);
			}
			sprite->SetRect(newRect);
			sprite->SetPreserveAspect(false);
		}

		last_rectSize = transform->rectTransform->rect.size;

		if (type == Image::Type::SLICED || type == Image::Type::TILED) {
			material->shader->Use();
			material->shader->SetVector("image_size", transform->rectTransform->rect.size * (float)sprite->pixelPerUnit);  // in pixels
			material->shader->Leave();
		}

		sprite->Build();
	}

	void Image::Render() {
		if (!isReady()) return;

		sprite->VAO->Use();
		material->shader->Use();
		material->shader->SetRenderColor(material->color);
		material->shader->SetMVP(transform->uMVP());
		material->update();

		sprite->VAO->GetBuffer(VertexBuffer::INDEXES)->Bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
		sprite->VAO->GetBuffer(VertexBuffer::INDEXES)->Unbind();

		material->shader->Leave();
		sprite->VAO->Leave();
	}

	void Image::OnDestroy() {
		if (!isReady()) return;

		sprite->VAO->Leave();
		material->shader->Leave();
		material->shader->exit();
		delete (material);
		material = nullptr;
	}
}  // namespace se
