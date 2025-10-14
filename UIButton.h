#pragma once
#include "BaseUI.h"

class UIButton : public BaseUI
{
public:
	UIButton();
	~UIButton();

	void Add()override {}
	void Create()override {}
	void Load(const nlohmann::json& jsonData)override;
	void Initialize()override {}
	void Update()override {}
	void Draw()override;
	void ResultCreate()override;
	void ResultInitialize()override;
	void ResultUpdate()override;

private:
	int backGroundHandle;
	int titleButtonHandle;
	int alpha;
	int addAlpha;

	float backGroundX;
	float backGroundY;
	float titleButtonX;
	float titleButtonY;
	float backGroundWidth;
	float backGroundHeight;
	float titleButtonWidth;
	float titleButtonHeight;
};
