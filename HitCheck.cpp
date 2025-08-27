#include "common.h"
#include "HitCheck.h"
#include "Calculation.h"
#include "DebugDrawer.h"


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

/// <summary>
///  rayの当たり判定
/// </summary>
/// <param name="modelHandle"></param>
/// <param name="frameIndex"></param>
/// <param name="linePos_start"></param>
/// <param name="linePos_end"></param>
/// <param name="hitPoly"></param>
/// <returns></returns>
bool HitCheck::RayHitJudge(const int& modelHandle, int frameIndex,
	VECTOR linePos_start, VECTOR linePos_end, MV1_COLL_RESULT_POLY& hitPoly)
{
	hitPoly = MV1CollCheck_Line(modelHandle, frameIndex, linePos_start, linePos_end);

	return hitPoly.HitFlag;
}

/// <summary>
/// 球の当たり判定
/// </summary>
/// <param name="modelHandle"></param>
/// <param name="frameIndex"></param>
/// <param name="radius"></param>
/// <param name="linePos_end"></param>
/// <param name="hitPoly"></param>
/// <returns></returns>
bool HitCheck::SphereHitJudge(const int& modelHandle, 
	int frameIndex, const float& radius,
	VECTOR linePos_end, MV1_COLL_RESULT_POLY_DIM& hitPoly)
{
	hitPoly = MV1CollCheck_Sphere(modelHandle, frameIndex, linePos_end, radius);

	if (hitPoly.HitNum >= 1)return true;
	return false;
}

/// <summary>
/// カプセルの当たり判定
/// </summary>
/// <param name="modelHandle"></param>
/// <param name="frameIndex"></param>
/// <param name="radius"></param>
/// <param name="linePos_start"></param>
/// <param name="linePos_end"></param>
/// <param name="hitPoly"></param>
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

/// <summary>
///  面との接触座標の計算
/// </summary>
/// <param name="centerPos"></param>
/// <param name="a"></param>
/// <param name="b"></param>
/// <param name="c"></param>
/// <returns></returns>
VECTOR HitCheck::ClosestPtToPointTriangle(VECTOR centerPos, VECTOR a, VECTOR b, VECTOR c)
{

	//PがAの外側の頂点座標の中にあるかどうかチェック
	VECTOR ab = VSub(b, a);
	VECTOR ac = VSub(c, a);
	VECTOR ap = VSub(centerPos, a);

	float d1 = VDot(ab, ap);
	float d2 = VDot(ac, ap);

	//PがBの外側の頂点領域の中にあるかどうかチェック
	VECTOR bp = VSub(centerPos, b);
	float d3 = VDot(ab, bp);
	float d4 = VDot(ac, bp);

	//PがABの辺領域の中にあるかどうかチェックし、あればPのAB上に対する射影を返す
	float vc = d1 * d4 - d3 * d2;

	//PがCの外側の頂点領域の中にあるかどうかチェック
	VECTOR cp = VSub(centerPos, c);
	float d5 = VDot(ab, cp);
	float d6 = VDot(ac, cp);

	//PがACの辺領域の中にあるかどうかチェックし、あればPのAC上に対する射影を返す
	float vb = d5 * d2 - d1 * d6;

	//PがBCの辺領域の中にあるかどうかチェックし、あればPのBC上に対する射影を返す
	float va = d3 * d6 - d5 * d4;

	//Pは面領域の中にある。Qをその重心座標(u,v,w)を用いて計算
	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;

	return VAdd(a, VAdd(VScale(ab, v), VScale(ac, w)));

}


