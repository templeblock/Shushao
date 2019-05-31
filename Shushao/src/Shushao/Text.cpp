#include "sepch.h"

#include <glad/glad.h>

#include "Debug.h"
#include "Design.h"
#include "Entity.h"
#include "Font.h"
#include "GLManager.h"
#include "Rect.h"
#include "Shaders/fontShader.h"
#include "Shaders/Shader.h"
#include "Text.h"
#include "Transform.h"
#include "VertexArray.h"

namespace se {

	Text::~Text() {
		delete (VAO);
		delete (shader);
		VAO = nullptr;
		shader = nullptr;
	}

	void Text::setup() {
		entity->transform->isRectTransform = true;
		shader = new FontShader();
		VAO = new VertexArray();
		VAO->AddBuffer(VertexBuffer::VERTICES, VBO_CONFIG_FONT);
	}

	bool Text::isReady() {
		return (font != nullptr && shader != nullptr && transform != nullptr);
	}

	void Text::Awake() {
		shader->awake();
		VAO->Init();

		if (entity->canvas != nullptr && entity->canvas->renderMode == RenderMode::SCREEN) {
			shader->Use();
			shader->SetVector("viewport", GLManager::VIEWPORT);
			shader->Enable("viewport");
			shader->Leave();
		}
	}

	int Text::getWidth(std::wstring text) {
		const wchar_t* p;
		float width = 0;
		for (p = text.c_str(); *p; p++) {
			if (!font->LoadCharTexture(*p)) {
				continue;
			}
			if (*p == '\n') {
				continue;
			}
			width += (((font->face->glyph->advance.x) / 64) * scale.x);
		}
		return width;
	}

