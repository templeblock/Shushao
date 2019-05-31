#ifndef SE_TIMELINE
#define SE_TIMELINE

#include "Core.h"
#include "sepch.h"

#include "Object.h"
#include "Sprite.h"
#include "SpriteSheet.h"

namespace se {

enum class PropertyType {
    INT,
    FLOAT,
    // VEC2,
    VEC3,
    // VEC4,
    SPRITE
};

template <class T>
class TimelineProperty {
public:
    ~TimelineProperty() {
        clear();
    }

    T* target = nullptr;
    T backupValue;

    std::vector<T*> pointers;
    std::vector<T> values;

    void setTarget(T& var) {
        //if (&var == nullptr) return;
        target = &var;
        backupValue = var;
    }

    void setTarget(T*& var) {
        //if (&var == nullptr) return;
        target = var;
        backupValue = *var;
    }

    void loadValue(unsigned int pos) {
        //if (target == nullptr) return;
        if (pos >= values.size()) return;
        *target = values[pos];
    }

    void loadPointer(unsigned int pos) {
        //if (target == nullptr) return;
        if (pos >= pointers.size()) return;
        *target = *pointers[pos];
    }

    void reset() {
        //if (target == nullptr) return;
        *target = backupValue;
    }

    void backup() {
        backupValue = *target;
    }

    void clear() {
        target = nullptr;
        pointers.clear();
        values.clear();
    }
};

class Timeline : public Object {
public:
    Timeline(std::string);
    ~Timeline();

    Timeline* loadFrame(unsigned int);
    Timeline* reset();
    Timeline* backup();
    unsigned int getFrameCount();

    // INT
    Timeline* setTarget(int&);
    Timeline* load(std::initializer_list<int>);
    Timeline* add(int var, int nFrames = 1);

    // VEC3
    Timeline* setTarget(glm::vec3&);
    Timeline* setTarget(const glm::vec3&);
    Timeline* load(std::initializer_list<glm::vec3>);
    Timeline* add(glm::vec3 var, int nFrames = 1);

    // FLOAT
    Timeline* setTarget(float&);
    Timeline* load(std::initializer_list<float>);

    // SPRITE *
    Timeline* setTarget(Sprite*& var);
    Timeline* load(std::initializer_list<Sprite*>);
    Timeline* load(std::vector<Sprite*>);
    Timeline* load(SpriteSheet*);

    void clear();

private:
    PropertyType propertyType;

    TimelineProperty<float> vFloat;
    TimelineProperty<int> vInt;
    TimelineProperty<glm::vec3> vVec3;
    TimelineProperty<Sprite> pSprite;
};

}  // namespace se

#endif