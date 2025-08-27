#pragma once
#include "IObject.h"

class BaseUI : public IObject
{
public:
	BaseUI();
	~BaseUI(){}

	std::string GetJsonTag() { return jsonTag; }
	void Add()override{}

protected:
	int x;
	int y;
	std::string jsonTag;

};

