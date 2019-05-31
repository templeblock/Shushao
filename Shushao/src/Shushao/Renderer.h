#pragma once

#include "Core.h"
#include "Component.h"
#include "sepch.h"

namespace se {

class Renderer : public Component {
public:
    virtual void setup() {
        name = "Renderer";
    }

    bool enabled;  // Makes the rendered 3D object visible if enabled.
    bool overlay = false;

    bool isVisible;  //Is this renderer visible in any camera? (Read Only)

    void Copy(Renderer* other);

protected:
    virtual void Awake(){};
    virtual void Update(){};
    virtual void Render(){};
    virtual void OnDestroy(){};

    // messages

    void OnBecameInvisible();  // OnBecameInvisible is called when the object is no longer visible by any camera.
    void OnBecameVisible();  // OnBecameVisible is called when the object became visible by any camera.

private:
};

}  // namespace se
