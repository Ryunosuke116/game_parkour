#pragma once
#include "common.h"
#include <nlohmann/json.hpp>
#include "ObjectManager_interface.h"

class BaseGameObjectManager : public ObjectManager_interface
{
public:
	void HandOver(nlohmann::json j)
	{
		jsonData = j;
	}

	std::string GetTag() { return tag; }

protected:

	nlohmann::json	jsonData;
	VECTOR			pos_addObject;
	std::string tag;

};

