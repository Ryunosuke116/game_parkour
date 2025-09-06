#pragma once
#include <nlohmann/json.hpp>
#include "IObject.h"
#include "ObjectMediator.h"

class BaseGameObjectManager : public IObject
{
public:
	BaseGameObjectManager() = default;
	~BaseGameObjectManager() = default;

	void HandOver(nlohmann::json j)
	{
		jsonData = j;
	}

	template<typename T>
	void Add(const int modelHandle, 
		const VECTOR& position,
		std::vector<BaseObject>& objects);

	virtual void Add()abstract;

	std::string GetTag() { return tag; }

protected:
	nlohmann::json	jsonData;
	VECTOR posAddObject;
	std::string tag;

};

