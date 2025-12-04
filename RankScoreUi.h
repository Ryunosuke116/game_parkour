#pragma once
#include "BaseUI.h"
#include <nlohmann/json.hpp>

class RankScoreUpdater;

class RankScoreUi : 
	public BaseUI,
	public std::enable_shared_from_this<RankScoreUi>
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

	std::string GetRankHandleKey()const { return rankHandleKey; }
	std::shared_ptr<RankScoreUpdater> GetRankScoreUpdater() const { return rankScoreUpdater; }

private:
	std::unordered_map<std::string, int> umRankHandles;
	std::shared_ptr<RankScoreUpdater> rankScoreUpdater;

	std::string rankHandleKey;

	int rankUpCount;

	float rankPosX;
	float rankPosY;
	float rankWidth;
	float rankHeight;
	float drawRankRate;

	static constexpr float kResetRankRate = 0.05f;
};

