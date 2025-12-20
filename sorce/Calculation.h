#pragma once

struct NearestResult;

class Calculation
{
public:
	/// <summary>
	/// 面積計算
	/// </summary>
	/// <param name="vertexA"></param>
	/// <param name="vertexB"></param>
	/// <param name="vertexC"></param>
	/// <returns></returns>
	static float area(const VECTOR& vertexA, const VECTOR& vertexB, const VECTOR& vertexC);

	/// <summary>
	///  線分と点の最接近点
	/// </summary>
	/// <param name="startLinePos"></param>
	/// <param name="endLinePos"></param>
	/// <param name="point"></param>
	/// <returns></returns>
	static VECTOR NearestPoint(const VECTOR& startLinePos, 
		const VECTOR& endLinePos,
		 const VECTOR& point);

	/// <summary>
	/// 点に対して最も近い三角形の辺の座標を返す
	/// </summary>
	/// <param name="subjectPoly"></param>
	/// <param name="point"></param>
	/// <returns></returns>
	static VECTOR NearestPointOnTriangleEdge(const MV1_COLL_RESULT_POLY& subjectPoly,
		const VECTOR& point);

	/// <summary>
	/// 点に対して最も近い三角形の辺の情報を返す
	/// </summary>
	/// <param name="subjectPoly"></param>
	/// <param name="point"></param>
	/// <returns></returns>
	static NearestResult SphereMeshOutsideTriangleLine(const MV1_COLL_RESULT_POLY& subjectPoly,
		const VECTOR& point);
	
	/// <summary>
	/// 投影の正規化ベクトルを返す
	/// </summary>
	/// <param name="normalPlane"></param>
	/// <param name="velocity"></param>
	/// <returns></returns>
	static VECTOR Projection(const VECTOR& normalPlane,
		const VECTOR& velocity);

	/// <summary>
	///  射影方向
	/// </summary>
	/// <param name="point"></param>
	/// <param name="capsulePos1"></param>
	/// <param name="capsulePos2"></param>
	/// <returns></returns>
	static VECTOR ProjectionDirection(const VECTOR& point,
		const VECTOR& capsulePos1,
		const VECTOR& capsulePos2);

	/// <summary>
	/// ラープ
	/// </summary>
	/// <param name="changePosition"></param>
	/// <param name="latestPosition"></param>
	/// <param name="speed"></param>
	/// <returns></returns>
	static VECTOR LeapVector(const VECTOR& changePosition,
		const VECTOR& latestPosition,
		const float speed);

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

	/// <summary>
	/// 回転値を度数で返す
	/// </summary>
	/// <param name="targetDegree"></param>
	/// <param name="nowDegree"></param>
	/// <param name="rotationSpeed"></param>
	/// <returns></returns>
	static float RotationAngleDegree(
		const float targetDegree,
		const float nowDegree,
		const float rotationSpeed);

	/// <summary>
	/// 進行度に基づいて結果を計算
	/// </summary>
	/// <param name="easedValue"></param>
	/// <param name="maxValue"></param>
	/// <param name="minValue"></param>
	/// <returns></returns>
	static float InterpolationCalc(const float progressValue,
		const float maxValue,
		const float minValue);

	/// <summary>
	/// 進行値(0～1)を現在の値から求める
	/// </summary>
	/// <param name="maxValue"></param>
	/// <param name="minValue"></param>
	/// <param name="nowValue"></param>
	/// <returns></returns>
	static float CalculateBackProgress(const float maxValue,
		const float minValue,
		const float nowValue);

	/// <summary>
	/// イースアウト
	/// </summary>
	/// <param name="normalDistanceProgress"></param>
	/// <returns></returns>
	static float EaseOutQuad(const float normalDistanceProgress);

	/// <summary>
	/// 進行値に基づいて値を求める
	/// </summary>
	/// <param name="nowValue"></param>
	/// <returns></returns>
	static float CalculateBackEaseOutValue(const float nowValue);;

	/// <summary>
	/// 二つのべクトルのなす角
	/// </summary>
	/// <param name="direction1"></param>
	/// <param name="direction2"></param>
	/// <returns></returns>
	static float AngleBetWeenTwoVectors(const VECTOR& direction1,
		const VECTOR& direction2);

	/// <summary>
	/// ラジアン値から度数に変換
	/// </summary>
	/// <param name="radian"></param>
	/// <returns></returns>
	static float RadToDeg(const float radian);

	/// <summary>
	/// 度数からラジアン値に変換
	/// </summary>
	/// <param name="degree"></param>
	/// <returns></returns>
	static float DegToRad(const float degree);

	/// <summary>
	/// 角度値から方向ベクトルを求める
	/// (0,0,1) = 0°とする
	/// </summary>
	/// <param name="degree"></param>
	/// <returns></returns>
	static VECTOR DirectionFromDegree(const float degree);

	/// <summary>
	/// ロドリゲスの回転
	/// </summary>
	/// <param name="center"></param>
	/// <param name="rotatePos"></param>
	/// <param name="dir"></param>
	/// <param name="degree"></param>
	/// <returns></returns>
	static VECTOR RotateLineSegment(const VECTOR& center,
		const VECTOR& rotatePos,
		const VECTOR& dir,
		const float& degree);

private:
	Calculation() = default;
	~Calculation() = default;
};


