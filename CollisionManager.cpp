#include "common.h"
#include <utility>
#include <vector>
#include "HitCheck.h"
#include "Calculation.h"
#include "PlayerStateActionBase.h"
#include "CollisionManager.h"
#include "BaseChara.h"
#include "DebugDrawer.h"

/// @brief 更新
/// @param player 
/// @param modelHandle 
/// @return 
void CollisionManager::Update(BaseChara& chara,
	const std::vector<std::shared_ptr<BaseObject>>& fieldObjects,
	const PlayerData& playerData)
{
	if (chara.GetIsCollisionCheck())
	{
		chara.SetCollision_result(Check_all(fieldObjects,
			chara.GetPosition(), chara.GetMoveVec(), chara.GetRadius(),
			chara.GetPositionData(),playerData));
	}

}

/// <summary>
/// 全衝突判定チェック
/// </summary>
/// <param name="fieldObjects"></param>
/// <param name="playerPos"></param>
/// <param name="moveVec"></param>
/// <param name="radius"></param>
/// <param name="positionData"></param>
/// <returns></returns>
CollisionResult CollisionManager::Check_all(const std::vector<std::shared_ptr<BaseObject>>& fieldObjects,
	const VECTOR& playerPos, const VECTOR& moveVec, const float& radius,
	const PositionData& positionData,const PlayerData& playerData)
{
	VECTOR oldPos = playerPos;
	VECTOR newPos = VAdd(oldPos, moveVec);
	VECTOR moveVec_new = VGet(0.0f, 0.0f, 0.0f);

	bool isCalc = rayPoly_ground_now.HitFlag;

	//床に沿うように移動する
	if (isCalc && VSize(moveVec) != 0 &&
		!playerData.isJump)
	{
		//移動量を計算
		float size = VSize(moveVec);
		
		//方向ベクトルを計算
		moveVec_new = Calculation::Projection(rayPoly_ground_now.Normal, moveVec);
		
		//移動量を再計算
		moveVec_new = VScale(moveVec_new, size);

		newPos = VAdd(oldPos, moveVec_new);

		projection_ray_start = newPos;
		projection_ray_end = VAdd(newPos, VScale(VNorm(moveVec_new),20.0f));
	}
	else
	{
		moveVec_new = moveVec;
	}
	DebugDrawer::Instance().InformationInput_line(projection_ray_start, projection_ray_end, GetColor(255, 0, 255));


	CollisionResult result;

	//壁衝突判定
	auto hitWall = WallCollisionCheck(fieldObjects, newPos, moveVec_new, positionData, radius);

	//頭上衝突判定
	HeadCollisionCheck(fieldObjects, newPos, moveVec_new, positionData, radius);

	//床衝突判定
	auto [isGround,tag] = GroundCollisionCheck(fieldObjects, oldPos, newPos, moveVec_new, positionData);

	//対象のオブジェクトの移動量を加算
	for (const auto& fieldObject : fieldObjects)
	{
		if (tag == fieldObject->GetTag())
		{
			newPos = VAdd(newPos, fieldObject->GetPos_difference());
		}
	}

	//床衝突判定
	return {
		newPos,
		isGround,
		hitWall.first,
		hitWall.second,
		tag 
	};
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
bool CollisionManager::HeadCollisionCheck(const std::vector<std::shared_ptr<BaseObject>>& fieldObjects,
	VECTOR& newPos, const VECTOR& moveVec, const PositionData& positionData, const float& radius)
{
	MV1_COLL_RESULT_POLY_DIM hitPoly_head;
	VECTOR position_top_new = VAdd(positionData.position_top_ray, moveVec);
	VECTOR position_bottom_new = VAdd(positionData.position_bottom_ray, moveVec);

	//fieldObjectの要素分確認
	for (auto& fieldObject : fieldObjects)
	{
		bool isHitHead = HitCheck::SphereHitJudge(fieldObject->GetModelHandle(), -1, radius, position_top_new, hitPoly_head);

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
					hitPos_head = HitCheck::ClosestPtToPointTriangle(position_top_new, poly.Position[0], poly.Position[1], poly.Position[2]);

					VECTOR hitDirection = VSub(hitPos_head, position_top_new);
					hitDirection = VNorm(hitDirection);
					hitDirection = VScale(hitDirection, radius);

					VECTOR hitPos_sphere = VAdd(position_top_new, hitDirection);


					newAddPos.y = hitPos_head.y - hitPos_sphere.y;
				}

				if (addPos.y > newAddPos.y)
				{
					addPos = newAddPos;
				}
			}

			newPos.y = newPos.y + addPos.y;
		}
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
std::pair<bool, std::string> CollisionManager::GroundCollisionCheck(const std::vector<std::shared_ptr<BaseObject>>& fieldObjects,
	const VECTOR& oldPos, VECTOR& newPos, const VECTOR& moveVec,
	const PositionData& positionData)
{
	bool isHitGround = false;
	bool returnFlag = false;
	std::string returnTag = "";

	VECTOR position_top_new = VAdd(positionData.position_top_ray, moveVec);
	VECTOR position_bottom_new = VAdd(positionData.position_bottom_ray, moveVec);

	topPos_ray = position_top_new;
	bottomPos_ray = position_bottom_new;
	MV1_COLL_RESULT_POLY rayPoly_ground;

	for (const auto& fieldObject : fieldObjects)
	{

		//rayが当たっていれば
		isHitGround = HitCheck::RayHitJudge(fieldObject->GetModelHandle(), -1, position_top_new,
			position_bottom_new, rayPoly_ground);

		if (isHitGround)
		{
			rayPoly_ground_now = rayPoly_ground;

			VECTOR playerNormal = VSub(position_top_new,
				position_bottom_new);

			playerNormal = VNorm(playerNormal);

			//playerと床のなす角を求める
			float cosTheta = VDot(playerNormal, rayPoly_ground.Normal) / (VSquareSize(playerNormal) * VSquareSize(rayPoly_ground.Normal));
			float radian = std::acos(cosTheta);
			tiltAngle_degree = radian * 180.0f / DX_PI_F;

			VECTOR newPlayerPos = VGet(0.0f, 0.0f, 0.0f);

			//床 - プレイヤーの足元で押し戻し量を計算
			newPlayerPos.y = rayPoly_ground.HitPosition.y - newPos.y;
			newPos.y = newPos.y + newPlayerPos.y;
			
			returnFlag = true;
			returnTag = fieldObject->GetTag();
		}

	}

	//どこにもあたっていない場合nullにする
	if (returnTag == "")
	{
		rayPoly_ground_now = { NULL };
	}

	//接地しているか
	return std::make_pair(returnFlag, returnTag);

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
std::pair<bool, VECTOR> CollisionManager::GroundCollisionCheck_Hang_to_Crouch(const std::vector<std::shared_ptr<BaseObject>>& fieldObjects,
	const VECTOR& oldPos, const VECTOR& newPos, const VECTOR& foot,
	const PositionData& positionData)
{

	//prevとnewのposを作る
	VECTOR newTopPosition = newPos;
	VECTOR newBottomPosition = newPos;
	VECTOR nowBottomPos = oldPos;
	VECTOR nowTopPos = oldPos;
	bool isHitSphere;

	bool isHitGround = false;

	MV1_COLL_RESULT_POLY rayPoly_ground;
	VECTOR returnPos = newPos;

	for (const auto& fieldObject : fieldObjects)
	{
		//rayが当たっていれば
		isHitGround = HitCheck::RayHitJudge(fieldObject->GetModelHandle(), -1, nowTopPos, nowBottomPos, rayPoly_ground);

		if (isHitGround)
		{
			VECTOR newPlayerPos = VGet(0.0f, 0.0f, 0.0f);

			//床 - プレイヤーの足元で押し戻し量を計算
			newPlayerPos.y = rayPoly_ground.HitPosition.y - foot.y;
			returnPos.y = returnPos.y + newPlayerPos.y;
		}
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
std::pair<bool, VECTOR> CollisionManager::WallCollisionCheck(const std::vector<std::shared_ptr<BaseObject>>& fieldObjects,
	VECTOR& newPos, const VECTOR& moveVec, const PositionData& positionData,
	const float& radius)
{

	VECTOR position_top_new = VAdd(positionData.position_top_Capsule, moveVec);
	VECTOR position_bottom_new = VAdd(positionData.position_bottom_Capsule, moveVec);
	bool flag = false;
	VECTOR hitPoly_normal = { 0.0f };

	for (auto& fieldObject : fieldObjects)
	{
		//壁と衝突しているか
		HitCheck::CapsuleHitWallJudge(fieldObject->GetModelHandle(), -1, radius, position_top_new,
			VAdd(position_bottom_new, VGet(0.0f, 1.0f, 0.0f)), hitPoly_Wall);

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
					position_top_new = VGet(newPos.x, position_top_new.y, newPos.z);
					position_bottom_new = VGet(newPos.x, position_bottom_new.y, newPos.z);

					normal = poly.Normal;
					normal.y = 0.0f;
					normal = VNorm(normal);

					//線分のどこに当たったか
					auto result = HitCheck::SegmentTriangleDistance(position_top_new,
						position_bottom_new, poly.Position[0], poly.Position[1],
						poly.Position[2], poly.Normal);

					VECTOR addPos = VScale(poly.Normal, -3.5f);
					addPos.y = 0.0f;

					//当たったところから現在と次フレームの座標でraycast
					VECTOR pos_now = VAdd(result.first, addPos);
					MV1_COLL_RESULT_POLY poly_ray;

					HitCheck::RayHitJudge(fieldObject->GetModelHandle(), -1, pos_now,
						result.first, poly_ray);

					//rayが当たっている場合当たった座標で衝突判定
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

						//球の接触座標→面の接触座標を求める
						VECTOR pos = VSub(result.second, contact);
						pos.y = 0.0f;

						newPos = VAdd(newPos, pos);
						flag = true;
						hitPoly_normal = poly_ray.Normal;
					}
				}
			}
		}
	}

	// 検出したプレイヤーの周囲のポリゴン情報を開放する
	MV1CollResultPolyDimTerminate(hitPoly_Wall);
	return std::make_pair(flag, hitPoly_normal);

}

