#pragma once

#include "Shushao/Core.h"
#include "sepch.h"

namespace se {

class LevelMap : private std::vector<std::string> {
public:
    using std::vector<std::string>::at;
    using std::vector<std::string>::begin;
    using std::vector<std::string>::end;

    LevelMap();
    LevelMap(std::initializer_list<std::string>);
    LevelMap(std::vector<std::string> list);
    void operator=(std::initializer_list<std::string>);
    void operator+=(std::string);
    void operator-=(std::string);
    int operator[](std::string);
    std::string operator[](unsigned int);
    void setDefaults(std::initializer_list<std::string>);
    void setMaxElements(unsigned int);
    int top();
    int over();
    void toString(std::string title = "");

private:
    unsigned int maxElements = 32;
    std::vector<std::string> defaults = {
        "Default"  //
    };
};

}  // namespace se
