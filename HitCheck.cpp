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

bool HitCheck::HitRayJudge(const int& modelHandle, int frameIndex,
	VECTOR linePos_start, VECTOR linePos_end, MV1_COLL_RESULT_POLY& hitPoly)
{
	hitPoly = MV1CollCheck_Line(modelHandle, frameIndex, linePos_start, linePos_end);

//	MV1_COLL_RESULT_POLY_DIM hitPoly = MV1CollCheck_Sphere(modelHandle, frameIndex, linePos_start, 13.0f);

	DrawSphere3D(hitPoly.HitPosition, 2.0f, 30, GetColor(0, 0, 0),
		GetColor(255, 0, 0), FALSE);

	return hitPoly.HitFlag;
}

void HitCheck::CapsuleHitWallJudge(const int& modelHandle, int frameIndex,
	VECTOR linePos_start, VECTOR linePos_end, MV1_COLL_RESULT_POLY_DIM& hitPoly)
{
	hitPoly = MV1CollCheck_Sphere(modelHandle, frameIndex, linePos_end, 3.5f);
	
	//hitPoly = MV1CollCheck_Capsule(modelHandle, frameIndex,linePos_start, linePos_end, 3.5f);

	//for (int i = 0; i < hitPoly.HitNum; i++)
	//{
	//	auto poly = hitPoly.Dim[i];
	//	
	//	
	//	//�����̃x�N�g���̓���(2��)
	//	VECTOR line_se = VSub(linePos_end, linePos_start);
	//	auto len2 = VDot(line_se,line_se);

	//	//�ˉe
	//	float t;
	//	float projection[6];

	//	//�X�J���[��(�������������ɑ傫������������)
	//	float D[3];

	//	//�O���_���̃X�J���[�ʂ����߂�
	//	for (int j = 0; j < 3; j++)
	//	{
	//		
	//		//�����̒[�_���璸�_�܂ł̋���
	//		VECTOR vertex_S[3];
	//		VECTOR vertex_E[3];

	//		vertex_S[j] = VSub(poly.Position[j], linePos_start);
	//		vertex_E[j] = VSub(poly.Position[j], linePos_end);

	//		t = VDot(vertex_S[j], line_se) / len2;

	//		//0 <= t <= 1�̂Ƃ�D=t
	//		if (0.0f <= t || t <= 1.0f)
	//		{
	//			D[j] = t;
	//		}
	//		//t < 0�̂Ƃ��͎ˉe��linePos_start��
	//		else if (t < 0)
	//		{
	//			D[j] = VSize(vertex_S[j]);
	//		}
	//		//t > 0�̂Ƃ��͎ˉe��linePos_end��
	//		else if (t > 1.0f)
	//		{
	//			D[j] = VSize(vertex_E[j]);
	//		}
	//	}

	//	for (int i = 0;  i < 3;  i++)
	//	{
	//		VECTOR A = poly.Position[0];
	//		VECTOR B = poly.Position[1];
	//		VECTOR C = poly.Position[2];

	//		projection[i] = VDot(VSub(linePos_start, poly.Position[i]), VSub(poly.Position[i]))
	//	}

	//}


}

float HitCheck::projectionCalc(const VECTOR& point, const VECTOR& P, const VECTOR& Q)
{
	//�����x�N�g��
	VECTOR PQ = VSub(Q, P);
	VECTOR P_point = VSub(point, P);
	
	//PQ�̓���(2��)
	float len2 = VDot(PQ, PQ);

	float t = VDot(PQ, P_point) / len2;

	return t;
}