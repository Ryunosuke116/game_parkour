#pragma once
#include "HitCheck.h"
#include "BaseObject.h"

class EffectManager;

class Coin : public BaseObject
{
public:
	Coin();
	~Coin();

	void Initialize()override;
	void Update()override;
	bool Update(const VECTOR& topPlayerPos,
		const VECTOR& bottomPlayerPos,
		const float playerRadius);
	void Draw()override;
	void Load(const nlohmann::json& jsonData)override{}
	void Create()override{}
	void ResultCreate()override;
	void ResultInitialize()override;
	void ResultUpdate()override;

	void HitPlayerAction();
	void Rotate();
	void Load(const int listNumber, const int handle, const VECTOR& pos);
	bool IsHitPlayer(const VECTOR& topPlayerPos,
		const VECTOR& bottomPlayerPos,
		const float playerRadius);

	HitCheck hitCheck;

	bool GetIsHitPlayer()const { return isHitPlayer; }
	bool GetIsDelete()const { return isDelete; }
	int GetListNumber()const { return listNumber; }
	VECTOR GetBoundsMin()const { return boundsMin; }
	VECTOR GetBoundsMax()const { return boundsMax; }

private:
	bool isHitPlayer;
	bool isDelete;
	bool isSound;

	int listNumber;
	float radianY;
	float velocity_Y;

	VECTOR flyAwayDirection;
	VECTOR flyAwayVelocity;			//吹っ飛ぶときの移動量
	VECTOR boundsMin;			//AABBの最小値
	VECTOR boundsMax;			//AABBの最大値

	static constexpr float radius = 7.0f;		//半径
	static constexpr float objectScale = 0.3f;	//オブジェクトの大きさ
	static constexpr float addVelocity = 0.4f;	//移動量
	static constexpr float flyPower = 5.0f;		//飛ぶ大きさ
};

