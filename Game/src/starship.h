#pragma once

#include "globals.h"

class Starship : public se::Entity {
	public:
		void Awake();
		se::MeshRenderer* mr;
};
