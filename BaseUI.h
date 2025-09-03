#pragma once
#include "IObject.h"

class BaseUI : public IObject
{
public:
	BaseUI();
	~BaseUI(){}

	std::string GetJsonTag() { return jsonTag; }
	virtual void Add()									abstract;
	virtual void Load(const nlohmann::json& jsonData)	abstract;

protected:
	int x;
	int y;
	std::string jsonTag;

};

