#pragma once

enum animNum : int
{
	braced_Hang_To_Crouch,		//しゃがんでぶらさがる
	falling_Idle,				//落ちているとき
	falling_To_Roll,			//着地して転がる
	hanging_Idle,				//ぶらさがる
	hard_Landing,				//着地する
	idle,						//静止時
	idle_To_Sprint,				//走り出し
	jump,						//ジャンプ
	jump_Over,					//ロールジャンプ
	quick_Roll,					//転がる
	run,						//走る
	run_Jump,					//走りながらジャンプ
	run_To_Stop,				//止まる
	running_Forward_Flip		//走りながら回転ジャンプ
};

////上入力されたとき
 //if (PadInput::isUp())
 //{
 //    if (animNumber_Now != animNum::run && !playerData.isJump)
 //    {
 //       // ChangeMotion(animNum::run, PlayAnimSpeed);
 //    }
 //    moveVec = VAdd(moveVec, upMove);
 //    playerData.isMove = true;
 //    playerData.isStopRun = true;
 //}

 ////下入力されたとき
 //if (PadInput::isDown())
 //{
 //    if (animNumber_Now != animNum::run && !playerData.isJump)
 //    {
 //       // ChangeMotion(animNum::run, PlayAnimSpeed);
 //    }
 //    moveVec = VAdd(moveVec, VScale(upMove, -1.0f));
 //    playerData.isMove = true;
 //    playerData.isStopRun = true;
 //}

 ////左入力されたとき
 //if (PadInput::isLeft())
 //{
 //    if (animNumber_Now != animNum::run && !playerData.isJump)
 //    {
 //       // ChangeMotion(animNum::run, PlayAnimSpeed);
 //    }
 //    moveVec = VAdd(moveVec, rightMove);
 //    playerData.isMove = true;
 //    playerData.isStopRun = true;
 //}

 ////右入力されたとき
 //if (PadInput::isRight())
 //{
 //    if (animNumber_Now != animNum::run && !playerData.isJump)
 //    {
 //        //ChangeMotion(animNum::run, PlayAnimSpeed);
 //    }
 //    moveVec = VAdd(moveVec, VScale(rightMove, -1.0f));
 //    playerData.isMove = true;
 //    playerData.isStopRun = true;
 //}

/*   moveVec = VGet(PadInput::GetJoyPad_x_left(),
       0.0f, -PadInput::GetJoyPad_y_left());*/


	   ////床と衝突しているか
	   //isHitGround = HitCheck::SphereHitJudge(modelHandle, -1, bottomPosition, hitPoly_Ground_sphere);

	   //isHitSphere = isHitGround;
	   //
	   ////ジャンプ中ではない場合に処理
	   //if (isHitGround && !isJump)
	   //{
	   //	VECTOR addPos = VGet(0.0f, 0.0f, 0.0f);

	   //	for (int i = 0; i < hitPoly_Ground_sphere.HitNum; i++)
	   //	{
	   //		MV1_COLL_RESULT_POLY poly = hitPoly_Ground_sphere.Dim[i];

	   //		if (poly.Normal.y >= 1.0f)
	   //		{
	   //			//未来座標の球の最下部座標
	   //			VECTOR bottomPos_sphere = bottomPosition;
	   //			bottomPos_sphere.y -= radius;
	   //			
	   //			//次のフレームのplayerの接触座標を求める
	   //			hitPos_ground = HitCheck::ClosestPtToPointTriangle(bottomPosition, poly.Position[0], poly.Position[1], poly.Position[2]);

	   //			//現在のプレイヤーのカプセルと面の接触座標を求める
	   //			VECTOR nowHitPos_ground = HitCheck::ClosestPtToPointTriangle(nowBottomPos, poly.Position[0], poly.Position[1], poly.Position[2]);

	   //			MV1_COLL_RESULT_POLY rayPoly;

	   //			//現在の座標からrayを伸ばし、当たっていなかったらif文内に行く
	   //			if (!HitCheck::HitRayJudge(modelHandle, -1, nowTopPos, nowBottomPos, rayPoly))
	   //			{
	   //				//カプセルの中心座標が三角形の外側のとき球とメッシュの接触部分で押し戻し量を求める
	   //				if (!HitCheck::TriangleAreaCheck_ground(nowHitPos_ground, poly.Position[0], poly.Position[1], poly.Position[2]))
	   //				{
	   //					addPos = CalcPushBack_SphereMeshOutsideTriangle(poly, hitPos_ground, bottomPosition, radius);
	   //				}
	   //			}

	   //			//中心座標が三角形の内側だった場合
	   //			//rayが当たっていれば球の最下部ととメッシュで押し戻し量計算
	   //			else
	   //			{
	   //				//球と面の接触しているは球の最下部と面の接触座標と同じなのでリセット
	   //				hitSphere = VGet(0.0f, 0.0f, 0.0f);

	   //				//平面であればそのまま足元で計算
	   //				if (poly.Normal.y >= 1.0f)
	   //				{

	   //					VECTOR newAddPos = VGet(0.0f, 0.0f, 0.0f);

	   //					//中心座標から半径分を引くことによって球の最下部を算出
	   //					VECTOR footPos = VGet(0.0f, bottomPosition.y - radius, 0.0f);
	   //					
	   //					//接触座標と球の最下部で押し戻し量を計算
	   //					newAddPos.y = hitPos_ground.y - footPos.y;

	   //					//押し戻し量が一番大きいものを加算する
	   //					if (addPos.y < newAddPos.y)
	   //					{
	   //						addPos = newAddPos;
	   //					}
	   //				}
	   //			}
	   //		}
	   //	}

	   //	//足元と床との差が0.1以上の場合のみplayerの位置に加算
	   //	if (addPos.y >= 0.1f)
	   //	{
	   //		newPos.y = newPos.y + addPos.y;
	   //	}
	   //}
	   //
	   //MV1_COLL_RESULT_POLY lastPoly = {};

	   ////hitPolyの一番最後の情報を受け取る
	   //if (hitPoly_Ground_sphere.HitNum >= 1)
	   //{
	   //	lastPoly = hitPoly_Ground_sphere.Dim[hitPoly_Ground_sphere.HitNum - 1];
	   //}

	   ////カプセルが当たっていないときrayをチェック
	   ////下り坂はrayでチェック
	   //if (!isJump && !isHitSphere || (lastPoly.Normal.y < 1.0f && lastPoly.Normal.y >= 0.7f))
	   //{
	   //	MV1_COLL_RESULT_POLY rayPoly_ground;

	   //	//rayが当たっていれば
	   //	isHitGround = HitCheck::HitRayJudge(modelHandle, -1, nowTopPos, nowBottomPos, rayPoly_ground);

	   //	if (isHitGround && !isJump)
	   //	{
	   //		VECTOR newPlayerPos = VGet(0.0f, 0.0f, 0.0f);
	   //		VECTOR footPos = VGet(0.0f, bottomPosition.y - radius, 0.0f);

	   //		//床 - プレイヤーの足元で押し戻し量を計算
	   //		newPlayerPos.y = rayPoly_ground.HitPosition.y - footPos.y;
	   //		newPos.y = newPos.y + newPlayerPos.y;
	   //	}
	   //}