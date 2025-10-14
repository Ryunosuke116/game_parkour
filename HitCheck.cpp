#include "Common.h"
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

bool HitCheck::RayHitJudge(const int modelHandle, 
	const int frameIndex,
	const VECTOR& startLinePos, 
	const VECTOR& endLinePos,
	MV1_COLL_RESULT_POLY& hitPoly)
{
	hitPoly = MV1CollCheck_Line(modelHandle, frameIndex, startLinePos, endLinePos);

	return hitPoly.HitFlag;
}

bool HitCheck::SphereHitJudge(const int modelHandle, 
	const int frameIndex,
	const float radius,
	const VECTOR& endLinePos,
	MV1_COLL_RESULT_POLY_DIM& hitPoly)
{
	hitPoly = MV1CollCheck_Sphere(modelHandle, frameIndex, endLinePos, radius);

	if (hitPoly.HitNum >= 1)
	{
		return true;
	}
	return false;
}

bool HitCheck::CapsuleHitJudge(const int modelHandle, 
	const int frameIndex,
	const float radius,
	const VECTOR& startLinePos,
	const VECTOR& endLinePos, 
	MV1_COLL_RESULT_POLY_DIM& hitPoly)
{
	hitPoly = MV1CollCheck_Capsule(modelHandle, frameIndex, startLinePos, endLinePos, radius);
	
	if (hitPoly.HitNum >= 1)return true;
	return false;
}

VECTOR HitCheck::ClosestPtToPointTriangle(const VECTOR& point,
	const VECTOR& vertex1,
	const VECTOR& vertex2,
	const VECTOR& vertex3)
{
	//PがAの外側の頂点座標の中にあるかどうかチェック
	VECTOR edgeAB = VSub(vertex2, vertex1);
	VECTOR edgeAC = VSub(vertex3, vertex1);
	VECTOR vecAP = VSub(point, vertex1);

	float dotAB_AP = VDot(edgeAB, vecAP);
	float dotAC_AP = VDot(edgeAC, vecAP);

	//PがBの外側の頂点領域の中にあるかどうかチェック
	VECTOR vecBP = VSub(point, vertex2);
	float dotAB_BP = VDot(edgeAB, vecBP);
	float dotAC_BP = VDot(edgeAC, vecBP);

	//PがABの辺領域の中にあるかどうかチェックし、あればPのAB上に対する射影を返す
	float areaAB = dotAB_AP * dotAC_BP - dotAB_BP * dotAC_AP;

	//PがCの外側の頂点領域の中にあるかどうかチェック
	VECTOR vecCP = VSub(point, vertex3);
	float dotAB_CP = VDot(edgeAB, vecCP);
	float dotAC_CP = VDot(edgeAC, vecCP);

	//PがACの辺領域の中にあるかどうかチェックし、あればPのAC上に対する射影を返す
	float areaAC = dotAB_CP * dotAC_AP - dotAB_AP * dotAC_CP;

	//PがBCの辺領域の中にあるかどうかチェックし、あればPのBC上に対する射影を返す
	float areaBC = dotAB_BP * dotAC_CP - dotAB_CP * dotAC_BP;

	//Pは面領域の中にある。Qをその重心座標(u,v,w)を用いて計算
	float baryDenom = 1.0f / (areaBC + areaAC + areaAB);
	float baryV = areaAC * baryDenom;
	float baryW = areaAB * baryDenom;

	return VAdd(vertex1, VAdd(VScale(edgeAB, baryV), VScale(edgeAC, baryW)));

}

bool HitCheck::AABBHitJudge(const AABB& AABB1, const AABB& AABB2)
{
	if (AABB1.min.x <= AABB2.max.x &&
		AABB1.max.x >= AABB2.min.x &&
		AABB1.min.y <= AABB2.max.y &&
		AABB1.max.y >= AABB2.min.y &&
		AABB1.min.z <= AABB2.max.z &&
		AABB1.max.z >= AABB2.min.z)
	{
		return true;
	}

	return false;
}

bool HitCheck::HitConfirmation(
	const VECTOR& spherePos1,
	const VECTOR& spherePos2, 
	const float radius1,
	const float radius2)
{
	VECTOR positionSub = VSub(spherePos1, spherePos2);

	//球と球の中心点との距離
	float positionRange = VSize(positionSub);

	//半径の合計
	float radiusAdd = radius1 + radius2;

	// 1と2の半径よりも球と球の中心点との距離が近ければ当たっている
	if (radiusAdd > positionRange)
	{
		return true;
	}
	return false;
}