/// <summary>
/// AABB同士が接触しているか
/// </summary>
/// <param name="a"></param>
/// <param name="b"></param>
/// <returns></returns>
bool HitCheck::AABBHitJudge(AABB a, AABB b)
{
	if (a.min.x <= b.max.x &&
		a.max.x >= b.min.x &&
		a.min.y <= b.max.y &&
		a.max.y >= b.min.y &&
		a.min.z <= b.max.z &&
		a.max.z >= b.min.z)
	{
		return true;
	}

	return false;

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

	//単位ベクトル(正規化)
	VECTOR unitVector = VGet(0, 0, 0);
	unitVector.x = AB.x / vectorLength;
	unitVector.y = AB.y / vectorLength;
	unitVector.z = AB.z / vectorLength;

	//点の射影位置を計算(スカラー値)
	float productionVector = (unitVector.x * AP.x) + (unitVector.y * AP.y) + (unitVector.z * AP.z);

	//線分上の最近点を計算
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

/// <summary>
/// 面積を使った点の内外判定計算
/// </summary>
/// <param name="p"></param>
/// <param name="q"></param>
/// <param name="a"></param>
/// <param name="b"></param>
/// <param name="c"></param>
/// <param name="normal"></param>
/// <returns></returns>
std::pair<VECTOR, VECTOR> HitCheck::SegmentTriangleDistance(const VECTOR& p, const VECTOR& q, const VECTOR& a, const VECTOR& b, const VECTOR& c, const VECTOR& normal)
{
	//線分の方向ベクトル
	VECTOR PQ = VSub(q, p);

	//線分をx分割して一つずつ調べる
	const int num = 100;
	float minSize = 1000;
	VECTOR line_segment_point_closestSurface;		//面と一番近い線分点
	VECTOR hittingPoint_surface;					//面との接触点

	VECTOR point_P = ClosestPtToPointTriangle(p, a, b, c);
	VECTOR point_Q = ClosestPtToPointTriangle(q, a, b, c);

 	if (TriangleAreaCheck(point_P, a, b, c) && TriangleAreaCheck(point_Q, a, b, c))
	{
		//小さい方を返す
		if (VSize(point_P) < VSize(point_Q))
		{
			return std::make_pair(p, point_P);
		}
		else
		{
			return std::make_pair(q, point_Q);
		}
	}
	

	bool flag = false;

	for (int i = 0; i < num; i++)
	{
		float t = float(i) / num;

		//線分のどこを調べるか
		VECTOR PT = VAdd(p, VScale(PQ, t));

		//面のどこに当たっているか
		VECTOR point = ClosestPtToPointTriangle(PT, a, b, c);

		//三角形の内側かどうか
		if (TriangleAreaCheck(point, a, b, c))
		{

			//線分の点から接触面までの大きさ
			VECTOR size = VSub(point, PT);

			//一番距離が近いものを選択
			if (minSize > VSize(size))
			{
				minSize = VSize(size);
				line_segment_point_closestSurface = PT;
				hittingPoint_surface = point;
				flag = true;
			}
		}
	}

	if (!flag)
	{
		return std::make_pair(q, point_Q);
	}

	//一番近い線分の点と面の衝突座標を返す
	return std::make_pair(line_segment_point_closestSurface,
		hittingPoint_surface);
	
}

/// <summary>
/// 三角形の内側に点があるか
/// </summary>
/// <param name="point"></param>
/// <param name="a"></param>
/// <param name="b"></param>
/// <param name="c"></param>
/// <returns></returns>
bool HitCheck::TriangleAreaCheck(const VECTOR& point, const VECTOR& a, const VECTOR& b, const VECTOR& c)
{
	//面積を求める
	float area = fabs(Calculation::area(a, b, c));
	float area_1 = fabs(Calculation::area(a, b, point));
	float area_2 = fabs(Calculation::area(b, c, point));
	float area_3 = fabs(Calculation::area(c, a, point));

	//総面積と点を使った面積の合計の差が無いか
	float abs_ = abs((area_1 + area_2 + area_3) - area);

	//0より上かつ差があるか
	bool area_equal = abs_ < 1e-10;
	bool inside = area_1 > 0 && area_2 > 0 && area_3 > 0;

	return (area_equal && inside) ? true : false;
}

/// <summary>
/// 三角形の内側に点があるか
///		床用
/// </summary>
/// <param name="point"></param>
/// <param name="a"></param>
/// <param name="b"></param>
/// <param name="c"></param>
/// <returns></returns>
bool HitCheck::TriangleAreaCheck_ground(const VECTOR& point, const VECTOR& a, const VECTOR& b, const VECTOR& c)
{
	VECTOR point_ = point;
	VECTOR a_ = a;
	VECTOR b_ = b;
	VECTOR c_ = c;

	point_.y = 0.0f;
	a_.y = 0.0f;
	b_.y = 0.0f;
	c_.y = 0.0f;

	//面積を求める
	float area = fabs(Calculation::area(a_, b_, c_));
	float area_1 = fabs(Calculation::area(a_, b_, point_));
	float area_2 = fabs(Calculation::area(b_, c_, point_));
	float area_3 = fabs(Calculation::area(c_, a_, point_));

	//総面積と点を使った面積の合計の差が無いか
	float abs_ = abs((area_1 + area_2 + area_3) - area);

	//0より上かつ差があるか
	bool area_equal = abs_ < 1e-1;
	bool inside = area_1 > 0 && area_2 > 0 && area_3 > 0;

	return (area_equal && inside) ? true : false;
}

/// <summary>
/// 崖つかみ判定
/// </summary>
/// <param name="player"></param>
/// <param name="modelHandle"></param>
HangingData HitCheck::CliffGrabbing(
	const std::vector<std::weak_ptr<BaseObject>>& collisionObjects,
	const VECTOR& position,
	const VECTOR& topPosition,
	const VECTOR& moveDirection,
	const float& radius)
{
	const float max_velocity = 11.2f;			//移動量の最大値
	const float checkWidth = 4.0f;				//床確認の幅

	VECTOR spherePos = VAdd(topPosition, VScale(moveDirection, 5.0f));

	VECTOR startUpperCheckPos = VAdd(topPosition, VGet(0.0f, 5.0f, 0.0f));
	VECTOR endUpperCheckPos = VAdd(startUpperCheckPos, VScale(moveDirection, 8.0f));
	endUpperCheckPos.y = startUpperCheckPos.y - 4.0f;

	bool returnFlag = false;
	HangingData hangingData = { false,VGet(0.0f,0.0f,0.0f),NULL };

	DebugDrawer::Instance().InformationInput_line(startUpperCheckPos, endUpperCheckPos, GetColor(255, 0, 255));

	//TODO::崖を掴む条件を厳しくする
	// 1、プレイヤーの肩幅より少し大きめに横幅を計算
	// 2、横幅の両端から下にrayを飛ばして床があるか確認
	// 3、奥行計算

	//落下中にplayerの上部の球で判定を取る
	for (const auto& fieldObject : collisionObjects)
	{
		auto sharedCollisionObject = fieldObject.lock();

		MV1_COLL_RESULT_POLY_DIM poly_dim;
		MV1_COLL_RESULT_POLY polyRayCheck;

		HitCheck::SphereHitJudge(sharedCollisionObject->GetModelHandle(),
			-1,
			radius,
			spherePos, 
			poly_dim);

	/*	HitCheck::RayHitJudge(sharedCollisionObject->GetModelHandle(),
			-1,
			startUpperCheckPos,
			endUpperCheckPos,
			polyRayCheck);*/

		//if (polyRayCheck.HitFlag)
		//{
		//	if (sharedCollisionObject->GetTag() != "field")
		//	{
		//		continue;
		//	}

		//	float minSize = NULL;
		//	//平面に当たっていればtrueに
		//	if (polyRayCheck.Normal.y >= 0.8f)
		//	{
		//		//三角形の一番近い辺から一番近い点を求める
		//		VECTOR nearestOutSide = Calculation::SphereMeshOutsideTriangle(
		//			polyRayCheck,
		//			position);

		//		//一番近い点の元の辺を求める
		//		Calculation::NearestResult nearestResult =
		//			Calculation::SphereMeshOutsideTriangle_line(
		//				polyRayCheck,
		//				position);

		//		//奥行を調べるための座標
		//		VECTOR depthDirection = VSub(nearestOutSide, position);
		//		depthDirection = VNorm(depthDirection);

		//		depthDirection = Calculation::Projection(polyRayCheck.Normal, depthDirection);

		//		VECTOR rightRayPoint = VNorm(VSub(nearestResult.linePos_end, nearestOutSide));
		//		VECTOR leftRayPoint = VNorm(VSub(nearestResult.linePos_start, nearestOutSide));

		//		rightRayPoint = VScale(rightRayPoint, checkWidth);
		//		leftRayPoint = VScale(leftRayPoint, checkWidth);

		//		rightRayPoint = VAdd(nearestOutSide, rightRayPoint);
		//		leftRayPoint = VAdd(nearestOutSide, leftRayPoint);

		//		rightRayPoint = VAdd(rightRayPoint, VScale(depthDirection, 0.5f));
		//		leftRayPoint = VAdd(leftRayPoint, VScale(depthDirection, 0.5f));

		//		VECTOR endRightRayPoint = VAdd(rightRayPoint, VGet(0.0f, -1.0f, 0.0f));
		//		VECTOR endLeftRayPoint = VAdd(leftRayPoint, VGet(0.0f, -1.0f, 0.0f));
		//		MV1_COLL_RESULT_POLY leftRayCheck;
		//		MV1_COLL_RESULT_POLY rightRayCheck;

		//		DebugDrawer::Instance().InformationInput_line(leftRayPoint, endLeftRayPoint, GetColor(255, 255, 255));
		//		DebugDrawer::Instance().InformationInput_line(rightRayPoint, endRightRayPoint, GetColor(255, 255, 255));
		//		DebugDrawer::Instance().InformationInput_sphere(nearestOutSide, 2.5f, GetColor(255, 0, 255));
		//		for (const auto& fieldObject : collisionObjects)
		//		{
		//			//掴む所の幅を確認して、一定の幅がないと掴めないようにする
		//			auto sharedCollisionObject = fieldObject.lock();
		//			HitCheck::RayHitJudge(
		//				sharedCollisionObject->GetModelHandle(),
		//				-1,
		//				leftRayPoint,
		//				endLeftRayPoint,
		//				leftRayCheck);

		//			HitCheck::RayHitJudge(
		//				sharedCollisionObject->GetModelHandle(),
		//				-1,
		//				rightRayPoint,
		//				endRightRayPoint,
		//				rightRayCheck);

		//			if (leftRayCheck.HitFlag && 
		//				rightRayCheck.HitFlag)break;
		//		}

		//		if (!leftRayCheck.HitFlag || 
		//			!rightRayCheck.HitFlag)continue;

		//		VECTOR depthDistance = VScale(depthDirection, max_velocity);
		//		depthDistance = VAdd(nearestOutSide, depthDistance);

		//		//少し浮かせる
		//		VECTOR startWallCheckLine = VAdd(nearestOutSide, VScale(polyRayCheck.Normal, 0.2f));
		//		VECTOR endWallCheckLine = VAdd(depthDistance, VScale(polyRayCheck.Normal, 0.2f));

		//		DebugDrawer::Instance().InformationInput_capsule(startWallCheckLine, endWallCheckLine, 1.0f, GetColor(255, 0, 255));

		//		MV1_COLL_RESULT_POLY wallCheck = {};

		//		//壁に当たっていたら崖掴みができない
		//		for (const auto& collisionObject : collisionObjects)
		//		{
		//			auto sharedCollisionObject = fieldObject.lock();

		//			HitCheck::RayHitJudge(
		//				sharedCollisionObject->GetModelHandle(),
		//				-1,
		//				startWallCheckLine,
		//				endWallCheckLine,
		//				wallCheck);

		//			if (wallCheck.HitFlag)break;
		//		}

		//		//奥行がない場合掴めない
		//		if (wallCheck.HitFlag) continue;


		//		VECTOR sub = VSub(nearestOutSide, spherePos);
		//		float sub_size = VSize(sub);

		//		//一番差が小さい情報を取得
		//		if (minSize == NULL || minSize >= sub_size)
		//		{
		//			minSize = sub_size;
		//			hangingData.hangingPoly = polyRayCheck;
		//		}

		//		hangingData.isHitHanging = true;
		//	}
		//}

		if (poly_dim.HitNum >= 1)
		{
			if (sharedCollisionObject->GetTag() != "field")
			{
				continue;
			}

			float minSize = NULL;

			for (int i = 0; i < poly_dim.HitNum; i++)
			{
				MV1_COLL_RESULT_POLY poly = poly_dim.Dim[i];

				//平面に当たっていればtrueに
				if (poly.Normal.y >= 0.8f)
				{
					//三角形の一番近い辺から一番近い点を求める
					VECTOR nearestOutSide = Calculation::SphereMeshOutsideTriangle(
						poly,
						position);

					Calculation::NearestResult nearestResult = 
						Calculation::SphereMeshOutsideTriangle_line(
							poly,
							position);

				//奥行を調べるための座標
				VECTOR depthDirection = VSub(nearestOutSide, position);
				depthDirection = VNorm(depthDirection);

				depthDirection = Calculation::Projection(poly.Normal, depthDirection);

				VECTOR rightRayPoint = VNorm(VSub(nearestResult.linePos_end, nearestOutSide));
				VECTOR leftRayPoint = VNorm(VSub(nearestResult.linePos_start, nearestOutSide));

				rightRayPoint = VScale(rightRayPoint, checkWidth);
				leftRayPoint = VScale(leftRayPoint, checkWidth);

				rightRayPoint = VAdd(nearestOutSide, rightRayPoint);
				leftRayPoint = VAdd(nearestOutSide, leftRayPoint);

				rightRayPoint = VAdd(rightRayPoint, VScale(depthDirection, 0.5f));
				leftRayPoint = VAdd(leftRayPoint, VScale(depthDirection, 0.5f));

				VECTOR endRightRayPoint = VAdd(rightRayPoint, VGet(0.0f, -1.0f, 0.0f));
				VECTOR endLeftRayPoint = VAdd(leftRayPoint, VGet(0.0f, -1.0f, 0.0f));
				MV1_COLL_RESULT_POLY leftRayCheck;
				MV1_COLL_RESULT_POLY rightRayCheck;

				DebugDrawer::Instance().InformationInput_line(leftRayPoint, endLeftRayPoint, GetColor(255, 255, 255));
				DebugDrawer::Instance().InformationInput_line(rightRayPoint, endRightRayPoint, GetColor(255, 255, 255));
				DebugDrawer::Instance().InformationInput_sphere(nearestOutSide, 2.5f, GetColor(255, 0, 255));
				for (const auto& fieldObject : collisionObjects)
				{
					//掴む所の幅を確認して、一定の幅がないと掴めないようにする
					auto sharedCollisionObject = fieldObject.lock();
					HitCheck::RayHitJudge(
						sharedCollisionObject->GetModelHandle(),
						-1,
						leftRayPoint,
						endLeftRayPoint,
						leftRayCheck);

					HitCheck::RayHitJudge(
						sharedCollisionObject->GetModelHandle(),
						-1,
						rightRayPoint,
						endRightRayPoint,
						rightRayCheck);

					if (leftRayCheck.HitFlag && 
						rightRayCheck.HitFlag)break;
				}

				if (!leftRayCheck.HitFlag || 
					!rightRayCheck.HitFlag)continue;

					depthDirection = Calculation::Projection(poly.Normal, depthDirection);

					//奥行確認のためのrayの長さ
					VECTOR depthDistance = VScale(depthDirection, max_velocity);
					depthDistance = VAdd(nearestOutSide, depthDistance);

					//少し浮かせる
					VECTOR startWallCheckLine = VAdd(nearestOutSide, VScale(poly.Normal, 0.2f));
					VECTOR endWallCheckLine = VAdd(depthDistance, VScale(poly.Normal, 0.2f));


					DebugDrawer::Instance().InformationInput_capsule(startWallCheckLine, endWallCheckLine,1.0f, GetColor(255, 0, 255));

					MV1_COLL_RESULT_POLY wallCheck = {};

					//壁に当たっていたら崖掴みができない
					HitCheck::RayHitJudge(
						sharedCollisionObject->GetModelHandle(),
						-1,
						startWallCheckLine,
						endWallCheckLine,
						wallCheck);

					//奥行がない場合掴めない
					if (wallCheck.HitFlag) continue;

					//if (max_velocity > depth) continue;

					//VECTOR neareast = Calculation::SphereMeshOutsideTriangle(poly, spherePos);
					//DebugDrawer::Instance().InformationInput_sphere(neareast, 2.0f, GetColor(255, 0, 255));

					//VECTOR line_end = Calculation::Projection(poly.Normal, moveDirection);

					//line_end = VAdd(neareast, VScale(line_end, max_velocity));

					//if (HitCheck::RayHitJudge(fieldObject->GetModelHandle(),
					//	-1,
					//	neareast,
					//	line_end,
					//	ray_poly
					//)) continue;

					VECTOR sub = VSub(nearestOutSide, topPosition);
					float sub_size = VSize(sub);

					//一番差が小さい情報を取得
					if (minSize == NULL || minSize >= sub_size)
					{
						minSize = sub_size;
						hangingData.hangingPoly = poly;

					}
					
					hangingData.isHitHanging = true;
				}
			}

			//平面に当たっていなければfalse
			if (minSize == NULL)
			{
				hangingData.isHitHanging = false;
			}
		}

		// 検出したプレイヤーの周囲のポリゴン情報を開放する
		MV1CollResultPolyDimTerminate(poly_dim);
	}
	DebugDrawer::Instance().InformationInput_sphere(spherePos, radius, GetColor(255, 255, 255));

	return hangingData;

	//trueの場合に崖をつかむようにする
}