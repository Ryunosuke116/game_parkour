#include "Include.h"


/// @brief �R���X�g���N�^
/// @return 
HitCheck::HitCheck()
{

}

/// @brief �f�X�g���N�^
/// @return 
HitCheck::~HitCheck()
{

}

/// @brief ray�̓����蔻��
/// @param modelHandle 
/// @param frameIndex 
/// @param linePos_start 
/// @param linePos_end 
/// @param hitPoly 
/// @return 
bool HitCheck::HitRayJudge(const int& modelHandle, int frameIndex,
	VECTOR linePos_start, VECTOR linePos_end, MV1_COLL_RESULT_POLY& hitPoly)
{
	hitPoly = MV1CollCheck_Line(modelHandle, frameIndex, linePos_start, linePos_end);

	return hitPoly.HitFlag;
}

/// @brief ���̓����蔻��
/// @param modelHandle 
/// @param frameIndex 
/// @param linePos_end 
/// @param hitPoly 
/// @return 
bool HitCheck::SphereHitJudge(const int& modelHandle, int frameIndex, VECTOR linePos_end, MV1_COLL_RESULT_POLY_DIM& hitPoly)
{
	hitPoly = MV1CollCheck_Sphere(modelHandle, frameIndex, linePos_end, 3.5f);

	if (hitPoly.HitNum >= 1)return true;
	return false;
}

/// @brief �J�v�Z���̓����蔻��
/// @param modelHandle 
/// @param frameIndex 
/// @param linePos_start 
/// @param linePos_end 
/// @param hitPoly 
void HitCheck::CapsuleHitWallJudge(const int& modelHandle, int frameIndex,float radius,
	VECTOR linePos_start, VECTOR linePos_end, MV1_COLL_RESULT_POLY_DIM& hitPoly)
{
	hitPoly = MV1CollCheck_Capsule(modelHandle, frameIndex, linePos_start, linePos_end, radius);
}

/// <summary>
/// �����̑��̌v�Z
/// </summary>
/// <param name="point"></param>
/// <param name="P"></param>
/// <param name="Q"></param>
/// <returns></returns>
float HitCheck::projectionCalc(const VECTOR& point, const VECTOR& P, const VECTOR& Q)
{
	////�����x�N�g��
	//VECTOR PQ = VSub(Q, P);
	//VECTOR P_point = VSub(point, P);
	//
	////PQ�̓���(2��)
	//float len2 = VDot(PQ, PQ);

	//float t = VDot(PQ, P_point) / len2;

	//VECTOR foot = VAdd(P, VScale(PQ, t));
	//foot = VSub(point, foot);
	//float D = VSize(foot);

	VECTOR a, normal;
	VECTOR AP = VSub(point, a);

	float D = VDot(AP, normal) / VDot(normal, normal);

	return D;
}



void HitCheck::AABB()
{
	float r = 3.5f;
	float min_X, min_Y, min_Z;
	float max_X, max_Y, max_Z;

	min_X = r * -1.0f;
	min_X = r * -1.0f;
	min_Z = r * -1.0f;
	max_X = r * 1.0f;
	max_Y = r * 1.0f;
	max_Z = r * 1.0f;


}

/// <summary>
/// ���Ƌ��̓����蔻��
/// </summary>
/// <param name="position_1"></param>
/// <param name="position_2"></param>
/// <param name="radius_1"></param>
/// <param name="radius_2"></param>
/// <returns></returns>
bool HitCheck::HitConfirmation(VECTOR position_1, VECTOR position_2, const float radius_1, const float radius_2)
{

	VECTOR positionSub = VSub(position_1, position_2);

	//���Ƌ��̒��S�_�Ƃ̋���
	float positionRange = VSize(positionSub);

	//���a�̍��v
	float radiusAdd = radius_1 + radius_2;

	// 1��2�̔��a�������Ƌ��̒��S�_�Ƃ̋������߂���Γ������Ă���
	if (radiusAdd > positionRange)
	{
		return true;
	}
	return false;
}

/// <summary>
/// �J�v�Z������ł��߂������v�Z
/// </summary>
/// <param name="capsulePosition_1"></param>
/// <param name="capsulePosition_2"></param>
/// <param name="position"></param>
/// <param name="radius_1"></param>
/// <param name="radius_2"></param>
/// <returns></returns>
VECTOR HitCheck::CapsuleHitConfirmation(VECTOR capsulePosition_1, VECTOR capsulePosition_2, VECTOR position, const float radius_1, const float radius_2)
{
	VECTOR AB = VSub(capsulePosition_2, capsulePosition_1);
	VECTOR AP = VSub(position, capsulePosition_1);

	//�x�N�g���̒���
	float vectorLength = pow((AB.x * AB.x) + (AB.y * AB.y) + (AB.z * AB.z), 0.5f);

	//�P�ʃx�N�g��
	VECTOR unitVector = VGet(0, 0, 0);
	unitVector.x = AB.x / vectorLength;
	unitVector.y = AB.y / vectorLength;
	unitVector.z = AB.z / vectorLength;
	float productionVector = (unitVector.x * AP.x) + (unitVector.y * AP.y) + (unitVector.z * AP.z);

	//
	VECTOR AX;
	AX.x = capsulePosition_1.x + (unitVector.x * productionVector);
	AX.y = capsulePosition_1.y + (unitVector.y * productionVector);
	AX.z = capsulePosition_1.z + (unitVector.z * productionVector);

	if (AX.y <= capsulePosition_2.y)
	{
		AX.y = capsulePosition_2.y;
	}
	if (AX.y >= capsulePosition_1.y)
	{
		AX.y = capsulePosition_1.y;
	}

	return AX;

}

void HitCheck::SegmentTriangleDistance(const VECTOR& p, const VECTOR& q, const VECTOR& a, const VECTOR& b, const VECTOR& c)
{
	VECTOR PQ = VSub(q, p);

	const int num = 50;
	for (int i = 0; i < num; i++)
	{
		float t = float(i) / num;

		VECTOR PT = VAdd(p, VScale(PQ, t));
	}
	
}