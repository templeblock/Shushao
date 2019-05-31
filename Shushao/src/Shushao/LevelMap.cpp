#include "sepch.h"

#include "LevelMap.h"
#include "Debug.h"

namespace se {

	LevelMap::LevelMap() {
		for (std::string i : defaults) {
			if (size() >= maxElements) return;
			push_back(i);
		}
	}

	LevelMap::LevelMap(std::initializer_list<std::string> ilist) {
		for (std::string i : ilist) {
			if (size() >= maxElements) return;
			push_back(i);
		}
	}

	LevelMap::LevelMap(std::vector<std::string> list) {
		for (std::string i : list) {
			if (size() >= maxElements) return;
			push_back(i);
		}
	}

	void LevelMap::operator=(std::initializer_list<std::string> ilist) {
		clear();
		for (std::string i : defaults) {
			if (size() >= maxElements) return;
			push_back(i);
		}
		for (std::string i : ilist) {
			if (size() >= maxElements) return;
			push_back(i);
		}
	}

	void LevelMap::operator+=(std::string label) {
		if (size() >= maxElements) return;
		push_back(label);
	}

	void LevelMap::operator-=(std::string label) {
		erase(remove(begin(), end(), label), end());
	}

	int LevelMap::operator[](std::string label) {
		auto it = find(begin(), end(), label);
		if (it != end()) {
			auto d = distance(begin(), it);
			return d;
		}
		return -1;
	}

	std::string LevelMap::operator[](unsigned int index) {
		if (index == maxElements) return "^overall";
		if (index > (size() + 1)) return "?undefined";
		return at(index);
	}

	void LevelMap::setDefaults(std::initializer_list<std::string> ilist) {
		for (std::string i : ilist) defaults.push_back(i);
		for (std::string i : ilist) push_back(i);
	}

	int LevelMap::top() {
		return size() - 1;
	}

	int LevelMap::over() {
		return maxElements;
	}

	void LevelMap::setMaxElements(unsigned int m) {
		maxElements = m;
	}

	void LevelMap::toString(std::string title) {
		Logger::setColor(ConsoleColor::GREY);
		std::cout << " " << util::classtitle(typeid(*this).name()) << " " << title << " [" << maxElements << "] :" << std::endl;
		for (auto it = begin(); it != end(); ++it) {
			int key = distance(begin(), it);
			std::cout << "  (" << key << ") " << *it << std::endl;
		}
		// cout << "  (" << maxElements << ") " << "^overall" << std::endl;
		Logger::setColor(ConsoleColor::LIGHTGREY);
	}

}  // namespace se
