#include "Include.h"


void CollisionManager::Update(Player& player, int modelHandle)
{
	//playerの状態によって当たり判定の優先順位を決める
	//何回か当たり判定を繰り返す
	//prevとnewのposを作る

	VECTOR oldPos = player.GetPosition();
	VECTOR newPos = VAdd(oldPos, player.GetMoveVec());

	VECTOR topPosition = newPos;
	VECTOR bottomPosition = newPos;
	topPosition.y = topPosition.y + 17.0f;
	bottomPosition.y = bottomPosition.y + 3.0f;

	//bool isHitGround = hitCheck.HitRayJudge(modelHandle, -1, player.GetCenterPos(), player.GetFootPos(),hitPoly_Ground);
	bool isHitGround;

	bool hitWall = WallCollisionCheck(player, modelHandle);		//壁に衝突していたら押し戻す
	
	//もう一度衝突チェック
	hitCheck.CapsuleHitWallJudge(modelHandle, -1, player.GetTopPos(), player.GetBottomPos(), hitPoly_Ground_sphere);


	if (hitPoly_Ground_sphere.HitNum >= 1)
	{
		isHitGround = true;
	}
	else
	{
		isHitGround = false;
	}

	////////////////////////
	// これでいきたい
	// //////////////////////
	//bool isHitGround = false;

	VECTOR addPos = VGet(0.0f, 0.0f, 0.0f);


	if (isHitGround && !player.GetData().isJump)
	{
		hitCheck.CapsuleHitWallJudge(modelHandle, -1, topPosition, bottomPosition, hitPoly_Ground_sphere);

		for (int i = 0; i < hitPoly_Ground_sphere.HitNum; i++)
		{
			MV1_COLL_RESULT_POLY poly = hitPoly_Ground_sphere.Dim[i];

			if (poly.Normal.y >= 0.7f)
			{
				VECTOR newPlayerPos = VGet(0.0f, 0.0f, 0.0f);

				//面と球の接触座標を調べる
				bool flag = TestSphereTriangle(bottomPosition, poly.Position[0], poly.Position[1], poly.Position[2], hitPos);

				//足元と床の差を計算
				newPlayerPos.y = hitPos.y - player.GetFootPos().y;
				subPos.y = poly.Normal.y;

				//足元と床との差が0.1以上の場合のみplayerの位置に加算
				if (newPlayerPos.y >= 0.1f)
				{
					newPlayerPos = VAdd(newPos, newPlayerPos);
					player.SetPos(newPlayerPos);
				}
			}
		}
	}
	

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

	VECTOR oldPos = player.GetPosition();
	VECTOR newPos = VAdd(oldPos, player.GetMoveVec());

	VECTOR topPosition = newPos;
	VECTOR bottomPosition = newPos;
	topPosition.y = topPosition.y + 17.0f;
	bottomPosition.y = bottomPosition.y + 3.0f;

	hitCheck.CapsuleHitWallJudge(modelHandle, -1, topPosition,VAdd(bottomPosition,VGet(0.0f,1.0f,0.0f)), hitPoly_Wall);

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
				VECTOR centerPos = VAdd(topPosition, bottomPosition);
				centerPos = VScale(centerPos, 0.5f);

				//面と球の接触座標を調べる
				bool flag = TestSphereTriangle(centerPos, poly.Position[0], poly.Position[1], poly.Position[2], hitPos);

				//球の接触している座標を求める
				// そうするには？↓
				//法線方向とは逆の方向にセンターポジションから加算する
				VECTOR contact = VScale(normal, -3.5f);
				
				//接触している座標
				contact = VAdd(centerPos, contact);

				//少し膜をはる
				contact = VAdd(contact, VScale(poly.Normal, -0.1f));

				
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
 			newPos = VAdd(newPos, addPos);
			player.SetPos(newPos);

			// プレイヤーのモデルの座標を更新する
			MV1SetPosition(player.GetModelHandle(), player.GetPosition());
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