#pragma once
#include "IObject.h"
#include "nlohmann/json.hpp"
#include <string>

class BaseUI : public IObject
{
public:
	BaseUI();
	~BaseUI(){}

	std::string GetJsonTag() { return jsonTag; }
	virtual void Add()									abstract;
	virtual void Load(const nlohmann::json& jsonData)	abstract;
protected:
	std::string jsonTag;
};

