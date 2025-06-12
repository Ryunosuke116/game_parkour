#pragma once
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
	
	void AABB();

	/*MV1_COLL_RESULT_POLY GetHitPoly_Ground() { return hitPoly_Ground; }*/

	const std::vector< MV1_COLL_RESULT_POLY>& GetHitPoly_Wall() const { return hitPoly_Wall; }


private:
	std::vector<MV1_COLL_RESULT_POLY> hitPoly_Wall;
};

