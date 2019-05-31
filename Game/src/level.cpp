#include "level.h"
#include "coroutinetest.h"
#include "globals.h"
#include "hermite.h"
#include "meshtest.h"
#include "mover.h"
#include "pancrazio.h"
#include "part.h"
#include "starship.h"

using namespace se;

Level::Level() {
    testParticles();
    testSprites();
    testCoroutines();
    testCanvas();
}

void Level::testParticles() {
    Part* po2 = AddEntity<Part>("Particle Object 2");
    po2->transform->SetLocalPosition(glm::vec3(2, 2, 0));
    po2->GetComponent<ParticleSystem>()->sortingLayerID = 1;

    Part* po = AddEntity<Part>("Particle Object");
    po->GetComponent<ParticleSystem>()->sortingLayerID = 1;

    // AddEntity<Hermite>();
    // Part* po3 = (Part*)Factory::InstantiateEntity(po);
    ParticleSystem* pops = po->GetComponent<ParticleSystem>();
    ParticleSystem* ps = (ParticleSystem*)Factory::InstantiateComponent<Part>(pops, glm::vec3(-4, -4, 0));
    ps->sortingLayerID = 2;
}

void Level::testMesh() {
    // AddEntity<Meshtest>();
}

void Level::testCoroutines() {
    Entity* test = AddEntity("Test Coroutines");
    test->AddComponent<CoroutineTest>();
}

void Level::testSprites() {
    Entity* ground = AddEntity("Ground");

    ground->transform->SetLocalPosition({0.0f, -4.0f, 0.0f});
    Rigidbody2D* groundrb = ground->AddComponent<Rigidbody2D>();
    groundrb->SetType(RigidbodyType::STATIC);
    BoxCollider2D* ec = ground->AddComponent<BoxCollider2D>();
    ec->SetShape({6, 0.5f});

    Entity* door = AddEntity("Door");
    door->transform->SetLocalPosition({4.0f, -4.0f, 0.0f});
    Rigidbody2D* doorrb = door->AddComponent<Rigidbody2D>();
    doorrb->SetType(RigidbodyType::STATIC);
    BoxCollider2D* dc = door->AddComponent<BoxCollider2D>();
    dc->SetSensor(true);
    dc->SetShape({0.1, 4.0f});

    //ground->transform->SetLocalRotation({0,0,20});

    Pancrazio* pancrazio = AddEntity<Pancrazio>();
    //pancrazio->transform->SetParent(container->transform);
    pancrazio->transform->SetLocalScale({1.0f, 1.0f, 0.0f});
    pancrazio->transform->SetLocalPosition({0.0f, 0.0f, 0.0f});
    //pancrazio->GetComponent<Animation>()->Disable();
    pancrazio->transform->SetLocalRotation({0, 0, 30});

    pancrazio->AddProgram<Mover>();

    Pancrazio* sonOfPancrazio = AddEntity<Pancrazio>("Son of Pancrazio");
    sonOfPancrazio->SetParent(pancrazio);
    //sonOfPancrazio->transform->isFree = true;
    //sonOfPancrazio->GetComponent<Animation>()->Disable();
    //sonOfPancrazio->GetComponent<Mover>()->Disable();
    sonOfPancrazio->transform->SetLocalPosition({-1.0f, 0.0f, 0.0f});
    sonOfPancrazio->transform->SetLocalScale({0.5f, 0.5f, 0.0f});
    sonOfPancrazio->GetComponent<Rigidbody2D>()->SetType(RigidbodyType::STATIC);
    sonOfPancrazio->GetComponent<BoxCollider2D>("box")->ResetShape();  // <---- collegare al transform addirittura? fare dei callback transformChanged
    sonOfPancrazio->GetComponent<BoxCollider2D>("trigger")->ResetShape();  // <---- collegare al transform addirittura? fare dei callback transformChanged
    //sonOfPancrazio->GetComponent<CircleCollider2D>()->ResetShape(); // <---- collegare al transform addirittura? fare dei callback transformChanged
    //sonOfPancrazio->transform->setLocalRotation({0,0,20});

    Entity* bkg = AddEntity("Background");
    //bkg->transform->SetLocalPosition({0.0f, 0.0f, -0.5f});
    SpriteRenderer* backgroundRenderer = bkg->AddComponent<SpriteRenderer>();
    // backgroundRenderer->shader = Resources::Get<Shader>("standard");
    backgroundRenderer->name = "Background Renderer";
    backgroundRenderer->sortingLayerID = Config::SortingLayers["Background"];
    backgroundRenderer->sprite = new Sprite("Background", Resources::Get<Texture>("night"));
    backgroundRenderer->sprite->SetPixelPerUnit(64);
    backgroundRenderer->sprite->Build();
}

