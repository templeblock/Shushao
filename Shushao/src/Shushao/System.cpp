#include "sepch.h"

#include "Debug.h"
#include "System.h"

namespace se {

	void System::init() {
		for (Service* s : Services) {
			s->Init();
		}
	}

	void System::update() {
		for (Service* s : Services) {
			s->Update();
		}
	}

	void System::exit() {
		for (Service* s : Services) {
			s->Exit();
		}

		Clear();
	}

	void System::Clear() {
		for (Service* s : Services) {
			delete (s);
		}
		Services.clear();
	}

	void System::ListServices() {
		Logger::setColor(ConsoleColor::YELLOW);
		std::cout << " System" << std::endl;
		Logger::setColor(ConsoleColor::DARKYELLOW);
		for (Service* s : Services) {
			std::cout << " " << (char)192 << (char)196;
			std::cout << " " << s->name << (s->active ? "+" : "");
		}

		std::cout << std::endl;

		Logger::setColor(ConsoleColor::WHITE);
	}

	std::vector<Service*> System::Services;

	//Service* System::Input = System::AddService<InputManager>();
	//InputManager& System::Input = System::AddService<InputManager>();
	//Application* System::Game;

}  // namespace se
