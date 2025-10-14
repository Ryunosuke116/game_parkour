#pragma once
#include "Common.h"
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

	/// <summary>
	/// rayの当たり判定
	/// </summary>
	/// <param name="modelHandle"></param>
	/// <param name="frameIndex"></param>
	/// <param name="startLinePos"></param>
	/// <param name="endLinePos"></param>
	/// <param name="hitPoly"></param>
	/// <returns></returns>
	static bool RayHitJudge(const int modelHandle,
		const int frameIndex,
		const VECTOR& startLinePos,
		const VECTOR& endLinePos,
		MV1_COLL_RESULT_POLY& hitPoly);

	/// <summary>
	/// 球の当たり判定
	/// </summary>
	/// <param name="modelHandle"></param>
	/// <param name="frameIndex"></param>
	/// <param name="radius"></param>
	/// <param name="endLinePos"></param>
	/// <param name="hitPoly"></param>
	/// <returns></returns>
	static bool SphereHitJudge(const int modelHandle,
		const int frameIndex,
		const float radius,
		const VECTOR& endLinePos,
		MV1_COLL_RESULT_POLY_DIM& hitPoly);

	/// <summary>
	/// カプセルの当たり判定
	/// </summary>
	/// <param name="modelHandle"></param>
	/// <param name="frameIndex"></param>
	/// <param name="radius"></param>
	/// <param name="startLinePos"></param>
	/// <param name="endLinePos"></param>
	/// <param name="hitPoly"></param>
	/// <returns></returns>
	static bool CapsuleHitJudge(const int modelHandle,
		const int frameIndex,
		const float radius,
		const VECTOR& startLinePos,
		const VECTOR& endLinePos,
		MV1_COLL_RESULT_POLY_DIM& hitPoly);

	/// <summary>
	/// 面との接触座標の計算
	/// </summary>
	/// <param name="point"></param>
	/// <param name="vertex1"></param>
	/// <param name="vertex2"></param>
	/// <param name="vertex3"></param>
	/// <returns></returns>
	static VECTOR ClosestPtToPointTriangle(const VECTOR& point,
		const VECTOR& vertex1,
		const VECTOR& vertex2,
		const VECTOR& vertex3);

	/// <summary>
	/// AABB同士が接触しているか
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <returns></returns>
	static bool AABBHitJudge(const AABB& AABB1, const AABB& AABB2);
	
	/// <summary>
	/// 球と球の当たり判定
	/// </summary>
	/// <param name="spherePos1"></param>
	/// <param name="spherePos2"></param>
	/// <param name="radius1"></param>
	/// <param name="radius2"></param>
	/// <returns></returns>
	static bool HitConfirmation(const VECTOR& spherePos1,
		const VECTOR& spherePos2,
		const float radius1,
		const float radius2);

	/// <summary>
	/// 対象の座標から最も近いカプセルの軸座標を算出
	/// </summary>
	/// <param name="capsulePos1"></param>
	/// <param name="capsulePos2"></param>
	/// <param name="point"></param>
	/// <returns></returns>
	static VECTOR CapsuleHitConfirmation(const VECTOR& capsulePos1,
		const VECTOR& capsulePos2,
		const VECTOR& point);
	static bool TriangleAreaCheck(const VECTOR& point, const VECTOR& a, const VECTOR& b, const VECTOR& c);
	static bool TriangleAreaCheck_ground(const VECTOR& point, const VECTOR& a, const VECTOR& b, const VECTOR& c);


	
	static std::pair<VECTOR,VECTOR> SegmentTriangleDistance(const VECTOR& p, const VECTOR& q, const VECTOR& a, const VECTOR& b, const VECTOR& c, const VECTOR& normal);
	static HangingData CliffGrabbing(
		const std::vector<std::weak_ptr<BaseObject>>& collisionObjects,
		const VECTOR& position,
		const VECTOR& topPosition,
		const VECTOR& moveDirection,
		const float& radius);

private:

};