VECTOR HitCheck::CapsuleHitConfirmation(const VECTOR& capsulePos1,
	const VECTOR& capsulePos2,
	const VECTOR& point)
{
	VECTOR edgeAB = VSub(capsulePos2, capsulePos1);
	VECTOR edgeAP = VSub(point, capsulePos1);

	//単位ベクトル(正規化)
	VECTOR unitVector = VNorm(edgeAB);

	//点の射影位置を計算(スカラー値)
	float productionVector = VDot(edgeAP, unitVector);

	//線分上の最近点を計算
	VECTOR edgeAX;
	edgeAX.x = capsulePos1.x + (unitVector.x * productionVector);
	edgeAX.y = capsulePos1.y + (unitVector.y * productionVector);
	edgeAX.z = capsulePos1.z + (unitVector.z * productionVector);


	if (edgeAX.y <= capsulePos2.y)
	{
		edgeAX.y = capsulePos2.y;
	}
	if (edgeAX.y >= capsulePos1.y)
	{
		edgeAX.y = capsulePos1.y;
	}

	return edgeAX;
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
std::pair<VECTOR, VECTOR> HitCheck::SegmentTriangleDistance(const VECTOR& p,
	const VECTOR& q,
	const VECTOR& a, 
	const VECTOR& b, 
	const VECTOR& c, 
	const VECTOR& normal)
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
		float normalDistanceProgress = float(i) / num;

		//線分のどこを調べるか
		VECTOR PT = VAdd(p, VScale(PQ, normalDistanceProgress));

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
/// 崖掴み判定
/// </summary>
/// <param name="collisionObjects"></param>
/// <param name="position"></param>
/// <param name="topPosition"></param>
/// <param name="moveDirection"></param>
/// <param name="radius"></param>
/// <returns></returns>
HangingData HitCheck::CliffGrabbing(
	const std::vector<std::weak_ptr<BaseObject>>& collisionObjects,
	const VECTOR& position,
	const VECTOR& topPosition,
	const VECTOR& moveDirection,
	const float& radius)
{
	const int kFrameIndex = -1;
	const float kMaxVelocity = 11.2f;			//移動量の最大値
	const float kCheckWidth = 4.0f;				//床確認の幅
	const float kAngleRange = 50.0f;
	const VECTOR kLengthDirection = VGet(0.0f, 1.0f, 0.0f);

	VECTOR spherePos = VAdd(topPosition, VScale(moveDirection, 5.0f));

	VECTOR startUpperCheckPos = VAdd(topPosition, VGet(0.0f, 5.0f, 0.0f));
	VECTOR endUpperCheckPos = VAdd(startUpperCheckPos, VScale(moveDirection, 8.0f));
	endUpperCheckPos.y = startUpperCheckPos.y - 4.0f;

	HangingData hangingData = { false,VGet(0.0f,0.0f,0.0f),NULL };

	//落下中にplayerの上部の球で判定を取る
	for (const auto& fieldObject : collisionObjects)
	{
		auto sharedCollisionObject = fieldObject.lock();

		MV1_COLL_RESULT_POLY_DIM poly_dim;

		HitCheck::SphereHitJudge(
			sharedCollisionObject->GetModelHandle(),
			-1,
			radius,
			spherePos, 
			poly_dim);

		if (poly_dim.HitNum >= 1)
		{
			if (sharedCollisionObject->GetTag() != "field")
			{
				continue;
			}

			float minSize = NULL;

			for (int i = 0; i < poly_dim.HitNum; i++)
			{
				MV1_COLL_RESULT_POLY subjectPoly = poly_dim.Dim[i];
				MV1_COLL_RESULT_POLY rayCheckWall;

				//三角形ポリゴンの法線と上方向ベクトルとの
				// なす角を求める
				float tiltAngleDegree =
					Calculation::AngleBetWeenTwoVectors(
						kLengthDirection,
						subjectPoly.Normal);

				tiltAngleDegree = abs(tiltAngleDegree);

				//平面に当たっていればtrueに
				if (tiltAngleDegree <= kAngleRange)
				{
					//三角形の一番近い辺から一番近い点を求める
					VECTOR nearestOutSide = 
						Calculation::SphereMeshOutsideTriangle(
						subjectPoly,
						position);

					//playerの座標から三角形のnearestOurSideとの間に壁があったら飛ばす
					HitCheck::RayHitJudge(sharedCollisionObject->GetModelHandle(),
						kFrameIndex,
						position,
						nearestOutSide,
						rayCheckWall);

					if (rayCheckWall.HitFlag)continue;

					//座標に対して最も近い三角形の辺を求める
					Calculation::NearestResult nearestResult = 
						Calculation::SphereMeshOutsideTriangleLine(
							subjectPoly,
							position);

					//奥行を調べるための座標
					VECTOR depthDirection = VSub(nearestOutSide, position);
					depthDirection = VNorm(depthDirection);

					//対象の三角形の表面を沿うための方向ベクトルを算出
					depthDirection = Calculation::Projection(subjectPoly.Normal, depthDirection);

					//一番近い三角形の辺の座標からキャラの肩幅くらい距離を
					// 左右に取ってその座標から下にrayを飛ばして
					// 床があるか確認する
					VECTOR rightRayPoint = VNorm(VSub(nearestResult.endLinePos, nearestOutSide));
					VECTOR leftRayPoint = VNorm(VSub(nearestResult.startLinePos, nearestOutSide));

					rightRayPoint = VScale(rightRayPoint, kCheckWidth);
					leftRayPoint = VScale(leftRayPoint, kCheckWidth);

					rightRayPoint = VAdd(nearestOutSide, rightRayPoint);
					leftRayPoint = VAdd(nearestOutSide, leftRayPoint);

					rightRayPoint = VAdd(rightRayPoint, VScale(depthDirection, 0.5f));
					leftRayPoint = VAdd(leftRayPoint, VScale(depthDirection, 0.5f));

					VECTOR endRightRayPoint = VAdd(rightRayPoint, VGet(0.0f, -1.0f, 0.0f));
					VECTOR endLeftRayPoint = VAdd(leftRayPoint, VGet(0.0f, -1.0f, 0.0f));
					rightRayPoint = VAdd(rightRayPoint, VGet(0.0f, 1.0f, 0.0f));
					leftRayPoint = VAdd(leftRayPoint, VGet(0.0f, 1.0f, 0.0f));
				
					MV1_COLL_RESULT_POLY leftRayCheck;
					MV1_COLL_RESULT_POLY rightRayCheck;

					//デバック
					DebugDrawer::GetInstance().InformationInput_line(leftRayPoint, endLeftRayPoint, GetColor(255, 255, 255));
					DebugDrawer::GetInstance().InformationInput_line(rightRayPoint, endRightRayPoint, GetColor(255, 255, 255));
					DebugDrawer::GetInstance().InformationInput_sphere(nearestOutSide, 2.5f, GetColor(255, 0, 255));
				
					//rayを飛ばして確認
					for (const auto& wpCollisionObject : collisionObjects)
					{
						//掴む所の幅を確認して、一定の幅がないと掴めないようにする
						auto spCollisionObject = wpCollisionObject.lock();
						HitCheck::RayHitJudge(
							spCollisionObject->GetModelHandle(),
							kFrameIndex,
							leftRayPoint,
							endLeftRayPoint,
							leftRayCheck);

						HitCheck::RayHitJudge(
							spCollisionObject->GetModelHandle(),
							kFrameIndex,
							rightRayPoint,
							endRightRayPoint,
							rightRayCheck);

						//両方当たっていれば次の確認へ
						if (leftRayCheck.HitFlag && 
							rightRayCheck.HitFlag)break;
					}

					//いずれかが当たっていなければ飛ばす
					if (!leftRayCheck.HitFlag ||
						!rightRayCheck.HitFlag)
					{
						continue;
					}

					//奥行確認のためのrayの長さ
					VECTOR depthDistance = VScale(depthDirection, kMaxVelocity);
					depthDistance = VAdd(nearestOutSide, depthDistance);

					//少し浮かせる
					VECTOR startWallCheckLine = VAdd(nearestOutSide, VScale(subjectPoly.Normal, 0.2f));
					VECTOR endWallCheckLine = VAdd(depthDistance, VScale(subjectPoly.Normal, 0.2f));

					DebugDrawer::GetInstance().InformationInput_capsule(startWallCheckLine, endWallCheckLine,1.0f, GetColor(255, 0, 255));

					MV1_COLL_RESULT_POLY wallCheck = {};

					//壁に当たっていたら崖掴みができない
					HitCheck::RayHitJudge(
						sharedCollisionObject->GetModelHandle(),
						kFrameIndex,
						startWallCheckLine,
						endWallCheckLine,
						wallCheck);

					//奥行がない場合掴めない
					if (wallCheck.HitFlag) continue;

					VECTOR sub = VSub(nearestOutSide, topPosition);
					float sub_size = VSize(sub);

					//一番差が小さい情報を取得
					if (minSize == NULL || minSize >= sub_size)
					{
						minSize = sub_size;
						hangingData.hangingPoly = subjectPoly;

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
	
	DebugDrawer::GetInstance().InformationInput_sphere(spherePos, radius, GetColor(255, 255, 255));
	DebugDrawer::GetInstance().InformationInput_line(startUpperCheckPos, endUpperCheckPos, GetColor(255, 0, 255));

	return hangingData;

	//trueの場合に崖をつかむようにする
}