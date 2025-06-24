#pragma once
#include <iostream>
#include <cassert>
#include <unordered_map>
#include <string_view>
#include <array>
#include <vector>
#include <math.h>    
#include <cmath>
#include <memory>
#include "DxLib.h"

class HitCheck
{
public:
	HitCheck();
	~HitCheck();

	//当たり判定
	//線とメッシュ
	static bool HitRayJudge(const int& modelHandle, int frameIndex,
		VECTOR linePos_start, VECTOR linePos_end, MV1_COLL_RESULT_POLY& hitPoly);
	//球とメッシュ
	static bool SphereHitJudge(const int& modelHandle, int frameIndex,
		VECTOR linePos_end, MV1_COLL_RESULT_POLY_DIM& hitPoly);
	static bool TriangleAreaCheck(const VECTOR& point, const VECTOR& a, const VECTOR& b, const VECTOR& c);
	static bool HitConfirmation(VECTOR position_1, VECTOR position_2, const float radius_1, const float radius_2);
	static bool TriangleAreaCheck_ground(const VECTOR& point, const VECTOR& a, const VECTOR& b, const VECTOR& c);

	static void CapsuleHitWallJudge(const int& modelHandle, int frameIndex, float radius,
		VECTOR linePos_start, VECTOR linePos_end, MV1_COLL_RESULT_POLY_DIM& hitPoly);

	static float projectionCalc(const VECTOR& point, const VECTOR& P, const VECTOR& Q);


	static VECTOR CapsuleHitConfirmation(VECTOR capsulePosition_1, VECTOR capsulePosition_2, VECTOR position, const float radius_1, const float radius_2);
	static VECTOR ClosestPtToPointTriangle(VECTOR centerPos, VECTOR a, VECTOR b, VECTOR c);

	static void AABB();
	
	static std::pair<VECTOR,VECTOR> SegmentTriangleDistance(const VECTOR& p, const VECTOR& q, const VECTOR& a, const VECTOR& b, const VECTOR& c, const VECTOR& normal);


private:

	static HitCheck* instance;

};

