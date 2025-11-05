#pragma once
#include "CollisionData.h"
#include "IObject.h"
#include "Calculation.h"
#include"nlohmann/json.hpp"

class BaseChara : public IObject
{
public:
	BaseChara();
	~BaseChara();

	virtual void Load(const nlohmann::json& jsonData)abstract;
	void Draw()override;

	void UpdateAngle(const VECTOR& direction);
	void PositionUpdate();

	/////////////////////////////////////////////
	// ゲッター
	/////////////////////////////////////////////
	int GetCoinCount()const { return coinCount; }
	int GetModelHandle()const { return modelHandle; }
	bool GetIsCollisionCheck()const { return isCollisionCheck; }
	float GetRadian()const { return radian; }
	float GetDegree()const { return Calculation::RadToDeg(radian); }
	VECTOR GetPosition()const { return position; }
	VECTOR GetVelocity()const { return velocity; }
	PositionData GetPositionData() const { return positionData; }
	CollisionResult GetCollisionResult()const { return resultCollision; }
	CollisionHitPart GetCollisionHitPart()const { return collisionHitPart; }
	virtual float GetRadius()const abstract;

	void SetResultCollision(const CollisionResult& newResult) { resultCollision = newResult; }
	void SetRotateX(const float& set) { rotateX = set; }
	void SetIsCollisionCheck(const bool& set) { isCollisionCheck = set; }

protected:
	int modelHandle;		//モデルハンドル
	int nowFrameNumber;
	int coinCount;		//コインの所持枚数

	float radian;
	float rotateX;

	bool isCollisionCheck;

	VECTOR position;
	VECTOR framePosition;
	VECTOR velocity;
	VECTOR targetMoveDirection;
	VECTOR moveDirection;

	PositionData positionData;
	CollisionResult resultCollision;
	CollisionHitPart collisionHitPart;

	static constexpr float	MoveSpeed = 0.4f;				// 移動速度
	static constexpr float kRotationSpeedDegree = 11.0f;	// キャラクターが方向転換するスピード

	const VECTOR kInitializeCameraPos = VGet(-0.17f, 53.75f, -1224.4f);
	const VECTOR kInitializeSpherePos = VGet(-1.3f, 24.0f, -1172.35f);
};

