#include "Common.h"
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
		chara.SetResultCollision(
			AllCheck(WorldSubSystem::GetInstance().GetSubSystem<CollisionObjectManager>()->GetCollisionObjects(),
				chara.GetPosition(),
				chara.GetVelocity(),
				chara.GetRadius(),
				chara.GetPositionData(),
				chara.GetCollisionHitPart()));
	}
}

/// <summary>
/// 全衝突判定チェック
/// </summary>
/// <param name="wpCollisionObjects"></param>
/// <param name="playerPos"></param>
/// <param name="charaVelocity"></param>
/// <param name="charaRadius"></param>
/// <param name="charaPositionData"></param>
/// <returns></returns>
CollisionResult CollisionManager::AllCheck(
	const std::vector<std::weak_ptr<BaseObject>>& wpCollisionObjects,
	const VECTOR& playerPos,
	const VECTOR& charaVelocity,
	const float charaRadius,
	const PositionData& charaPositionData,
	const CollisionHitPart& collisionHitPart)
{
	const float kHeadRadius = 2.0f;

	VECTOR oldPosition = playerPos;
	VECTOR newPosition = VAdd(oldPosition, charaVelocity);

	bool isCalc = nowGroundRayPoly.HitFlag;

	CollisionResult collisionResult;
	collisionResult.newPosition = newPosition;

	//----------------------------------//
	// 優先順に衝突判定を行う
	//----------------------------------//

	if (collisionHitPart.isHitWall)
	{
		//壁衝突判定
		collisionResult.newPosition = WallCollisionCheck(wpCollisionObjects,
			collisionResult.newPosition,
			charaVelocity,
			charaPositionData, 
			charaRadius);
	}

	if (collisionHitPart.isHitHead)
	{
		//頭上衝突判定
		collisionResult.newPosition = HeadCollisionCheck(wpCollisionObjects,
			collisionResult.newPosition,
			charaVelocity,
			charaPositionData,
			kHeadRadius);
	}

	if (collisionHitPart.isHitFloor)
	{
		//床衝突判定
		collisionResult = GroundCollisionCheck(
			wpCollisionObjects,
			oldPosition,
			collisionResult.newPosition,
			charaVelocity,
			charaPositionData);
	}

	return collisionResult;
}

/// <summary>
/// 頭上の当たり判定
/// </summary>
/// <param name="wpCollisionObjects"></param>
/// <param name="charaPosition"></param>
/// <param name="charaVelocity"></param>
/// <param name="charaPositionData"></param>
/// <param name="charaRadius"></param>
/// <returns></returns>
VECTOR CollisionManager::HeadCollisionCheck(
	const std::vector<std::weak_ptr<BaseObject>>& wpCollisionObjects,
	const VECTOR& charaPosition,
	const VECTOR& charaVelocity, 
	const PositionData& charaPositionData,
	const float& charaRadius)
{
	MV1_COLL_RESULT_POLY_DIM hitHeadPoly;
	VECTOR newTopPosition = VAdd(charaPositionData.capsuleTopPosition, charaVelocity);
	VECTOR newPosition = charaPosition;

	//fieldObjectの要素分確認
	for (auto& wpCollisionObject : wpCollisionObjects)
	{
		auto collisionObject = wpCollisionObject.lock();

		if (HitCheck::SphereHitJudge(
			collisionObject->GetModelHandle(),
			kFrameIndex,
			charaRadius,
			newTopPosition,
			hitHeadPoly))
		{
			VECTOR addPos = VGet(0.0f, 0.0f, 0.0f);
			VECTOR hitHeadPosition;

			for (int i = 0; i < hitHeadPoly.HitNum; i++)
			{
				MV1_COLL_RESULT_POLY subjectPoly = hitHeadPoly.Dim[i];
				VECTOR newAddPos = VGet(0.0f, 0.0f, 0.0f);

				//三角形ポリゴンの法線と上方向ベクトルとの
				// なす角を求める
				float headTiltAngleDegree = Calculation::AngleBetWeenTwoVectors(
					kLengthDirection,
					subjectPoly.Normal);

				if (headTiltAngleDegree <= kAngleRange)
				{
					//面との接触座標を算出
					hitHeadPosition = HitCheck::ClosestPtToPointTriangle(
						newTopPosition,
						subjectPoly.Position[0],
						subjectPoly.Position[1], 
						subjectPoly.Position[2]);

					//球の中心から三角形の接触座標までの方向
					VECTOR hitDirection = VSub(hitHeadPosition, newTopPosition);
					hitDirection = VNorm(hitDirection);
					hitDirection = VScale(hitDirection, charaRadius);

					//接触座標までの方向に球の中心から半径分を加算して
					//球の表面の座標を求める
					VECTOR hitSpherePosition = VAdd(newTopPosition, hitDirection);

					//押し戻し量を計算
					newAddPos.y = hitHeadPosition.y - hitSpherePosition.y;
				}

				//負の値の絶対値が大きければplayerに
				// 加算するvectorを更新
				if (addPos.y > newAddPos.y)
				{
					addPos = newAddPos;
				}
			}

			newPosition.y += addPos.y;
		}

		// 検出したプレイヤーの周囲のポリゴン情報を開放する
		MV1CollResultPolyDimTerminate(hitHeadPoly);
	}

	return newPosition;
}

