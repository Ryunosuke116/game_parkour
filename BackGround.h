#pragma once
#include "BaseObject.h"

class BackGround : public BaseObject
{
public:
	BackGround();
	~BackGround();

	void Load(const nlohmann::json& jsonData)override;
	void Create()override;
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void ResultCreate()override{}
	void ResultInitialize()override{}
	void ResultUpdate()override{}

private:
	int backGroundHandle;
	int startButtonHandle;

};

