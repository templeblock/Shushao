#include "sepch.h"
#include "Core.h"

#include "Application.h"
#include "Config.h"
#include "Debug.h"
#include "Design.h"
#include "Events/ApplicationEvent.h"
#include "Font.h"
#include "GLManager.h"
#include "Input.h"
#include "Physics/Physics.h"
#include "Resources.h"
#include "SceneManager.h"
#include "System.h"
#include "Time.h"

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

namespace se {

	Application* Application::instance = nullptr;

	Application::~Application() {
		DEBUG_CORE_INFO("Application Destructor");
	}

	bool Application::Init() {
		for (bool& k : keys) {
			k = false;
		}

		Debug::Init();

		// Load engine configuration files
		if (!Config::LoadEngineConfig()) {
			DEBUG_CORE_ERROR("Error Initializing Engine Configuration");
			::exit(5);
		}

		// Load user configuration files
		if (!Config::LoadUserConfig()) {
			DEBUG_CORE_ERROR("Error Initializing User Configuration");
		}

		// Load configuration overrides
		if (!InitConfig()) {
			DEBUG_CORE_ERROR("Error Initializing Configuration overrides");
		}

		// Init Window
		window = std::unique_ptr<Window>(Window::Create({
			Config::title,
			Config::displayWidth,
			Config::displayHeight,
			Config::fullscreen  //
			}));
		window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		// OpenGL Context init
		if (!GLManager::Init()) {
			DEBUG_CORE_ERROR("Error Initializing GL");
			::exit(5);
		}

		// load resoruces (derived)
		if (!InitResources()) {
			DEBUG_CORE_ERROR("Error loading resources");
		}

		// engine resources
		Resources::Load<Font>("consolas", FONT_CONSOLAS, LIB_SHUSHAO);

		// Time init
		Time::setFrameRateLimit(Config::Time::frameRateLimit);
		Time::setFixedRateLimit(Config::Time::fixedRateLimit);

		// Physics 2d init
		if (Config::Physics::enabled) {
			if (!Physics::Init()) {
				DEBUG_CORE_ERROR("Error Initializing Physics");
			}
		}
		// Init Input service
		// Input::init();

		// Init System services
		System::init();

		// Init input mappings (derived)
		if (!InitMapping()) {
			DEBUG_CORE_ERROR("Error In input mapping");
		}

		// init and load starting scene (derived)
		if (!InitScene()) {
			DEBUG_CORE_ERROR("Error Initializing Scene");
		}

		// Awake method (derived)
		Awake();

		// init all entities
		SceneManager::activeScene->ScanEntities();
		SceneManager::activeScene->ScanActiveComponents();

		SceneManager::activeScene->init();  // vengono chiamati qui gli Awake di tutti gli oggetti attivi

		if (Debug::enabled) {
			SceneManager::activeScene->PrintHierarchy();
			SceneManager::activeScene->PrintActiveComponentsInScene();
			SceneManager::activeScene->PrintActiveRenderersInScene();
			SceneManager::activeScene->PrintActiveLightsInScene();
			System::ListServices();
			SceneManager::activeScene->activeCamera->print();
			Resources::toString();
			Config::Layers.toString("Layers");
			Config::SortingLayers.toString("SortingLayers");
		}

		Camera* activeCamera = SceneManager::activeScene->activeCamera;

		window->Clear(activeCamera->backgroundColor.r, activeCamera->backgroundColor.g, activeCamera->backgroundColor.b, 1.0f, 1.0f);

		return true;
	}

	void Application::Run() {
		if (!SceneManager::sceneSet)
			return;

		// Start method (derived)
		Start();

		while (RUNNING) {
			Time::Update();

			std::thread tInput;
			std::thread tUpdate;
			std::thread tFixed;

			// scan scene (se invalid) e init objects
			InitScan();

			// Process input method (derived)
			//tInput = std::thread(&Application::GetInput, this);
			GetInput();

			// Update Application
			//tUpdate = std::thread(&Application::update, this);
			update();

			if (Time::fixedDeltaTime >= Time::fixedLimitDuration) {
				// Fixed Update Application (physics)
				//tFixed = std::thread(&Application::fixed, this);
				fixed();
			}

			if (tInput.joinable()) tInput.join();
			if (tUpdate.joinable()) tUpdate.join();
			if (tFixed.joinable()) tFixed.join();

			if (Config::Time::frameRateLimit == 0 || (Time::renderDeltaTime >= Time::frameLimitDuration)) {
				// Main render cycle
				render();
			}
		}

		exit();
	}

	void Application::Stop() {
		RUNNING = false;
	}

	void Application::InitScan() {
		if (SceneManager::activeScene->invalid) {
			SceneManager::activeScene->ScanEntities();
			SceneManager::activeScene->ScanActiveComponents();
		}

		// chiamo awake di tutti i componenti non ancora svegli
		SceneManager::activeScene->init();
	}

	void Application::render() {
		Time::renderTime = Time::GetTime();
		window->Clear();
		window->OnUpdate();
		SceneManager::activeScene->render();
		Render();  // (derived)
		SceneManager::activeScene->renderOverlay();
		// if (Physics::enabled && Physics::debug) ((b2World*)Physics::GetWorld())->DrawDebugData();
		Time::frameCount++;
	}

	void Application::update() {
		Time::realtimeSinceStartup = Time::GetTime();
		// Input::update();  // Update Input Service
		System::update();  // update dei system services
		GLManager::Update();
		SceneManager::activeScene->update();
		Update();  // (derived)
	}

	void Application::fixed() {
		Time::fixedTime = Time::GetTime();
		Time::inFixedTimeStep = true;
		if (Physics::enabled) Physics::Update();
		SceneManager::activeScene->fixed();
		FixedUpdate();  // (derived)
		Time::inFixedTimeStep = false;
	}

	void Application::exit() {
		End();  // (derived)
		SceneManager::activeScene->exit();
		// Input::exit();
		System::exit();
		Physics::Exit();
		SceneManager::Clear();
		Resources::Clear();
		System::Clear();
		GLManager::Quit();
	}

	void Application::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		// TODO: capire sta cosa dei layer

		/* for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		} */
	}

	bool Application::OnWindowClose(WindowCloseEvent& e) {
		RUNNING = false;
		return true;
	}

}  // namespace se
