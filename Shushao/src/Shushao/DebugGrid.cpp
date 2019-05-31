#include "sepch.h"

#include "Color.h"
#include "Debug.h"
#include "GLManager.h"
#include "GeometryRenderer.h"
#include "SceneManager.h"

namespace se {

	void DebugGrid::Awake() {
		GeometryRenderer* LR = AddComponent<GeometryRenderer>();
		LR->sortingLayerID = Config::SortingLayers.over();

		Color xcol = { 0.0f, 1.0f, 0.0f, 0.5f };
		Color ycol = { 1.0f, 0.0f, 0.0f, 0.5f };
		Color zcol = { 0.0f, 0.0f, 1.0f, 0.5f };
		Color gcol = { 0.3f, 0.3f, 0.5f, 0.07f };

		Color gco = gcol;
		float alpha = 0.05f;
		for (float i = 0.0f; i < gridSize; i++) {
			alpha -= 0.005f;
			gco.a = alpha;

			if (fmod(i, 4.0f) == 0 || i == 0.0f) {
				gco.a *= 3.0f;
			}

			if (Debug::debugGridMode == GridMode::ORTHOGRAFIC) {
				// parallele asse Y lungo X
				LR->AddLine({ i, -gridSize, 0.0f }, { i, gridSize, 0.0f }, gco);
				LR->AddLine({ -i, -gridSize, 0.0f }, { -i, gridSize, 0.0f }, gco);

				// parallele asse X lungo Y
				LR->AddLine({ -gridSize, i, 0.0f }, { gridSize, i, 0.0f }, gco);
				LR->AddLine({ -gridSize, -i, 0.0f }, { gridSize, -i, 0.0f }, gco);
			}
			else {
				//parallele asse X lungo Z
				LR->AddLine({ -gridSize, 0.0f, i }, { gridSize, 0.0f, i }, gco);
				LR->AddLine({ -gridSize, 0.0f, -i }, { gridSize, 0.0f, -i }, gco);

				//parallele asse Z lungo X
				LR->AddLine({ i, 0.0f, -gridSize }, { i, 0.0f, gridSize }, gco);
				LR->AddLine({ -i, 0.0f, -gridSize }, { -i, 0.0f, gridSize }, gco);
			}
		}

		float arrowWidth = gizmoMagnitude / 18;

		LR->AddLine({ 0.0f, 0.0f, 0.0f }, { 0.0f, gizmoMagnitude, 0.0f }, xcol);
		LR->AddLine({ 0.0f, 0.0f, 0.0f }, { gizmoMagnitude, 0.0f, 0.0f }, ycol);
		LR->AddLine({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, gizmoMagnitude }, zcol);

		LR->AddLine({ 0.0f, gizmoMagnitude, 0.0f }, { arrowWidth, gizmoMagnitude - arrowWidth, 0.0f }, xcol);
		LR->AddLine({ 0.0f, gizmoMagnitude, 0.0f }, { -arrowWidth, gizmoMagnitude - arrowWidth, 0.0f }, xcol);

		LR->AddLine({ gizmoMagnitude, 0.0f, 0.0f }, { gizmoMagnitude - arrowWidth, arrowWidth, 0.0f }, ycol);
		LR->AddLine({ gizmoMagnitude, 0.0f, 0.0f }, { gizmoMagnitude - arrowWidth, -arrowWidth, 0.0f }, ycol);

		if (Debug::debugGridMode != GridMode::ORTHOGRAFIC) {
			LR->AddLine({ 0.0f, 0.0f, gizmoMagnitude }, { arrowWidth, 0.0f, gizmoMagnitude - arrowWidth }, zcol);
			LR->AddLine({ 0.0f, 0.0f, gizmoMagnitude }, { -arrowWidth, 0.0f, gizmoMagnitude - arrowWidth }, zcol);
		}

		// Screen canvas
		/* GeometryRenderer* scr = AddComponent<GeometryRenderer>();
			scr->renderMode = RenderMode::SCREEN;

			for (float i = 0.0f; i < gridSize; i++) {

				int p = i;//  * Config::pixelPerUnit;
				scr->AddLine({p, 0.0f, 0.0f}, {p, 0.1f, 0.0f}, ycol);
				scr->AddLine({0.0f, p, 0.0f}, {0.1f, p, 0.0f}, xcol);

				scr->AddLine({p, GLManager::VIEWPORT.y, 0.0f}, {p, GLManager::VIEWPORT.y - 0.1f, 0.0f}, ycol);
				scr->AddLine({GLManager::VIEWPORT.x, p, 0.0f}, {GLManager::VIEWPORT.x - 0.1f, i, 0.0f}, xcol);
			} */
	}

}  // namespace se
