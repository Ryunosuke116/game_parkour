#pragma once
#include "BaseUI.h"
class Result_backGround : public BaseUI
{
public:
	Result_backGround();
	~Result_backGround();

	void Add()override{}
	void Create()override {}
	void Load(const nlohmann::json& jsonData)override;
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void ResultInitialize()override{}
	void ResultUpdate()override{}

private:
	int backGroundHandle;
};

