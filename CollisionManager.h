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
	std::pair<bool, std::string> GroundCollisionCheck(const std::vector<std::weak_ptr<BaseObject>>& collisionObjects,
		const VECTOR& oldPos, VECTOR& newPos, const VECTOR& moveVec,
		const PositionData& positionData);

	bool HeadCollisionCheck(const std::vector<std::weak_ptr<BaseObject>>& collisionObjects, VECTOR& newPos,
		const VECTOR& moveVec, const PositionData& positionData, const float& radius);

	std::pair<bool, VECTOR> WallCollisionCheck(const std::vector<std::weak_ptr<BaseObject>>& collisionObjects,
		VECTOR& newPos, const VECTOR& moveVec, const PositionData& positionData,
		const float& radius);

	CollisionResult Check_all(const std::vector<std::weak_ptr<BaseObject>>& collisionObjects,
		const VECTOR& playerPos, const VECTOR& moveVec, const float& radius,
		const PositionData& positionData, const PlayerData& playerData);

	void Update(BaseChara& chara,
		const PlayerData& playerData);

	std::pair<bool, VECTOR>GroundCollisionCheck_Hang_to_Crouch(const std::vector<std::weak_ptr<BaseObject>>& collisionObjects,
		const VECTOR& oldPos, const VECTOR& newPos, const VECTOR& foot, const PositionData& positionData);

	float GetTiltAngle_degree()const { return tiltAngle_degree; }

	bool Draw();
private:
	MV1_COLL_RESULT_POLY hitPoly_Ground;
	MV1_COLL_RESULT_POLY rayPoly_ground_now;

	MV1_COLL_RESULT_POLY_DIM hitPoly_Wall;

	VECTOR hitPos_head;
	VECTOR hitPos_ground;

	VECTOR ray_start_hanging_log;
	VECTOR ray_end_hanging_log;
	float tiltAngle_degree;

	PositionData positionData;

	PositionData GetPositionData() { return positionData; }

};

