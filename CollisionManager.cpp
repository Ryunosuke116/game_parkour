#include "common.h"
#include "HitCheck.h"
#include "Calculation.h"
#include "PlayerStateActionBase.h"

#include "CollisionManager.h"
#include <utility>
#include <vector>

/// @brief 更新
/// @param player 
/// @param modelHandle 
/// @return 
std::pair<bool, VECTOR> CollisionManager::Update(const std::vector<std::shared_ptr<BaseObject>>& fieldObjects,
	const VECTOR& playerPos,const VECTOR& moveVec, 
	const PositionData& positionData,
	const PlayerStateActionBase::PlayerData& playerData)
{
	VECTOR oldPos = playerPos;
	VECTOR newPos = VAdd(oldPos, moveVec);	
	VECTOR topPos = newPos;
	topPos.y += positionData.addTopPos;
	
	//壁衝突判定
	WallCollisionCheck(modelHandle, newPos, oldPos, positionData);

	HeadCollisionCheck(modelHandle, newPos, positionData);

	//床衝突判定
	return std::make_pair(GroundCollisionCheck(modelHandle, oldPos, newPos, positionData, playerData), newPos);
}

/// <summary>
/// 頭上の当たり判定
/// </summary>
/// <param name="modelHandle"></param>
/// <param name="newPos"></param>
/// <param name="positionData.addTopPos"></param>
/// <param name="positionData.radius"></param>
/// <param name="positionData.addBottomPos"></param>
/// <returns></returns>
bool CollisionManager::HeadCollisionCheck(int modelHandle, VECTOR& newPos, 
	const PositionData& positionData)
{
	VECTOR topPos = newPos;
	topPos.y += positionData.addTopPos;
	MV1_COLL_RESULT_POLY_DIM hitPoly_head;

	bool isHitHead = HitCheck::SphereHitJudge(modelHandle, -1, topPos, hitPoly_head);

	if (isHitHead)
	{
		VECTOR addPos = VGet(0.0f, 0.0f, 0.0f);

		for (int i = 0; i < hitPoly_head.HitNum; i++)
		{
			MV1_COLL_RESULT_POLY poly = hitPoly_head.Dim[i];
			VECTOR newAddPos = VGet(0.0f, 0.0f, 0.0f);

			////////////////////////////////////////
			// todo::
			// 法線ではなく角度でできるように
			/////////////////////////////////////////
			if (poly.Normal.y <= -0.7f || poly.Normal.y >= 0.7f)
			{
				hitPos_head = HitCheck::ClosestPtToPointTriangle(topPos, poly.Position[0], poly.Position[1], poly.Position[2]);

				VECTOR hitDirection = VSub(hitPos_head, topPos);
				hitDirection = VNorm(hitDirection);
				hitDirection = VScale(hitDirection, positionData.radius);

				VECTOR hitPos_sphere = VAdd(topPos, hitDirection);


				newAddPos.y = hitPos_head.y - hitPos_sphere.y;
			}

			if (addPos.y > newAddPos.y)
			{
				addPos = newAddPos;
			}
		}

		newPos.y = newPos.y + addPos.y;
	}

	return false;
}

