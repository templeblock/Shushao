#include "sepch.h"

#include "Resources.h"
#include "Utility.h"
#include "Shaders/Shader.h"
#include "Texture.h"

namespace se {

std::vector<char> Resources::GetEmbeddedData(int IDRES, std::string library, LPWSTR type) {
    std::vector<char> output;

    HMODULE module;

    if (library != "") {
        module = LoadLibrary((LPCWSTR)library.c_str());
    } else {
        module = GetModuleHandle(NULL);
    }

    auto resourceHandle = FindResource(module, MAKEINTRESOURCE(IDRES), type);

    if (resourceHandle != nullptr) {
        auto dataHandle = LoadResource(module, resourceHandle);
        if (dataHandle != nullptr) {
            auto resourceSize = ::SizeofResource(module, resourceHandle);
            if (resourceSize != 0) {
                auto firstByte = reinterpret_cast<const char*>(::LockResource(dataHandle));
                if (firstByte != nullptr) {
                    output.resize(resourceSize);
                    copy(firstByte, firstByte + resourceSize, output.begin());
                }
            }
        }
    } else {
        Debug::Log(WARNING) << "Cant load Resource " << IDRES << (library != "" ? " from " + library : "") << std::endl;
        return std::vector<char>();
    }

    return output;
}

std::string Resources::GetEmbeddedText(int IDRES, std::string library) {
    auto data = se::Resources::GetEmbeddedData(IDRES, library, RT_RCDATA);
    return std::string(data.begin(), data.end());
}

void Resources::Clear() {
    for (auto& asset : Assets) {
        if (asset.second != nullptr) {
            Debug::Log << "Cancello " << asset.second->name << " (" + util::classtitle(typeid(*asset.second).name()) + ")" << std::endl;
            delete (asset.second);
            asset.second = nullptr;
        }
    }
    Assets.clear();
}

void Resources::toString() {
    Logger::setColor(ConsoleColor::DARKBLUE);
    std::cout << " Resources [" << Assets.size() << "] :" << std::endl;
    for (auto const& asset : Assets) {
        std::cout << "  - (" << util::classtitle(typeid(*asset.second).name()) << ") " << asset.first << std::endl;
    }
    Logger::setColor(ConsoleColor::LIGHTGREY);
}

std::map<std::string, Object*> Resources::Assets;

}  // namespace se
