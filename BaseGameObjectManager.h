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

protected:

	nlohmann::json	jsonData;
	VECTOR			pos_addObject;

};