/// <summary>
/// 床との衝突判定処理
/// </summary>
/// <param name="modelHandle"></param>
/// <param name="oldPosition"></param>
/// <param name="newPosition"></param>
/// <param name="charaPositionData.addTopPos"></param>
/// <param name="charaPositionData.addBottomPos"></param>
/// <param name="charaPositionData.charaRadius"></param>
/// <param name="isJump"></param>
/// <returns></returns>
CollisionResult CollisionManager::GroundCollisionCheck(
	const std::vector<std::weak_ptr<BaseObject>>& wpCollisionObjects,
	const VECTOR& oldPosition, 
	const VECTOR& subjectPosition, 
	const VECTOR& charaVelocity,
	const PositionData& charaPositionData)
{
	bool isHitGround = false;
	CollisionResult collisionResult;
	collisionResult.newPosition = subjectPosition;

	VECTOR newTopPosition = VAdd(charaPositionData.centerPosition, charaVelocity);
	VECTOR newBottomPosition = VAdd(charaPositionData.rayBottomPosition, charaVelocity);

	MV1_COLL_RESULT_POLY groundRayPoly;

	for (const auto& wpCollisionObject : wpCollisionObjects)
	{
		auto collisionObject = wpCollisionObject.lock();

		//rayが当たっていれば
		isHitGround = HitCheck::RayHitJudge(
			collisionObject->GetModelHandle(), 
			kFrameIndex,
			newTopPosition,
			newBottomPosition, 
			groundRayPoly);

		if (isHitGround)
		{
			//三角形ポリゴンの法線と上方向ベクトルとの
			// なす角を求める
			tiltAngleDegree = Calculation::AngleBetWeenTwoVectors(
				kLengthDirection,
				groundRayPoly.Normal);

			tiltAngleDegree = abs(tiltAngleDegree);
			
			//kAngleRangeより角度が下回っていれば床とみなす
			if (tiltAngleDegree <= kAngleRange)
			{
				//三角形データを保存
				collisionResult.nowGroundRayPoly = groundRayPoly;

				VECTOR addNewPlayerPosition = VGet(0.0f, 0.0f, 0.0f);

				//床 - プレイヤーの足元で押し戻し量を計算
				addNewPlayerPosition.y = 
					groundRayPoly.HitPosition.y - 
					collisionResult.newPosition.y;

				collisionResult.newPosition.y += addNewPlayerPosition.y;

				//接触しているオブジェクトのtagを渡す
				collisionResult.objectTag = collisionObject->GetTag();
			}
		}
	}

	//どこにもあたっていない場合nullにする
	if (collisionResult.objectTag == "")
	{
		nowGroundRayPoly = { NULL };
		return collisionResult;
	}

	//接触していれば
	//対象のオブジェクトの移動量を加算
	for (const auto& wpCollisionObject : wpCollisionObjects)
	{
		auto collisionObject = wpCollisionObject.lock();

		if (collisionResult.objectTag == collisionObject->GetTag())
		{
			collisionResult.newPosition = VAdd(
				collisionResult.newPosition,
				collisionObject->GetDifferencePosition());

			break;
		}
	}

	return collisionResult;
}

