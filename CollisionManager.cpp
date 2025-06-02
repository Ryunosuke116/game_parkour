#include "Include.h"


void CollisionManager::Update(Player& player,int modelHandle)
{
	//bool isHitGround = false;

	////当たった場合
	//if (hitPoly.HitNum >= 1)
	//{
	//	float maxY = -FLT_MAX;
	//	int groundIndex = -1;
	//	//ヒットした全ポリゴンを調べる
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
	//	//	//足元と床の差を計算
	//	//	newPlayerPos.y = groundPoly.HitPosition.y - player.GetFootPos().y;
	//	//	//足元と床との差が0.1以上の場合のみplayerの位置に加算
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

//	bool hitWall = WallCollisionCheck(player, modelHandle);		//壁に衝突しているか
	bool isHitGround = false;;


	hitCheck.CapsuleHitWallJudge(modelHandle, -1, player.GetTopPos(), player.GetBottomPos(), hitPoly_Wall);

	int wallNum = 0;
	int floorNum = 0;

	//検出されたポリゴンを判定
	for (int i = 0; i < hitPoly_Wall.HitNum; i++)
	{
		//壁判定
		if (hitPoly_Wall.Dim[i].Normal.x >= 0.7f || hitPoly_Wall.Dim[i].Normal.z >= 0.7f ||
			hitPoly_Wall.Dim[i].Normal.x <= -0.7f || hitPoly_Wall.Dim[i].Normal.z <= -0.7f)
		{
			wall[wallNum] = &hitPoly_Wall.Dim[i];
			wallNum++;
		}
		//床判定
		else if(hitPoly_Wall.Dim[i].Normal.y >= 0.7f)
		{
			floor[floorNum] = &hitPoly_Wall.Dim[i];
			floorNum++;
			isHitGround = true;
		}
	}

	//床の押し戻し
	for (int i = 0; i < floorNum; i++)
	{
		VECTOR newPlayerPos = VGet(0.0f, 0.0f, 0.0f);

		//面と球の接触座標を調べる
		bool flag = TestSphereTriangle(player.GetBottomPos(), floor[i]->Position[0], floor[i]->Position[1], floor[i]->Position[2], hitPos);

		//足元と床の差を計算
		newPlayerPos.y = hitPos.y - player.GetFootPos().y;

		//足元と床との差が0.1以上の場合のみplayerの位置に加算
		if (newPlayerPos.y >= 0.1f)
		{
			newPlayerPos = VAdd(player.GetPosition(), newPlayerPos);
			player.SetPos(newPlayerPos);
		}
	}

	VECTOR addPos = VGet(0.0f, 0.0f, 0.0f);

	//壁の押し戻し
	for (int i = 0; i < wallNum; i++)
	{
		normal = wall[i]->Normal;
		normal.y = 0.0f;
		normal = VNorm(normal);

		//カプセルの中心座標を求める
		VECTOR centerPos = VAdd(player.GetTopPos(), player.GetBottomPos());
		centerPos = VScale(centerPos, 0.5f);

		//面と球の接触座標を調べる
		bool flag = TestSphereTriangle(centerPos, wall[i]->Position[0], wall[i]->Position[1], wall[i]->Position[2], hitPos);


		//球の接触している座標を求める
		// そうするには？↓
		//法線方向とは逆の方向にセンターポジションから加算する
		VECTOR contact = VScale(normal, -3.5f);

		//接触している座標
		contact = VAdd(centerPos, contact);

		//球の接触座標→面の接触座標を求める
		VECTOR pos = VSub(hitPos, contact);
		pos.y = 0.0f;

		//これまでの押し戻し量よりも大きければ更新する
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

	//		//	//足元と床の差を計算
	//		//	newPlayerPos.y = hitPoly_Ground.HitPosition.y - player.GetFootPos().y;
	//		//	subPos.y = hitPoly_Ground.Normal.y;

	//		//	//足元と床との差が0.1以上の場合のみplayerの位置に加算
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

	//				//面と球の接触座標を調べる
	//				bool flag = TestSphereTriangle(player.GetBottomPos(), poly.Position[0], poly.Position[1], poly.Position[2], hitPos);

	//				//足元と床の差を計算
	//				newPlayerPos.y = hitPos.y - player.GetFootPos().y;
	//				subPos.y = poly.Normal.y;

	//				//足元と床との差が0.1以上の場合のみplayerの位置に加算
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
	
	
	//接地しているか
	player.SetIsGround(isHitGround);

}

/// <summary>
/// 壁との当たり判定
/// </summary>
/// <param name="player"></param>
/// <param name="modelHandle"></param>
/// <returns></returns>
bool CollisionManager::WallCollisionCheck(Player& player, int modelHandle)
{
	//hitCheck.SphereHitWallJudge(modelHandle, -1, player.GetTopPos(), player.GetBottomPos(), hitPoly_Wall);

	//当たった場合
	//if (hitPoly_Wall.HitNum >= 1)
	//{
	//	float maxY = -FLT_MAX;
	//	int groundIndex = -1;
	//	VECTOR addPos = VGet(0.0f, 0.0f, 0.0f);

	//	//ヒットした全ポリゴンを調べる
	//	for (int i = 0; i < hitPoly_Wall.HitNum; i++)
	//	{
	//		MV1_COLL_RESULT_POLY poly = hitPoly_Wall.Dim[i];

	//		//壁かどうかを調べる
	//		if (poly.Normal.x >= 0.7f || poly.Normal.z >= 0.7f)
	//		{

	//			float difference_Y = poly.HitPosition.y - player.GetFootPos().y;
	//			
	//			//小さな段差だったら壁として扱わない
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

			//ヒットした全ポリゴンを調べる
		for (int i = 0; i < hitPoly_Wall.HitNum; i++)
		{
			MV1_COLL_RESULT_POLY poly = hitPoly_Wall.Dim[i];

			//壁かどうかを調べる
			if (poly.Normal.x >= 0.7f || poly.Normal.z >= 0.7f ||
				poly.Normal.x <= -0.7f || poly.Normal.z <= -0.7f)
			{
				normal = poly.Normal;
				normal.y = 0.0f;
				normal = VNorm(normal);
				
				//カプセルの中心座標を求める
				VECTOR centerPos = VAdd(player.GetTopPos(), player.GetBottomPos());
				centerPos = VScale(centerPos, 0.5f);

				//面と球の接触座標を調べる
				bool flag = TestSphereTriangle(centerPos, poly.Position[0], poly.Position[1], poly.Position[2], hitPos);


				//球の接触している座標を求める
				// そうするには？↓
				//法線方向とは逆の方向にセンターポジションから加算する
					VECTOR contact = VScale(normal, -3.5f);
				
				//接触している座標
				contact = VAdd(centerPos, contact);
				

			//	VECTOR pos = VScale(VSub(hitPos, player.GetPosition()), 2.0f);
				
				//球の接触座標→面の接触座標を求める
				VECTOR pos = VSub(hitPos, contact);
				pos.y = 0.0f;

				//これまでの押し戻し量よりも大きければ更新する
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

	// 検出したプレイヤーの周囲のポリゴン情報を開放する
	MV1CollResultPolyDimTerminate(hitPoly_Wall);
	return false;

}

VECTOR CollisionManager::ClosestPtToPointTriangle(VECTOR centerPos, VECTOR a, VECTOR b, VECTOR c)
{

	//PがAの外側の頂点座標の中にあるかどうかチェック
	VECTOR ab = VSub(b, a);
	VECTOR ac = VSub(c, a);
	VECTOR ap = VSub(centerPos, a);

	float d1 = VDot(ab, ap);
	float d2 = VDot(ac, ap);

	if (d1 <= 0.0f && d2 <= 0.0f)
	{
		//return a;	//重心座標(1,0,0)
	}

	//PがBの外側の頂点領域の中にあるかどうかチェック
	VECTOR bp = VSub(centerPos, b);
	float d3 = VDot(ab, bp);
	float d4 = VDot(ac, bp);

	if (d3 >= 0.0f && d4 <= d3)
	{
		//return b;
	}

	//PがABの辺領域の中にあるかどうかチェックし、あればPのAB上に対する射影を返す
	float vc = d1 * d4 - d3 * d2;
	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
	{
		float v = d1 / (d1 - d3);
		//return VAdd(a, VScale(ab, v));
	}

	//PがCの外側の頂点領域の中にあるかどうかチェック
	VECTOR cp = VSub(centerPos, c);
	float d5 = VDot(ab, cp);
	float d6 = VDot(ac, cp);
	if (d6 >= 0.0f && d5 <= d6)
	{
		//return c;
	}

	//PがACの辺領域の中にあるかどうかチェックし、あればPのAC上に対する射影を返す
	float vb = d5 * d2 - d1 * d6;

	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
	{
		float w = d2 / (d2 - d6);
		//return VAdd(a, VScale(ac, w));
	}

	//PがBCの辺領域の中にあるかどうかチェックし、あればPのBC上に対する射影を返す
	float va = d3 * d6 - d5 * d4;

	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
	{
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		//return VAdd(b, VScale(VSub(c, b), w));
	}

	//Pは面領域の中にある。Qをその重心座標(u,v,w)を用いて計算
	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	
	return VAdd(a,VAdd(VScale(ab, v),VScale(ac,w)));

}

/// <summary>
/// 球sが三角形ABCと交差している場合はtrueを返し、そうでなければfalseを返す
/// 球の中心に対するabc上の最近接点である点pも返す
/// </summary>
/// <returns></returns>
bool CollisionManager::TestSphereTriangle(VECTOR centerPos, VECTOR a, VECTOR b, VECTOR c,VECTOR& q)
{
	//球の中心対する最近接点である三角形ABC上にある点pを見つける
	q = ClosestPtToPointTriangle(centerPos,a,b,c);

	//球と三角形が交差するのは、球の中心から点qまでの(平方した)距離が(平方した)球の半径よりも小さい場合
	VECTOR v = VSub(q, centerPos);

	return VDot(v, v) <= 3.5f * 3.5f;
}

/// <summary>
/// 描画
/// </summary>
void CollisionManager::Draw()
{
	printfDx("NormalPos.y %f\n", subPos.y);
	printfDx("NormalPos_Wall.x %f\n", normal.x);
	printfDx("NormalPos_Wall.z %f\n", normal.z);
	DrawSphere3D(hitPos, 2.0f, 30, GetColor(0, 0, 0),
		GetColor(255, 0, 0), FALSE);
}