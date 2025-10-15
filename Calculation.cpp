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

float Calculation::area(const VECTOR& vertexA,
	const VECTOR& vertexB, 
	const VECTOR& vertexC)
{
	VECTOR edgeAB = VSub(vertexB, vertexA);
	VECTOR edgeAC = VSub(vertexC, vertexA);
	VECTOR crossAB_AC = VCross(edgeAB, edgeAC);

	//三角形の面積を返す
	return 0.5f * VSize(crossAB_AC);
}

VECTOR Calculation::NearestPoint(
	const VECTOR& startLinePos, 
	const VECTOR& endLinePos,
	const VECTOR& point)
{
	//線分と点の最近点
	VECTOR edgeAB = VSub(endLinePos, startLinePos);
	VECTOR edgeAP = VSub(point, startLinePos);

	VECTOR lineDirection = VNorm(edgeAB);
	VECTOR directionAP = VNorm(edgeAP);

	//内積が負ならstartLinePosが最接近点
	if (VDot(lineDirection, directionAP) < 0)
	{
		return startLinePos;
	}

	VECTOR directionBP = VNorm(VSub(point, endLinePos));

	//内積が正ならendLinePosが最接近点
	if (VDot(lineDirection, directionBP) > 0)
	{
		return endLinePos;
	}

	//ベクトルの長さ
	float vectorLength = pow(VSquareSize(edgeAB), 0.5f);

	//単位ベクトル
	VECTOR unitVector = VNorm(edgeAB);
	
	//内積
	float productionVector = VDot(unitVector, edgeAP);

	return VAdd(startLinePos, VScale(unitVector, productionVector));
}

VECTOR Calculation::NearestPointOnTriangleEdge(const MV1_COLL_RESULT_POLY& subjectPoly,
	const VECTOR& point)
{
	//線分上の点との最近点
	VECTOR nearPoint1 = NearestPoint(subjectPoly.Position[0], subjectPoly.Position[1], point);
	VECTOR nearPoint2 = NearestPoint(subjectPoly.Position[0], subjectPoly.Position[2], point);
	VECTOR nearPoint3 = NearestPoint(subjectPoly.Position[1], subjectPoly.Position[2], point);

	//各距離の大きさを求める
	float distanceSize1 = VSize(VSub(nearPoint1, point));
	float distanceSize2 = VSize(VSub(nearPoint2, point));
	float distanceSize3 = VSize(VSub(nearPoint3, point));

	//一番近い座標を選択する
	if (distanceSize1 <= distanceSize2 &&
		distanceSize1 <= distanceSize3)
	{
		return nearPoint1;
	}
	else if (distanceSize2 <= distanceSize3)
	{
		return nearPoint2;
	}

	return nearPoint3;
}

Calculation::NearestResult Calculation::SphereMeshOutsideTriangleLine(
	const MV1_COLL_RESULT_POLY& subjectPoly,
	const VECTOR& point)
{
	NearestResult result;

	//線分上の点との最近点
	VECTOR nearPoint1 = NearestPoint(subjectPoly.Position[0], subjectPoly.Position[1], point);
	VECTOR nearPoint2 = NearestPoint(subjectPoly.Position[0], subjectPoly.Position[2], point);
	VECTOR nearPoint3 = NearestPoint(subjectPoly.Position[1], subjectPoly.Position[2], point);

	//各距離を求める
	float distanceSize1 = VSize(VSub(nearPoint1, point));
	float distanceSize2 = VSize(VSub(nearPoint2, point));
	float distanceSize3 = VSize(VSub(nearPoint3, point));

	//一番近い座標を選択する
	if (distanceSize1 <= distanceSize2 &&
		distanceSize1 <= distanceSize3)
	{
		result.nearestPoint = nearPoint1;
		result.startLinePos = subjectPoly.Position[0];
		result.endLinePos = subjectPoly.Position[1];
	}
	else if (distanceSize2 <= distanceSize3)
	{
		result.nearestPoint = nearPoint2;
		result.startLinePos = subjectPoly.Position[0];
		result.endLinePos = subjectPoly.Position[2];
	}
	else
	{
		result.nearestPoint = nearPoint3;
		result.startLinePos = subjectPoly.Position[1];
		result.endLinePos = subjectPoly.Position[2];
	}

	return result;
}

VECTOR Calculation::Projection(const VECTOR& normalPlane,
	const VECTOR& velocity)
{
	float dot = VDot(normalPlane, velocity);	//内積
	
	VECTOR projection = VSub(velocity, VScale(normalPlane, dot));

	return  VNorm(projection);
}

