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
#include "DebugDrawer.h"


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
	unitVector = VNorm(AB);
	/*unitVector.x = AB.x / vectorLength;
	unitVector.y = AB.y / vectorLength;
	unitVector.z = AB.z / vectorLength;*/
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
/// 三角形のdir方向の奥行を調べる
/// </summary>
/// <param name="dir"></param>
/// <param name="a"></param>
/// <param name="b"></param>
/// <param name="c"></param>
/// <returns></returns>
float Calculation::Check_depth_Triangle(const VECTOR& dir, const VECTOR& a, const VECTOR& b, const VECTOR& c)
{
	float dot_1 = VDot(a, dir);
	float dot_2 = VDot(b, dir);
	float dot_3 = VDot(c, dir);

	float max_projection = max(dot_1, dot_2);
	max_projection = max(max_projection, dot_3);

	float min_projection = min(dot_1, dot_2);
	min_projection = min(min_projection, dot_3);

	return max_projection - min_projection;
}

/// <summary>
/// 点に対して最も近い三角形の辺
/// </summary>
/// <param name="poly"></param>
/// <param name="pos"></param>
/// <returns></returns>
VECTOR Calculation::SphereMeshOutsideTriangle(const MV1_COLL_RESULT_POLY& poly, const VECTOR& pos)
{
	VECTOR nearestPoint;

	//線分上の点との最近点
	VECTOR nearPoint_1 = NearestPoint(poly.Position[0], poly.Position[1], pos);
	VECTOR nearPoint_2 = NearestPoint(poly.Position[0], poly.Position[2], pos);
	VECTOR nearPoint_3 = NearestPoint(poly.Position[1], poly.Position[2], pos);

	//各距離を求める
	float d1 = VSize(VSub(nearPoint_1, pos));
	float d2 = VSize(VSub(nearPoint_2, pos));
	float d3 = VSize(VSub(nearPoint_3, pos));

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
/// 最も近い三角形の辺
/// </summary>
/// <param name="poly"></param>
/// <param name="HitPos_ground"></param>
/// <returns></returns>
Calculation::NearestResult Calculation::SphereMeshOutsideTriangle_line(const MV1_COLL_RESULT_POLY& poly, const VECTOR& pos)
{
	VECTOR nearestPoint;

	NearestResult result;

	//線分上の点との最近点
	VECTOR nearPoint_1 = NearestPoint(poly.Position[0], poly.Position[1], pos);
	VECTOR nearPoint_2 = NearestPoint(poly.Position[0], poly.Position[2], pos);
	VECTOR nearPoint_3 = NearestPoint(poly.Position[1], poly.Position[2], pos);

	//各距離を求める
	float d1 = VSize(VSub(nearPoint_1, pos));
	float d2 = VSize(VSub(nearPoint_2, pos));
	float d3 = VSize(VSub(nearPoint_3, pos));

	//DebugDrawer::Instance().InformationInput_sphere(nearPoint_1, 2.0f, GetColor(255, 0, 0));
	//DebugDrawer::Instance().InformationInput_sphere(nearPoint_2, 2.0f, GetColor(0, 255, 0));
	//DebugDrawer::Instance().InformationInput_sphere(nearPoint_3, 2.0f, GetColor(0, 0, 255));
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

/// <summary>
/// 投影の正規化ベクトルを返す
/// </summary>
/// <param name="plane_normal"></param>
/// <param name="moveVec"></param>
/// <returns></returns>
VECTOR Calculation::Projection(const VECTOR& plane_normal,const VECTOR& moveVec)
{
	float dot = VDot(plane_normal, moveVec);	//内積
	
	VECTOR projection = VSub(moveVec, VScale(plane_normal, dot));
	projection = VNorm(projection);

	return projection;
}


/// <summary>
/// 射影方向
/// </summary>
/// <param name="point"></param>
/// <param name="a"></param>
/// <param name="b"></param>
/// <returns></returns>
VECTOR Calculation::ProjectionDirection(const VECTOR& point, const VECTOR& a, const VECTOR& b)
{
	VECTOR AB = VSub(b, a);
	VECTOR AP = VSub(point, a);

	//ベクトルの長さ
	float vectorLength = pow((AB.x * AB.x) + (AB.y * AB.y) + (AB.z * AB.z), 0.5f);

	//単位ベクトル(正規化)
	VECTOR unitVector = VGet(0, 0, 0);
	unitVector = VNorm(AB);
	/*unitVector.x = AB.x / vectorLength;
	unitVector.y = AB.y / vectorLength;
	unitVector.z = AB.z / vectorLength;*/

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
/// 縦の長さを求める
/// </summary>
/// <param name="a"></param>
/// <param name="b"></param>
/// <returns></returns>
float Calculation::GetVerticalLength(const VECTOR& a, const VECTOR& b)
{
	VECTOR sub = VSub(a, b);
	sub = VGet(0.0f, sub.y, 0.0f);

	float vertical_length = VSize(sub);

	return vertical_length;
}

/// <summary>
/// 三角形のY軸の長さを求める
/// </summary>
/// <param name="a"></param>
/// <param name="b"></param>
/// <param name="c"></param>
/// <returns></returns>
float Calculation::Triangle_by_verticalLength(const VECTOR& a, const VECTOR& b, const VECTOR& c)
{
	VECTOR highestPosition = a;

	if (highestPosition.y < b.y)
	{
		highestPosition = b;
	}

	if (highestPosition.y < c.y)
	{
		highestPosition = c;
	}

	VECTOR lowestPosition = a;

	if (lowestPosition.y > b.y)
	{
		lowestPosition = b;
	}

	if (lowestPosition.y > c.y)
	{
		lowestPosition = c;
	}

	return GetVerticalLength(highestPosition,lowestPosition);
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
/// ラープ
/// float
/// </summary>
/// <param name="change"></param>
/// <param name="latest"></param>
/// <param name="speed"></param>
/// <returns></returns>
float Calculation::Leap_float(const float& change, const float& latest, const float& speed)
{
	float sub = latest - change;
	float scale = 0;
	scale = sub * speed;

	return change + scale;
}

MATRIX Calculation::Rotate(const VECTOR& wall_normal)
{
	VECTOR up = wall_normal;
	VECTOR forward = VGet(0.0f, 1.0f, 0.0f);
	VECTOR right = VGet(1.0f, 0.0f, 0.0f);
	forward = VNorm(VCross(up, right));

	MATRIX rotate;
	rotate.m[0][0] = right.x;	rotate.m[0][1] = right.y;   rotate.m[0][2] = right.z;   rotate.m[0][3] = 0;
	rotate.m[1][0] = up.x;      rotate.m[1][1] = up.y;      rotate.m[1][2] = up.z;      rotate.m[1][3] = 0;
	rotate.m[2][0] = forward.x; rotate.m[2][1] = forward.y; rotate.m[2][2] = forward.z; rotate.m[2][3] = 0;
	rotate.m[3][0] = 0;         rotate.m[3][1] = 0;         rotate.m[3][2] = 0;         rotate.m[3][3] = 1;

	return rotate;
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

float Calculation::DegToRad(float degree)
{
	return degree * DX_PI_F / 180.0f;
}

/// <summary>
/// イースアウト
/// </summary>
/// <param name="t"></param>
/// <returns></returns>
float Calculation::EaseOutQuad(float t)
{
	return 1 - (1 - t) * (1 - t);
}

/// <summary>
/// 角度値から方向ベクトルを求める
/// (0,0,1) = 0°
/// </summary>
/// <param name="degree"></param>
/// <returns></returns>
VECTOR Calculation::GetDirectionFromDegree(float degree)
{
	VECTOR dir = VGet(0.0f, 0.0f, 1.0f);

	//0°の場合はそのまま返す
	if (degree == 0.0f)
	{
		return dir;
	}

	//ラジアン値に変換
	float radian = degree * DX_PI_F / 180.0f;

	return VGet(sinf(radian), 0.0f, cosf(radian));

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
