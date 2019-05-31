#pragma once

#include "Core.h"
#include "Component.h"

namespace se {

class Program : public Component {
public:
    virtual void setup() { name = "Program"; }
    ~Program();

protected:
private:
};

}  // namespace se
