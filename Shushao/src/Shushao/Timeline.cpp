#include "sepch.h"

#include "Sprite.h"
#include "timeline.h"

namespace se {

	Timeline::Timeline(std::string _name) : Object(_name) {};

	Timeline::~Timeline() {
		clear();
	};

	void Timeline::clear() {
		vInt.clear();
		vFloat.clear();
		vVec3.clear();
		pSprite.clear();
	}

	Timeline* Timeline::loadFrame(unsigned int fr) {
		switch (propertyType) {
		case PropertyType::INT:
			vInt.loadValue(fr);
			break;
		case PropertyType::FLOAT:
			vFloat.loadValue(fr);
			break;
		case PropertyType::VEC3:
			vVec3.loadValue(fr);
			break;
		case PropertyType::SPRITE:
			pSprite.loadPointer(fr);
			break;
		}
		return this;
	}

	Timeline* Timeline::reset() {
		switch (propertyType) {
		case PropertyType::INT:
			vInt.reset();
			break;
		case PropertyType::FLOAT:
			vFloat.reset();
			break;
		case PropertyType::VEC3:
			vVec3.reset();
			break;
		case PropertyType::SPRITE:
			pSprite.reset();
			break;
		}
		return this;
	}

	Timeline* Timeline::backup() {
		switch (propertyType) {
		case PropertyType::INT:
			vInt.backup();
			break;
		case PropertyType::FLOAT:
			vFloat.backup();
			break;
		case PropertyType::VEC3:
			vVec3.backup();
			break;
		case PropertyType::SPRITE:
			pSprite.backup();
			break;
		}
		return this;
	}

	unsigned int Timeline::getFrameCount() {
		switch (propertyType) {
		case PropertyType::INT:
			return vInt.values.size();
			break;
		case PropertyType::FLOAT:
			return vFloat.values.size();
			break;
		case PropertyType::VEC3:
			return vVec3.values.size();
			break;
		case PropertyType::SPRITE:
			return pSprite.pointers.size();
			break;
		}
		return 0;
	}

	//{ #region INT

	Timeline* Timeline::setTarget(int& var) {
		vInt.setTarget(var);
		propertyType = PropertyType::INT;
		return this;
	}

	Timeline* Timeline::load(std::initializer_list<int> ilist) {
		if (propertyType != PropertyType::INT) return this;
		vInt.values = ilist;
		return this;
	}

	Timeline* Timeline::add(int var, int nFrames) {
		if (propertyType != PropertyType::INT) return this;
		if (nFrames > 1) {
			vInt.values.insert(vInt.values.end(), nFrames, var);
		}
		else {
			vInt.values.push_back(var);
		}
		return this;
	}

	//}

	//{ #region VEC3

	Timeline* Timeline::setTarget(glm::vec3& var) {
		vVec3.setTarget(var);
		propertyType = PropertyType::VEC3;
		return this;
	}

	/*Timeline* Timeline::setTarget(const glm::vec3& var) {
			vVec3.setTarget(var);
			cout << vVec3.backupValue.x << " " << vVec3.backupValue.y << std::endl;
			propertyType = PropertyType::VEC3;
			return this;
		}*/

	Timeline* Timeline::load(std::initializer_list<glm::vec3> ilist) {
		if (propertyType != PropertyType::VEC3) return this;
		vVec3.values = ilist;
		return this;
	}

	Timeline* Timeline::add(glm::vec3 var, int nFrames) {
		if (propertyType != PropertyType::VEC3) return this;
		if (nFrames > 1) {
			vVec3.values.insert(vVec3.values.end(), nFrames, var);
		}
		else {
			vVec3.values.push_back(var);
		}
		return this;
	}

	//}

	//{ #region FLOAT

	Timeline* Timeline::setTarget(float& var) {
		vFloat.setTarget(var);
		propertyType = PropertyType::FLOAT;
		return this;
	}

	Timeline* Timeline::load(std::initializer_list<float> ilist) {
		if (propertyType != PropertyType::FLOAT) return this;
		vFloat.values = ilist;
		return this;
	}

	//}

	//{ #region SPRITE

	Timeline* Timeline::setTarget(Sprite*& var) {
		pSprite.setTarget(var);
		propertyType = PropertyType::SPRITE;
		return this;
	}

	Timeline* Timeline::load(std::initializer_list<Sprite*> ilist) {
		if (propertyType != PropertyType::SPRITE) return this;
		pSprite.pointers = ilist;
		return this;
	}

	Timeline* Timeline::load(std::vector<Sprite*> elements) {
		if (propertyType != PropertyType::SPRITE) return this;
		pSprite.pointers = elements;
		return this;
	}

	Timeline* Timeline::load(SpriteSheet* spriteSheet) {
		if (propertyType != PropertyType::SPRITE) return this;
		pSprite.pointers = *spriteSheet;
		return this;
	}

	//}

}  // namespace se
