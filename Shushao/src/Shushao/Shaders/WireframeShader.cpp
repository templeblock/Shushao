#include "sepch.h"

#include "WireframeShader.h"
#include "Shushao/Resources.h"

namespace se {

	WireframeShader::WireframeShader() {
		name = "Wireframe Shader";

		VertexShaderCode = Resources::GetEmbeddedText(WIREFRAMESHADER_VERT, LIB_SHUSHAO);
		FragmentShaderCode = Resources::GetEmbeddedText(WIREFRAMESHADER_FRAG, LIB_SHUSHAO);
	}

}  // namespace se
