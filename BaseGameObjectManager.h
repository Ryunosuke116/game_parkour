#pragma once
#include "Common.h"
#include <vector>
#include <nlohmann/json.hpp>
#include "IObject.h"

class BaseGameObjectManager : public IObject
{
public:
	BaseGameObjectManager() = default;
	~BaseGameObjectManager() = default;

	void HandOver(nlohmann::json j)
	{
		jsonData = j;
	}

	virtual void Add()abstract;

	std::string GetTag() { return tag; }
protected:
	nlohmann::json	jsonData;
	VECTOR posAddObject;
	std::string tag;
};