/// <summary>
/// 床との衝突判定処理
/// </summary>
/// <param name="modelHandle"></param>
/// <param name="oldPos"></param>
/// <param name="newPos"></param>
/// <param name="positionData.addTopPos"></param>
/// <param name="positionData.addBottomPos"></param>
/// <param name="positionData.radius"></param>
/// <param name="isJump"></param>
/// <returns></returns>
bool CollisionManager::GroundCollisionCheck(int modelHandle,const VECTOR& oldPos,
	VECTOR& newPos, const PositionData& positionData,
	const PlayerStateActionBase::PlayerData& playerData)
{
	//playerの状態によって当たり判定の優先順位を決める
	//何回か当たり判定を繰り返す
	//prevとnewのposを作る
	VECTOR topPosition = newPos;
	VECTOR bottomPosition = newPos;
	VECTOR nowBottomPos = oldPos;
	VECTOR nowTopPos = oldPos;
	bool isHitSphere;

	topPosition.y = topPosition.y + 13.0f;
	nowTopPos.y = nowTopPos.y + 13.0f;

	//※ジャンプしている間ではなく落下中にしたい
	if (playerData.isFalling || playerData.isJump)
	{
		nowBottomPos.y = newPos.y;
	}
	else
	{
		nowBottomPos.y = newPos.y - 5.0f;
	}

	bool isHitGround = false;

	MV1_COLL_RESULT_POLY rayPoly_ground;

	//rayが当たっていれば
	isHitGround = HitCheck::HitRayJudge(modelHandle, -1, topPosition, nowBottomPos, rayPoly_ground);

	if (isHitGround)
	{
		VECTOR playerNormal = VSub(topPosition, bottomPosition);
		playerNormal = VNorm(playerNormal);

		//playerと床のなす角を求める
		float cosTheta = Calculation::Radian(playerNormal, rayPoly_ground.Normal);
		float radian = std::acos(cosTheta);
		tiltAngle_degree = radian * 180.0f / DX_PI_F;

		VECTOR newPlayerPos = VGet(0.0f, 0.0f, 0.0f);
		
		//床 - プレイヤーの足元で押し戻し量を計算
		newPlayerPos.y = rayPoly_ground.HitPosition.y - newPos.y;
		newPos.y = newPos.y + newPlayerPos.y;
	}

	//log用
	topPos_ray = topPosition;
	bottomPos_ray = nowBottomPos;

	//接地しているか
	return isHitGround;

}

/// <summary>
/// 床との衝突判定処理
/// </summary>
/// <param name="modelHandle"></param>
/// <param name="oldPos"></param>
/// <param name="newPos"></param>
/// <param name="positionData.addTopPos"></param>
/// <param name="positionData.addBottomPos"></param>
/// <param name="positionData.radius"></param>
/// <param name="isJump"></param>
/// <returns></returns>
std::pair<bool, VECTOR> CollisionManager::GroundCollisionCheck_Hang_to_Crouch(int modelHandle, 
	const VECTOR& oldPos, const VECTOR& newPos, const VECTOR& foot,
	const PositionData& positionData)
{

	//prevとnewのposを作る
	VECTOR newTopPosition = newPos;
	VECTOR newBottomPosition = newPos;
	VECTOR nowBottomPos = oldPos;
	VECTOR nowTopPos = oldPos;
	bool isHitSphere;

	newTopPosition.y = newTopPosition.y + positionData.addTopPos;
	nowTopPos.y = nowTopPos.y + positionData.addTopPos;
	newBottomPosition.y = newBottomPosition.y + positionData.addBottomPos;

	nowBottomPos.y = nowBottomPos.y + positionData.addBottomPos - positionData.radius;

	bool isHitGround = false;

	MV1_COLL_RESULT_POLY rayPoly_ground;
	VECTOR returnPos = newPos;

	//rayが当たっていれば
	isHitGround = HitCheck::HitRayJudge(modelHandle, -1, nowTopPos, nowBottomPos, rayPoly_ground);

	if (isHitGround)
	{
		VECTOR newPlayerPos = VGet(0.0f, 0.0f, 0.0f);
		//VECTOR footPos = VGet(0.0f, newBottomPosition.y - positionData.radius, 0.0f);

		//床 - プレイヤーの足元で押し戻し量を計算
		newPlayerPos.y = rayPoly_ground.HitPosition.y - foot.y;
		returnPos.y = returnPos.y + newPlayerPos.y;
	}

	//接地しているか
	return std::make_pair(isHitGround, returnPos);

}

