#pragma once

#include "globals.h"

class CoroutineTest : public se::Program {
public:
protected:
    void Start();

private:
    se::Routine coroutineA();
    se::Routine coroutineB();
};
