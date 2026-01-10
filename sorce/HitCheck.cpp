#include "Common.h"
#include "HitCheck.h"
#include "Calculation.h"
#include "DebugDrawer.h"

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
	const VECTOR& vertexA,
	const VECTOR& vertexB,
	const VECTOR& vertexC)
{
	//PがAの外側の頂点座標の中にあるかどうかチェック
	VECTOR edgeAB	= VSub(vertexB, vertexA);
	VECTOR edgeAC	= VSub(vertexC, vertexA);
	VECTOR vecAP	= VSub(point, vertexA);

	float dotAB_AP	= VDot(edgeAB, vecAP);
	float dotAC_AP	= VDot(edgeAC, vecAP);

	//PがBの外側の頂点領域の中にあるかどうかチェック
	VECTOR vecBP	= VSub(point, vertexB);
	float dotAB_BP	= VDot(edgeAB, vecBP);
	float dotAC_BP	= VDot(edgeAC, vecBP);

	//PがABの辺領域の中にあるかどうかチェックし、あればPのAB上に対する射影を返す
	float areaAB	= dotAB_AP * dotAC_BP - dotAB_BP * dotAC_AP;

	//PがCの外側の頂点領域の中にあるかどうかチェック
	VECTOR vecCP	= VSub(point, vertexC);
	float dotAB_CP	= VDot(edgeAB, vecCP);
	float dotAC_CP	= VDot(edgeAC, vecCP);

	//PがACの辺領域の中にあるかどうかチェックし、あればPのAC上に対する射影を返す
	float areaAC	= dotAB_CP * dotAC_AP - dotAB_AP * dotAC_CP;

	//PがBCの辺領域の中にあるかどうかチェックし、あればPのBC上に対する射影を返す
	float areaBC	= dotAB_BP * dotAC_CP - dotAB_CP * dotAC_BP;

	//Pは面領域の中にある。Qをその重心座標(u,v,w)を用いて計算
	float baryDenom = 1.0f / (areaBC + areaAC + areaAB);
	float baryV		= areaAC * baryDenom;
	float baryW		= areaAB * baryDenom;

	return VAdd(vertexA, VAdd(VScale(edgeAB, baryV), VScale(edgeAC, baryW)));

}

bool HitCheck::AABBHitJudge(const AABB& AABB1, 
	const AABB& AABB2)
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

std::pair<VECTOR, VECTOR> HitCheck::SegmentTriangleDistance(
	const VECTOR& startLinePos,
	const VECTOR& endLinePos,
	const VECTOR& vertexA,
	const VECTOR& vertexB,
	const VECTOR& vertexC,
	const VECTOR& normal)
{
	//線分の方向ベクトル
	VECTOR lineDirection = VSub(endLinePos, startLinePos);

	//線分を100分割して一つずつ調べる
	const int num = 100;
	float minSize = 1000;
	bool isReturnMinSizePoint = false;
	VECTOR lineSegmentPointClosestSurface = VGet(0.0f, 0.0f, 0.0f);			//面と一番近い線分点
	VECTOR hittingPointSurface = VGet(0.0f, 0.0f, 0.0f);					//面との接触点

	VECTOR startPoint = ClosestPtToPointTriangle(startLinePos, vertexA, vertexB, vertexC);
	VECTOR endPoint = ClosestPtToPointTriangle(endLinePos, vertexA, vertexB, vertexC);

	//線分の両端が面の内側に存在するか
	//存在するなら距離が短い方を返す
 	if (TriangleAreaCheck(startPoint, vertexA, vertexB, vertexC) &&
		TriangleAreaCheck(endPoint, vertexA, vertexB, vertexC))
	{
		//小さい方を返す
		if (VSize(startPoint) < VSize(endPoint))
		{
			return std::make_pair(startLinePos, startPoint);
		}
		else
		{
			return std::make_pair(endLinePos, endPoint);
		}
	}
	
	//線分を分割して各点から三角形への最近点を調べる
	for (int i = 0; i < num; i++)
	{
		float normalDistanceProgress = float(i) / num;

		//線分のどこのポイントを調べるか
		VECTOR linePoint = VAdd(startLinePos,
			VScale(lineDirection, normalDistanceProgress));

		//面のどこに接触しているか
		VECTOR contactPoint = ClosestPtToPointTriangle(linePoint, vertexA, vertexB, vertexC);

		//三角形の内側かどうか
		if (TriangleAreaCheck(contactPoint, vertexA, vertexB, vertexC))
		{
			//線分のポイントから接触面までの距離
			VECTOR distance = VSub(contactPoint, linePoint);
			float distanceSize = VSize(distance);

			//一番距離が近いポイントを代入
			if (minSize > distanceSize)
			{
				minSize							= distanceSize;
				lineSegmentPointClosestSurface	= linePoint;
				hittingPointSurface				= contactPoint;
				isReturnMinSizePoint			= true;
			}
		}
	}

	//最近点が見つからなければ終点の情報を返す
	if (!isReturnMinSizePoint)
	{
		return std::make_pair(endLinePos, endPoint);
	}

	//一番近い線分の点と面の衝突座標を返す
	return std::make_pair(lineSegmentPointClosestSurface,
		hittingPointSurface);
}

