#include "Include.h"
#include <utility>

/// @brief �X�V
/// @param player 
/// @param modelHandle 
/// @return 
std::tuple<bool, bool, VECTOR> CollisionManager::Update(int modelHandle, const VECTOR& playerPos,
	const VECTOR& moveVec, VECTOR& moveDirection, float radius, float addTopPos, float addBottomPos, bool isJump)
{
	VECTOR oldPos = playerPos;
	VECTOR newPos = VAdd(oldPos, moveVec);	

//	WallCollisionCheck(player, modelHandle, newPos, oldPos);
	VECTOR playerDirAfterPos = VAdd(oldPos, VScale(moveDirection, 3.0f));
	VECTOR topPosition = oldPos;
	VECTOR bottomPosition = oldPos;
	topPosition.y = topPosition.y + addTopPos;
	bottomPosition.y = bottomPosition.y + addBottomPos;
	bool afterWallCheck = false;

	
	if (!afterWallCheck)
	{
		//�ǏՓ˔���
		WallCollisionCheck(modelHandle, newPos, oldPos, radius,addTopPos, addBottomPos);
	}

	if (!isJump)
	{
		//�ǂƏՓ˂��Ă��邩
		hitCheck.CapsuleHitWallJudge(modelHandle, -1, 3.5f, topPosition, VAdd(bottomPosition, VGet(0.0f, 1.0f, 0.0f)), hitPoly_Wall);

		if (hitPoly_Wall.HitNum >= 1)
		{
			for (int i = 0; i < hitPoly_Wall.HitNum - 1; i++)
			{
				MV1_COLL_RESULT_POLY nowPoly = hitPoly_Wall.Dim[i];
				MV1_COLL_RESULT_POLY poly = hitPoly_Wall.Dim[i + 1];

				if (poly.Normal.x!= nowPoly.Normal.x ||
					poly.Normal.z != nowPoly.Normal.z)
				{
					afterWallCheck = true;
					newPos = oldPos;
				}
			}
		}
	}

	//���Փ˔���
	return std::make_tuple(afterWallCheck,GroundCollisionCheck(modelHandle, newPos, addTopPos, addBottomPos,radius, isJump), newPos);
}

/// @brief ���Ƃ̏Փ˔��菈��
/// @param player 
/// @param modelHandle 
/// @param newPos 
/// @return 
bool CollisionManager::GroundCollisionCheck(int modelHandle, VECTOR& newPos, float addTopPos, float addBottomPos, float radius, bool isJump)
{
	//player�̏�Ԃɂ���ē����蔻��̗D�揇�ʂ����߂�
	//���񂩓����蔻����J��Ԃ�
	//prev��new��pos�����
	VECTOR topPosition = newPos;
	VECTOR bottomPosition = newPos;
	topPosition.y = topPosition.y + addTopPos;
	bottomPosition.y = bottomPosition.y + addBottomPos;

	//���ƏՓ˂��Ă��邩
	bool isHitGround = hitCheck.SphereHitJudge(modelHandle, -1, bottomPosition, hitPoly_Ground_sphere);

	VECTOR addPos = VGet(0.0f, 0.0f, 0.0f);

	//�W�����v���ł͂Ȃ��ꍇ�ɏ���
	if (isHitGround && !isJump)
	{

		for (int i = 0; i < hitPoly_Ground_sphere.HitNum; i++)
		{
			MV1_COLL_RESULT_POLY poly = hitPoly_Ground_sphere.Dim[i];

			if (poly.Normal.y >= 0.7f)
			{
				VECTOR newPlayerPos = VGet(0.0f, 0.0f, 0.0f);

				//�ʂƋ��̐ڐG���W�𒲂ׂ�
				bool flag = TestSphereTriangle(bottomPosition, poly.Position[0], poly.Position[1], poly.Position[2], hitPos_ground, radius);

				//�����Ə��̍����v�Z
 				//newPlayerPos.y = hitPos_ground.y - footPos.y;

				newPlayerPos.y = hitPos_ground.y;

				//�����Ə��Ƃ̍���0.1�ȏ�̏ꍇ�̂�player�̈ʒu�ɉ��Z
				if (newPlayerPos.y >= 0.1f)
				{
					//newPos = VAdd(newPos, newPlayerPos);
					newPos.y = newPlayerPos.y;
					oldPolyPos = hitPos_ground;
				}
			}
		}
	}
	//�ڒn���Ă��邩
	return isHitGround;

}

