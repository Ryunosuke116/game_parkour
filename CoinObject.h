#pragma once
#include "HitCheck.h"
#include "BaseObject.h"
#include "ISoundPlayer.h"
#include "IEffectManager.h"

class EffectManager;

class CoinObject : public BaseObject
{
public:
	CoinObject();
	~CoinObject();

	void Initialize()override;
	void Update()override {}
	bool Update(const VECTOR& playerPos,
		const VECTOR& playerPos_bottom,
		const float radius);
	void Draw()override;
	void Load(const nlohmann::json& jsonData)override{}
	void Create()override{}
	void ResultInitialize()override;
	void ResultUpdate()override;

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

	VECTOR flyAwayDirection;
	VECTOR flyAwayVelocity;			//吹っ飛ぶときの移動量

	static constexpr float radius = 6.0f;		//半径
	static constexpr float objectScale = 0.3f;	//オブジェクトの大きさ
	static constexpr float addVelocity = 0.4f;	//移動量
	static constexpr float flyPower = 5.0f;		//飛ぶ大きさ
};

