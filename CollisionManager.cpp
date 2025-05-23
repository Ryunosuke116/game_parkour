#include "Include.h"


void CollisionManager::Update(Player& player,int modelHandle)
{
	//bool isHitGround = hitCheck.HitRayJudge(modelHandle, -1, player.GetCenterPos(), player.GetFootPos(),hitPoly_Ground);

	hitPoly = MV1CollCheck_Capsule(modelHandle, -1, player.GetTopPosition(),
		player.GetBottomPosition(), 3.0f);

	bool isHitGround = false;

	//当たった場合
	if (hitPoly.HitNum >= 1)
	{
		//float maxY = -FLT_MAX;
		float maxY = -9999.0f;
		int groundIndex = -1;
		//ヒットした全ポリゴンを調べる

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
				//position.yが一番高いものを保存
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

		//		//position.yが一番高いものを保存
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
			//足元と床の差を計算
			//newPlayerPos.y = groundPoly.HitPosition.y - player.GetFootPos().y;
			//newPlayerPos.y = maxY - player.GetFootPos().y;
			newPlayerPos.y = maxY - (player.GetBottomPosition().y - 3.0f);

			//足元と床との差が0.1以上の場合のみplayerの位置に加算
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

/// <summary>
/// カプセルの中心線とポリゴンの平面との交点を計算
/// </summary>
/// <param name="rayStartPos"></param>
/// <param name="rayEndPos"></param>
/// <param name="planePoint"></param>
/// <param name="planeNormal"></param>
/// <param name="intersection"></param>
bool CollisionManager::CalculateIntersection(const VECTOR& rayStartPos, const VECTOR& rayEndPos,
	const VECTOR& planePoint, const VECTOR& planeNormal, VECTOR& intersection)
{
	//線分の方向ベクトル
	VECTOR rayDirection = VSub(rayEndPos, rayStartPos);

	//平面の法線と線分の方向ベクトルの内積を計算
	float dot = VDot(planeNormal, rayDirection);

	//内積が限りなく0に近い場合、交点は存在しない
	if(fabs(dot) < 0.00001f)
	{
		//線分と平面が平行な場合
		return false;
	}



}