#pragma once
#include "Common.h"
#include <string>

struct CollisionResult
{
	MV1_COLL_RESULT_POLY nowGroundRayPoly = { NULL };
	VECTOR newPosition = VGet(0.0f, 0.0f, 0.0f);
	std::string objectTag = "";
};

struct NearestResult
{
	VECTOR startLinePos;
	VECTOR endLinePos;
	VECTOR nearestPoint;
};

struct PositionData
{
	VECTOR capsuleTopPosition;
	VECTOR capsuleBottomPosition;
	VECTOR rayTopPosition;
	VECTOR rayBottomPosition;
	VECTOR centerPosition;
	VECTOR oldPosition;
	VECTOR sideShaft;				//キャラクターの横軸
};

struct HangingData
{
	bool isHitHanging;					//掴めるか
	MV1_COLL_RESULT_POLY hangingPoly;	
};

struct AABB
{
	VECTOR min;
	VECTOR max;
};

struct CollisionHitPart
{
	bool isHitWall;
	bool isHitHead;
	bool isHitFloor;
};