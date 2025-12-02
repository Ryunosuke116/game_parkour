#pragma once
#include "BaseUI.h"
#include <nlohmann/json.hpp>

class CoinObserver;

class RankScoreUi : 
	public BaseUI,
	public CoinObserver
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

	void OnCoinPicked(int amount)override;
	
	

private:
	void RankUpdate();
	void RankUp();
	void RankDown();
private:
	std::unordered_map<std::string, int> umRankHandles;

	std::string rankHandleKey;

	int rankUpCount;

	float rankPosX;
	float rankPosY;
	float rankWidth;
	float rankHeight;
	float drawRankRate;

	static constexpr int kMaxDrawRankRate = 1;
	static constexpr int kMinDrawRankRate = 0;
	static constexpr float kChangeAfterAddValue = 0.95f;
};

