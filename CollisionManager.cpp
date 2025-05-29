#include "Include.h"


void CollisionManager::Update(Player& player,int modelHandle)
{
	//bool isHitGround = false;

	////���������ꍇ
	//if (hitPoly.HitNum >= 1)
	//{
	//	float maxY = -FLT_MAX;
	//	int groundIndex = -1;
	//	//�q�b�g�����S�|���S���𒲂ׂ�
	//	for (int i = 0; i < hitPoly.HitNum; i++)
	//	{
	//		MV1_COLL_RESULT_POLY poly = hitPoly.Dim[i];

	//		if (poly.Normal.x >= 0.9f || poly.Normal.z >= 0.9f)
	//		{
	//			if (poly.HitPosition.y > maxY)
	//			{
	//				maxY = poly.HitPosition.y;
	//				groundIndex = i;
	//			}
	//		}
	//	}

	//	//if (groundIndex != -1)
	//	//{
	//	//	MV1_COLL_RESULT_POLY groundPoly = hitPoly.Dim[groundIndex];

	//	//	VECTOR newPlayerPos = VGet(0.0f, 0.0f, 0.0f);
	//	//	//�����Ə��̍����v�Z
	//	//	newPlayerPos.y = groundPoly.HitPosition.y - player.GetFootPos().y;
	//	//	//�����Ə��Ƃ̍���0.1�ȏ�̏ꍇ�̂�player�̈ʒu�ɉ��Z
	//	//	if (newPlayerPos.y >= 0.1f)
	//	//	{
	//	//		newPlayerPos = VAdd(player.GetPosition(), newPlayerPos);
	//	//		player.SetPos(newPlayerPos);
	//	//	}
	//	//	isHitGround = groundPoly.HitFlag;
	//	//}


	//}

	bool isHitGround = hitCheck.HitRayJudge(modelHandle, -1, player.GetCenterPos(), player.GetFootPos(),hitPoly_Ground);

	bool hitWall = WallCollisionCheck(player, modelHandle);

	if (isHitGround)
	{
		if (!hitWall)
		{
			if (hitPoly_Ground.Normal.y >= 0.7f)
			{
				VECTOR newPlayerPos = VGet(0.0f, 0.0f, 0.0f);

				//�����Ə��̍����v�Z
				newPlayerPos.y = hitPoly_Ground.HitPosition.y - player.GetFootPos().y;
				subPos.y = hitPoly_Ground.Normal.y;

				//�����Ə��Ƃ̍���0.1�ȏ�̏ꍇ�̂�player�̈ʒu�ɉ��Z
				if (newPlayerPos.y >= 0.1f)
				{
					newPlayerPos = VAdd(player.GetPosition(), newPlayerPos);
					player.SetPos(newPlayerPos);
				}
			}
		}
	}
	//�ڒn���Ă��邩
	player.SetIsGround(isHitGround);

}

bool CollisionManager::WallCollisionCheck(Player& player, int modelHandle)
{
	hitCheck.CapsuleHitWallJudge(modelHandle, -1, player.GetTopPos(), player.GetBottomPos(), hitPoly_Wall);

	//���������ꍇ
	if (hitPoly_Wall.HitNum >= 1)
	{
		float maxY = -FLT_MAX;
		int groundIndex = -1;
		VECTOR addPos = VGet(0.0f, 0.0f, 0.0f);

		//�q�b�g�����S�|���S���𒲂ׂ�
		for (int i = 0; i < hitPoly_Wall.HitNum; i++)
		{
			MV1_COLL_RESULT_POLY poly = hitPoly_Wall.Dim[i];

			//�ǂ��ǂ����𒲂ׂ�
			if (poly.Normal.x >= 0.7f || poly.Normal.z >= 0.7f)
			{

				float difference_Y = poly.HitPosition.y - player.GetFootPos().y;
				
				//�����Ȓi����������ǂƂ��Ĉ���Ȃ�
			/*	if (difference_Y <= 0.5f)
				{
 					break;
				}*/
				
				normal = poly.Normal;
				VECTOR hitDir = VSub(poly.HitPosition, player.GetBottomPos());
				hitDir = VNorm(hitDir);
				hitDir = VScale(hitDir, 3.5f);
				hitDir = VAdd(player.GetBottomPos(), hitDir);

				VECTOR pos= VSub(poly.HitPosition, hitDir);
				pos.y = 0.0f;

				if (VSize(addPos) <= VSize(pos))
				{
					addPos = pos;
				}
			}
		}

		if (VSize(addPos) != 0)
		{
			VECTOR newPos = VAdd(player.GetPosition(), addPos);
			player.SetPos(newPos);
			return true;
		}
	}
	// ���o�����v���C���[�̎��͂̃|���S�������J������
	MV1CollResultPolyDimTerminate(hitPoly_Wall);
	return false;
}

void CollisionManager::Draw()
{
	printfDx("NormalPos.y %f\n", subPos.y);
	printfDx("NormalPos_Wall.x %f\n", normal.x);
	printfDx("NormalPos_Wall.z %f\n", normal.z);
}