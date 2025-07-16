#pragma once
#include <string>

class BaseObject
{
protected:
	int modelHandle;
	VECTOR position;
	std::string tag;
	VECTOR pos_difference;

public:
	BaseObject();
	~BaseObject();

	//èÉêàâºëzä÷êî	
	virtual void Initialize() abstract;
	virtual void Update() abstract;
	virtual bool Draw() abstract;

	const int GetModelHandle() { return modelHandle; }
	const VECTOR GetPosition() { return position; }
	const std::string GetTag() { return tag; }
	const VECTOR GetPos_difference() { return pos_difference; }
};

