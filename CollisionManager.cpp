#include "Include.h"
#include <utility>

/// @brief 更新
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
		//壁衝突判定
		WallCollisionCheck(modelHandle, newPos, oldPos, radius,addTopPos, addBottomPos);
	}

	if (!isJump)
	{
		//壁と衝突しているか
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

	//床衝突判定
	return std::make_tuple(afterWallCheck,GroundCollisionCheck(modelHandle, newPos, addTopPos, addBottomPos,radius, isJump), newPos);
}

/// @brief 床との衝突判定処理
/// @param player 
/// @param modelHandle 
/// @param newPos 
/// @return 
bool CollisionManager::GroundCollisionCheck(int modelHandle, VECTOR& newPos, float addTopPos, float addBottomPos, float radius, bool isJump)
{
	//playerの状態によって当たり判定の優先順位を決める
	//何回か当たり判定を繰り返す
	//prevとnewのposを作る
	VECTOR topPosition = newPos;
	VECTOR bottomPosition = newPos;
	topPosition.y = topPosition.y + addTopPos;
	bottomPosition.y = bottomPosition.y + addBottomPos;

	//床と衝突しているか
	bool isHitGround = hitCheck.SphereHitJudge(modelHandle, -1, bottomPosition, hitPoly_Ground_sphere);

	VECTOR addPos = VGet(0.0f, 0.0f, 0.0f);

	//ジャンプ中ではない場合に処理
	if (isHitGround && !isJump)
	{

		for (int i = 0; i < hitPoly_Ground_sphere.HitNum; i++)
		{
			MV1_COLL_RESULT_POLY poly = hitPoly_Ground_sphere.Dim[i];

			if (poly.Normal.y >= 0.7f)
			{
				VECTOR newPlayerPos = VGet(0.0f, 0.0f, 0.0f);

				//面と球の接触座標を調べる
				bool flag = TestSphereTriangle(bottomPosition, poly.Position[0], poly.Position[1], poly.Position[2], hitPos_ground, radius);

				//足元と床の差を計算
 				//newPlayerPos.y = hitPos_ground.y - footPos.y;

				newPlayerPos.y = hitPos_ground.y;

				//足元と床との差が0.1以上の場合のみplayerの位置に加算
				if (newPlayerPos.y >= 0.1f)
				{
					//newPos = VAdd(newPos, newPlayerPos);
					newPos.y = newPlayerPos.y;
					oldPolyPos = hitPos_ground;
				}
			}
		}
	}
	//接地しているか
	return isHitGround;

}

/// <summary>
/// 壁との当たり判定
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

	//壁と衝突しているか
	hitCheck.CapsuleHitWallJudge(modelHandle, -1, 3.5f,topPosition,VAdd(bottomPosition,VGet(0.0f,1.0f,0.0f)), hitPoly_Wall);

	//衝突しているとこを全部調べて押し戻し量を計算する
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
				poly.Normal.x <= -0.7f || poly.Normal.z <= -0.7f &&
				poly.Normal.y <= 0.8f)
			{

				//カプセルの大きさ
 				topPosition = newPos;
				bottomPosition = newPos;
				topPosition.y = topPosition.y + addTopPos;
				bottomPosition.y = bottomPosition.y + addBottomPos;

				normal = poly.Normal;
				normal.y = 0.0f;
				normal = VNorm(normal);
				
				//カプセルの中心座標を求める
				VECTOR centerPos = VAdd(topPosition, bottomPosition);
				centerPos = VScale(centerPos, 0.5f);

				//面と球の接触座標を調べる
				bool flag = TestSphereTriangle(centerPos, poly.Position[0], poly.Position[1], poly.Position[2], hitPos_wall,radius);

				//球の接触している座標を求める
				// そうするには？↓
				//法線方向とは逆の方向にセンターポジションから加算する
				VECTOR contact = VScale(normal, -3.5f);
				
				//接触している座標
				contact = VAdd(centerPos, contact);

				//面に少し膜をはる
				contact = VAdd(contact, VScale(poly.Normal, -0.1f));

				//球の接触座標→面の接触座標を求める
				VECTOR pos = VSub(hitPos_wall, contact);
				pos.y = 0.0f;

				newPos = VAdd(newPos, pos);
				flag = true;

				////これまでの押し戻し量よりも大きければ更新する
				//if (VSize(addPos) <= VSize(pos))
				//{
				//	addPos = pos;
				//	oldPoly = poly;
				//}
			}
		}

		//押し戻し量を適用する
		/*if (VSize(addPos) != 0)
		{
 			newPos = VAdd(newPos, addPos);
			flag = true;
		}*/
	}

	// 検出したプレイヤーの周囲のポリゴン情報を開放する
	MV1CollResultPolyDimTerminate(hitPoly_Wall);
	return flag;

}

/// <summary>
/// 崖つかみ判定
/// </summary>
/// <param name="player"></param>
/// <param name="modelHandle"></param>
void CollisionManager::CliffGrabbing(int modelHandle)
{

}

/// @brief 面との接触座標の計算
/// @param centerPos 
/// @param a 
/// @param b 
/// @param c 
/// @return 
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
bool CollisionManager::TestSphereTriangle(VECTOR centerPos, VECTOR a, VECTOR b, VECTOR c,VECTOR& q, const float radius)
{
	//球の中心対する最近接点である三角形ABC上にある点pを見つける
	q = ClosestPtToPointTriangle(centerPos,a,b,c);

	//球と三角形が交差するのは、球の中心から点qまでの(平方した)距離が(平方した)球の半径よりも小さい場合
	VECTOR v = VSub(q, centerPos);

	return VDot(v, v) <= radius * radius;
}

/// <summary>
/// 描画
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