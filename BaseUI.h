#pragma once
#include "Object_interface.h"

class BaseUI : public Object_interface
{
public:
	BaseUI();
	~BaseUI(){}

	std::string GetJsonTag() { return jsonTag; }

protected:
	int x;
	int y;
	std::string jsonTag;

};

