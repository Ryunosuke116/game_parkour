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

	//bool isHitGround = hitCheck.HitRayJudge(modelHandle, -1, player.GetCenterPos(), player.GetFootPos(),hitPoly_Ground);

	//bool isHitGround = hitCheck.SphereHitJudge(modelHandle, -1, player.GetBottomPos(), hitPoly_Ground_sphere);

//	bool hitWall = WallCollisionCheck(player, modelHandle);		//�ǂɏՓ˂��Ă��邩
	bool isHitGround = false;;


	hitCheck.CapsuleHitWallJudge(modelHandle, -1, player.GetTopPos(), player.GetBottomPos(), hitPoly_Wall);

	int wallNum = 0;
	int floorNum = 0;

	//���o���ꂽ�|���S���𔻒�
	for (int i = 0; i < hitPoly_Wall.HitNum; i++)
	{
		//�ǔ���
		if (hitPoly_Wall.Dim[i].Normal.x >= 0.7f || hitPoly_Wall.Dim[i].Normal.z >= 0.7f ||
			hitPoly_Wall.Dim[i].Normal.x <= -0.7f || hitPoly_Wall.Dim[i].Normal.z <= -0.7f)
		{
			wall[wallNum] = &hitPoly_Wall.Dim[i];
			wallNum++;
		}
		//������
		else if(hitPoly_Wall.Dim[i].Normal.y >= 0.7f)
		{
			floor[floorNum] = &hitPoly_Wall.Dim[i];
			floorNum++;
			isHitGround = true;
		}
	}

	//���̉����߂�
	for (int i = 0; i < floorNum; i++)
	{
		VECTOR newPlayerPos = VGet(0.0f, 0.0f, 0.0f);

		//�ʂƋ��̐ڐG���W�𒲂ׂ�
		bool flag = TestSphereTriangle(player.GetBottomPos(), floor[i]->Position[0], floor[i]->Position[1], floor[i]->Position[2], hitPos);

		//�����Ə��̍����v�Z
		newPlayerPos.y = hitPos.y - player.GetFootPos().y;

		//�����Ə��Ƃ̍���0.1�ȏ�̏ꍇ�̂�player�̈ʒu�ɉ��Z
		if (newPlayerPos.y >= 0.1f)
		{
			newPlayerPos = VAdd(player.GetPosition(), newPlayerPos);
			player.SetPos(newPlayerPos);
		}
	}

	VECTOR addPos = VGet(0.0f, 0.0f, 0.0f);

	//�ǂ̉����߂�
	for (int i = 0; i < wallNum; i++)
	{
		normal = wall[i]->Normal;
		normal.y = 0.0f;
		normal = VNorm(normal);

		//�J�v�Z���̒��S���W�����߂�
		VECTOR centerPos = VAdd(player.GetTopPos(), player.GetBottomPos());
		centerPos = VScale(centerPos, 0.5f);

		//�ʂƋ��̐ڐG���W�𒲂ׂ�
		bool flag = TestSphereTriangle(centerPos, wall[i]->Position[0], wall[i]->Position[1], wall[i]->Position[2], hitPos);


		//���̐ڐG���Ă�����W�����߂�
		// ��������ɂ́H��
		//�@�������Ƃ͋t�̕����ɃZ���^�[�|�W�V����������Z����
		VECTOR contact = VScale(normal, -3.5f);

		//�ڐG���Ă�����W
		contact = VAdd(centerPos, contact);

		//���̐ڐG���W���ʂ̐ڐG���W�����߂�
		VECTOR pos = VSub(hitPos, contact);
		pos.y = 0.0f;

		//����܂ł̉����߂��ʂ����傫����΍X�V����
		if (VSize(addPos) <= VSize(pos))
		{
			addPos = pos;
		}

	}

	if (VSize(addPos) != 0)
	{
		VECTOR newPos = VAdd(player.GetPosition(), addPos);
		player.SetPos(newPos);
	}

	//if (isHitGround)
	//{
	//	if (!hitWall)
	//	{
	//		//if (hitPoly_Ground.Normal.y >= 0.7f)
	//		//{
	//		//	VECTOR newPlayerPos = VGet(0.0f, 0.0f, 0.0f);

	//		//	//�����Ə��̍����v�Z
	//		//	newPlayerPos.y = hitPoly_Ground.HitPosition.y - player.GetFootPos().y;
	//		//	subPos.y = hitPoly_Ground.Normal.y;

	//		//	//�����Ə��Ƃ̍���0.1�ȏ�̏ꍇ�̂�player�̈ʒu�ɉ��Z
	//		//	if (newPlayerPos.y >= 0.1f)
	//		//	{
	//		//		newPlayerPos = VAdd(player.GetPosition(), newPlayerPos);
	//		//		player.SetPos(newPlayerPos);
	//		//	}
	//		//}

	//		for (int i = 0; i < hitPoly_Ground_sphere.HitNum; i++)
	//		{
	//			MV1_COLL_RESULT_POLY poly = hitPoly_Ground_sphere.Dim[i];

	//			if (poly.Normal.y >= 0.7f)
	//			{
	//				VECTOR newPlayerPos = VGet(0.0f, 0.0f, 0.0f);

	//				//�ʂƋ��̐ڐG���W�𒲂ׂ�
	//				bool flag = TestSphereTriangle(player.GetBottomPos(), poly.Position[0], poly.Position[1], poly.Position[2], hitPos);

	//				//�����Ə��̍����v�Z
	//				newPlayerPos.y = hitPos.y - player.GetFootPos().y;
	//				subPos.y = poly.Normal.y;

	//				//�����Ə��Ƃ̍���0.1�ȏ�̏ꍇ�̂�player�̈ʒu�ɉ��Z
	//				if (newPlayerPos.y >= 0.1f)
	//				{
	//					newPlayerPos = VAdd(player.GetPosition(), newPlayerPos);
	//					player.SetPos(newPlayerPos);
	//				}
	//			}
	//			else
	//			{
	//				isHitGround = false;
	//			}
	//		}

	//	}
	//}
	
	
	//�ڒn���Ă��邩
	player.SetIsGround(isHitGround);

}

