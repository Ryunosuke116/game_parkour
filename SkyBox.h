#pragma once
#include "BaseObject.h"

class SkyBox : public BaseObject
{
private:

public:
	SkyBox();
	~SkyBox();
	void Initialize()override;
	void Update()override;
	//void Update(const VECTOR& playerPos);
	void Load(const nlohmann::json& jsonData)override;
	void Create()override {}

};


