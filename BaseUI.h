#pragma once
#include "IObject.h"

class BaseUI : public IObject
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

