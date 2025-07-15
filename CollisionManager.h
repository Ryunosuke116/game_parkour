#pragma once
#include "PlayerStateActionBase.h"
#include "BaseObject.h"

class HitCheck;

class CollisionManager
{
public:
	struct PositionData
	{
		static constexpr float addTopPos = 17.0f;
		static constexpr float addBottomPos = 3.0f;
		static constexpr float radius = 3.5f;
		VECTOR footPosition;
	};

	bool GroundCollisionCheck(int modelHandle, const VECTOR& oldPos,
		VECTOR& newPos, const PositionData& positionData,
		const PlayerStateActionBase::PlayerData& playerData);
	bool HeadCollisionCheck(int modelHandle, VECTOR& newPos, 
		const PositionData& positionData);
	bool WallCollisionCheck(int modelHandle, VECTOR& newPos,
		VECTOR& oldPos, const PositionData& positionData);
	//bool Update(Player& player, int modelHandle);
	bool Draw();

	std::pair<bool, VECTOR> Update(const std::vector<std::shared_ptr<BaseObject>>& fieldObjects,
		const VECTOR& playerPos,
		const VECTOR& moveVec, const PositionData& positionData,
		const PlayerStateActionBase::PlayerData& playerData);

	bool TestSphereTriangle(VECTOR centerPos, VECTOR a, VECTOR b, VECTOR c, VECTOR& q, const float radius);

	std::pair<bool, VECTOR> CliffGrabbing(int modelHandle,
		const VECTOR& topPosition, const VECTOR& moveDirection, const bool isFalling);
	VECTOR PushBackCalculation_sphere_mesh(const MV1_COLL_RESULT_POLY& poly, const VECTOR& bottomPos, const VECTOR& newPlayerPos, const float& radius);
	VECTOR CalcPushBack_SphereMeshOutsideTriangle(const MV1_COLL_RESULT_POLY& poly, const VECTOR& HitPos_ground, const VECTOR& bottomPos, const float& radius);

	std::pair<bool, VECTOR>GroundCollisionCheck_Hang_to_Crouch(int modelHandle,
		const VECTOR& oldPos, const VECTOR& newPos, const VECTOR& foot, const PositionData& positionData);

	float GetTiltAngle_degree()const { return tiltAngle_degree; }
	MV1_COLL_RESULT_POLY GetHangingPoly() const { return HangingPoly; }

private:
	MV1_COLL_RESULT_POLY hitPoly_Ground;
	MV1_COLL_RESULT_POLY oldPoly;
	MV1_COLL_RESULT_POLY HangingPoly;

	MV1_COLL_RESULT_POLY_DIM hitPoly_Wall;
	MV1_COLL_RESULT_POLY_DIM hitPoly_Ground_sphere;

	VECTOR hitPos_head;
	VECTOR subPos;
	VECTOR normal;
	VECTOR hitPos_wall;
	VECTOR hitPos_ground;
	VECTOR playerCenterPos;
	VECTOR pos_now;
	VECTOR pos_new;
	VECTOR nearestPoint;
	VECTOR hitSphere;
	VECTOR topPos_ray;
	VECTOR bottomPos_ray;
	VECTOR hitHangingPos;
	float tiltAngle_degree;


	PositionData positionData;

	PositionData GetPositionData() { return positionData; }

};