/// <summary>
/// 壁との当たり判定
/// </summary>
/// <param name="player"></param>
/// <param name="modelHandle"></param>
/// <returns></returns>
bool CollisionManager::WallCollisionCheck(int modelHandle, VECTOR& newPos, 
	VECTOR& oldPos, const PositionData& positionData)
{

	VECTOR topPosition = newPos;
	VECTOR bottomPosition = newPos;
	VECTOR topPos_now = oldPos;
	topPosition.y = topPosition.y + positionData.addTopPos;
	bottomPosition.y = bottomPosition.y + positionData.addBottomPos;
	topPos_now.y += positionData.addTopPos;

	pos_new = topPosition;
	pos_now = topPos_now;

	bool flag = false;

	//壁と衝突しているか
	HitCheck::CapsuleHitWallJudge(modelHandle, -1, positionData.radius, topPosition,
		VAdd(bottomPosition, VGet(0.0f, 1.0f, 0.0f)), hitPoly_Wall);

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

			float degree_x = Calculation::radToDeg(poly.Normal.x);
			float degree_z = Calculation::radToDeg(poly.Normal.z);


			//壁かどうかを調べる
			if ((poly.Normal.x >= 0.7f || poly.Normal.z >= 0.7f ||
				poly.Normal.x <= -0.7f || poly.Normal.z <= -0.7f) &&
				poly.Normal.y <= 0.7f)
			{

				//カプセルの大きさ
 				topPosition = newPos;
				bottomPosition = newPos;
				topPosition.y = topPosition.y + positionData.addTopPos;
				bottomPosition.y = bottomPosition.y + positionData.addBottomPos;

				normal = poly.Normal;
				normal.y = 0.0f;
				normal = VNorm(normal);
			
				//線分のどこに当たったか
				auto result = HitCheck::SegmentTriangleDistance(topPosition,
					bottomPosition, poly.Position[0], poly.Position[1],
					poly.Position[2],poly.Normal);

				VECTOR addPos = VScale(poly.Normal, -3.5f);
				addPos.y = 0.0f;

				//当たったところから現在と次フレームの座標でraycast
				VECTOR pos_now = VAdd(result.first, addPos);
				MV1_COLL_RESULT_POLY poly_ray;

				HitCheck::HitRayJudge(modelHandle, -1, pos_now, 
					result.first, poly_ray);

				if (poly_ray.HitFlag)
				{
					//球の接触している座標を求める
						// そうするには？↓
						//法線方向とは逆の方向にセンターポジションから加算する
					VECTOR contact = VScale(poly.Normal, -3.5f);

					//面の接触点
					//hitPos_wall = poly_ray.HitPosition;
					hitPos_wall = result.second;

					//球の接触している座標
					//面に対して一番近い線分点から面に向かって球の半径を加算する
					contact = VAdd(result.first, contact);

					//面に少し膜をはる
					contact = VAdd(contact, VScale(poly.Normal, -0.1f));

					//球の接触座標→面の接触座標を求める
					VECTOR pos = VSub(result.second, contact);
					pos.y = 0.0f;

					newPos = VAdd(newPos, pos);
					flag = true;
				}

					
			}
		}
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
std::pair<bool, VECTOR> CollisionManager::CliffGrabbing(int modelHandle,
	const VECTOR& topPosition, const VECTOR& moveDirection, const bool isFalling)
{
	VECTOR linePos_end = VAdd(topPosition, VScale(moveDirection, 6.0f));
	linePos_end.y = topPosition.y - 5.0f;
	bool isHitHanging = false;

	//落下中にplayerの上部から出ているrayで判定を取る
	if (isFalling)
	{
		isHitHanging = HitCheck::HitRayJudge(modelHandle, -1, topPosition, linePos_end, HangingPoly);
		
		if (isHitHanging && HangingPoly.Normal.y >= 0.8f)
		{
			hitHangingPos = HangingPoly.HitPosition;
		}
		else
		{
			isHitHanging = false;
		}
	}

	return std::make_pair(isHitHanging, hitHangingPos);
	
	//trueの場合に崖をつかむようにする
}


