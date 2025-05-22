#include "Include.h"


void CollisionManager::Update(Player& player,int modelHandle)
{
	//bool isHitGround = hitCheck.HitRayJudge(modelHandle, -1, player.GetCenterPos(), player.GetFootPos(),hitPoly_Ground);

	hitPoly = MV1CollCheck_Sphere(modelHandle, -1, player.GetCenterPos(), 13.0f);

	bool isHitGround = false;

	//当たった場合
	if (hitPoly.HitNum >= 1)
	{
		float maxY = -FLT_MAX;
		int groundIndex = -1;
		//ヒットした全ポリゴンを調べる
		for (int i = 0; i < hitPoly.HitNum; i++)
		{
			MV1_COLL_RESULT_POLY poly = hitPoly.Dim[i];

			if (poly.Normal.y >= 0.7f)
			{
				if (poly.HitPosition.y > maxY)
				{
					maxY = poly.HitPosition.y;
					groundIndex = i;
				}
			}
		}

		if (groundIndex != -1)
		{
			MV1_COLL_RESULT_POLY groundPoly = hitPoly.Dim[groundIndex];

			VECTOR newPlayerPos = VGet(0.0f, 0.0f, 0.0f);
			//足元と床の差を計算
			newPlayerPos.y = groundPoly.HitPosition.y - player.GetFootPos().y;
			//足元と床との差が0.1以上の場合のみplayerの位置に加算
			if (newPlayerPos.y >= 0.1f)
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

	//	//足元と床の差を計算
	//	newPlayerPos.y = hitPoly_Ground.HitPosition.y - player.GetFootPos().y;

	//	//足元と床との差が0.1以上の場合のみplayerの位置に加算
	//	if (newPlayerPos.y >= 0.1f)
	//	{
	//		newPlayerPos = VAdd(player.GetPosition(), newPlayerPos);
	//		player.SetPos(newPlayerPos);
	//	}

	//}
	//接地しているか
	player.SetIsGround(isHitGround);

}

