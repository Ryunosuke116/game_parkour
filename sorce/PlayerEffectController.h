#pragma once
#include "EffectColor.h"
#include <memory>

class EffectManager;

class PlayerEffectController
{
public:
	PlayerEffectController();
	~PlayerEffectController();

	void Create();
	void Initialize();
	void Update(const VECTOR& playerPos,
		const PlayerData& playerData);

	void PlayCoinTouchEffect();

private:
	void ChangeColorByScoreRank();

private:
	float effectTimer;

	std::weak_ptr<EffectManager> wpEffectManager;

	static constexpr EffectColor scoreDColor = { 0,255,0,255 };			//緑色
	static constexpr EffectColor scoreCColor = { 255,255,0,255 };		//黄色
	static constexpr EffectColor scoreBColor = { 0,0,255,255 };			//青色
	static constexpr EffectColor scoreAColor = { 255,100,100,255 };		//赤色
	static constexpr EffectColor scoreSColor = { 255,100,100,255 };		//虹色
};