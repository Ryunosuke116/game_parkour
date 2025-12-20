#pragma once
#include "BaseObject.h"

class SkyBox : public BaseObject
{
private:

public:
	SkyBox();
	~SkyBox();
	void Create()override;
	void Load(const nlohmann::json& jsonData)override;
	void Initialize()override;
	void Update()override;
	void ResultCreate()override;
	void ResultInitialize()override;
	void ResultUpdate()override;

};


