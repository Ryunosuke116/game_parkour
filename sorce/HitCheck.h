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
	/// <summary>
	/// ray‚Ì“–‚½‚è”»’è
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
	/// ‹…‚Ì“–‚½‚è”»’è
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
	/// ƒJƒvƒZƒ‹‚Ì“–‚½‚è”»’è
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
	/// –Ê‚Æ‚ÌÚGÀ•W‚ÌŒvZ
	/// </summary>
	/// <param name="point"></param>
	/// <param name="vertexA"></param>
	/// <param name="vertexB"></param>
	/// <param name="vertexC"></param>
	/// <returns></returns>
	static VECTOR ClosestPtToPointTriangle(const VECTOR& point,
		const VECTOR& vertexA,
		const VECTOR& vertexB,
		const VECTOR& vertexC);

	/// <summary>
	/// AABB“¯m‚ªÚG‚µ‚Ä‚¢‚é‚©
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <returns></returns>
	static bool AABBHitJudge(const AABB& AABB1, const AABB& AABB2);
	
	/// <summary>
	/// ‹…‚Æ‹…‚Ì“–‚½‚è”»’è
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
	/// ü•ª‚ÆOŠpŒ`‚ÌÅ’Z‹——£
	/// Å’Z‹——£‚Ìü•ª“_‚Æ–Ê‚ÌÚGÀ•W‚ğ•Ô‚·
	/// </summary>
	/// <param name="startLinePos"></param>
	/// <param name="endLinePos"></param>
	/// <param name="vertexA"></param>
	/// <param name="vertexB"></param>
	/// <param name="vertexC"></param>
	/// <param name="normal"></param>
	/// <returns></returns>
	static std::pair<VECTOR,VECTOR> SegmentTriangleDistance(
		const VECTOR& startLinePos,
		const VECTOR& endLinePos,
		const VECTOR& vertexA,
		const VECTOR& vertexB,
		const VECTOR& vertexC,
		const VECTOR& normal);

	/// <summary>
	/// OŠpŒ`‚Ì“à‘¤‚É“_‚ª‚ ‚é‚©
	/// </summary>
	/// <param name="point"></param>
	/// <param name="vertexA"></param>
	/// <param name="vertexB"></param>
	/// <param name="vertexC"></param>
	/// <returns></returns>
	static bool TriangleAreaCheck(const VECTOR& point,
		const VECTOR& vertexA,
		const VECTOR& vertexB,
		const VECTOR& vertexC);

	/// <summary>
	/// ŠR’Í‚İ”»’è
	/// </summary>
	/// <param name="collisionObjects"></param>
	/// <param name="position"></param>
	/// <param name="topPosition"></param>
	/// <param name="moveDirection"></param>
	/// <param name="radius"></param>
	/// <returns></returns>
	static HangingData CliffGrabbing(
		const std::vector<std::weak_ptr<BaseObject>>& collisionObjects,
		const VECTOR& position,
		const VECTOR& topPosition,
		const VECTOR& moveDirection,
		const float radius);

private:
	HitCheck() = default;
	~HitCheck() = default;

	static bool CliffGrabbingWidthCheck(const int collisionObjectModelHandle,
		const int frameIndex,
		const VECTOR& depthDirection,
		const VECTOR& nearestOutSide,
		const NearestResult & nearestResult);

	static bool CliffGrabbingCliffFaceCheck(const int collisionObjectModelHandle,
		const int frameIndex, 
		const VECTOR& topPosition,
		VECTOR& nearestOutSide,
		MV1_COLL_RESULT_POLY& subjectPoly,
		MV1_COLL_RESULT_POLY_DIM& polyDim);
};