/// <summary>
/// 崖つかみ判定
/// </summary>
/// <param name="player"></param>
/// <param name="modelHandle"></param>
std::pair<bool, VECTOR> CollisionManager::CliffGrabbing(const std::vector<std::shared_ptr<BaseObject>>& fieldObjects,
	const VECTOR& topPosition, const VECTOR& moveDirection, const bool isFalling)
{
	VECTOR linePos_end = VAdd(topPosition, VScale(moveDirection, 6.0f));
	linePos_end.y = topPosition.y - 7.0f;

	//log用
	ray_start_hanging_log = topPosition;
	ray_end_hanging_log = linePos_end;

	bool isHitHanging = false;
	bool returnFlag = false;

	//落下中にplayerの上部から出ているrayで判定を取る
	if (isFalling)
	{
		for (const auto& fieldObject : fieldObjects)
		{
			MV1_COLL_RESULT_POLY poly;
			isHitHanging = HitCheck::RayHitJudge(fieldObject->GetModelHandle(), -1, topPosition, linePos_end, poly);
		
			if (isHitHanging && poly.Normal.y >= 0.8f)
			{
				hitHangingPos = poly.HitPosition;
				returnFlag = isHitHanging;
				HangingPoly = poly;
			}
			else
			{
				isHitHanging = false;
			}
		}
	}

	return std::make_pair(returnFlag, hitHangingPos);
	
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

	return VDot(v, v) <= radius * radius;
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
	hitSphere = VScale(hitSphere, radius);

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

	/*DrawSphere3D(projection_ray_start, 2.0f, 30, GetColor(0, 0, 0),
		GetColor(0, 255, 0), FALSE);

	DrawSphere3D(projection_ray_end, 2.0f, 30, GetColor(0, 0, 0),
		GetColor(0, 255, 0), FALSE);*/

	DrawLine3D(topPos_ray, bottomPos_ray, GetColor(255, 0, 0));
	DrawLine3D(ray_start_hanging_log, ray_end_hanging_log, GetColor(0, 255, 0));

	DrawLine3D(pos_now, pos_new, GetColor(0, 0, 255));
	DrawLine3D(projection_ray_start, projection_ray_end, GetColor(255, 0, 255));
	return true;
}