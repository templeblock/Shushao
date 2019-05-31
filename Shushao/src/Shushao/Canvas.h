#ifndef SE_CANVAS
#define SE_CANVAS

#include "sepch.h"

#include "Core.h"
#include "Camera.h"
#include "Config.h"
#include "Rect.h"
#include "Renderer.h"

namespace se {

	class Canvas : public Component {
	public:
		virtual void setup();
		~Canvas();

		const RenderMode& renderMode = _renderMode;
		Camera* camera = nullptr;

		std::vector<Component*> ChildRenderers;

		Canvas* SetRenderMode(RenderMode value);
		Canvas* SetCamera(Camera* camera_);

		void Awake();
		void Update();
		void Render();
		void OnDestroy();

		void Copy(Canvas* other);

	private:
		RenderMode _renderMode = RenderMode::WORLD;
		RenderMode _lastRenderMode;
		void processRenderMode();
	};

}  // namespace se

#endif
