#pragma once
#include "common.h"
#include <string>
#include <nlohmann/json.hpp>
#include "IObject.h"

class BaseObject : public IObject
{
protected:
	int modelHandle;
	VECTOR position;
	std::string tag;
	std::string jsonTag;
	VECTOR pos_difference;

public:
	BaseObject();
	~BaseObject();
	void Draw()override;

	const int GetModelHandle() { return modelHandle; }
	const VECTOR GetPosition() { return position; }
	const std::string GetTag() { return tag; }
	std::string GetJsonTag() { return jsonTag; }
	const VECTOR GetPos_difference() { return pos_difference; }
};