/// <summary>
/// 球sが三角形ABCと交差している場合はtrueを返し、そうでなければfalseを返す
/// 球の中心に対するabc上の最近接点である点pも返す
/// </summary>
/// <returns></returns>
bool CollisionManager::TestSphereTriangle(VECTOR centerPos, VECTOR a, VECTOR b, VECTOR c,VECTOR& q, const float radius)
{
	//球の中心対する最近接点である三角形ABC上にある点pを見つける
	q = HitCheck::ClosestPtToPointTriangle(centerPos, a, b, c);

	//球と三角形が交差するのは、球の中心から点qまでの(平方した)距離が(平方した)球の半径よりも小さい場合
	VECTOR v = VSub(q, centerPos);

	return VDot(v, v) <= positionData.radius * positionData.radius;
}

VECTOR CollisionManager::PushBackCalculation_sphere_mesh(const MV1_COLL_RESULT_POLY& poly, const VECTOR& bottomPos, const VECTOR& newPlayerPos, const float& radius)
{
	//球と面の接触しているは球の最下部と面の接触座標と同じなのでリセット
	hitSphere = VGet(0.0f, 0.0f, 0.0f);

	////平面であればそのまま足元で計算
	//if (poly.Normal.y >= 1.0f)
	//{
	//	VECTOR footPos = VGet(0.0f, bottomPos.y - positionData.radius, 0.0f);

	//	newPlayerPos.y = hitPos_ground.y - footPos.y;

	//	//押し戻し量が一番大きいものを加算する
	//	if (addPos.y < newPlayerPos.y)
	//	{
	//		addPos = newPlayerPos;
	//	}
	//}
	return hitSphere;
}


VECTOR CollisionManager::CalcPushBack_SphereMeshOutsideTriangle(const MV1_COLL_RESULT_POLY& poly, const VECTOR& HitPos_ground, const VECTOR& bottomPos, const float& radius)
{

	nearestPoint = Calculation::SphereMeshOutsideTriangle(poly, HitPos_ground);

	//球と面の接触点を求める
	//方向計算
	hitSphere = VSub(nearestPoint, bottomPos);
	hitSphere = VNorm(hitSphere);

	//接触点の方向に半径を加算
	hitSphere = VScale(hitSphere, positionData.radius);

	//球の中心点から接触点の方向に半径分のベクトルを加算して球の接触点を計算
	hitSphere = VAdd(bottomPos, hitSphere);

	//面の接触座標と球の接触点で押し戻し量を計算
	VECTOR addPos = VGet(0.0f, 0.0f, 0.0f);
	addPos.y = nearestPoint.y - hitSphere.y;
	return addPos;
}

/// <summary>
/// 描画
/// </summary>
bool CollisionManager::Draw()
{
	//printfDx("NormalPos.y %f\n", subPos.y);
	//printfDx("NormalPos_Wall.x %f\n", normal.x);
	//printfDx("NormalPos_Wall.z %f\n", normal.z);
	//printfDx("hitPos_ground.x %f\n", hitPos_ground.x);
	//printfDx("hitPos_ground.y %f\n", hitPos_ground.y);
	//printfDx("hitPos_ground.z %f\n", hitPos_ground.z);

	printfDx("tiltAngle_degree %f\n", tiltAngle_degree);

	DrawSphere3D(hitPos_wall, 2.0f, 30, GetColor(0, 0, 0),
		GetColor(0, 255, 0), FALSE);
	DrawSphere3D(hitPos_ground, 2.0f, 30, GetColor(0, 0, 0),
		GetColor(255, 0, 0), FALSE);

	DrawSphere3D(hitPos_head, 2.0f, 30, GetColor(0, 0, 0),
		GetColor(0, 0, 255), FALSE);
	DrawSphere3D(hitHangingPos, 2.0f, 30, GetColor(0, 0, 0),
		GetColor(0, 255, 0), FALSE);

	DrawLine3D(topPos_ray, bottomPos_ray, GetColor(255, 0, 0));

	DrawLine3D(pos_now, pos_new, GetColor(0, 0, 255));
	return true;
}