/// <summary>
/// 三角形の内側に点があるか
/// </summary>
/// <param name="point"></param>
/// <param name="vertexA"></param>
/// <param name="vertexB"></param>
/// <param name="vertexC"></param>
/// <returns></returns>
bool HitCheck::TriangleAreaCheck(const VECTOR& point, 
	const VECTOR& vertexA,
	const VECTOR& vertexB, 
	const VECTOR& vertexC)
{
	//面積を求める
	float area	= fabs(Calculation::area(vertexA, vertexB, vertexC));
	float area1 = fabs(Calculation::area(vertexA, vertexB, point));
	float area2 = fabs(Calculation::area(vertexB, vertexC, point));
	float area3 = fabs(Calculation::area(vertexC, vertexA, point));

	//総面積と点を使った面積の合計の差を絶対値にして取得
	float areaAbs = abs((area1 + area2 + area3) - area);

	//面積の差がほぼ無いか
	bool isAreaEqual = areaAbs < 1e-10f;

	//それぞれの面積が0より大きいか
	bool isInside = area1 > 0 && area2 > 0 && area3 > 0;

	return (isAreaEqual && isInside) ? true : false;
}

HangingData HitCheck::CliffGrabbing(
	const std::vector<std::weak_ptr<BaseObject>>& wpCollisionObjects,
	const VECTOR& position,
	const VECTOR& topPosition,
	const VECTOR& moveDirection,
	const float radius)
{
	const int kFrameIndex			= -1;
	const float kMaxVelocity		= 11.2f;			//最大移動値
	const float kAngleRange			= 50.0f;
	const float kScaleWallCheckLine = 0.2f;			//移動量の補間スピード
	const float kAddSpherePos		= 5.0f;
	const float kRightAngle			= 90.0f;
	const VECTOR kLengthDirection	= VGet(0.0f, 1.0f, 0.0f);
	const VECTOR kVerticalShaft		= VGet(0.0f, 1.0f, 0.0f);

	float minDistanceSize			= FLT_MAX;

	VECTOR nearestOutSide			= VGet(0.0f, 0.0f, 0.0f);
	VECTOR spherePos				= VAdd(topPosition, VScale(moveDirection, kAddSpherePos));

	HangingData hangingData			= { false, NULL };

	MV1_COLL_RESULT_POLY subjectPoly;

	DebugDrawer::GetInstance().InformationInputSphere(spherePos, radius, GetColor(255, 255, 255));

	for (const auto& wpCollisionObject : wpCollisionObjects)
	{
		auto spCollisionObject		= wpCollisionObject.lock();
		
		if (spCollisionObject->GetTag() != "field")
		{
			continue;
		}

		MV1_COLL_RESULT_POLY_DIM polyDim;

		SphereHitJudge(spCollisionObject->GetModelHandle(),
			kFrameIndex,
			radius,
			spherePos, 
			polyDim);

		if (polyDim.HitNum >= 1)
		{
			bool isCliffGrabbingCliffFace = CliffGrabbingCliffFaceCheck(
				spCollisionObject->GetModelHandle(),
				kFrameIndex,
				position,
				nearestOutSide,
				subjectPoly,
				polyDim);

			//対象になるものがなければ返す
			if (!isCliffGrabbingCliffFace)
			{
				// 検出したプレイヤーの周囲のポリゴン情報を開放する
				MV1CollResultPolyDimTerminate(polyDim);
				return hangingData;
			}

			//座標に対して最も近い三角形の辺を求める
			NearestResult nearestResult =
				Calculation::SphereMeshOutsideTriangleLine(
					subjectPoly,
					position);

			//奥行を調べるための座標
			VECTOR depthDirection	= VSub(nearestOutSide, position);
			depthDirection.y		= 0.0f;
			depthDirection			= VNorm(depthDirection);

			//対象の三角形の表面を沿うための方向ベクトルを算出
			depthDirection = Calculation::Projection(subjectPoly.Normal, depthDirection);

			bool isCliffGrabbingWidth = CliffGrabbingWidthCheck(
				spCollisionObject->GetModelHandle(),
				kFrameIndex,
				depthDirection,
				nearestOutSide,
				nearestResult);

			//幅がなければ返す
			if (!isCliffGrabbingWidth)
			{
				// 検出したプレイヤーの周囲のポリゴン情報を開放する
				MV1CollResultPolyDimTerminate(polyDim);
				return hangingData;
			}

			//奥行確認のためのrayの長さ
			VECTOR depthDistance	= VScale(depthDirection, kMaxVelocity);
			depthDistance			= VAdd(nearestOutSide, depthDistance);

			//少し浮かせる
			VECTOR startWallCheckLine	= VAdd(nearestOutSide, VScale(subjectPoly.Normal, kScaleWallCheckLine));
			VECTOR endWallCheckLine		= VAdd(depthDistance, VScale(subjectPoly.Normal, kScaleWallCheckLine));

			DebugDrawer::GetInstance().InformationInputCapsule(startWallCheckLine, endWallCheckLine, 1.0f, GetColor(255, 0, 255));

			MV1_COLL_RESULT_POLY wallCheck = {};

			RayHitJudge(spCollisionObject->GetModelHandle(),
				kFrameIndex,
				startWallCheckLine,
				endWallCheckLine,
				wallCheck);

			//壁に当たっていたら崖掴みができない
			if (wallCheck.HitFlag)
			{
				// 検出したプレイヤーの周囲のポリゴン情報を開放する
				MV1CollResultPolyDimTerminate(polyDim);
				return hangingData;
			}

			//崖掴みの情報を取得
			hangingData.hangingPoly  = subjectPoly;
			hangingData.isHitHanging = true;
		}

		// 検出したプレイヤーの周囲のポリゴン情報を開放する
		MV1CollResultPolyDimTerminate(polyDim);
	}

	return hangingData;
}

