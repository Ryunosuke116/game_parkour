#pragma once
#include "Common.h"
#include <nlohmann/json.hpp>
#include "IObject.h"

class BaseGameObjectManager : public IObject
{
public:
	BaseGameObjectManager() :
		posAddObject(VGet(0.0f, 0.0f, 0.0f)),
		tag("")
	{

	}

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