	void Text::renderText() {
		// NB: y punta in alto anche qui, non confondere con il web

		if (text == L"") return;

		const wchar_t* p;
		const wchar_t* ctext = _text.c_str();
		bool baseline = true;
		size_t i = 0;
		int row = 0;

		// imposto il colore
		shader->SetRenderColor(color);

		// pos di partenza in pixel
		glm::vec2 pos = offset * (float)pixelPerUnit;
		float leftpos = pos.x;  // memorizzo la leftpos

		// calcolo altezza linea e spaziatura utilizzando le percentuali
		int pixelLineHeight = font->size * lineHeight;

		// calcolo il rect transform in pixel
		Rect pixelRect = transform->rectTransform->rect * (float)pixelPerUnit;

		// ottengo il Glyph dal font face
		FT_GlyphSlot glyph = font->face->glyph;

		// genero la texture
		GLuint tex;
		glGenTextures(1, &tex);
		glActiveTexture(shader->GetTextureIndex("diffuse_map"));
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		// estraggo tutte le words di una linea - solo se il testo è cambiato
		if (wordWrap && text != lastText) {
			words.clear();
			std::wstring word = L"";
			for (p = ctext, i = 0; i < text.size(); ++p, ++i) {
				if (isspace(*p) || *p == '\0') {
					words.insert({ i - word.length(), getWidth(word) });
					word = L"";
				}
				else {
					word += *p;
				}
			}
			words.insert({ i - word.length(), getWidth(word) });
		}

		// ciclo tutti i caratteri della linea per calcolare le dimensioni del testo - solo se il testo è cambiato
		if (text != lastText) {
			baselineGap = 0.0f;
			width = 0.0f;
			height = 0.0f;
			rowwidth.clear();
			rowheight.clear();
			rowwidth.push_back(0.0f);
			rowheight.push_back(0.0f);

			for (p = ctext, i = 0; i < text.size(); ++p, ++i) {
				if (!font->LoadCharTexture(*p)) {
					continue;
				}

				// aggiungo il carattere al calcolo solo se il carattere non è newline
				if (*p != '\n') {
					rowwidth[row] += (((glyph->advance.x) / 64) * scale.x);
					rowheight[row] = std::max(rowheight[row], (glyph->bitmap.rows * scale.y));
					width = std::max(width, rowwidth[row]);
				}

				// se il carattere è newline oppure è attivato il wordwrap e la prossima parola (i+1) supererebbe la larghezza del rect, aggiungo una riga
				if (*p == '\n' || (wordWrap && words.find(i + 1) != words.end() && rowwidth[row] + words[i + 1] > pixelRect.width)) {
					row++;
					rowheight.push_back(0.0f);
					rowwidth.push_back(0.0f);
					baselineGap = 0;  // lo azzero, tanto serve solo quello dell'ultima riga
				}

				// calcolo il baseline gap
				if (glyph->bitmap.rows > (unsigned int)glyph->bitmap_top) {
					baselineGap = std::max(baselineGap, (unsigned int)(glyph->bitmap.rows * scale.y - glyph->bitmap_top * scale.y));
				}
			}

			// calcolo l'altezza del testo
			for (size_t i = 0; i < rowheight.size(); ++i) {
				height += rowheight[i];
				if (i < rowheight.size() - 1) {
					height += pixelLineHeight - rowheight[i];
				}
			}
		}

		// mi sposto sul bottomleft
		pos += (transform->rectTransform->rect.bottomleft * (float)pixelPerUnit);

		// allineo il testo (si parte da bottomleft)
		switch (align) {
		case Align::TOPLEFT:
			pos.y += pixelRect.height - rowheight[0];
			break;

		case Align::TOPRIGHT:
			pos.y += pixelRect.height - rowheight[0];
			pos.x += pixelRect.width - rowwidth[0];
			break;

		case Align::BOTTOMLEFT:
			pos.y += height - rowheight[0];  // se è una sola riga height = rowheight[0]
			break;

		case Align::BOTTOMRIGHT:
			pos.y += height - rowheight[0];
			pos.x += pixelRect.width - rowwidth[0];
			break;

		case Align::LEFT:
			pos.y += (pixelRect.height / 2 + height / 2) - rowheight[0];
			break;

		case Align::RIGHT:
			pos.y += (pixelRect.height / 2 + height / 2) - rowheight[0];
			pos.x += pixelRect.width - rowwidth[0];
			break;

		case Align::TOP:
			pos.x += (pixelRect.width - rowwidth[0]) / 2;
			pos.y += pixelRect.height - rowheight[0];
			break;

		case Align::BOTTOM:
			pos.y += height - rowheight[0];
			pos.x += (pixelRect.width - rowwidth[0]) / 2;
			break;

		case Align::CENTER:
			pos.x += (pixelRect.width - rowwidth[0]) / 2;
			pos.y += (pixelRect.height / 2 + height / 2) - rowheight[0];
			break;

		case Align::CUSTOM:
		default:
			break;
		}

		// creo il textRect (debug)
		//textRect = {pos.x / (float)pixelPerUnit, pos.y / (float)pixelPerUnit, width / (float)pixelPerUnit, height / (float)pixelPerUnit};

		if (bottomAlign == BottomAlign::HEIGHT && (align == Align::BOTTOMLEFT || align == Align::BOTTOM || align == Align::BOTTOMRIGHT)) {
			baseline = false;
		}

		bool first = true;
		row = 0;
		float rwidth = 0.0f;

		// ciclo tutti i caratteri per stamparli
		for (p = ctext, i = 0; i < text.size(); ++p, ++i) {
			if (!font->LoadCharTexture(*p)) {
				continue;
			}

			float xAdjust = (first && alignToGeometry ? 0 : glyph->bitmap_left);
			float yAdjust = glyph->bitmap_top + (baseline ? 0 : baselineGap);

			float x2 = (pos.x + xAdjust * scale.x) / (float)pixelPerUnit;
			float y2 = -(pos.y + yAdjust * scale.y) / (float)pixelPerUnit;
			float w = (glyph->bitmap.width * scale.x) / (float)pixelPerUnit;
			float h = (glyph->bitmap.rows * scale.y) / (float)pixelPerUnit;

			// creo il box del carattere
			std::vector<glm::vec4> box{
				{x2, -y2, 0.0f, 0.0f},
				{x2 + w, -y2, 1.0f, 0.0f},
				{x2, -y2 - h, 0.0f, 1.0f},
				{x2 + w, -y2 - h, 1.0f, 1.0f}  //
			};

			first = false;

			if (*p != '\n') {
				rwidth += (glyph->advance.x / 64) * scale.x;

				VAO->Load<glm::vec4>(VertexBuffer::VERTICES, box);
				VAO->GetBuffer(VertexBuffer::VERTICES)->Bind();
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
				VAO->GetBuffer(VertexBuffer::VERTICES)->Unbind();
			}

			if (*p == '\n' || (wordWrap && words.find(i + 1) != words.end() && rwidth + words[i + 1] > pixelRect.width)) {  // c'è una nuova riga, imposto il testo successivo

				// imposto la x del testo successivo
				switch (align) {  // si parte da bottomleft
				case Align::TOPRIGHT:
					pos.x = leftpos + pixelRect.width - rowwidth[row + 1];
					break;
				case Align::BOTTOMRIGHT:
					pos.x = leftpos + pixelRect.width - rowwidth[row + 1];
					break;
				case Align::RIGHT:
					pos.x = leftpos + pixelRect.width - rowwidth[row + 1];
					break;
				case Align::TOP:
				case Align::BOTTOM:
					pos.x = leftpos + (pixelRect.width - rowwidth[row + 1]) / 2;
					break;
				case Align::CENTER:
					pos.x = leftpos + (pixelRect.width - rowwidth[row + 1]) / 2;
					break;
				case Align::CUSTOM:
				case Align::TOPLEFT:
				case Align::BOTTOMLEFT:
				case Align::LEFT:
					pos.x = leftpos;
					first = true;
				default:
					break;
				}

				// mi sposto alla riga successiva (imposto la y del testo successivo)
				pos.y -= (glyph->advance.y / 64) * scale.y + pixelLineHeight;
				row++;
				rwidth = 0;

			}
			else {  // non c'è una nuova riga, vado avanti su x
				pos.x += (glyph->advance.x / 64) * scale.x;
				pos.y += (glyph->advance.y / 64) * scale.y;
			}
		}

		lastText = text;
		glDeleteTextures(1, &tex);
	}

