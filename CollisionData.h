#pragma once
#include "common.h"
#include <string>

struct CollisionResult
{
	VECTOR position_new;
	bool isHitGround;
	bool isHitWall;
	VECTOR isHitWall_normal;
	std::string objectTag;
};

struct PositionData
{
	VECTOR position_top_Capsule;
	VECTOR position_bottom_Capsule;
	VECTOR position_top_ray;
	VECTOR position_bottom_ray;
};

struct HangingData
{
	bool isHitHanging;
	VECTOR hitHangingPos;
	MV1_COLL_RESULT_POLY hangingPoly;
};