void Level::testCanvas() {
    Font* f = Resources::Get<Font>("modenine");

    Entity* label = AddEntity("Label");
    Text* text = label->AddComponent<Text>();
    text->sortingLayerID = Config::SortingLayers["UI"];
    text->SetAlignToGeometry(true);
    text->SetFont(f)->SetText(L"Hello game engine!")->SetColor(color::blue)->SetSize(0.5f);
    text->transform->SetPivot(PivotPosition::TOPLEFT);
    text->transform->rectTransform->SetRectSize({8, 3});
    text->transform->SetLocalPosition({5, -1, 0});
    //text->transform->SetLocalScale({2,2,0});
    text->SetAlign(Align::TOPLEFT);

    Camera* screenSpaceCamera = root->AddComponent<Camera>("Screen Space Camera");
    screenSpaceCamera->setOrthographic(true);
    screenSpaceCamera->setNearClipPlane(12.0f);
    screenSpaceCamera->setFarClipPlane(-1.0f);
    screenSpaceCamera->setOrthographicSize(5.0f);
    screenSpaceCamera->transform->localPosition = {0.0f, 0.0f, 10.0f};
    screenSpaceCamera->transform->localRotation = Transform::QUATERNION_IDENTITY;

    Entity* canvas = AddEntity<ui::CanvasUI>("Canvas");

    canvas->GetComponent<Canvas>()->SetCamera(screenSpaceCamera)->SetRenderMode(RenderMode::WORLD);
    canvas->transform->rectTransform->SetRectSize({10, 4});
    canvas->transform->SetLocalPosition({-1, 0, 0});
    canvas->transform->SetPivot(PivotPosition::CENTER);
    // Componente text direttamente agganciato alla canvas
    Text* text1 = canvas->AddComponent<Text>();
    text1->SetAlignToGeometry(true);
    text1->sortingLayerID = Config::SortingLayers["Background"];
    text1->SetFont(f)->SetText(L"TL")->SetColor(color::red)->SetSize(0.5f);
    text1->SetAlign(Align::TOPLEFT);
    Text* text2 = canvas->AddComponent<Text>();
    text2->SetAlignToGeometry(true);
    text2->sortingLayerID = Config::SortingLayers["Background"];
    text2->SetFont(f)->SetText(L"TR")->SetColor(color::red)->SetSize(0.5f);
    text2->SetAlign(Align::TOPRIGHT);
    Text* text3 = canvas->AddComponent<Text>();
    text3->SetAlignToGeometry(true);
    text3->sortingLayerID = Config::SortingLayers["Background"];
    text3->SetFont(f)->SetText(L"BR")->SetColor(color::red)->SetSize(0.5f);
    text3->SetAlign(Align::BOTTOMRIGHT);
    Text* text4 = canvas->AddComponent<Text>();
    text4->SetAlignToGeometry(true);
    text4->sortingLayerID = Config::SortingLayers["Background"];
    text4->SetFont(f)->SetText(L"BL")->SetColor(color::red)->SetSize(0.5f);
    text4->SetAlign(Align::BOTTOMLEFT);

    Text* text5 = canvas->AddComponent<Text>();
    text5->SetAlignToGeometry(true);
    text5->sortingLayerID = Config::SortingLayers["Background"];
    text5->SetFont(f)->SetText(L"CE")->SetColor(color::red)->SetSize(0.5f);
    text5->SetAlign(Align::CENTER);

    Text* text6 = canvas->AddComponent<Text>();
    text6->sortingLayerID = Config::SortingLayers["Background"];
    text6->SetFont(f)->SetText(L"L")->SetColor(color::red)->SetSize(0.5f);
    text6->SetAlign(Align::LEFT);

    Text* text7 = canvas->AddComponent<Text>();
    text7->sortingLayerID = Config::SortingLayers["Background"];
    text7->SetFont(f)->SetText(L"R")->SetColor(color::red)->SetSize(0.5f);
    text7->SetAlign(Align::RIGHT);

    Text* text8 = canvas->AddComponent<Text>();
    text8->sortingLayerID = Config::SortingLayers["Background"];
    text8->SetFont(f)->SetText(L"T")->SetColor(color::red)->SetSize(0.5f);
    text8->SetAlign(Align::TOP);

    Text* text9 = canvas->AddComponent<Text>();
    text9->sortingLayerID = Config::SortingLayers["Background"];
    text9->SetFont(f)->SetText(L"B")->SetColor(color::red)->SetSize(0.5f);
    text9->SetAlign(Align::BOTTOM);

    // compoenente text in una child entity
    ui::TextUI* uiText = AddEntity<ui::TextUI>("Label su canvas");
    uiText->SetParent(canvas);
    uiText->transform->SetPivot(PivotPosition::BOTTOMLEFT);
    uiText->transform->rectTransform->SetAnchor(AnchorPreset::CENTER);
    uiText->transform->rectTransform->SetRectSize({2, 1});
    uiText->transform->SetLocalPosition({-2, -2, 0});
    uiText->transform->SetPivot(PivotPosition::BOTTOMRIGHT);
    auto tComp = uiText->GetComponent<Text>();
    tComp->SetWordWrap(false);
    tComp->SetFont(f)->SetText(L"Label su canvas")->SetColor(color::green)->SetSize(0.5f)->SetAlign(Align::BOTTOMRIGHT);

    // compoenente text in una child entity - ancorata diversamente
    ui::TextUI* anchoredText = AddEntity<ui::TextUI>("Label 2");
    anchoredText->SetParent(canvas);
    anchoredText->transform->SetPivot(PivotPosition::CENTER);

    anchoredText->transform->rectTransform->SetAnchor(AnchorPreset::TOPSTRETCH);
    anchoredText->transform->rectTransform->RegisterPositionLRYH(1.0f, 1.0f, -1.0f, 1.0f);

    /* anchoredText->transform->rectTransform->SetAnchor(AnchorPreset::STRETCH);
	anchoredText->transform->rectTransform->RegisterPositionLRTB(1.0f, 1.0f, 1.0f, 1.0f); */

    /* anchoredText->transform->rectTransform->SetAnchorMax({1, 1});
	anchoredText->transform->rectTransform->SetAnchorMin({0, 0});
	anchoredText->transform->rectTransform->RegisterPositionLRTH(1.0f, 1.0f, 1.0f, 4.0f); */

    /* anchoredText->transform->rectTransform->SetAnchorMax({0, 1});
	anchoredText->transform->rectTransform->SetAnchorMin({0, 0});
	anchoredText->transform->rectTransform->RegisterPositionTBXW(1.0f, 1.0f, 2.0f, 3.0f); */

    auto t = anchoredText->GetComponent<Text>();
    t->SetFont(f)->SetText(L"Con punti ancoraggio")->SetColor(color::yellow)->SetSize(0.3f)->SetAlign(Align::CENTER);

    /* ui::RawImage* uiImage = AddEntity<ui::RawImage>("Immagine su canvas");
	uiImage->SetParent(canvas);

	RawImage* image1 = uiImage->GetComponent<RawImage>();
	image1->SetTexture(Resources::Get<Texture>("avatar"));
	image1->transform->rectTransform->SetAnchorMax({1, 1});
	image1->transform->rectTransform->SetAnchorMin({0, 0});
	image1->transform->rectTransform->RegisterPositionLRTB(1.0f, 1.0f, 1.0f, 1.0f);
	image1->SetUvRect({0.0f, 0.0f, 1.0f, 1.0f})->SetColor({0.5f, 0.1f, 0.1f, 0.4f});
	image1->sortingLayerID = Config::SortingLayers["UI"]; */

    //ui::Image* uiImage2 = AddEntity<ui::Image>("Immagine su canvas");
    //uiImage2->SetParent(canvas);
    //
    //Image* image2 = uiImage2->GetComponent<Image>();
    //image2->SetSprite(Resources::Get<Sprite>("eye_sprite"));
    ///* image2->transform->rectTransform->SetAnchor({0.5f, 0.5f});
    //image2->transform->rectTransform->SetRectSize({6, 3});
    //image2->transform->SetLocalPosition({3, 1, 0});*/
    //image2->transform->rectTransform->SetAnchorMax({1, 1});
    //image2->transform->rectTransform->SetAnchorMin({0, 0});
    //image2->transform->rectTransform->RegisterPositionLRTB(1.0f, 1.0f, 1.0f, 1.0f);
    //image2->sortingLayerID = Config::SortingLayers["UI"];
    // image2->SetPreserveAspect(false);

    ui::ImageUI* sliced = AddEntity<ui::ImageUI>("Immagine sliced");
    sliced->SetParent(canvas);

    Image* imagesl = sliced->GetComponent<Image>();
    imagesl->SetSprite(Resources::Get<Sprite>("dialog_sprite"));
    imagesl->transform->rectTransform->SetAnchor(AnchorPreset::STRETCH);
    imagesl->transform->rectTransform->RegisterPositionLRTB(1.0f, 1.0f, 1.0f, 1.0f);
    imagesl->sortingLayerID = Config::SortingLayers["UI"];
    imagesl->SetPreserveAspect(false)->SetImageType(Image::Type::TILED)->SetBorder({40, 40, 40, 40})->SetFillCenter(false);
}
