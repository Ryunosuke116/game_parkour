#pragma once
#include "HitCheck.h"
#include "BaseObject.h"
#include "ISoundPlayer.h"
#include "IEffectManager.h"

class EffectManager;

class CoinObject : public BaseObject
{
public:
	CoinObject(std::shared_ptr<ISoundPlayer> sound,
		std::shared_ptr<IEffectManager> effect);
	~CoinObject();

	void Initialize()override;
	void Update()override{}
	bool Update(const VECTOR& playerPos,
		const VECTOR& playerPos_bottom, const float radius);
	void Draw()override;
	void Load(const nlohmann::json& jsonData)override{}

	void HitPlayerAction();
	void Rotate();
	void Load(const int& handle, const VECTOR& pos);

	HitCheck hitCheck;
	bool GetHitFlag() { return hitFlag; }

private:

	bool isHitPlayer;
	bool hitFlag;
	bool deleteFlag;
	bool isSound;
	float radian_Y;
	float velocity_Y;
	int soundHandle;

	std::shared_ptr<IEffectManager> effectManager;
	std::shared_ptr<ISoundPlayer>	soundPlayer;

	static constexpr float radius = 4.5f;
	static constexpr float objectScale = 0.3f;
	static constexpr float addVelocity = 0.4f;
};

