#include "common.h"
#include <utility>
#include <vector>
#include "HitCheck.h"
#include "Calculation.h"
#include "PlayerStateBase.h"
#include "CollisionManager.h"
#include "BaseChara.h"
#include "DebugDrawer.h"
#include "WorldSubSystem.h"

/// @brief 更新
/// @param player 
/// @param modelHandle 
/// @return 
void CollisionManager::Update(
	BaseChara& chara,
	const PlayerData& playerData)
{
	if (chara.GetIsCollisionCheck())
	{
		chara.SetCollision_result(
			Check_all(WorldSubSystem::Instance().GetSubSystem<CollisionObjectManager>()->GetCollisionObjects(),
				chara.GetPosition(),
				chara.GetMoveVec(),
				chara.GetRadius(),
			chara.GetPositionData(),playerData));
	}

}

/// <summary>
/// 全衝突判定チェック
/// </summary>
/// <param name="collisionObjects"></param>
/// <param name="playerPos"></param>
/// <param name="moveVec"></param>
/// <param name="radius"></param>
/// <param name="positionData"></param>
/// <returns></returns>
CollisionResult CollisionManager::Check_all(
	const std::vector<std::weak_ptr<BaseObject>>& collisionObjects,
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
	}
	else
	{
		moveVec_new = moveVec;
	}
	VECTOR projection_ray_start = newPos;
	VECTOR projection_ray_end = VAdd(newPos, VScale(VNorm(moveVec_new), 20.0f));

	DebugDrawer::Instance().InformationInput_line(projection_ray_start, projection_ray_end, GetColor(255, 0, 255));

	CollisionResult result;

	//壁衝突判定
	auto hitWall = WallCollisionCheck(collisionObjects, newPos, moveVec_new, positionData, radius);

	//頭上衝突判定
	HeadCollisionCheck(collisionObjects, newPos, moveVec_new, positionData, 2.0f);

	//床衝突判定
	auto [isGround,tag] = GroundCollisionCheck(collisionObjects, oldPos, newPos, moveVec_new, positionData);

	//対象のオブジェクトの移動量を加算
	for (const auto& fieldObject : collisionObjects)
	{
		auto collisionObject = fieldObject.lock();

		if (tag == collisionObject->GetTag())
		{
			newPos = VAdd(newPos, collisionObject->GetPos_difference());
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
bool CollisionManager::HeadCollisionCheck(const std::vector<std::weak_ptr<BaseObject>>& collisionObjects,
	VECTOR& newPos, const VECTOR& moveVec, const PositionData& positionData, const float& radius)
{
	MV1_COLL_RESULT_POLY_DIM hitPoly_head;
	VECTOR position_top_new = VAdd(positionData.position_top_ray, moveVec);
	VECTOR position_bottom_new = VAdd(positionData.position_bottom_ray, moveVec);

	//fieldObjectの要素分確認
	for (auto& fieldObject : collisionObjects)
	{
		auto collisionObject = fieldObject.lock();

		bool isHitHead = HitCheck::SphereHitJudge(collisionObject->GetModelHandle(),
			-1,
			radius,
			position_top_new,
			hitPoly_head);

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
std::pair<bool, std::string> CollisionManager::GroundCollisionCheck(const std::vector<std::weak_ptr<BaseObject>>& collisionObjects,
	const VECTOR& oldPos, VECTOR& newPos, const VECTOR& moveVec,
	const PositionData& positionData)
{
	bool isHitGround = false;
	bool returnFlag = false;
	std::string returnTag = "";

	VECTOR position_top_new = VAdd(positionData.position_top_ray, moveVec);
	VECTOR position_bottom_new = VAdd(positionData.position_bottom_ray, moveVec);

	MV1_COLL_RESULT_POLY rayPoly_ground;

	for (const auto& fieldObject : collisionObjects)
	{
		auto collisionObject = fieldObject.lock();

		//rayが当たっていれば
		isHitGround = HitCheck::RayHitJudge(collisionObject->GetModelHandle(), -1, position_top_new,
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
			returnTag = collisionObject->GetTag();
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
std::pair<bool, VECTOR> CollisionManager::GroundCollisionCheck_Hang_to_Crouch(const std::vector<std::weak_ptr<BaseObject>>& collisionObjects,
	const VECTOR& oldPos, const VECTOR& newPos, const VECTOR& foot,
	const PositionData& positionData)
{

	//prevとnewのposを作る
	VECTOR newTopPosition = newPos;
	VECTOR newBottomPosition = newPos;
	VECTOR nowBottomPos = oldPos;
	VECTOR nowTopPos = oldPos;

	bool isHitGround = false;

	MV1_COLL_RESULT_POLY rayPoly_ground;
	VECTOR returnPos = newPos;

	for (const auto& fieldObject : collisionObjects)
	{
		auto collisionObject = fieldObject.lock();

		//rayが当たっていれば
		isHitGround = HitCheck::RayHitJudge(collisionObject->GetModelHandle(), -1, nowTopPos, nowBottomPos, rayPoly_ground);

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
std::pair<bool, VECTOR> CollisionManager::WallCollisionCheck(const std::vector<std::weak_ptr<BaseObject>>& collisionObjects,
	VECTOR& newPos, const VECTOR& moveVec, const PositionData& positionData,
	const float& radius)
{
	VECTOR position_top_new = VAdd(positionData.position_top_Capsule, moveVec);
	VECTOR position_bottom_new = VAdd(positionData.position_bottom_Capsule, moveVec);
	VECTOR capsule_axis_top = VAdd(positionData.position_top_ray, moveVec);					//カプセルの軸
	VECTOR capsule_axis_bottom = VAdd(positionData.position_bottom_ray, moveVec);			//カプセルの軸

	bool flag = false;
	VECTOR hitPoly_normal = { 0.0f };

	for (auto& fieldObject : collisionObjects)
	{
		auto collisionObject = fieldObject.lock();

		//壁と衝突しているか
		HitCheck::CapsuleHitWallJudge(collisionObject->GetModelHandle(),
			-1, 
			radius, 
			position_top_new,
			VAdd(position_bottom_new, VGet(0.0f, 1.0f, 0.0f)), hitPoly_Wall);

		//衝突しているとこを全部調べて押し戻し量を計算する
		if (hitPoly_Wall.HitNum >= 1)
		{
			float maxY = -FLT_MAX;
			int groundIndex = -1;

			//ヒットした全ポリゴンを調べる
			for (int i = 0; i < hitPoly_Wall.HitNum; i++)
			{
				MV1_COLL_RESULT_POLY poly = hitPoly_Wall.Dim[i];

				float degree_x = Calculation::radToDeg(poly.Normal.x);
				float degree_z = Calculation::radToDeg(poly.Normal.z);

				capsule_axis_top = VGet(newPos.x, capsule_axis_top.y, newPos.z);
				capsule_axis_bottom = VGet(newPos.x, capsule_axis_bottom.y, newPos.z);

				//壁かどうかを調べる
				if ((poly.Normal.x >= 0.7f || poly.Normal.z >= 0.7f ||
					poly.Normal.x <= -0.7f || poly.Normal.z <= -0.7f) &&
					poly.Normal.y <= 0.7f)
				{
					capsule_axis_top = VGet(newPos.x,
						capsule_axis_top.y,
						newPos.z);

					capsule_axis_bottom = VGet(newPos.x,
						capsule_axis_bottom.y,
						newPos.z);

					//面の接触点と
					// プレイヤーのy軸の線分の
					// 面に対して一番近い点を調べる
					auto result = HitCheck::SegmentTriangleDistance(
						capsule_axis_top,
						capsule_axis_bottom,
						poly.Position[0],
						poly.Position[1],
						poly.Position[2],
						poly.Normal);

					VECTOR lineSegmentPointClosestSurface = result.first;		//面と一番近い線分点
					VECTOR hittingPointSurface = result.second;					//面との接触点

					DebugDrawer::Instance().InformationInput_sphere(lineSegmentPointClosestSurface, 2.0f, GetColor(255, 255, 255));
					DebugDrawer::Instance().InformationInput_sphere(hittingPointSurface, 2.0f, GetColor(0, 0, 0));

					//カプセルの半径分
					VECTOR addPos = VScale(poly.Normal, -3.5f);
					addPos.y = 0.0f;

					//面と一番近い線分の点からカプセルの半径分、
					//壁に向かって線を伸ばす
					VECTOR newCapsulePos = VAdd(lineSegmentPointClosestSurface, addPos);
					VECTOR oldCapsulePos = positionData.oldPosition;
					oldCapsulePos.y = lineSegmentPointClosestSurface.y;
					MV1_COLL_RESULT_POLY poly_ray;

					//カプセルの外側から軸にむかってrayCastする
					HitCheck::RayHitJudge(collisionObject->GetModelHandle(),
						-1, 
						oldCapsulePos,
						newCapsulePos,
						poly_ray);

					//rayが当たっている場合当たった座標で衝突判定
					if (poly_ray.HitFlag)
					{
						//rayの衝突座標と面に対して一番近い線分点で
						// 押し戻し量を求める
						VECTOR velocity = VSub(poly_ray.HitPosition, newCapsulePos);
						velocity.y = 0.0f;
						hittingPointSurface.y = 0.0f;

						newPos = VAdd(newPos, velocity);

						//-----------------------------------------//
						// 以下、ウォールラン用
						//-----------------------------------------//

						//衝突している壁の法線ベクトルを求める
						hitPoly_normal = poly_ray.Normal;

						//接触している三角形の縦の幅がplayerのY軸の幅より高ければtrueにする
						float vertical_length_triangle = Calculation::Triangle_by_verticalLength(poly.Position[0],
							poly.Position[1],
							poly.Position[2]);
							
						float vertical_length_player = Calculation::GetVerticalLength(position_top_new, position_bottom_new);

						if (vertical_length_player <= vertical_length_triangle)
						{
							flag = true;
						}

					}
				}
			}
		}

		// 検出したプレイヤーの周囲のポリゴン情報を開放する
		MV1CollResultPolyDimTerminate(hitPoly_Wall);
	}

	return std::make_pair(flag, hitPoly_normal);

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

	/*printfDx("tiltAngle_degree %f\n", tiltAngle_degree);*/

	/*DrawSphere3D(hitPos_ground, 2.0f, 30, GetColor(0, 0, 0),
		GetColor(255, 0, 0), FALSE);

	DrawSphere3D(hitPos_head, 2.0f, 30, GetColor(0, 0, 0),
		GetColor(0, 0, 255), FALSE);
	DrawSphere3D(hitHangingPos, 2.0f, 30, GetColor(0, 0, 0),
		GetColor(0, 255, 0), FALSE);*/

	/*DrawSphere3D(projection_ray_start, 2.0f, 30, GetColor(0, 0, 0),
		GetColor(0, 255, 0), FALSE);

	DrawSphere3D(projection_ray_end, 2.0f, 30, GetColor(0, 0, 0),
		GetColor(0, 255, 0), FALSE);*/

	/*DrawLine3D(topPos_ray, bottomPos_ray, GetColor(255, 0, 0));
	DrawLine3D(ray_start_hanging_log, ray_end_hanging_log, GetColor(0, 255, 0));

	DrawLine3D(pos_now, pos_new, GetColor(0, 0, 255));
	DrawLine3D(projection_ray_start, projection_ray_end, GetColor(255, 0, 255));*/
	return true;
}