#pragma once
#include "common.h"
#include <string>
#include <nlohmann/json.hpp>
#include "Object_interface.h"

class BaseObject : public Object_interface
{
protected:
	int modelHandle;
	VECTOR position;
	std::string tag;
	VECTOR pos_difference;

public:
	BaseObject();
	~BaseObject();
	void Draw()override;

	const int GetModelHandle() { return modelHandle; }
	const VECTOR GetPosition() { return position; }
	const std::string GetTag() { return tag; }
	const VECTOR GetPos_difference() { return pos_difference; }
};

