#include "sepch.h"

#include <glad/glad.h>

#include "GLManager.h"
#include "Application.h"
#include "Config.h"
#include "InputInterface.h"

namespace se {

	bool GLManager::Init() {
		WIDTH = Config::displayWidth;
		HEIGHT = Config::displayHeight;

		// SDL_Init(SDL_INIT_EVERYTHING );  //  | SDL_INIT_JOYSTICK
		// SDL_Init(SDL_INIT_AUDIO);
		//IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

		// audio init
		//Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG);
		//Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
		//Mix_AllocateChannels(16);

		// fonts freetype init
		FT_Init_FreeType(&lFreetype);

#if 0

		SDL_GL_SetSwapInterval(1);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		/* SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
			SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4); */
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		SDL_DisplayMode dm;
		SDL_GetDesktopDisplayMode(0, &dm);
		DESKTOP_WIDTH = dm.w;
		DESKTOP_HEIGHT = dm.h;

		if (fullscreen) {
			gWindow = SDL_CreateWindow(title.c_str(), 0, 0, DESKTOP_WIDTH, DESKTOP_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP);
		}
		else {
			gWindow = SDL_CreateWindow(title.c_str(), DESKTOP_WIDTH / 2 - WIDTH / 2, DESKTOP_HEIGHT / 2 - HEIGHT / 2, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
		}

		SDL_SetWindowResizable(gWindow, SDL_TRUE);

		//gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

		gContext = SDL_GL_CreateContext(gWindow);
		SDL_GL_MakeCurrent(gWindow, gContext);

		// Init GLEW
		// glewExperimental = GL_TRUE;
		// glewInit();

#endif

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glClearDepth(1.0f);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_ALWAYS);  // questo per il 2d
		// glDepthFunc(GL_LEQUAL); // senza questo il 3d non funziona

		// glShadeModel(GL_SMOOTH);
		// glEnable(GL_MULTISAMPLE);
		// glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		// glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
		// glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
		// glEnable(GL_LINE_SMOOTH);
		// glEnable(GL_POLYGON_SMOOTH);

		Update();

		ready = true;
		return true;
	}

	void GLManager::Update() {
		VIEWPORT = { (float)Config::displayWidth / (Config::pixelPerUnit / 2), (float)Config::displayHeight / (Config::pixelPerUnit / 2) };
		ASPECT = (float)WIDTH / (float)HEIGHT;
	}

	void GLManager::Quit() {
		// Mix_CloseAudio();
		// Mix_Quit();
		// SDL_Quit();
	}

	// inizialize static properties

	FT_Library GLManager::lFreetype = nullptr;

	bool GLManager::fullscreen = false;
	bool GLManager::ready = false;

	unsigned int GLManager::DESKTOP_WIDTH;
	unsigned int GLManager::DESKTOP_HEIGHT;
	unsigned int GLManager::WIDTH;
	unsigned int GLManager::HEIGHT;
	glm::vec2 GLManager::VIEWPORT;
	float GLManager::ASPECT;

}  // namespace se
