#pragma once

class Calculation
{
private:

	Calculation(){}
	~Calculation(){}


public:
	struct NearestResult
	{
		VECTOR startLinePos;
		VECTOR endLinePos;
		VECTOR nearestPoint;
	};

	static void Initialize();

	static void UnInitialize();
	static float RadToDeg(float radian);
	static float DegToRad(float degree);

	/// <summary>
	/// 面積計算
	/// </summary>
	/// <param name="vertex1"></param>
	/// <param name="vertex2"></param>
	/// <param name="vertex3"></param>
	/// <returns></returns>
	static float area(const VECTOR& vertex1, const VECTOR& vertex2, const VECTOR& vertex3);
	
	/// <summary>
	/// 三角形のY軸の長さを求める
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <param name="c"></param>
	/// <returns></returns>
	static float TriangleByVerticalLength(const VECTOR& a, const VECTOR& b, const VECTOR& c);
	static float GetVerticalLength(const VECTOR& a, const VECTOR& b);
	
	/// <summary>
	/// ラープ
	/// </summary>
	/// <typeparam name="Type"></typeparam>
	/// <param name="change"></param>
	/// <param name="latest"></param>
	/// <param name="speed"></param>
	/// <returns></returns>
	template <class Type>
	static Type Leap(const Type& change, const Type& latest, const Type& speed)
	{
		Type sub = latest - change;
		Type scale = 0;
		scale = sub * speed;

		return change + scale;
	}
	static float Check_depth_Triangle(const VECTOR& dir, const VECTOR& a, const VECTOR& b, const VECTOR& c);
	static float CalculateBackProgress(float maxValue, float minValue, float nowValue);
	static float InterpolationCalc(const float progressValue,
		const float maxValue,
		const float minValue);
	static float EaseOutQuad(float normalDistanceProgress);
	static float CalculateBackEaseOutValue(float nowValue);
	static VECTOR NearestPoint(const VECTOR& position_1, const VECTOR& position_2,
		 const VECTOR& point);
	static VECTOR Projection(const VECTOR& plane_normal, const VECTOR& velocity);
	static MATRIX Rotate(const VECTOR& wall_normal);
	static VECTOR ProjectionDirection(const VECTOR& point, const VECTOR& a, const VECTOR& b);
	static VECTOR Leap(const VECTOR& changePosition, const VECTOR& latestPosition, const float& speed);
	static VECTOR GetDirectionFromDegree(float degree);
	static VECTOR SphereMeshOutsideTriangle(const MV1_COLL_RESULT_POLY& subjectPoly, const VECTOR& pos);
	static VECTOR RotateLineSegment(
		const VECTOR& center,
		const VECTOR& rotatePos,
		const VECTOR& dir,
		const float& degree);
	static float AngleBetWeenTwoVectors(const VECTOR& direction1, const VECTOR& direction2);

	static float RotationAngleDegree(
		const float targetDegree,
		const float nowDegree,
		const float rotationSpeed);
	/// <summary>
	/// 点に対して最も近い三角形の辺
	/// </summary>
	/// <param name="subjectPoly"></param>
	/// <param name="HitPos_ground"></param>
	/// <returns></returns>
	static NearestResult SphereMeshOutsideTriangleLine(const MV1_COLL_RESULT_POLY& subjectPoly, const VECTOR& pos);
};


