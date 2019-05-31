#include "coroutinetest.h"

using namespace se;

void CoroutineTest::Start() {
    StartCoroutine(coroutineA());
}

Routine CoroutineTest::coroutineA() {
    return COROUTINE {
        Debug::Log(WARNING) << "coroutineA created" << std::endl;
        yield_return(new WaitForSeconds(2.0f));
        yield_return(StartCoroutine(coroutineB()));
        Debug::Log(WARNING) << "coroutineA running again" << std::endl;
    };
}

Routine CoroutineTest::coroutineB() {
    return COROUTINE {
        Debug::Log(WARNING) << "coroutineB created" << std::endl;
        yield_return(new WaitForSeconds(4.5f));
        Debug::Log(WARNING) << "coroutineB enables coroutineA to run" << std::endl;
    };
}