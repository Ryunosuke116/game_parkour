#include "Include.h"


/// @brief コンストラクタ
/// @return 
HitCheck::HitCheck()
{

}

/// @brief デストラクタ
/// @return 
HitCheck::~HitCheck()
{

}

bool HitCheck::HitRayJudge(const int& modelHandle, int frameIndex,
	VECTOR linePos_start, VECTOR linePos_end, MV1_COLL_RESULT_POLY& hitPoly)
{
	hitPoly = MV1CollCheck_Line(modelHandle, frameIndex, linePos_start, linePos_end);

//	MV1_COLL_RESULT_POLY_DIM hitPoly = MV1CollCheck_Sphere(modelHandle, frameIndex, linePos_start, 13.0f);

	DrawSphere3D(hitPoly.HitPosition, 2.0f, 30, GetColor(0, 0, 0),
		GetColor(255, 0, 0), FALSE);

	return hitPoly.HitFlag;
}

//bool HitCheck::HitWallJudge(const int& modelHandle, int frameIndex,
//	VECTOR linePos_start, VECTOR linePos_end)
//{
//	hitPoly_Wall.push_back()
//	return false;
//}
