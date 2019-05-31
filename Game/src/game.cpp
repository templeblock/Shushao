#include "globals.h"
#include "level.h"

using namespace se;

class Game : public Application {
    bool perspectiveTest = false;

    /*
	    [INIT] RESOURCE LOADING
    */
    bool InitResources() {
        //Resources::Load<Textfile>("assets/cube.obj", "textcube");
        Resources::Load<Texture>("avatar", IMG_AVATAR, LIB_SHUSHAO);
        Resources::Load<Texture>("assets/particlestar.png");
        Resources::Load<Texture>("assets/pancrazio.png");
        Resources::Load<Texture>("assets/dialog.png");
        Resources::Load<Texture>("assets/night.jpg");
        Resources::Load<Texture>("assets/eye.jpg");
        Resources::Load<Texture>("assets/pancsmile.png");
        Resources::Load<TextureAtlas>("assets/walking.png")->AddGrid(glm::vec2(16, 16), Align::BOTTOM);
        Resources::Add<Sprite>("pancrazio_sprite")->SetTexture(Resources::Get<Texture>("pancrazio"))->SetPixelPerUnit(16)->Build();
        Resources::Add<Sprite>("eye_sprite")->SetTexture(Resources::Get<Texture>("eye"))->SetPixelPerUnit(200)->Build();

        Resources::Load<Mesh>("assets/cube.obj");
        Resources::Load<Texture>("assets/cube_diffuse.png");
        Resources::Load<Texture>("assets/cube_diffuse_brutto.png");
        Resources::Load<Texture>("assets/cube_specular.png");
        Resources::Load<Texture>("assets/tap.jpg");

        Resources::Add<Sprite>("dialog_sprite")->SetTexture(Resources::Get<Texture>("dialog"))->SetPixelPerUnit(200)->SetBorder({10, 10, 10, 10})->Build();

        Resources::Load<Music>("assets/fizz.mp3");
        Resources::Load<Effect>("assets/hit.wav");
        Resources::Load<Effect>("assets/shoot.wav")->setVolume(40);

        // Resources::Add<SpriteSheet>("walking")->Load(Resources::Get<TextureAtlas>("walking"));

        Resources::Load<Font>("assets/modenine.ttf", "modenine");
        Resources::Load<Font>("assets/consres.ttf", "consolas_res");
        Resources::Load<Font>("assets/pixelperfect.ttf", "Pixel Perfect");

        return true;
    }

    /*
        [INIT] CONFIG OVERRIDE
    */
    bool InitConfig() {
        perspectiveTest = false;

        Debug::debugGridMode = perspectiveTest ? GridMode::PERSPECTIVE : GridMode::ORTHOGRAFIC;

        return true;
    }

    /*
        [INIT] INPUT MAPPINGS
    */
    bool InitMapping() {
#if 0
        Input::printActiveControllers();

        InputMapping* fire = Input::addMapping("fire");

        fire->type = InputType::BUTTON;
        fire->positiveButton = SDL_SCANCODE_LCTRL;
        fire->positiveControllerButton = SDL_CONTROLLER_BUTTON_A;
        fire->mouseButton = SDL_BUTTON_LEFT;

        InputMapping* fire2 = Input::addMapping("fire2");

        fire2->type = InputType::BUTTON;
        fire2->positiveButton = SDL_SCANCODE_RCTRL;
        fire2->positiveControllerButton = SDL_CONTROLLER_BUTTON_Y;
        fire2->mouseButton = SDL_BUTTON_RIGHT;

        InputMapping* horizontal = Input::addMapping("horizontal");

        horizontal->type = InputType::AXIS;
        horizontal->positiveButton = SDL_SCANCODE_D;
        horizontal->negativeButton = SDL_SCANCODE_A;
        horizontal->altPositiveButton = SDL_SCANCODE_RIGHT;
        horizontal->altNegativeButton = SDL_SCANCODE_LEFT;
        horizontal->controllerAxis = SDL_CONTROLLER_AXIS_LEFTX;
        horizontal->deadZone = 0.2f;

        InputMapping* vertical = Input::addMapping("vertical");

        vertical->type = InputType::AXIS;
        vertical->positiveButton = SDL_SCANCODE_W;
        vertical->negativeButton = SDL_SCANCODE_S;
        vertical->altPositiveButton = SDL_SCANCODE_UP;
        vertical->altNegativeButton = SDL_SCANCODE_DOWN;
        vertical->controllerAxis = SDL_CONTROLLER_AXIS_MAX;
        vertical->deadZone = 0.2f;
#endif
        return true;
    }