	void Text::Render() {
		if (!isReady() || text == L"") return;

		// currentLine = 0;
		// lastYpos = 0.0f;

		shader->Use();
		shader->SetMVP(transform->uMVP());
		shader->SetM(transform->uM());
		shader->SetRenderColor(color);
		shader->update();

		VAO->Use();
		renderText();
		VAO->Leave();
		shader->Leave();

		// RenderMode rm = entity->canvas != nullptr ? entity->canvas->renderMode : RenderMode::WORLD;
		// Design::DrawRect(Transform::VEC3_ZERO, textRect, {0.0f, 1.0f, 0.0f, 1.0f}, 1, DrawMode::HOLLOW, rm, transform->MVP);
	}

	Text* Text::SetText(std::wstring value, Color col) {
		_text = value;
		_color = col;
		return this;
	}

	Text* Text::SetText(std::wstring value) {
		_text = value;
		return this;
	}

	Text* Text::SetColor(Color value) {
		_color = value;
		return this;
	}

	Text* Text::SetFont(Font* value) {
		font = value;
		return this;
	}

	Text* Text::SetWordWrap(bool value) {
		_wordWrap = value;
		return this;
	}

	Text* Text::SetAlignToGeometry(bool value) {
		_alignToGeometry = value;
		return this;
	}

	Text* Text::SetScale(glm::fvec2 value) {  // scale locale del carattere (si aggiunge alla scale del transform, decidere se togliere, qual'è la migliore?)
		_scale = value;
		return this;
	}

	Text* Text::SetOffset(glm::fvec2 value) {
		_offset = value;
		return this;
	}

	Text* Text::SetSize(float unit_size) {
		font->SetSize(unit_size);
		return this;
	}

	Text* Text::SetSize(int pixel_size) {
		font->SetSize(pixel_size / (float)pixelPerUnit);
		return this;
	}

	Text* Text::SetBottomAlign(BottomAlign value) {
		_bottomAlign = value;
		return this;
	}

	Text* Text::SetAlign(Align value) {
		_align = value;
		return this;
	}

	Text* Text::SetPixelSize(int value) {
		font->SetPixelSize(value);
		return this;
	}

	Text* Text::SetLineHeight(float value) {
		_lineHeight = value;
		return this;
	}

	Text* Text::SetPixelPerUnit(unsigned int value) {
		_pixelPerUnit = value;
		return this;
	}

	Text* Text::Clear() {
		_text = L"";
		return this;
	}

}  // namespace se
