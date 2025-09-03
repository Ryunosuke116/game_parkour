#pragma once
#include "common.h"
#include <string>

struct CollisionResult
{
	VECTOR position_new = VGet(0.0f, 0.0f, 0.0f);
	bool isHitGround = false;
	bool isPossible_wallRun = false;
	VECTOR isHitWall_normal = VGet(0.0f, 0.0f, 0.0f);
	std::string objectTag = "";
};

struct PositionData
{
	VECTOR position_top_Capsule;
	VECTOR position_bottom_Capsule;
	VECTOR position_top_ray;
	VECTOR position_bottom_ray;
	VECTOR oldPosition;
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