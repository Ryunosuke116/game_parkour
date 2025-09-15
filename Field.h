#pragma once
#include "BaseObject.h"
#include <nlohmann/json.hpp>

class Field : public BaseObject
{
public:
	Field();
	~Field();

	void Create()override;
	void Load(const nlohmann::json& jsonData)override;
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void ResultCreate()override;
	void ResultInitialize()override;
	void ResultUpdate()override;

private:
	bool isPush;
	bool isPoly;
	int bullHandle;

};

