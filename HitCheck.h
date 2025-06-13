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

	bool HitRayJudge(const int& modelHandle, int frameIndex,
		VECTOR linePos_start, VECTOR linePos_end, MV1_COLL_RESULT_POLY& hitPoly);
	bool SphereHitJudge(const int& modelHandle, int frameIndex,
		VECTOR linePos_end, MV1_COLL_RESULT_POLY_DIM& hitPoly);

	void CapsuleHitWallJudge(const int& modelHandle, int frameIndex, float radius,
		VECTOR linePos_start, VECTOR linePos_end, MV1_COLL_RESULT_POLY_DIM& hitPoly);

	float projectionCalc(const VECTOR& point, const VECTOR& P, const VECTOR& Q);

	bool HitConfirmation(VECTOR position_1, VECTOR position_2, const float radius_1, const float radius_2);
	VECTOR CapsuleHitConfirmation(VECTOR capsulePosition_1, VECTOR capsulePosition_2, VECTOR position, const float radius_1, const float radius_2);
	
	void AABB();

	/*MV1_COLL_RESULT_POLY GetHitPoly_Ground() { return hitPoly_Ground; }*/

	const std::vector< MV1_COLL_RESULT_POLY>& GetHitPoly_Wall() const { return hitPoly_Wall; }


private:
	std::vector<MV1_COLL_RESULT_POLY> hitPoly_Wall;
};

