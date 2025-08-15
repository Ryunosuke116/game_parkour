#pragma once

#include "CollisionData.h"
#include "Object_interface.h"

class BaseChara : public Object_interface
{
protected:

	int modelHandle;
	int nowFrameNumber;
	int coinCount;		//コインの所持枚数

	float angle;
	float rotate_x;

	bool isCollisionCheck;

	VECTOR position;
	VECTOR framePosition;
	VECTOR moveVec;
	VECTOR targetMoveDirection;
	VECTOR moveDirection;

	PositionData positionData;
	CollisionResult collision_result;

	static constexpr float	MoveSpeed = 0.4f;	    // 移動速度
	static constexpr float angleSpeed = 0.2f;

public:

	BaseChara();
	~BaseChara();

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
	VECTOR GetMoveVec()const { return moveVec; }
	PositionData GetPositionData() const { return positionData; }
	CollisionResult GetCollisionResult()const { return collision_result; }
	virtual float GetRadius()const abstract;

	void SetCollision_result(const CollisionResult& newResult) { collision_result = newResult; }
	void SetRotata_x(const float& set) { rotate_x = set; }
	void SetIsCollisionCheck(const bool& set) { isCollisionCheck = set; }
	void SetRotate_y(const float& set) { angle = set; }
	void SetTargetMoveDirection(const VECTOR& set) { targetMoveDirection = set; }
};

