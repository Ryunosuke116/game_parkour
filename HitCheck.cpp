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

/// @brief rayの当たり判定
/// @param modelHandle 
/// @param frameIndex 
/// @param linePos_start 
/// @param linePos_end 
/// @param hitPoly 
/// @return 
bool HitCheck::HitRayJudge(const int& modelHandle, int frameIndex,
	VECTOR linePos_start, VECTOR linePos_end, MV1_COLL_RESULT_POLY& hitPoly)
{
	hitPoly = MV1CollCheck_Line(modelHandle, frameIndex, linePos_start, linePos_end);

	return hitPoly.HitFlag;
}

/// @brief 球の当たり判定
/// @param modelHandle 
/// @param frameIndex 
/// @param linePos_end 
/// @param hitPoly 
/// @return 
bool HitCheck::SphereHitJudge(const int& modelHandle, int frameIndex, VECTOR linePos_end, MV1_COLL_RESULT_POLY_DIM& hitPoly)
{
	hitPoly = MV1CollCheck_Sphere(modelHandle, frameIndex, linePos_end, 3.5f);

	if (hitPoly.HitNum >= 1)return true;
	return false;
}

/// @brief カプセルの当たり判定
/// @param modelHandle 
/// @param frameIndex 
/// @param linePos_start 
/// @param linePos_end 
/// @param hitPoly 
void HitCheck::CapsuleHitWallJudge(const int& modelHandle, int frameIndex,
	VECTOR linePos_start, VECTOR linePos_end, MV1_COLL_RESULT_POLY_DIM& hitPoly)
{
	hitPoly = MV1CollCheck_Capsule(modelHandle, frameIndex, linePos_start, linePos_end, 3.5f);
}

/// <summary>
/// 垂線の足の計算
/// </summary>
/// <param name="point"></param>
/// <param name="P"></param>
/// <param name="Q"></param>
/// <returns></returns>
float HitCheck::projectionCalc(const VECTOR& point, const VECTOR& P, const VECTOR& Q)
{
	////線分ベクトル
	//VECTOR PQ = VSub(Q, P);
	//VECTOR P_point = VSub(point, P);
	//
	////PQの内積(2乗)
	//float len2 = VDot(PQ, PQ);

	//float t = VDot(PQ, P_point) / len2;

	//VECTOR foot = VAdd(P, VScale(PQ, t));
	//foot = VSub(point, foot);
	//float D = VSize(foot);

	VECTOR a, normal;
	VECTOR AP = VSub(point, a);

	float D = VDot(AP, normal) / VDot(normal, normal);

	return D;
}


void HitCheck::AABB()
{
	float r = 3.5f;
	float min_X, min_Y, min_Z;
	float max_X, max_Y, max_Z;

	min_X = r * -1.0f;
	min_X = r * -1.0f;
	min_Z = r * -1.0f;
	max_X = r * 1.0f;
	max_Y = r * 1.0f;
	max_Z = r * 1.0f;


}