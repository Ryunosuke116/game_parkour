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
#include "Calclation.h"


/// <summary>
/// 面積計算
/// </summary>
/// <param name="a"></param>
/// <param name="b"></param>
/// <param name="c"></param>
/// <returns></returns>
float Calclation::area(const VECTOR& a, const VECTOR& b, const VECTOR& c)
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
VECTOR Calclation::NearestPoint(const VECTOR& position_1, const VECTOR& position_2,
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

VECTOR Calclation::SphereMeshOutsideTriangle(const MV1_COLL_RESULT_POLY& poly, const VECTOR& HitPos_ground)
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
/// ラープ
/// </summary>
/// <param name="AimPosition"></param>
VECTOR Calclation::Leap(const VECTOR& changePosition, const VECTOR& playerPosition, const float& speed)
{
	VECTOR SubPosition = VSub(playerPosition, changePosition);
	VECTOR scalePosition = VGet(0, 0, 0);

	scalePosition = VScale(SubPosition, speed);

	return VAdd(changePosition, scalePosition);
}

void Calclation::Initialize()
{
	if (instance != nullptr)
	{
		return;
	}

	instance = new Calclation;
}

void Calclation::UnInitialize()
{
	if (instance)
	{
		delete instance;

		instance = nullptr;
	}
}

Calclation* Calclation::instance = nullptr;