/// <summary>
/// �ǂƂ̓����蔻��
/// </summary>
/// <param name="player"></param>
/// <param name="modelHandle"></param>
/// <returns></returns>
bool CollisionManager::WallCollisionCheck(Player& player, int modelHandle)
{
	//hitCheck.SphereHitWallJudge(modelHandle, -1, player.GetTopPos(), player.GetBottomPos(), hitPoly_Wall);

	//���������ꍇ
	//if (hitPoly_Wall.HitNum >= 1)
	//{
	//	float maxY = -FLT_MAX;
	//	int groundIndex = -1;
	//	VECTOR addPos = VGet(0.0f, 0.0f, 0.0f);

	//	//�q�b�g�����S�|���S���𒲂ׂ�
	//	for (int i = 0; i < hitPoly_Wall.HitNum; i++)
	//	{
	//		MV1_COLL_RESULT_POLY poly = hitPoly_Wall.Dim[i];

	//		//�ǂ��ǂ����𒲂ׂ�
	//		if (poly.Normal.x >= 0.7f || poly.Normal.z >= 0.7f)
	//		{

	//			float difference_Y = poly.HitPosition.y - player.GetFootPos().y;
	//			
	//			//�����Ȓi����������ǂƂ��Ĉ���Ȃ�
	//			if (difference_Y <= 0.5f)
	//			{
 //					continue;
	//			}
	//			
	//			normal = poly.Normal;
	//			normal.y = 0.0f;
	//			normal = VNorm(normal);

	//			VECTOR hitDir = VSub(poly.HitPosition, player.GetBottomPos());
	//			hitDir = VNorm(hitDir);
	//			//hitDir = VScale(hitDir, 3.5f);
	//			hitDir = VScale(normal, -3.5f);
	//			hitDir = VAdd(player.GetBottomPos(), hitDir);

	//			VECTOR pos= VSub(poly.HitPosition, hitDir);
	//			pos.y = 0.0f;

	//			if (VSize(addPos) <= VSize(pos))
	//			{
	//				addPos = pos;
	//			}
	//		}
	//	}

	//	if (VSize(addPos) != 0)
	//	{
	//		VECTOR newPos = VAdd(player.GetPosition(), addPos);
	//		player.SetPos(newPos);
	//		return true;
	//	}
	//}

	hitCheck.CapsuleHitWallJudge(modelHandle, -1, player.GetTopPos(),VAdd(player.GetBottomPos(),VGet(0.0f,1.0f,0.0f)), hitPoly_Wall);

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
			if (poly.Normal.x >= 0.7f || poly.Normal.z >= 0.7f ||
				poly.Normal.x <= -0.7f || poly.Normal.z <= -0.7f)
			{
				normal = poly.Normal;
				normal.y = 0.0f;
				normal = VNorm(normal);
				
				//�J�v�Z���̒��S���W�����߂�
				VECTOR centerPos = VAdd(player.GetTopPos(), player.GetBottomPos());
				centerPos = VScale(centerPos, 0.5f);

				//�ʂƋ��̐ڐG���W�𒲂ׂ�
				bool flag = TestSphereTriangle(centerPos, poly.Position[0], poly.Position[1], poly.Position[2], hitPos);


				//���̐ڐG���Ă�����W�����߂�
				// ��������ɂ́H��
				//�@�������Ƃ͋t�̕����ɃZ���^�[�|�W�V����������Z����
					VECTOR contact = VScale(normal, -3.5f);
				
				//�ڐG���Ă�����W
				contact = VAdd(centerPos, contact);
				

			//	VECTOR pos = VScale(VSub(hitPos, player.GetPosition()), 2.0f);
				
				//���̐ڐG���W���ʂ̐ڐG���W�����߂�
				VECTOR pos = VSub(hitPos, contact);
				pos.y = 0.0f;

				//����܂ł̉����߂��ʂ����傫����΍X�V����
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

VECTOR CollisionManager::ClosestPtToPointTriangle(VECTOR centerPos, VECTOR a, VECTOR b, VECTOR c)
{

	//P��A�̊O���̒��_���W�̒��ɂ��邩�ǂ����`�F�b�N
	VECTOR ab = VSub(b, a);
	VECTOR ac = VSub(c, a);
	VECTOR ap = VSub(centerPos, a);

	float d1 = VDot(ab, ap);
	float d2 = VDot(ac, ap);

	if (d1 <= 0.0f && d2 <= 0.0f)
	{
		//return a;	//�d�S���W(1,0,0)
	}

	//P��B�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	VECTOR bp = VSub(centerPos, b);
	float d3 = VDot(ab, bp);
	float d4 = VDot(ac, bp);

	if (d3 >= 0.0f && d4 <= d3)
	{
		//return b;
	}

	//P��AB�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����P��AB��ɑ΂���ˉe��Ԃ�
	float vc = d1 * d4 - d3 * d2;
	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
	{
		float v = d1 / (d1 - d3);
		//return VAdd(a, VScale(ab, v));
	}

	//P��C�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	VECTOR cp = VSub(centerPos, c);
	float d5 = VDot(ab, cp);
	float d6 = VDot(ac, cp);
	if (d6 >= 0.0f && d5 <= d6)
	{
		//return c;
	}

	//P��AC�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����P��AC��ɑ΂���ˉe��Ԃ�
	float vb = d5 * d2 - d1 * d6;

	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
	{
		float w = d2 / (d2 - d6);
		//return VAdd(a, VScale(ac, w));
	}

	//P��BC�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����P��BC��ɑ΂���ˉe��Ԃ�
	float va = d3 * d6 - d5 * d4;

	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
	{
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		//return VAdd(b, VScale(VSub(c, b), w));
	}

	//P�͖ʗ̈�̒��ɂ���BQ�����̏d�S���W(u,v,w)��p���Čv�Z
	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	
	return VAdd(a,VAdd(VScale(ab, v),VScale(ac,w)));

}

/// <summary>
/// ��s���O�p�`ABC�ƌ������Ă���ꍇ��true��Ԃ��A�����łȂ����false��Ԃ�
/// ���̒��S�ɑ΂���abc��̍ŋߐړ_�ł���_p���Ԃ�
/// </summary>
/// <returns></returns>
bool CollisionManager::TestSphereTriangle(VECTOR centerPos, VECTOR a, VECTOR b, VECTOR c,VECTOR& q)
{
	//���̒��S�΂���ŋߐړ_�ł���O�p�`ABC��ɂ���_p��������
	q = ClosestPtToPointTriangle(centerPos,a,b,c);

	//���ƎO�p�`����������̂́A���̒��S����_q�܂ł�(��������)������(��������)���̔��a�����������ꍇ
	VECTOR v = VSub(q, centerPos);

	return VDot(v, v) <= 3.5f * 3.5f;
}

/// <summary>
/// �`��
/// </summary>
void CollisionManager::Draw()
{
	printfDx("NormalPos.y %f\n", subPos.y);
	printfDx("NormalPos_Wall.x %f\n", normal.x);
	printfDx("NormalPos_Wall.z %f\n", normal.z);
	DrawSphere3D(hitPos, 2.0f, 30, GetColor(0, 0, 0),
		GetColor(255, 0, 0), FALSE);
}