/// <summary>
/// 壁との当たり判定
/// </summary>
/// <param name="wpCollisionObjects"></param>
/// <param name="charaPosition"></param>
/// <param name="charaVelocity"></param>
/// <param name="charaPositionData"></param>
/// <param name="charaRadius"></param>
/// <returns></returns>
VECTOR CollisionManager::WallCollisionCheck(
	const std::vector<std::weak_ptr<BaseObject>>& wpCollisionObjects,
	const VECTOR& charaPosition,
	const VECTOR& charaVelocity,
	const PositionData& charaPositionData,
	const float& charaRadius)
{
	const float kReviseBottomPosY = 1.0f;

	VECTOR newTopPosition	 = VAdd(charaPositionData.capsuleTopPosition, charaVelocity);
	VECTOR newBottomPosition = VAdd(charaPositionData.capsuleBottomPosition, charaVelocity);

	//ちょっとした段差を壁として扱わないように座標を調整
	newBottomPosition.y += kReviseBottomPosY;
	
	//カプセルの軸
	VECTOR topCapsuleAxis = VAdd(charaPositionData.rayTopPosition, charaVelocity);
	VECTOR bottomCapsuleAxis = VAdd(charaPositionData.rayBottomPosition, charaVelocity);

	VECTOR newPosition = charaPosition;

	MV1_COLL_RESULT_POLY_DIM hitWallPoly;

	for (auto& wpCollisionObject : wpCollisionObjects)
	{
		auto collisionObject = wpCollisionObject.lock();

		//壁と衝突しているか
		if (HitCheck::CapsuleHitJudge(
			collisionObject->GetModelHandle(),
			kFrameIndex,
			charaRadius,
			newTopPosition,
			newBottomPosition,
			hitWallPoly))
		{
			//衝突しているとこを全部調べて押し戻し量を計算する
			//ヒットした全ポリゴンを調べる
			for (int i = 0; i < hitWallPoly.HitNum; i++)
			{
				MV1_COLL_RESULT_POLY subjectPoly = hitWallPoly.Dim[i];
				
				//三角形ポリゴンの法線と上方向ベクトルとの
				// なす角を求める
				float wallDegree = Calculation::AngleBetWeenTwoVectors(
					kLengthDirection, 
					subjectPoly.Normal);

				// kAngleRangeよりもなす角が大きければ壁として扱う
				if (wallDegree >= kAngleRange)
				{
					//一個前にnewPositionが移動している可能性があるので
					// カプセルの軸を修正
					topCapsuleAxis = VGet(newPosition.x,
						topCapsuleAxis.y,
						newPosition.z);

					bottomCapsuleAxis = VGet(newPosition.x,
						bottomCapsuleAxis.y,
						newPosition.z);

					//面の接触点と
					// プレイヤーカプセルの軸の
					// 面に対して一番近い点を調べる
					auto result = HitCheck::SegmentTriangleDistance(
						topCapsuleAxis,
						bottomCapsuleAxis,
						subjectPoly.Position[0],
						subjectPoly.Position[1],
						subjectPoly.Position[2],
						subjectPoly.Normal);

					//面に最も近い線分点
					VECTOR closestLinePointToFace = result.first;

					//キャラのカプセルの半径分
					VECTOR addPos = VScale(subjectPoly.Normal, -charaRadius);
					addPos.y = 0.0f;

					//面に最も近い線分点を用いて
					// 前フレームの位置から現在の位置のカプセルのキャラの
					// カプセルの半径分、壁に向かって線を伸ばす
					VECTOR newCapsulePos = VAdd(closestLinePointToFace, addPos);
					VECTOR oldCapsulePos = charaPositionData.oldPosition;
					oldCapsulePos.y = closestLinePointToFace.y;
					MV1_COLL_RESULT_POLY rayPoly;

					//カプセルの外側から軸にむかってrayCastする
					HitCheck::RayHitJudge(collisionObject->GetModelHandle(),
						kFrameIndex,
						oldCapsulePos,
						newCapsulePos,
						rayPoly);

					//rayが当たっている場合当たった座標で衝突判定
					if (rayPoly.HitFlag)
					{
						//rayの衝突座標と面に対して一番近い線分点で
						// 押し戻し量を求める
						VECTOR velocity = VSub(rayPoly.HitPosition, newCapsulePos);
						velocity.y = 0.0f;

						newPosition = VAdd(newPosition, velocity);
					}
				}
			}
		}
		// 検出したプレイヤーの周囲のポリゴン情報を開放する
		MV1CollResultPolyDimTerminate(hitWallPoly);
	}

	return newPosition;
}

