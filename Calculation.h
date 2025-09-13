#pragma once

class Calculation
{
private:

	Calculation(){}
	~Calculation(){}


public:
	struct NearestResult
	{
		VECTOR linePos_start;
		VECTOR linePos_end;
		VECTOR nearestPoint;
	};

	static void Initialize();

	static void UnInitialize();
	static float radToDeg(float radian);
	static float DegToRad(float degree);

	static float area(const VECTOR& a, const VECTOR& b, const VECTOR& c);
	static float Triangle_by_verticalLength(const VECTOR& a, const VECTOR& b, const VECTOR& c);
	static float GetVerticalLength(const VECTOR& a, const VECTOR& b);
	static float LeapFloat(const float& change, const float& latest, const float& speed);
	static float Check_depth_Triangle(const VECTOR& dir, const VECTOR& a, const VECTOR& b, const VECTOR& c);
	static float CalculateBackProgress(float maxValue, float minValue, float nowValue);
	static float InterpolationCalc(float progressValue, float maxValue, float minValue);
	static float EaseOutQuad(float normalDistanceProgress);
	static float CalculateBackEaseOutValue(float nowValue);
	static VECTOR NearestPoint(const VECTOR& position_1, const VECTOR& position_2,
		 const VECTOR& point);
	static VECTOR Projection(const VECTOR& plane_normal, const VECTOR& moveVec);
	static MATRIX Rotate(const VECTOR& wall_normal);
	static VECTOR ProjectionDirection(const VECTOR& point, const VECTOR& a, const VECTOR& b);
	static VECTOR Leap(const VECTOR& changePosition, const VECTOR& latestPosition, const float& speed);
	static VECTOR GetDirectionFromDegree(float degree);
	static VECTOR SphereMeshOutsideTriangle(const MV1_COLL_RESULT_POLY& poly, const VECTOR& pos);
	static VECTOR RotateLineSegment(
		const VECTOR& center,
		const VECTOR& rotatePos,
		const VECTOR& dir,
		const float& degree);

	static float RotationAngleDegree(
		const float targetDegree,
		const float nowDegree,
		const float rotationSpeed);

	static NearestResult SphereMeshOutsideTriangle_line(const MV1_COLL_RESULT_POLY& poly, const VECTOR& pos);
	static VECTOR AxisRotationX(const VECTOR& rotation, const float angle);
};


