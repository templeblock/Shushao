#ifndef SE_TEXTFILE
#define SE_TEXTFILE

#include "Core.h"
#include "sepch.h"

#include "Object.h"
#include "Resource.h"

namespace se {

class Textfile : public Object, public Resource {
public:
    Textfile(std::string filename = "", std::string n = "");

    ~Textfile();

    std::string text;
    std::stringstream buffer;

    bool Load(std::string);
    bool LoadEmbedded(int IDRES, std::string library = "");
};

}  // namespace se

#endif