/// <summary>
/// �ǂƂ̓����蔻��
/// </summary>
/// <param name="player"></param>
/// <param name="modelHandle"></param>
/// <returns></returns>
bool CollisionManager::WallCollisionCheck(int modelHandle, VECTOR& newPos, VECTOR& oldPos, float radius, float addTopPos, float addBottomPos)
{

	VECTOR topPosition = newPos;
	VECTOR bottomPosition = newPos;
	topPosition.y = topPosition.y + addTopPos;
	bottomPosition.y = bottomPosition.y + addBottomPos;
	bool flag = false;

	//�ǂƏՓ˂��Ă��邩
	hitCheck.CapsuleHitWallJudge(modelHandle, -1, 3.5f,topPosition,VAdd(bottomPosition,VGet(0.0f,1.0f,0.0f)), hitPoly_Wall);

	//�Փ˂��Ă���Ƃ���S�����ׂĉ����߂��ʂ��v�Z����
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
				poly.Normal.x <= -0.7f || poly.Normal.z <= -0.7f &&
				poly.Normal.y <= 0.8f)
			{

				//�J�v�Z���̑傫��
 				topPosition = newPos;
				bottomPosition = newPos;
				topPosition.y = topPosition.y + addTopPos;
				bottomPosition.y = bottomPosition.y + addBottomPos;

				normal = poly.Normal;
				normal.y = 0.0f;
				normal = VNorm(normal);
				
				//�J�v�Z���̒��S���W�����߂�
				VECTOR centerPos = VAdd(topPosition, bottomPosition);
				centerPos = VScale(centerPos, 0.5f);

				//�ʂƋ��̐ڐG���W�𒲂ׂ�
				bool flag = TestSphereTriangle(centerPos, poly.Position[0], poly.Position[1], poly.Position[2], hitPos_wall,radius);

				//���̐ڐG���Ă�����W�����߂�
				// ��������ɂ́H��
				//�@�������Ƃ͋t�̕����ɃZ���^�[�|�W�V����������Z����
				VECTOR contact = VScale(normal, -3.5f);
				
				//�ڐG���Ă�����W
				contact = VAdd(centerPos, contact);

				//�ʂɏ��������͂�
				contact = VAdd(contact, VScale(poly.Normal, -0.1f));

				//���̐ڐG���W���ʂ̐ڐG���W�����߂�
				VECTOR pos = VSub(hitPos_wall, contact);
				pos.y = 0.0f;

				newPos = VAdd(newPos, pos);
				flag = true;

				////����܂ł̉����߂��ʂ����傫����΍X�V����
				//if (VSize(addPos) <= VSize(pos))
				//{
				//	addPos = pos;
				//	oldPoly = poly;
				//}
			}
		}

		//�����߂��ʂ�K�p����
		/*if (VSize(addPos) != 0)
		{
 			newPos = VAdd(newPos, addPos);
			flag = true;
		}*/
	}

	// ���o�����v���C���[�̎��͂̃|���S�������J������
	MV1CollResultPolyDimTerminate(hitPoly_Wall);
	return flag;

}

/// <summary>
/// �R���ݔ���
/// </summary>
/// <param name="player"></param>
/// <param name="modelHandle"></param>
void CollisionManager::CliffGrabbing(int modelHandle)
{

}

/// @brief �ʂƂ̐ڐG���W�̌v�Z
/// @param centerPos 
/// @param a 
/// @param b 
/// @param c 
/// @return 
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
bool CollisionManager::TestSphereTriangle(VECTOR centerPos, VECTOR a, VECTOR b, VECTOR c,VECTOR& q, const float radius)
{
	//���̒��S�΂���ŋߐړ_�ł���O�p�`ABC��ɂ���_p��������
	q = ClosestPtToPointTriangle(centerPos,a,b,c);

	//���ƎO�p�`����������̂́A���̒��S����_q�܂ł�(��������)������(��������)���̔��a�����������ꍇ
	VECTOR v = VSub(q, centerPos);

	return VDot(v, v) <= radius * radius;
}

/// <summary>
/// �`��
/// </summary>
void CollisionManager::Draw()
{
	printfDx("NormalPos.y %f\n", subPos.y);
	printfDx("NormalPos_Wall.x %f\n", normal.x);
	printfDx("NormalPos_Wall.z %f\n", normal.z);
	DrawSphere3D(hitPos_wall, 2.0f, 30, GetColor(0, 0, 0),
		GetColor(255, 0, 0), FALSE);
	DrawSphere3D(hitPos_ground, 2.0f, 30, GetColor(0, 0, 0),
		GetColor(255, 0, 0), FALSE);
}