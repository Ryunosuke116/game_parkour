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
void HitCheck::CapsuleHitWallJudge(const int& modelHandle, int frameIndex,float radius,
	VECTOR linePos_start, VECTOR linePos_end, MV1_COLL_RESULT_POLY_DIM& hitPoly)
{
	hitPoly = MV1CollCheck_Capsule(modelHandle, frameIndex, linePos_start, linePos_end, radius);
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

/// @brief 面との接触座標の計算
/// @param centerPos 
/// @param a 
/// @param b 
/// @param c 
/// @return 
VECTOR HitCheck::ClosestPtToPointTriangle(VECTOR centerPos, VECTOR a, VECTOR b, VECTOR c)
{

	//PがAの外側の頂点座標の中にあるかどうかチェック
	VECTOR ab = VSub(b, a);
	VECTOR ac = VSub(c, a);
	VECTOR ap = VSub(centerPos, a);

	float d1 = VDot(ab, ap);
	float d2 = VDot(ac, ap);

	if (d1 <= 0.0f && d2 <= 0.0f)
	{
		//return a;	//重心座標(1,0,0)
	}

	//PがBの外側の頂点領域の中にあるかどうかチェック
	VECTOR bp = VSub(centerPos, b);
	float d3 = VDot(ab, bp);
	float d4 = VDot(ac, bp);

	if (d3 >= 0.0f && d4 <= d3)
	{
		//return b;
	}

	//PがABの辺領域の中にあるかどうかチェックし、あればPのAB上に対する射影を返す
	float vc = d1 * d4 - d3 * d2;
	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
	{
		float v = d1 / (d1 - d3);
		//return VAdd(a, VScale(ab, v));
	}

	//PがCの外側の頂点領域の中にあるかどうかチェック
	VECTOR cp = VSub(centerPos, c);
	float d5 = VDot(ab, cp);
	float d6 = VDot(ac, cp);
	if (d6 >= 0.0f && d5 <= d6)
	{
		//return c;
	}

	//PがACの辺領域の中にあるかどうかチェックし、あればPのAC上に対する射影を返す
	float vb = d5 * d2 - d1 * d6;

	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
	{
		float w = d2 / (d2 - d6);
		//return VAdd(a, VScale(ac, w));
	}

	//PがBCの辺領域の中にあるかどうかチェックし、あればPのBC上に対する射影を返す
	float va = d3 * d6 - d5 * d4;

	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
	{
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		//return VAdd(b, VScale(VSub(c, b), w));
	}

	//Pは面領域の中にある。Qをその重心座標(u,v,w)を用いて計算
	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;

	return VAdd(a, VAdd(VScale(ab, v), VScale(ac, w)));

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

/// <summary>
/// 球と球の当たり判定
/// </summary>
/// <param name="position_1"></param>
/// <param name="position_2"></param>
/// <param name="radius_1"></param>
/// <param name="radius_2"></param>
/// <returns></returns>
bool HitCheck::HitConfirmation(VECTOR position_1, VECTOR position_2, const float radius_1, const float radius_2)
{

	VECTOR positionSub = VSub(position_1, position_2);

	//球と球の中心点との距離
	float positionRange = VSize(positionSub);

	//半径の合計
	float radiusAdd = radius_1 + radius_2;

	// 1と2の半径よりも球と球の中心点との距離が近ければ当たっている
	if (radiusAdd > positionRange)
	{
		return true;
	}
	return false;
}

/// <summary>
/// カプセルから最も近い球を計算
/// </summary>
/// <param name="capsulePosition_1"></param>
/// <param name="capsulePosition_2"></param>
/// <param name="position"></param>
/// <param name="radius_1"></param>
/// <param name="radius_2"></param>
/// <returns></returns>
VECTOR HitCheck::CapsuleHitConfirmation(VECTOR capsulePosition_1, VECTOR capsulePosition_2, VECTOR position, const float radius_1, const float radius_2)
{
	VECTOR AB = VSub(capsulePosition_2, capsulePosition_1);
	VECTOR AP = VSub(position, capsulePosition_1);

	//ベクトルの長さ
	float vectorLength = pow((AB.x * AB.x) + (AB.y * AB.y) + (AB.z * AB.z), 0.5f);

	//単位ベクトル
	VECTOR unitVector = VGet(0, 0, 0);
	unitVector.x = AB.x / vectorLength;
	unitVector.y = AB.y / vectorLength;
	unitVector.z = AB.z / vectorLength;
	float productionVector = (unitVector.x * AP.x) + (unitVector.y * AP.y) + (unitVector.z * AP.z);

	//
	VECTOR AX;
	AX.x = capsulePosition_1.x + (unitVector.x * productionVector);
	AX.y = capsulePosition_1.y + (unitVector.y * productionVector);
	AX.z = capsulePosition_1.z + (unitVector.z * productionVector);

	if (AX.y <= capsulePosition_2.y)
	{
		AX.y = capsulePosition_2.y;
	}
	if (AX.y >= capsulePosition_1.y)
	{
		AX.y = capsulePosition_1.y;
	}

	return AX;

}

std::pair<VECTOR, VECTOR> HitCheck::SegmentTriangleDistance(const VECTOR& p, const VECTOR& q, const VECTOR& a, const VECTOR& b, const VECTOR& c)
{
	//線分の方向ベクトル
	VECTOR PQ = VSub(q, p);

	//線分を50分割して一つずつ調べる
	const int num = 50;
	float minSize = 1000;
	VECTOR returnPoint;
	VECTOR returnPT;

	for (int i = 0; i < num; i++)
	{

		float t = float(i) / num;

		VECTOR PT = VAdd(p, VScale(PQ, t));

		VECTOR point = ClosestPtToPointTriangle(PT, a, b, c);

		VECTOR size = VSub(point, PT);



		//一番距離が近いものを選択
		if (minSize > VSize(size))
		{
			minSize = VSize(size);
			returnPT = PT;
			returnPoint = point;
		}

	}

	return std::make_pair(returnPT, returnPoint);
	
}