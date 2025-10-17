#pragma once
#include "PlayerStateBase.h"
#include "BaseObject.h"
#include "BaseChara.h"
#include "CollisionData.h"
#include "PlayerData.h"
#include <vector>
#include <string>

class HitCheck;

class CollisionManager
{
public:
	//////////////////////////////////////////
	//è’ìÀîªíË
	/////////////////////////////////////////
	CollisionResult GroundCollisionCheck(const std::vector<std::weak_ptr<BaseObject>>& collisionObjects,
		const VECTOR& oldPos, const VECTOR& subjectPosition, const VECTOR& velocity,
		const PositionData& positionData);

	VECTOR HeadCollisionCheck(const std::vector<std::weak_ptr<BaseObject>>& collisionObjects,
		const VECTOR& subjectPos,
		const VECTOR& velocity,
		const PositionData& positionData,
		const float& radius);

	VECTOR WallCollisionCheck(const std::vector<std::weak_ptr<BaseObject>>& collisionObjects,
		const VECTOR& subjectPos, const VECTOR& velocity, const PositionData& positionData,
		const float& radius);

	CollisionResult AllCheck(const std::vector<std::weak_ptr<BaseObject>>& collisionObjects,
		const VECTOR& playerPos, const VECTOR& velocity, const float& radius,
		const PositionData& positionData, const PlayerData& playerData);

	void Update(BaseChara& chara,
		const PlayerData& playerData);

	VECTOR WallGroundCollisionCheck(
		const std::vector<std::weak_ptr<BaseObject>>& collisionObjects,
		const VECTOR& oldPos,
		const VECTOR& newPos,
		const VECTOR& velocity,
		const VECTOR& gravityDirection,
		const float radius,
		const PositionData& positionData);

	float GetTiltAngle_degree()const { return tiltAngleDegree; }
private:
	MV1_COLL_RESULT_POLY hitPoly_Ground;
	MV1_COLL_RESULT_POLY nowGroundRayPoly;

	VECTOR hitPos_ground;

	VECTOR ray_start_hanging_log;
	VECTOR ray_end_hanging_log;
	float tiltAngleDegree;

	PositionData positionData;

	PositionData GetPositionData() { return positionData; }

	static constexpr float kAngleRange = 60.0f;
	static constexpr int kFrameIndex = -1;
	const VECTOR kLengthDirection = VGet(0.0f, 1.0f, 0.0f);
};

