#pragma once
#include "CoinObserver.h"

class CoinObserver;

class RankScoreUpdater: public CoinObserver
{
public:
	RankScoreUpdater();
	~RankScoreUpdater();

	void Initialize();
	void Update(float& rankWidth,
		float& rankHeight,
		float& drawRankRate,
		std::string& rankHandleKey);

	void OnCoinPicked(int amount)override;
	bool GetIsChangeRank()const { return isChangeRank; }

private:
	void RankUp(float& drawRankRate,
		std::string& rankHandleKey);
	void RankDown(float& drawRankRate,
		std::string& rankHandleKey);

private:
	int rankUpCount;		//スコアランクが上がる回数
	bool isChangeRank;		//ランクが変動したか

	static constexpr float kChangeAfterAddValue = 0.95f;	
	static constexpr int kMaxDrawRankRate = 1;
	static constexpr int kMinDrawRankRate = 0;
};

