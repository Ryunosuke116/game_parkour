#include "Include.h"


void CollisionManager::Update(Player& player,int modelHandle)
{
	//bool isHitGround = hitCheck.HitRayJudge(modelHandle, -1, player.GetCenterPos(), player.GetFootPos(),hitPoly_Ground);

	hitPoly = MV1CollCheck_Capsule(modelHandle, -1, player.GetTopPosition(),
		player.GetBottomPosition(), 3.0f);

	bool isHitGround = false;

	//���������ꍇ
	if (hitPoly.HitNum >= 1)
	{
		//float maxY = -FLT_MAX;
		float maxY = -9999.0f;
		int groundIndex = -1;
		//�q�b�g�����S�|���S���𒲂ׂ�

		for (int i = 0; i < hitPoly.HitNum; i++)
		{
			if (hitPoly.Dim[i].Normal.y >= 0.7f)
			{
				maxY = hitPoly.Dim[i].Position[0].y;
			}
		}

		for(int i = 0; i < hitPoly.HitNum; i++)
		{

			if (hitPoly.Dim[i].Normal.y >= 0.7f)
			{
				//position.y����ԍ������̂�ۑ�
				for (int j = 0; j < 3; j++)
				{
					if (maxY < hitPoly.Dim[i].Position[j].y)
					{
						maxY = hitPoly.Dim[i].Position[j].y;
						groundIndex = i;
					}

					/*if (maxY < poly.Position[j].y)
					{
						maxY = poly.Position[j].y;
						groundIndex = i;
					}*/
				}

			}
		}

		//maxY = hitPoly.Dim[0].Position[0].y;

		//for (int i = 0; i < hitPoly.HitNum; i++)
		//{
		//	MV1_COLL_RESULT_POLY poly = hitPoly.Dim[i];
		//	
		//	if (poly.Normal.y >= 0.7f)
		//	{

		//		//position.y����ԍ������̂�ۑ�
		//		for (int j = 0; j < 3; j++)
		//		{
		//			if (maxY > poly.Position[j].y)
		//			{
		//				maxY = poly.Position[j].y;
		//				groundIndex = i;
		//			}

		//			/*if (maxY < poly.Position[j].y)
		//			{
		//				maxY = poly.Position[j].y;
		//				groundIndex = i;
		//			}*/
		//		}
		//	}
		//}
		
		if (groundIndex != -1)
		{
			MV1_COLL_RESULT_POLY groundPoly = hitPoly.Dim[groundIndex];

			VECTOR newPlayerPos = VGet(0.0f, 0.0f, 0.0f);
			//�����Ə��̍����v�Z
			//newPlayerPos.y = groundPoly.HitPosition.y - player.GetFootPos().y;
			//newPlayerPos.y = maxY - player.GetFootPos().y;
			newPlayerPos.y = maxY - (player.GetBottomPosition().y - 3.0f);

			//�����Ə��Ƃ̍���0.1�ȏ�̏ꍇ�̂�player�̈ʒu�ɉ��Z
			if (newPlayerPos.y >= 0.05f)
			{
				newPlayerPos = VAdd(player.GetPosition(), newPlayerPos);
				player.SetPos(newPlayerPos);
			}
			isHitGround = groundPoly.HitFlag;
		}

	}

	//if (isHitGround)
	//{
	//	VECTOR newPlayerPos = VGet(0.0f, 0.0f, 0.0f);

	//	//�����Ə��̍����v�Z
	//	newPlayerPos.y = hitPoly_Ground.HitPosition.y - player.GetFootPos().y;

	//	//�����Ə��Ƃ̍���0.1�ȏ�̏ꍇ�̂�player�̈ʒu�ɉ��Z
	//	if (newPlayerPos.y >= 0.1f)
	//	{
	//		newPlayerPos = VAdd(player.GetPosition(), newPlayerPos);
	//		player.SetPos(newPlayerPos);
	//	}

	//}
	//�ڒn���Ă��邩
	player.SetIsGround(isHitGround);

}

/// <summary>
/// �J�v�Z���̒��S���ƃ|���S���̕��ʂƂ̌�_���v�Z
/// </summary>
/// <param name="rayStartPos"></param>
/// <param name="rayEndPos"></param>
/// <param name="planePoint"></param>
/// <param name="planeNormal"></param>
/// <param name="intersection"></param>
bool CollisionManager::CalculateIntersection(const VECTOR& rayStartPos, const VECTOR& rayEndPos,
	const VECTOR& planePoint, const VECTOR& planeNormal, VECTOR& intersection)
{
	//�����̕����x�N�g��
	VECTOR rayDirection = VSub(rayEndPos, rayStartPos);

	//���ʂ̖@���Ɛ����̕����x�N�g���̓��ς��v�Z
	float dot = VDot(planeNormal, rayDirection);

	//���ς�����Ȃ�0�ɋ߂��ꍇ�A��_�͑��݂��Ȃ�
	if(fabs(dot) < 0.00001f)
	{
		//�����ƕ��ʂ����s�ȏꍇ
		return false;
	}



}