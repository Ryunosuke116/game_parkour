#pragma once
#include "BaseObject.h"
#include "DxLib.h"

class EffectManager;

class Coin : public BaseObject
{
public:
	Coin();
	~Coin();

	void Initialize()							override;
	void Update()								override {}
	void Draw()									override;
	void Load(const nlohmann::json& jsonData)	override {}
	void Create()								override {}
	void ResultCreate()							override;
	void ResultInitialize()						override;
	void ResultUpdate()							override;

	void HitPlayerAction(const VECTOR& topPlayerPos,
		const VECTOR& bottomPlayerPos,
		const float playerRadius);

	bool Update(const VECTOR& topPlayerPos,
		const VECTOR& bottomPlayerPos,
		const float playerRadius);
	void Rotate();
	void Load(const int listNumber, const int handle, const VECTOR& pos);
	bool IsHitPlayer(const VECTOR& topPlayerPos,
		const VECTOR& bottomPlayerPos,
		const float playerRadius);

	bool GetIsHitPlayer()const { return isHitPlayer; }
	bool GetIsDelete()const { return isDelete; }
	int GetListNumber()const { return listNumber; }
	VECTOR GetBoundsMin()const { return boundsMin; }
	VECTOR GetBoundsMax()const { return boundsMax; }

private:
	bool isHitPlayer;
	bool isDelete;
	bool isSound;
	bool isIgnoreHitPlayer;

	int listNumber;

	float radianY;
	float velocityY;
	float flyAwaySpeed;

	VECTOR flyAwayDirection;	//吹っ飛ぶときの方向
	VECTOR flyAwayVelocity;		//吹っ飛ぶときの移動量
	VECTOR boundsMin;			//AABBの最小値
	VECTOR boundsMax;			//AABBの最大値

	static constexpr float kRadius = 9.0f;		//半径
	static constexpr float kObjectScale = 0.3f;	//オブジェクトの大きさ
	static constexpr float kAddVelocity = 0.4f;	//移動量
	static constexpr float kFlyPower = 5.0f;		//飛ぶ大きさ
};

