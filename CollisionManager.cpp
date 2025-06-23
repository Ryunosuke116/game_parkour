#include "Include.h"
#include <utility>

/// @brief 更新
/// @param player 
/// @param modelHandle 
/// @return 
std::pair<bool, VECTOR> CollisionManager::Update(int modelHandle, const VECTOR& playerPos,
	const VECTOR& moveVec, VECTOR& moveDirection, float radius, float addTopPos, float addBottomPos, bool isJump)
{
	VECTOR oldPos = playerPos;
	VECTOR newPos = VAdd(oldPos, moveVec);	

	VECTOR topPosition = oldPos;
	VECTOR bottomPosition = oldPos;
	topPosition.y = topPosition.y + addTopPos;
	bottomPosition.y = bottomPosition.y + addBottomPos;
	
	//壁衝突判定
	WallCollisionCheck(modelHandle, newPos, oldPos, radius,addTopPos, addBottomPos);

	//床衝突判定
	return std::make_pair(GroundCollisionCheck(modelHandle, oldPos, newPos, addTopPos, addBottomPos, radius, isJump), newPos);
}

/// @brief 床との衝突判定処理
/// @param player 
/// @param modelHandle 
/// @param newPos 
/// @return 
bool CollisionManager::GroundCollisionCheck(int modelHandle,const VECTOR& oldPos, VECTOR& newPos, float addTopPos, float addBottomPos, float radius, bool isJump)
{
	//playerの状態によって当たり判定の優先順位を決める
	//何回か当たり判定を繰り返す
	//prevとnewのposを作る
	VECTOR topPosition = newPos;
	VECTOR bottomPosition = newPos;
	VECTOR nowBottomPos = oldPos;
	VECTOR nowTopPos = oldPos;
	bool isHitSphere;

	topPosition.y = topPosition.y + addTopPos;
	nowTopPos.y = nowTopPos.y + addTopPos;
	bottomPosition.y = bottomPosition.y + addBottomPos;

	//球よりちょい大きめに
	nowBottomPos.y = nowBottomPos.y + addBottomPos - 5.0f;

	//床と衝突しているか
	bool isHitGround = hitCheck.SphereHitJudge(modelHandle, -1, bottomPosition, hitPoly_Ground_sphere);
	isHitSphere = isHitGround;
	
	MV1_COLL_RESULT_POLY rayPoly_ground;

	//ジャンプしていないときに坂道なのかどうか
	if (!isJump)
	{
		//bool isHitGround_ray = hitCheck.HitRayJudge(modelHandle, -1, topPosition, nowBottomPos, rayPoly_ground);
		//
		//if (rayPoly_ground.Normal.y < 1.0f && rayPoly_ground.Normal.y>0.7f)
		//{
		//	if (isHitGround_ray)
		//	{
		//		VECTOR newPlayerPos = VGet(0.0f, 0.0f, 0.0f);
		//		VECTOR footPos = VGet(0.0f, bottomPosition.y - radius, 0.0f);

		//		//床 - プレイヤーの足元で押し戻し量を計算
		//		newPlayerPos.y = rayPoly_ground.HitPosition.y - footPos.y;
		//		newPos.y = newPos.y + newPlayerPos.y;

		//		//log用
		//		topPos_ray = topPosition;
		//		bottomPos_ray = bottomPosition;
		//		bottomPos_ray.y = bottomPos_ray.y - 5.0f;
		//	
		//	}
		//}


	}

	VECTOR addPos = VGet(0.0f, 0.0f, 0.0f);

	//ジャンプ中ではない場合に処理
	if (isHitGround && !isJump)
	{

		for (int i = 0; i < hitPoly_Ground_sphere.HitNum; i++)
		{
			MV1_COLL_RESULT_POLY poly = hitPoly_Ground_sphere.Dim[i];

			if (poly.Normal.y >= 1.0f)
			{
  				VECTOR newPlayerPos = VGet(0.0f, 0.0f, 0.0f);

				//未来座標の球の最下部座標
				VECTOR bottomPos_sphere = bottomPosition;
				bottomPos_sphere.y -= radius;
				
				//次のフレームのplayerの接触座標を求める
				hitPos_ground = hitCheck.ClosestPtToPointTriangle(bottomPos_sphere, poly.Position[0], poly.Position[1], poly.Position[2]);

				//現在の球と面の接触座標を求める
				VECTOR nowHitPos_ground = hitCheck.ClosestPtToPointTriangle(nowBottomPos, poly.Position[0], poly.Position[1], poly.Position[2]);

				MV1_COLL_RESULT_POLY rayPoly;

				//現在の座標からrayを伸ばし、当たっていなかったらif文内に行く
				if (!hitCheck.HitRayJudge(modelHandle, -1, nowTopPos, nowBottomPos, rayPoly))
				{
					//三角形の外側のときのとき球とメッシュの接触部分で押し戻し量を求める
					if (!hitCheck.TriangleAreaCheck_ground(nowHitPos_ground, poly.Position[0], poly.Position[1], poly.Position[2]))
					{
						//線分と点の最近点
 						VECTOR nearPoint_1 = Calclation::NearestPoint(poly.Position[0], poly.Position[1], nowHitPos_ground);
						VECTOR nearPoint_2 = Calclation::NearestPoint(poly.Position[0], poly.Position[2], nowHitPos_ground);
						VECTOR nearPoint_3 = Calclation::NearestPoint(poly.Position[1], poly.Position[2], nowHitPos_ground);

						//一番近い座標を選択する
						float d1 = VSize(VSub(nearPoint_1, nowHitPos_ground));
						float d2 = VSize(VSub(nearPoint_2, nowHitPos_ground));
						float d3 = VSize(VSub(nearPoint_3, nowHitPos_ground));


						//球と面の接触点を求める
						if (d1 <= d2 && d1 <= d3)
						{
							nearestPoint = nearPoint_1;
						}
						else if (d2 <= d3)
						{
							nearestPoint = nearPoint_2;
						}
						else
						{
							nearestPoint = nearPoint_3;
						}

						//球の接触点を計算
						hitSphere = VSub(nearestPoint, bottomPosition);
						hitSphere = VNorm(hitSphere);
						hitSphere = VScale(hitSphere, radius);
						hitSphere = VAdd(bottomPosition, hitSphere);
						newPlayerPos.y = nearestPoint.y - hitSphere.y;
						//newPlayerPos.y = newPos.y + newPlayerPos.y;
					}
				}
				//rayが当たっていれば球の最下部ととメッシュで押し戻し量計算
				else
				{
					//球と面の接触しているは球の最下部と面の接触座標と同じなのでリセット
					hitSphere = VGet(0.0f, 0.0f, 0.0f);

					//平面であればそのまま足元で計算
					if (poly.Normal.y >= 1.0f)
					{
						VECTOR footPos = VGet(0.0f, bottomPosition.y - radius, 0.0f);
 						newPlayerPos.y = hitPos_ground.y - footPos.y;
					}
				}

				//足元と床との差が0.1以上の場合のみplayerの位置に加算
				if (newPlayerPos.y >= 0.1f)
				{
					newPos.y = newPos.y + newPlayerPos.y;
				}
			}
		}
	}
	
	MV1_COLL_RESULT_POLY lastPoly = {};

	//hitPolyの一番最後の情報を受け取る
	if (hitPoly_Ground_sphere.HitNum >= 1)
	{
		lastPoly = hitPoly_Ground_sphere.Dim[hitPoly_Ground_sphere.HitNum - 1];
	}

	//カプセルが当たっていないときrayをチェック
	//下り坂はrayでチェック
	if (!isHitSphere || lastPoly.Normal.y >= 0.7f)
	{
		isHitGround = hitCheck.HitRayJudge(modelHandle, -1, nowTopPos, nowBottomPos, rayPoly_ground);

		if (isHitGround && !isJump)
		{
			VECTOR newPlayerPos = VGet(0.0f, 0.0f, 0.0f);
			VECTOR footPos = VGet(0.0f, bottomPosition.y - radius, 0.0f);

			//床 - プレイヤーの足元で押し戻し量を計算
			newPlayerPos.y = rayPoly_ground.HitPosition.y - footPos.y;
			newPos.y = newPos.y + newPlayerPos.y;

		}
	}

	//log用
	topPos_ray = topPosition;
	bottomPos_ray = bottomPosition;
	bottomPos_ray.y = bottomPos_ray.y - 5.0f;

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
				float topHitPos = Triangle_Point_MinLength_Square(poly.Position[0], poly.Position[1], poly.Position[2], topPosition);
				VECTOR centerPos = VAdd(topPosition, bottomPosition);
				centerPos = VScale(centerPos, 0.5f);

				//面と球の接触座標を調べる
				bool flag = TestSphereTriangle(centerPos, poly.Position[0], poly.Position[1], poly.Position[2], hitPos_wall,radius);

				//線分のどこに当たったか
				auto result = hitCheck.SegmentTriangleDistance(topPosition, bottomPosition, poly.Position[0], poly.Position[1], poly.Position[2],poly.Normal);

				//oldとnewの間でoldから一番近い壁はどれか
		

				//球の接触している座標を求める
				// そうするには？↓
				//法線方向とは逆の方向にセンターポジションから加算する
				VECTOR contact = VScale(normal, -3.5f);

				hitPos_wall = result.second;
				
				////接触している座標
				//contact = VAdd(centerPos, contact);

				////面に少し膜をはる
				//contact = VAdd(contact, VScale(poly.Normal, -0.1f));

				////球の接触座標→面の接触座標を求める
				//VECTOR pos = VSub(hitPos_wall, contact);
				//pos.y = 0.0f;


				//接触している座標
				contact = VAdd(result.first, contact);

				//面に少し膜をはる
				contact = VAdd(contact, VScale(poly.Normal, -0.1f));

				//球の接触座標→面の接触座標を求める
				VECTOR pos = VSub(result.second, contact);
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


/// <summary>
/// 球sが三角形ABCと交差している場合はtrueを返し、そうでなければfalseを返す
/// 球の中心に対するabc上の最近接点である点pも返す
/// </summary>
/// <returns></returns>
bool CollisionManager::TestSphereTriangle(VECTOR centerPos, VECTOR a, VECTOR b, VECTOR c,VECTOR& q, const float radius)
{
	//球の中心対する最近接点である三角形ABC上にある点pを見つける
	q = hitCheck.ClosestPtToPointTriangle(centerPos, a, b, c);

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
	printfDx("hitPos_ground.x %f\n", hitPos_ground.x);
	printfDx("hitPos_ground.y %f\n", hitPos_ground.y);
	printfDx("hitPos_ground.z %f\n", hitPos_ground.z);
	DrawSphere3D(hitPos_wall, 2.0f, 30, GetColor(0, 0, 0),
		GetColor(255, 0, 0), FALSE);
	DrawSphere3D(hitPos_ground, 2.0f, 30, GetColor(0, 0, 0),
		GetColor(255, 0, 0), FALSE);
	DrawSphere3D(hitSphere, 2.0f, 30, GetColor(0, 0, 0),
		GetColor(255, 0, 0), FALSE);

	DrawLine3D(topPos_ray, bottomPos_ray, GetColor(255, 0, 0));
}