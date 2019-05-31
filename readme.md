# Shushao Engine

## How to build

-   premake.bat

## Update submodules

first time:

```bash
git submodule init

```

dopo:

```bash
git submodule update --init --recursive
```

## Resources

### Resource::Load\<type\>(string filename, string name)

Carica una Resource da disco e estituisce un pointer alla risorsa. Se name non è specificato la chiave diventa il nome file senza percorso e senza estensione.

```c++
Texture* tex = Resources::Load<Texture>("assets/particlestar.png");
Resources::Load<Textfile>("assets/listadellaspesa.txt", "listasp");
```

### Resource::Load\<type\>(string name, int IDRES, string library)

Carica una Resource embedded nell'exe o da una dll

```c++
Texture* avatar = Resources::Load<Texture>("avatar", IMG_AVATAR, LIB_SHUSHAO);
```

### Resource::Add\<type\>(string name)

Aggiunge una Resource vuota.

```c++
Texture* tex = Resources::Add<Texture>();
Texture* tex = Resources::Add<Texture>("texname");
```

### Resource::Add(Asset\*)

Aggiunge una Resource già esistente alle resources.

```c++
Sprite* sprite = new Sprite();
Sprite* spr = Resources::Add(sprite);
```

### Resource::Get\<type\>(string name):

Ottiene una risorsa dal nome. Il nome corrisponde al filename senza percorso e estensione per i file caricati da disco, o il nome specificato dal metodo Add.

```c++
Sprite* spr = Resources::Get<Sprite>("ominobuffo");
```

### std::vector\<char\> GetEmbeddedData(int IDRES, string library, LPCSTR type)

Recupera una risorsa embedded nell'eseguibile o in una dll esterna, senza aggiungerla alle resources.

```c++
Resources::GetEmbeddedData(IMG_AVATAR);
Resources::GetEmbeddedData(GONFIG_FILE, "libshushao.dll");
```

### std::string GetEmbeddedText(int IDRES, string library)

Recupera una risorsa embedded nell'eseguibile o in una dll esterna, senza aggiungerla alle resources.

```c++
Resources::GetEmbeddedText(JSON_OBJ, "libshushao.dll");
```

## Components

### Sprite Renderer

### Mesh Renderer

### Light

### Transform

### Canvas

### Text

```c++
// componente text su una normale entity
Entity* label = AddEntity("Label");
Text* text = label->AddComponent<Text>();
text->sortingLayerID = Config::SortingLayers["UI"];
text->alignToGeometry = true;
text->SetFont(f)->SetText("Hello game engine!")->SetColor(color::blue)->SetSize(0.5f);
text->transform->SetPivot(PivotPosition::TOPLEFT);
text->transform->rectTransform->SetRectSize({8,3});
text->transform->SetLocalPosition({5, -1, 0});

// Componente text in una canvas
Text* text1 = canvas->AddComponent<Text>();
text1->alignToGeometry = true;
text1->sortingLayerID = Config::SortingLayers["Background"];
text1->SetFont(f)->SetText("Hello game engine!")->SetColor(color::red)->SetSize(0.5f);
text1->SetAlign(Align::TOPLEFT);
```

### Raw Image

### Image

## UI

### Canvas

```c++
Entity* canvas = AddEntity<ui::Canvas>("Canvas");
canvas->GetComponent<Canvas>()->SetRenderMode(RenderMode::WORLD);
canvas->transform->rectTransform->SetRectSize({10,4});
canvas->transform->SetLocalPosition({-1, 0, 0});
canvas->transform->SetPivot(PivotPosition::CENTER);
```

### Text

```c++
// Label in una canvas ancorata a un singolo punto
ui::Text* uiText = AddEntity<ui::Text>("Label su canvas");
uiText->SetParent(canvas);
uiText->transform->SetPivot(PivotPosition::BOTTOMLEFT);
uiText->transform->rectTransform->SetAnchor({0.5f, 0.5f});
uiText->transform->rectTransform->SetRectSize({4, 1});
uiText->transform->SetLocalPosition({-2, -2, 0});
uiText->transform->SetPivot(PivotPosition::BOTTOMRIGHT);
auto tComp = uiText->GetComponent<Text>();
tComp->SetFont(f)->SetText("Label su canvas")->SetColor(color::green)->SetSize(0.5f)->SetAlign(Align::CENTER);

// Text label in una child entity - ancorata diversamente
ui::Text* anchoredText = AddEntity<ui::Text>("Label 2");
anchoredText->SetParent(canvas);
anchoredText->transform->SetPivot(PivotPosition::CENTER);
anchoredText->transform->rectTransform->SetAnchorMax({1, 1});
anchoredText->transform->rectTransform->SetAnchorMin({0, 1});
anchoredText->transform->rectTransform->RegisterPositionLRYH(1.0f, 1.0f, -1.0f, 1.0f);

```

### Raw Image

### Image

https://guides.github.com/features/mastering-markdown/
alt + 96