VECTOR Calculation::ProjectionDirection(const VECTOR& point,
	const VECTOR& capsulePos1,
	const VECTOR& capsulePos2)
{
	VECTOR edgeAB = VSub(capsulePos2, capsulePos1);
	VECTOR edgeAP = VSub(point, capsulePos1);

	//単位ベクトル(正規化)
	VECTOR unitVector = VNorm(edgeAB);

	//点の射影位置を計算(スカラー値)
	float productionVector = VDot(edgeAP, unitVector);

	//線分上の最近点を計算
	VECTOR edgeAX = VAdd(capsulePos1, VScale(unitVector, productionVector));

	/*if (edgeAX.y <= capsulePos2.y)
	{
		edgeAX.y = capsulePos2.y;
	}
	if (edgeAX.y >= capsulePos1.y)
	{
		edgeAX.y = capsulePos1.y;
	}*/

	return edgeAX;
}

VECTOR Calculation::LeapVector(const VECTOR& changePosition,
	const VECTOR& latestPosition,
	const float speed)
{
	VECTOR SubPosition = VSub(latestPosition, changePosition);
	VECTOR scalePosition = VGet(0, 0, 0);

	scalePosition = VScale(SubPosition, speed);

	return VAdd(changePosition, scalePosition);
}

float Calculation::RotationAngleDegree(
	const float targetDegree,
	const float nowDegree,
	const float rotationSpeed)
{
	float differenceAngle = targetDegree - nowDegree;

	// ある方向からある方向の差が１８０度以上になることは無いので
	// 差の値が１８０度以上になっていたら修正する
	if (differenceAngle < -180.0f)
	{
		differenceAngle += 360.0f;
	}
	else if (differenceAngle > 180.0f)
	{
		differenceAngle -= 360.0f;
	}

	// 角度の差が０に近づける
	if (differenceAngle > 0.0f)
	{
		// 差がプラスの場合は引く
		differenceAngle -= rotationSpeed;
		if (differenceAngle < 0.0f)
		{
			differenceAngle = 0.0f;
		}
	}
	else
	{
		// 差がマイナスの場合は足す
		differenceAngle += rotationSpeed;
		if (differenceAngle > 0.0f)
		{
			differenceAngle = 0.0f;
		}
	}
	return targetDegree - differenceAngle;
}

float Calculation::InterpolationCalc(
	const float progressValue,
	const float maxValue,
	const float minValue)
{
	return minValue + (maxValue - minValue) * progressValue;
}

float Calculation::CalculateBackProgress(
	const float maxValue,
	const float minValue,
	const float nowValue)
{
	if (nowValue < minValue) return 0.0f;
	if (nowValue > maxValue) return 1.0f;

	return (nowValue - minValue) / (maxValue - minValue);
}

float Calculation::EaseOutQuad(const float normalDistanceProgress)
{
	const float kMaxSize = 1.0f;

	//2乗する
	const float subSize = pow((kMaxSize - normalDistanceProgress), 2.0f);

	return kMaxSize - subSize;
}

float Calculation::CalculateBackEaseOutValue(const float nowValue)
{
	const float maxSize = 1.0f;

	return maxSize - (maxSize - pow(maxSize, nowValue));
}

float Calculation::AngleBetWeenTwoVectors(const VECTOR& direction1,
	const VECTOR& direction2)
{
	float cosTheta = VDot(direction1, direction2) /
		((VSize(direction1) * VSize(direction2)));

	float radian = std::acos(cosTheta);
	return RadToDeg(radian);
}

float Calculation::RadToDeg(const float radian)
{
	return radian * 180.0f / DX_PI_F;
}

float Calculation::DegToRad(const float degree)
{
	return degree * DX_PI_F / 180.0f;
}

VECTOR Calculation::DirectionFromDegree(const float degree)
{
	VECTOR dir = VGet(0.0f, 0.0f, 1.0f);

	//0°の場合はそのまま返す
	if (degree == 0.0f)
	{
		return dir;
	}

	//ラジアン値に変換
	float radian = RadToDeg(degree);

	return VGet(sinf(radian), 0.0f, cosf(radian));
}

VECTOR Calculation::RotateLineSegment(
	const VECTOR& center,
	const VECTOR& rotatePos,		//回転移動させたい座標
	const VECTOR& dir,				//回転軸方向
	const float& degree)			//回転°
{
	float radian = DegToRad(degree);
	VECTOR sub = VSub(rotatePos, center);
	float cos = cosf(radian);
	float sin = sinf(radian);

	VECTOR term1 = VScale(sub, cos);
	VECTOR term2 = VScale(VCross(dir, sub), sin);
	VECTOR term3 = VScale(dir, ((1 - cos) * VDot(sub, dir)));

	VECTOR rotate = VAdd(term1, VAdd(term2, term3));

	return VAdd(center, rotate);
}