bool HitCheck::CliffGrabbingWidthCheck(const int collisionObjectModelHandle,
	const int frameIndex,
	const VECTOR& depthDirection,
	const VECTOR& nearestOutSide,
	const NearestResult& nearestResult)
{
	const float kCheckWidth = 4.0f;				//床確認の幅

	//一番近い三角形の辺の座標からキャラの肩幅くらい距離を
	// 左右に取ってその座標から下にrayを飛ばして
	// 床があるか確認する
	VECTOR rightRayPoint	= VNorm(VSub(nearestResult.endLinePos, nearestOutSide));
	VECTOR leftRayPoint		= VNorm(VSub(nearestResult.startLinePos, nearestOutSide));

	rightRayPoint	= VScale(rightRayPoint, kCheckWidth);
	leftRayPoint	= VScale(leftRayPoint, kCheckWidth);

	rightRayPoint	= VAdd(nearestOutSide, rightRayPoint);
	leftRayPoint	= VAdd(nearestOutSide, leftRayPoint);

	rightRayPoint	= VAdd(rightRayPoint, VScale(depthDirection, 0.5f));
	leftRayPoint	= VAdd(leftRayPoint, VScale(depthDirection, 0.5f));

	VECTOR endRightRayPoint = VAdd(rightRayPoint, VGet(0.0f, -1.0f, 0.0f));
	VECTOR endLeftRayPoint  = VAdd(leftRayPoint, VGet(0.0f, -1.0f, 0.0f));
	rightRayPoint			= VAdd(rightRayPoint, VGet(0.0f, 1.0f, 0.0f));
	leftRayPoint			= VAdd(leftRayPoint, VGet(0.0f, 1.0f, 0.0f));

	MV1_COLL_RESULT_POLY leftRayCheck;
	MV1_COLL_RESULT_POLY rightRayCheck;

	//デバック
	DebugDrawer::GetInstance().InformationInputLine(leftRayPoint, endLeftRayPoint, GetColor(255, 255, 255));
	DebugDrawer::GetInstance().InformationInputLine(rightRayPoint, endRightRayPoint, GetColor(255, 255, 255));
	DebugDrawer::GetInstance().InformationInputSphere(nearestOutSide, 2.5f, GetColor(255, 0, 255));

	//掴む座標の幅を確認して、一定の幅がないと掴めないようにする
	RayHitJudge(
		collisionObjectModelHandle,
		frameIndex,
		leftRayPoint,
		endLeftRayPoint,
		leftRayCheck);

	RayHitJudge(
		collisionObjectModelHandle,
		frameIndex,
		rightRayPoint,
		endRightRayPoint,
		rightRayCheck);

	if (!leftRayCheck.HitFlag ||
		!rightRayCheck.HitFlag)
	{
		return false;
	}

	return true;
}

