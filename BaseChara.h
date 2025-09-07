#pragma once

#include "CollisionData.h"
#include "IObject.h"

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
	bool GetIsCollisionCheck()const { return isCollisionCheck; }
	float GetAngle()const { return angle; }
	VECTOR GetPosition()const { return position; }
	VECTOR GetFramePosition()const { return framePosition; }
	VECTOR GetMoveVec()const { return velocity; }
	PositionData GetPositionData() const { return positionData; }
	CollisionResult GetCollisionResult()const { return collision_result; }
	virtual float GetRadius()const abstract;

	void SetCollision_result(const CollisionResult& newResult) { collision_result = newResult; }
	void SetRotata_x(const float& set) { rotate_x = set; }
	void SetIsCollisionCheck(const bool& set) { isCollisionCheck = set; }
protected:

	int modelHandle;
	int nowFrameNumber;
	int coinCount;		//コインの所持枚数

	float angle;
	float rotate_x;

	bool isCollisionCheck;

	VECTOR position;
	VECTOR framePosition;
	VECTOR velocity;
	VECTOR targetMoveDirection;
	VECTOR moveDirection;

	PositionData positionData;
	CollisionResult collision_result;

	static constexpr float	MoveSpeed = 0.4f;	    // 移動速度
	static constexpr float angleSpeed = 0.2f;

	const VECTOR Initialize_aimPos = VGet(-0.169435501f, 53.7492065f, -1224.39844f);
	const VECTOR Initialize_spherePos = VGet(-1.28232884f, 24.0028648f, -1172.35425f);

};