    /*
        [INIT] LOAD STARTUP SCENE
    */
    bool InitScene() {
        SceneManager::LoadScene<Level>("Level 1");

        Camera* camera = SceneManager::activeScene->activeCamera;
        camera->backgroundColor = {0.05f, 0.05f, 0.2f, 1.0f};

        if (perspectiveTest) {
            camera->setOrthographic(false);
            camera->setFarClipPlane(20.0f);
            camera->transform->localPosition = {1.0f, 3.0f, 6.0f};
            camera->transform->localRotation = Transform::QUATERNION_IDENTITY;
            Debug::debugGridMode = GridMode::PERSPECTIVE;
        } else {
            camera->setOrthographic(true);
            camera->setNearClipPlane(12.0f);
            camera->setFarClipPlane(1.0f);
            camera->setOrthographicSize(5.0f);
            camera->transform->localPosition = {0.0f, 0.0f, 10.0f};
            camera->transform->localRotation = Transform::QUATERNION_IDENTITY;
        }

        return true;
    }

    /*
        PROCESS INPUT
    */
    void GetInput() {
#if 0
        if ((Input::getKey(SDL_SCANCODE_LCTRL) && Input::getKeyDown(SDL_SCANCODE_Q)) || Input::getKeyDown(SDL_SCANCODE_ESCAPE))
            Stop();

        // WINDOW
        /* if (Input::getKeyDown(SDL_SCANCODE_F11))
            GLManager::ToggleFullscreen(); */

        if (Input::getButtonDown("fire")) {
            Debug::Log << "FIRE!" << std::endl;
            Resources::Get<Effect>("shoot")->play();
        }

        if (Input::getButtonDown("fire2")) {
            Debug::Log << "FIRE 2!" << std::endl;
            Resources::Get<Effect>("hit")->play();
        }

        //double horizontal = Input::getAxis("horizontal");
        /* if (horizontal != 0) {
        Debug::Log << horizontal << std::endl;
    } */

        if (Input::getKeyDown(SDL_SCANCODE_D)) {
            Debug::enabled = !Debug::enabled;
        }

        if (Input::getKeyDown(SDL_SCANCODE_F)) {
            Config::Physics::debug = !Config::Physics::debug;
        }

        if (Input::getKeyDown(SDL_SCANCODE_P)) {
            if (Music::isPlaying()) {
                Debug::Log << "PAUSE" << std::endl;
                Music::pause();
            } else if (Music::isPaused()) {
                Debug::Log << "RESUME" << std::endl;
                Music::resume();
            } else {
                Debug::Log << "PLAY" << std::endl;
                Resources::Get<Music>("fizz")->play(-1);
            }
        } else if (Input::getKeyDown(SDL_SCANCODE_O)) {
            Debug::Log << "STOP" << std::endl;
            Music::stop();
        } else if (Input::getKeyDown(SDL_SCANCODE_M)) {
            if (!Music::isMuted()) {
                Debug::Log << "MUTE" << std::endl;
                Music::mute();
            } else {
                Debug::Log << "UNMUTE" << std::endl;
                Music::unMute();
            }
        }

        if (Input::getKey(SDL_SCANCODE_I)) {
            Debug::Log << Music::addVolume(1) << std::endl;
        } else if (Input::getKey(SDL_SCANCODE_U)) {
            Debug::Log << Music::addVolume(-1) << std::endl;
        }

        if (Input::getKey(SDL_SCANCODE_C)) {
            glm::vec3 pos = SceneManager::activeScene->activeCamera->transform->position;
            SceneManager::activeScene->activeCamera->transform->SetLocalPosition({pos.x - (2.4f * Time::deltaTime), pos.y, pos.z});
        } else if (Input::getKey(SDL_SCANCODE_V)) {
            glm::vec3 pos = SceneManager::activeScene->activeCamera->transform->position;
            SceneManager::activeScene->activeCamera->transform->SetLocalPosition({pos.x + (2.4f * Time::deltaTime), pos.y, pos.z});
        }

        if (Input::getKey(SDL_SCANCODE_B)) {
            Entity* lightEntity = SceneManager::activeScene->GetEntity("Point 1");
            glm::vec3 pos = lightEntity->transform->position;
            lightEntity->transform->SetLocalPosition({pos.x - (2.4f * Time::deltaTime), pos.y, pos.z});
        } else if (Input::getKey(SDL_SCANCODE_N)) {
            Entity* lightEntity = SceneManager::activeScene->GetEntity("Point 1");
            glm::vec3 pos = lightEntity->transform->position;
            lightEntity->transform->SetLocalPosition({pos.x + (2.4f * Time::deltaTime), pos.y, pos.z});
        }

        if (Input::getKey(SDL_SCANCODE_G)) {
            Entity* lightEntity = SceneManager::activeScene->GetEntity("Directional");
            glm::vec3 pos = lightEntity->transform->position;
            lightEntity->transform->SetLocalPosition({pos.x - (10 * Time::deltaTime), pos.y, pos.z});
        } else if (Input::getKey(SDL_SCANCODE_H)) {
            Entity* lightEntity = SceneManager::activeScene->GetEntity("Directional");
            glm::vec3 rot = lightEntity->transform->GetLocalEulerAngles();
            lightEntity->transform->SetLocalRotation({rot.x + (10 * Time::deltaTime), rot.y, rot.z});
        }

        if (Input::getKey(SDL_SCANCODE_Y)) {
            Entity* canvas = SceneManager::activeScene->GetEntity("Canvas");
            canvas->transform->rectTransform->SetRectSize({canvas->transform->rectTransform->rect.width + 1.0f * Time::deltaTime,
                                                           canvas->transform->rectTransform->rect.height});

        } else if (Input::getKey(SDL_SCANCODE_U)) {
            Entity* canvas = SceneManager::activeScene->GetEntity("Canvas");
            canvas->transform->rectTransform->SetRectSize({canvas->transform->rectTransform->rect.width - 1.0f * Time::deltaTime,
                                                           canvas->transform->rectTransform->rect.height});
        }
        if (Input::getKey(SDL_SCANCODE_I)) {
            Entity* canvas = SceneManager::activeScene->GetEntity("Canvas");
            canvas->transform->rectTransform->SetRectSize({canvas->transform->rectTransform->rect.width,
                                                           canvas->transform->rectTransform->rect.height + 1.0f * Time::deltaTime});

        } else if (Input::getKey(SDL_SCANCODE_O)) {
            Entity* canvas = SceneManager::activeScene->GetEntity("Canvas");
            canvas->transform->rectTransform->SetRectSize({canvas->transform->rectTransform->rect.width,
                                                           canvas->transform->rectTransform->rect.height - 1.0f * Time::deltaTime});
        }

        if (Input::getKey(SDL_SCANCODE_I)) {
            Entity* canvas = SceneManager::activeScene->GetEntity("Canvas");
            canvas->transform->rectTransform->SetRectSize({canvas->transform->rectTransform->rect.width,
                                                           canvas->transform->rectTransform->rect.height + 1.0f * Time::deltaTime});

        } else if (Input::getKey(SDL_SCANCODE_Q)) {
            Entity* canvas = SceneManager::activeScene->GetEntity("Canvas");
            canvas->transform->SetLocalPosition({canvas->transform->localPosition.x - 1.0f * Time::deltaTime,
                                                 canvas->transform->localPosition.y,
                                                 0.0f});
        }

        if (Input::getAxis("horizontal") != 0) {
            Entity* particleObj = SceneManager::activeScene->GetEntity("Particle Object");
            particleObj->transform->SetLocalPosition({particleObj->transform->localPosition.x + Input::getAxis("horizontal") * 2.0f * Time::deltaTime,
                                                      particleObj->transform->localPosition.y,
                                                      particleObj->transform->localPosition.z});
        }

        if (Input::getAxis("vertical") != 0) {
            Entity* particleObj = SceneManager::activeScene->GetEntity("Particle Object");
            particleObj->transform->SetLocalPosition({particleObj->transform->localPosition.x,
                                                      particleObj->transform->localPosition.y + Input::getAxis("vertical") * 2.0f * Time::deltaTime,
                                                      particleObj->transform->localPosition.z});
        }
#endif
    }
};

Application* CreateApplication() {
    return new Game();
}

int main(int argc, char** argv) {
    DEBUG_INFO("Test");

    //auto app = se::CreateApplication();
    //app->Init();
    //app->Run();
    //delete app;
}
