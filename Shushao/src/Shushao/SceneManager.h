#ifndef SE_SCENEMANAGER
#define SE_SCENEMANAGER

#include "Core.h"
#include "sepch.h"

#include "Utility.h"
#include "GameData.h"
#include "Scene.h"

namespace se {

//class Application;

class SceneManager {
public:
    ~SceneManager();

    static Scene* activeScene;

    static bool sceneSet;

    template <class T>
    static T* LoadScene(std::string _name = "") {  // Adds a scene of class T

        // todo pause e restore dopo
        T* scene = new T();
        scene->name = (_name == "" ? util::classtitle<T>() : _name);

        if (sceneSet) {
            delete (scene);
            GameData::DestroyAll();
        }
        Setscene(scene);
        return scene;
    }

    static void Setscene(Scene*);
    static void Clear();

    /*template<class T>
			static T* GetSceneByClass() {	// Returns the component of Type type if the game object has one attached, null if it doesn't.
				for(Scene* scene: Scenes) {
					if (dynamic_cast<T*>(scene))
						return dynamic_cast<T*>(scene);
				}
				return nullptr;
			}*/

protected:
    static std::vector<Scene*> Scenes;
};

}  // namespace se

#endif