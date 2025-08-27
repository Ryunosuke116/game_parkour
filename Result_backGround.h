#pragma once
#include "BaseUI.h"
class Result_backGround : public BaseUI
{
public:
	Result_backGround();
	~Result_backGround();

	void Create()override {}
	void Load(const nlohmann::json& jsonData)override;
	void Initialize()override;
	void Update(ObjectMediator& objectMediator)override;
	void Draw()override;

private:
	int backGroundHandle;
};

