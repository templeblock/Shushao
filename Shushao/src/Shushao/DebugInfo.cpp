#include "sepch.h"

#include "DebugInfo.h"
#include "Resources.h"
#include "Utility.h"
#include "Color.h"
#include "UI/TextUI.h"

namespace se {

	void DebugInfo::Awake() {
		Font* font = Resources::Get<Font>("consolas");

		ui::TextUI* uiText = entity->AddChild<ui::TextUI>("Benchmark");
		uiText->transform->SetPivot(PivotPosition::TOPLEFT);
		uiText->transform->rectTransform->SetAnchor(AnchorPreset::TOPLEFT);
		uiText->transform->rectTransform->SetRectSize({ 4, 6 });
		uiText->transform->SetLocalPosition({ 1, -1, 0 });

		times = uiText->GetComponent<Text>();
		times->sortingLayerID = Config::SortingLayers.over();
		times->SetFont(font)->SetColor(color::red)->SetSize(22)->SetWordWrap(true)->SetAlign(Align::RIGHT)->SetLineHeight(1.2f);
		times->SetText(L"RIGHT\nQuesta è una prova di\nQuesta è una  di scrittura\nQuesta è una prova scrittura\nQuesta è una prova di scrittura\n");

		Text* times2 = uiText->AddComponent<Text>();
		times2->sortingLayerID = Config::SortingLayers.over();
		times2->SetFont(font)->SetColor(color::yellow)->SetSize(22)->SetWordWrap(true)->SetAlign(Align::LEFT)->SetLineHeight(1.2f);
		times2->SetText(L"LEFT\nQuesta è una prova di\nQuesta è una  di scrittura\nQuesta è una prova scrittura\nQuesta è una prova di scrittura\n");

		Text* times3 = uiText->AddComponent<Text>();
		times3->sortingLayerID = Config::SortingLayers.over();
		times3->SetFont(font)->SetColor(color::blue)->SetSize(22)->SetWordWrap(true)->SetAlign(Align::TOPRIGHT)->SetLineHeight(1.2f);
		times3->SetText(L"TOPRIGHT\nQuesta è una prova di\nQuesta è una  di scrittura\nQuesta è una prova scrittura\nQuesta è una prova di scrittura\n");

		Text* times4 = uiText->AddComponent<Text>();
		times4->sortingLayerID = Config::SortingLayers.over();
		times4->SetFont(font)->SetColor(color::cyan)->SetSize(22)->SetWordWrap(true)->SetAlign(Align::BOTTOMRIGHT)->SetLineHeight(1.0f);
		times4->SetText(L"BOTTOMRIGHT\nQuesta è una prova di\nQuesta è una  di scrittura\nQuesta è una prova scrittura\nQuesta è una prova di scrittura\n");

		Text* times5 = uiText->AddComponent<Text>();
		times5->sortingLayerID = Config::SortingLayers.over();
		times5->SetFont(font)->SetColor(color::white)->SetSize(22)->SetWordWrap(true)->SetAlign(Align::TOPLEFT)->SetLineHeight(1.0f);
		times5->SetText(L"TOPLEFT\nQuesta è una prova di\nQuesta è una  di scrittura\nQuesta è una prova scrittura\nQuesta è una prova di scrittura\n");

		Text* times6 = uiText->AddComponent<Text>();
		times6->sortingLayerID = Config::SortingLayers.over();
		times6->SetFont(font)->SetColor(color::white)->SetSize(22)->SetWordWrap(true)->SetAlign(Align::BOTTOMLEFT)->SetLineHeight(1.0f);
		times6->SetText(L"BOTTOMLEFT\nQuesta è una prova di\nQuesta è una  di scrittura\nQuesta è una prova scrittura\nQuesta è una prova di scrittura\n");

		Text* times7 = uiText->AddComponent<Text>();
		times7->sortingLayerID = Config::SortingLayers.over();
		times7->SetFont(font)->SetColor(color::white)->SetSize(22)->SetWordWrap(true)->SetAlign(Align::CENTER)->SetLineHeight(1.0f);
		times7->SetText(L"CENTER\nQuesta è una prova di\nQuesta è una  di scrittura\n");
	}

	void DebugInfo::Update() {
		return;
		std::wstring fr = std::wstring(L"Frame Rate: ") + util::toWstring(Time::frameRate) + std::wstring(L"\n");
		std::wstring ut = std::wstring(L"Update Time: ") + util::toWstring(Time::deltaTime) + std::wstring(L"\n");
		std::wstring ft = std::wstring(L"Fixed Time: ") + util::toWstring(Time::fixedDeltaTime) + std::wstring(L"\n");
		std::wstring rt = std::wstring(L"Render Time: ") + util::toWstring(Time::renderDeltaTime) + std::wstring(L"\n");

		times->SetText(fr + ut + ft + rt);
	}

}  // namespace se
