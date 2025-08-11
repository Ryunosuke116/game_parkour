#pragma once
#include "BaseObject.h"
#include <nlohmann/json.hpp>

class Field : public BaseObject
{
private:
	bool isPush;
	bool isPoly;
public:

	Field();
	~Field();

	void Load(const nlohmann::json& jsonData)override;
	void Initialize()override;
	void Update()override;
	void Draw()override;

};