/// <summary>
/// 壁を床に見立てて
/// 床との当たり判定をする
/// </summary>
/// <param name="wpCollisionObjects"></param>
/// <param name="oldPosition"></param>
/// <param name="newPosition"></param>
/// <param name="charaVelocity"></param>
/// <param name="charaPositionData"></param>
/// <returns></returns>
VECTOR CollisionManager::WallGroundCollisionCheck(
	const std::vector<std::weak_ptr<BaseObject>>& wpCollisionObjects,
	const VECTOR& oldPosition,
	const VECTOR& newPosition,
	const VECTOR& charaVelocity,
	const VECTOR& gravityDirection,
	const float charaRadius,
	const PositionData& charaPositionData)
{
	const int kFrameIndex			= -1;
	const float kReverseScale		= -1.0f;		//方向ベクトル反転用
	const float kExtendRayScale		= 15.0f;		//rayの大きさ
	const VECTOR gravityForWallRun	= VScale(gravityDirection, kReverseScale);
	
	bool returnFlag			= false;
	bool isHitGround		= false;
	std::string returnTag	= "";

	//ray開始を少しずらさないと壁に埋まって反応しないためずらす
	VECTOR rayStartPosition = VGet(
		charaPositionData.centerPosition.x,
		charaPositionData.capsuleBottomPosition.y,
		charaPositionData.centerPosition.z);

	VECTOR rayEndPosition = VAdd(rayStartPosition, VScale(gravityForWallRun, kExtendRayScale));

	VECTOR returnNewPos			 = newPosition;
	VECTOR HitWallPlayerPosition = VAdd(newPosition, VScale(gravityForWallRun, charaRadius));

	MV1_COLL_RESULT_POLY groundRayPoly;

	for (const auto& fieldObject : wpCollisionObjects)
	{
		auto collisionObject = fieldObject.lock();

		//rayが当たっていれば
		isHitGround = HitCheck::RayHitJudge(
			collisionObject->GetModelHandle(),
			kFrameIndex,
			rayStartPosition,
			rayEndPosition,
			groundRayPoly);

		if (isHitGround)
		{
			nowGroundRayPoly = groundRayPoly;

			VECTOR newPlayerPos = VGet(0.0f, 0.0f, 0.0f);

			//横の座標だけ壁に沿って押し戻す
			newPlayerPos	= VSub(groundRayPoly.HitPosition, HitWallPlayerPosition);
			newPlayerPos.y	= 0.0f;
			returnNewPos	= VAdd(returnNewPos, newPlayerPos);
			returnFlag		= true;
			returnTag		= collisionObject->GetTag();
		}
	}

	//どこにもあたっていない場合nullにする
	if (returnTag == "")
	{
		nowGroundRayPoly = { NULL };
	}

	//接地しているか
	return returnNewPos;
}