#pragma once

/*
    Credits: https://github.com/exawon/CoroBehaviour
*/

#include "Shushao/Core.h"
#include "sepch.h"

#include "Coroutine.h"

namespace se {

class CoroutineHolder {
public:
    virtual ~CoroutineHolder();
    Coroutine* StartCoroutine(Routine InRoutine);
    void StopCoroutine(Coroutine* CoroutinePtr);
    void StopAllCoroutines();
    bool HasCoroutines();
    void ResumeCoroutines();

private:
    std::set<Coroutine*> Coroutines;
    Coroutine* CastAsCoroutine(YieldInstruction* YieldInstructionPtr);
    bool YieldCoroutine(Coroutine* CoroutinePtr);
    Coroutine* PushCoroutine(Coroutine* CoroutinePtr);
    Coroutine* PopCoroutine(Coroutine* CoroutinePtr);
};

}  // namespace se

/* EXAMPLE

class ExampleClass : public Component
{
    void Start() {
        StartCoroutine(coroutineA());
    }

    Routine coroutineA()
    {
        return [=](RoutinePush& yield_return)
        {
            // wait for 1 second
            std::cout << "coroutineA created";
            yield_return(new WaitForSeconds(1.0f));
            yield_return(StartCoroutine(coroutineB()));
            std::cout << "coroutineA running again";
        };
    }

    Routine coroutineB()
    {
        return [=](RoutinePush& yield_return)
        {
            std::cout << "coroutineB created";
            yield_return(new WaitForSeconds(2.5f));
            std::cout << "coroutineB enables coroutineA to run";
        };
    }
}
*/