bool HitCheck::CliffGrabbingCliffFaceCheck(const int collisionObjectModelHandle,
	const int frameIndex,
	const VECTOR& position,
	VECTOR& resultNearestOutSide,
	MV1_COLL_RESULT_POLY& subjectPoly,
	MV1_COLL_RESULT_POLY_DIM& polyDim)
{
	const float kAngleRange			= 50.0f;
	const VECTOR kLengthDirection	= VGet(0.0f, 1.0f, 0.0f);

	float minDistanceSize = FLT_MAX;

	for (int i = 0; i < polyDim.HitNum; i++)
	{
		MV1_COLL_RESULT_POLY poly = polyDim.Dim[i];
		MV1_COLL_RESULT_POLY rayCheckWall;

		//三角形ポリゴンの法線と上方向ベクトルとの
		// なす角を求める
		float tiltAngleDegree =
			Calculation::AngleBetWeenTwoVectors(
				kLengthDirection,
				poly.Normal);

		tiltAngleDegree = abs(tiltAngleDegree);

		if (tiltAngleDegree <= kAngleRange)
		{
			//三角形の一番近い辺から一番近い点を求める
			VECTOR nearestOutSide = Calculation::NearestPointOnTriangleEdge(
				poly,
				position);

			DebugDrawer::GetInstance().InformationInputLine(position, nearestOutSide, GetColor(255, 255, 255));

			//playerの座標から三角形のnearestOurSideとの間に障害物があったら飛ばす
			RayHitJudge(collisionObjectModelHandle,
				frameIndex,
				position,
				nearestOutSide,
				rayCheckWall);

			//障害物がなければ崖掴み判定用の面として扱う
			if (!rayCheckWall.HitFlag)
			{
				float distanceSize = VSize(VSub(position, nearestOutSide));

				if (minDistanceSize > distanceSize)
				{
					minDistanceSize		 = distanceSize;
					resultNearestOutSide = nearestOutSide;
					subjectPoly			 = poly;
				}
			}
		}
	}

	if (minDistanceSize == FLT_MAX)
	{
		return false;
	}

	return true;
}