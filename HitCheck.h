#pragma once
#include "common.h"
#include <cassert>
#include <unordered_map>
#include <string_view>
#include <array>
#include <vector>
#include <math.h>    
#include <cmath>
#include <memory>
#include "BaseObject.h"
#include "CollisionData.h"

class HitCheck
{
public:
	HitCheck();
	~HitCheck();

	//当たり判定
	//線とメッシュ
	static bool RayHitJudge(const int& modelHandle, int frameIndex,
		VECTOR linePos_start, VECTOR linePos_end, MV1_COLL_RESULT_POLY& hitPoly);

	//球とメッシュ
	static bool SphereHitJudge(const int& modelHandle,
		int frameIndex, const float& radius,
		VECTOR linePos_end, MV1_COLL_RESULT_POLY_DIM& hitPoly);
	static bool TriangleAreaCheck(const VECTOR& point, const VECTOR& a, const VECTOR& b, const VECTOR& c);
	static bool HitConfirmation(VECTOR position_1, VECTOR position_2, const float radius_1, const float radius_2);
	static bool TriangleAreaCheck_ground(const VECTOR& point, const VECTOR& a, const VECTOR& b, const VECTOR& c);

	static bool CapsuleHitJudge(const int& modelHandle, int frameIndex, float radius,
		VECTOR linePos_start, VECTOR linePos_end, MV1_COLL_RESULT_POLY_DIM& hitPoly);

	static float projectionCalc(const VECTOR& point, const VECTOR& P, const VECTOR& Q);

	static VECTOR CapsuleHitConfirmation(VECTOR capsulePosition_1, VECTOR capsulePosition_2, VECTOR position);

	/// <summary>
///  面との接触座標の計算
/// </summary>
/// <param name="centerPos"></param>
/// <param name="a"></param>
/// <param name="b"></param>
/// <param name="c"></param>
/// <returns></returns>
	static VECTOR ClosestPtToPointTriangle(VECTOR centerPos, VECTOR a, VECTOR b, VECTOR c);

	static bool AABBHitJudge(AABB a, AABB b);
	
	static std::pair<VECTOR,VECTOR> SegmentTriangleDistance(const VECTOR& p, const VECTOR& q, const VECTOR& a, const VECTOR& b, const VECTOR& c, const VECTOR& normal);
	static HangingData CliffGrabbing(
		const std::vector<std::weak_ptr<BaseObject>>& collisionObjects,
		const VECTOR& position,
		const VECTOR& topPosition,
		const VECTOR& moveDirection,
		const float& radius);

private:

};

