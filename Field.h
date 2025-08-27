#pragma once
#include "BaseObject.h"
#include <nlohmann/json.hpp>

class Field : public BaseObject
{
private:
	bool isPush;
	bool isPoly;
	int bullHandle;
public:
	Field();
	~Field();

	void Create()override{}
	void Load(const nlohmann::json& jsonData)override;
	void Initialize()override;
	void Update(ObjectMediator& objectMediator)override;
	void Draw()override;

};

