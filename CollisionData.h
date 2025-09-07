#pragma once
#include "common.h"
#include <string>

struct CollisionResult
{
	VECTOR position_new = VGet(0.0f, 0.0f, 0.0f);
	bool isHitGround = false;
	bool isPossibleWallRun = false;
	VECTOR isHitWall_normal = VGet(0.0f, 0.0f, 0.0f);
	std::string objectTag = "";
};

struct PositionData
{
	VECTOR capsuleTopPosition;
	VECTOR capsuleBottomPosition;
	VECTOR rayTopPosition;
	VECTOR rayBottomPosition;
	VECTOR centerPosition;
	VECTOR oldPosition;
	VECTOR sideShaft;				//‰¡Ž²
};

struct HangingData
{
	bool isHitHanging;
	VECTOR hitHangingPos;
	MV1_COLL_RESULT_POLY hangingPoly;
};

struct AABB
{
	VECTOR min;
	VECTOR max;
};