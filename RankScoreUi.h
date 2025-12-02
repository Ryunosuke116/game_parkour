#pragma once
#include "BaseUI.h"
#include <nlohmann/json.hpp>


class RankScoreUi : public BaseUI
{
public:
	RankScoreUi();
	~RankScoreUi();

	void Load(const nlohmann::json& jsonData)override;
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void Create()override;
	void Add()override {}
	void ResultCreate()override {}
	void ResultInitialize()override {}
	void ResultUpdate()override {}

private:
	std::unordered_map<std::string, int> umRankHandles;

	std::string rankHandleKey;

	float rankPosX;
	float rankPosY;
	float rankWidth;
	float rankHeight;
	float rate;
};

