#ifndef SE_UTILITY
#define SE_UTILITY

#include "Core.h"
#include "sepch.h"

#define ts(str) util::toString(str)

namespace se {

namespace util {

    std::string basename(std::string);
    std::string zerofill(int, int);

    int random(int min, int max);
    float random(float min, float max);
    bool happens(float probability);

    template <typename T>
    std::string toString(const T& n) {
        std::ostringstream stm;
        stm << n;
        return stm.str();
    }

    template <typename T>
    std::wstring toWstring(const T& n) {
        std::wostringstream stm;
        stm << n;
        return stm.str();
    }

    template <class T>
    bool is(void* obj) {
        return dynamic_cast<T>(obj) != nullptr;
    }

    const std::type_info& ty(void* obj);

    /// NB: modificare anche la versione nel cpp
    template <class T>
    std::string classtitle() {
        std::string type = typeid(T).name();
        std::regex pat("N2se|P6");
        type = regex_replace(type, pat, "");

        pat = "E$";
        type = regex_replace(type, pat, "");

        std::string title;
        bool inName = false;
        char prevc;
        for (char& c : type) {
            if (isdigit(c) && !inName) continue;
            if ((isupper(c) || isdigit(c)) && inName && !(c == 'D' && prevc == '2')) title += " ";
            title += (inName ? c : toupper(c));
            inName = true;
            prevc = c;
        }
        return title;
    }

    std::string classtitle(std::string);
    glm::vec3 toEulerAngles(const glm::quat&);

    std::vector<std::string> split(std::string text, char delim);
    std::vector<std::wstring> wsplit(std::wstring text, wchar_t delim);

    std::vector<std::string> filesindir(std::string path);
};  // namespace util

}  // namespace se

#endif