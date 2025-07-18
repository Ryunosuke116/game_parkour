#include <iostream>
#include <cassert>
#include <unordered_map>
#include <string_view>
#include <array>
#include <vector>
#include <math.h>    
#include <cmath>
#include <memory>
#include "DxLib.h"
#include "Calculation.h"


/// <summary>
/// 面積計算
/// </summary>
/// <param name="a"></param>
/// <param name="b"></param>
/// <param name="c"></param>
/// <returns></returns>
float Calculation::area(const VECTOR& a, const VECTOR& b, const VECTOR& c)
{
	VECTOR AB = VSub(b, a);
	VECTOR AC = VSub(c, a);
	VECTOR cross = VCross(AB, AC);

	float area = 0.5f * VSize(cross);
	return 0.5f * VSize(cross);
}

/// <summary>
/// 線分と点の最接近点
/// </summary>
/// <param name="position_1"></param>
/// <param name="position_2"></param>
/// <param name="point"></param>
/// <returns></returns>
VECTOR Calculation::NearestPoint(const VECTOR& position_1, const VECTOR& position_2,
	const VECTOR& point)
{
	//線分と点の最近点
	VECTOR AB = VSub(position_1, position_2);
	VECTOR AP = VSub(point, position_1);

	//ベクトルの長さ
	float vectorLength = pow(VSquareSize(AB), 0.5f);

	//単位ベクトル
	VECTOR unitVector = VGet(0, 0, 0);
	unitVector.x = AB.x / vectorLength;
	unitVector.y = AB.y / vectorLength;
	unitVector.z = AB.z / vectorLength;
	//内積
	float productionVector = VDot(unitVector, AP);

	VECTOR AX;
	AX = VAdd(position_1, VScale(unitVector, productionVector));

	if (AX.y <= position_2.y)
	{
		AX.y = position_2.y;
	}
	if (AX.y >= position_1.y)
	{
		AX.y = position_1.y;
	}

	return AX;
}

/// <summary>
/// 点に対して最も近い三角形の辺
/// </summary>
/// <param name="poly"></param>
/// <param name="HitPos_ground"></param>
/// <returns></returns>
VECTOR Calculation::SphereMeshOutsideTriangle(const MV1_COLL_RESULT_POLY& poly, const VECTOR& HitPos_ground)
{
	VECTOR nearestPoint;

	//線分上の点との最近点
	VECTOR nearPoint_1 = NearestPoint(poly.Position[0], poly.Position[1], HitPos_ground);
	VECTOR nearPoint_2 = NearestPoint(poly.Position[0], poly.Position[2], HitPos_ground);
	VECTOR nearPoint_3 = NearestPoint(poly.Position[1], poly.Position[2], HitPos_ground);

	//各距離を求める
	float d1 = VSize(VSub(nearPoint_1, HitPos_ground));
	float d2 = VSize(VSub(nearPoint_2, HitPos_ground));
	float d3 = VSize(VSub(nearPoint_3, HitPos_ground));


	//一番近い座標を選択する
	if (d1 <= d2 && d1 <= d3)
	{
		nearestPoint = nearPoint_1;
	}
	else if (d2 <= d3)
	{
		nearestPoint = nearPoint_2;
	}
	else
	{
		nearestPoint = nearPoint_3;
	}

	return nearestPoint;
}

/// <summary>
/// 点に対して最も近い三角形の辺
/// </summary>
/// <param name="poly"></param>
/// <param name="HitPos_ground"></param>
/// <returns></returns>
Calculation::NearestResult Calculation::SphereMeshOutsideTriangle_line(const MV1_COLL_RESULT_POLY& poly, const VECTOR& HitPos_ground)
{
	VECTOR nearestPoint;

	NearestResult result;

	//線分上の点との最近点
	VECTOR nearPoint_1 = NearestPoint(poly.Position[0], poly.Position[1], HitPos_ground);
	VECTOR nearPoint_2 = NearestPoint(poly.Position[0], poly.Position[2], HitPos_ground);
	VECTOR nearPoint_3 = NearestPoint(poly.Position[1], poly.Position[2], HitPos_ground);

	//各距離を求める
	float d1 = VSize(VSub(nearPoint_1, HitPos_ground));
	float d2 = VSize(VSub(nearPoint_2, HitPos_ground));
	float d3 = VSize(VSub(nearPoint_3, HitPos_ground));


	//一番近い座標を選択する
	if (d1 <= d2 && d1 <= d3)
	{
		result.nearestPoint = nearPoint_1;
		result.linePos_start = poly.Position[0];
		result.linePos_end = poly.Position[1];
	}
	else if (d2 <= d3)
	{
		result.nearestPoint = nearPoint_2;
		result.linePos_start = poly.Position[0];
		result.linePos_end = poly.Position[2];
	}
	else
	{
		result.nearestPoint = nearPoint_3;
		result.linePos_start = poly.Position[1];
		result.linePos_end = poly.Position[2];
	}

	return result;
}

/// @brief 
/// @param point 
/// @param a 
/// @param b 
/// @return 
VECTOR Calculation::ProjectionDirection(const VECTOR& point, const VECTOR& a, const VECTOR& b)
{
	VECTOR AB = VSub(b, a);
	VECTOR AP = VSub(point, a);

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
	AX.x = a.x + (unitVector.x * productionVector);
	AX.y = a.y + (unitVector.y * productionVector);
	AX.z = a.z + (unitVector.z * productionVector);

	return AX;
}

/// <summary>
/// ラープ
/// </summary>
/// <param name="AimPosition"></param>
VECTOR Calculation::Leap(const VECTOR& changePosition, const VECTOR& latestPosition, const float& speed)
{
	VECTOR SubPosition = VSub(latestPosition, changePosition);
	VECTOR scalePosition = VGet(0, 0, 0);

	scalePosition = VScale(SubPosition, speed);

	return VAdd(changePosition, scalePosition);
}

/// <summary>
/// ラジアン計算
/// </summary>
/// <param name="line_1"></param>
/// <param name="line_2"></param>
/// <returns></returns>
float Calculation::Radian(const VECTOR& line_1,const VECTOR& line_2)
{
	float dot = VDot(line_1, line_2);
	float length_1 = VSize(line_1);
	float length_2 = VSize(line_2);

	//Θ除算対策
	if (length_1 == 0 || length_2 == 0)
	{
		return 0.0f;
	}

	return dot / (length_1 * length_2);
}

/// <summary>
/// ラジアンから度に変換
/// </summary>
/// <param name="radian"></param>
/// <returns></returns>
float Calculation::radToDeg(float radian)
{
	return radian * 180.0f / DX_PI_F;
}

void Calculation::Initialize()
{
	/*if (instance != nullptr)
	{
		return;
	}

	instance = new Calculation;*/
}

void Calculation::UnInitialize()
{
	//if (instance)
	//{
	//	delete instance;

	//	instance = nullptr;
	//}
}
