#pragma once
#include "common.h"
#include <string>
#include <nlohmann/json.hpp>
#include "IObject.h"
#include "ObjectMediator.h"

class BaseObject : public IObject
{
public:
	BaseObject();
	~BaseObject();
	void Draw()override;

	const int GetModelHandle() { return modelHandle; }
	const VECTOR GetPosition() { return position; }
	const std::string GetTag() { return tag; }
	std::string GetJsonTag() { return jsonTag; }
	const VECTOR GetPos_difference() { return pos_difference; }

	void Add()override{}

protected:
	int modelHandle;
	VECTOR position;
	std::string tag;
	std::string jsonTag;
	VECTOR pos_difference;
};

