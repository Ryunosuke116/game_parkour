#pragma once
#include "common.h"
#include <nlohmann/json.hpp>
#include "IObjectManager.h"

class BaseGameObjectManager : public IObjectManager
{
public:

	BaseGameObjectManager() = default;
	~BaseGameObjectManager() = default;

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

