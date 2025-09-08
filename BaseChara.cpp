#include "common.h"
#include "BaseChara.h"
#include "DebugDrawer.h"


BaseChara::BaseChara() :
    modelHandle(-1),
    coinCount(-1),
    nowFrameNumber(-1),
    angle(-1),
    rotate_x(-1.0f),
    isCollisionCheck(false),
    position(VGet(-1.0f, -1.0f, -1.0f)),
    framePosition(VGet(-1.0f, -1.0f, -1.0f)),
    velocity(VGet(-1.0f, -1.0f, -1.0f)),
    targetMoveDirection(VGet(-1.0f, -1.0f, -1.0f)),
    moveDirection(VGet(-1.0f, -1.0f, -1.0f)),
    positionData({ NULL })
{

}

BaseChara::~BaseChara()
{
    MV1DeleteModel(modelHandle);
}

/// <summary>
/// 描画
/// </summary>
void BaseChara::Draw()
{
    //MV1SetWireFrameDrawFlag(modelHandle,TRUE);
    MV1DrawModel(modelHandle);
}

/// <summary>
/// 向き
/// </summary>
void BaseChara::UpdateAngle(const VECTOR& direction)
{
    // プレイヤーの移動方向にモデルの方向を近づける
    float targetAngle;			// 目標角度
    float difference;			// 目標角度と現在の角度との差

    // 目標の方向ベクトルから角度値を算出する
    targetAngle = static_cast<float>(atan2(direction.x, direction.z));

    // 目標の角度と現在の角度との差を割り出す
    // 最初は単純に引き算
    difference = targetAngle - angle;

    // ある方向からある方向の差が１８０度以上になることは無いので
    // 差の値が１８０度以上になっていたら修正する
    if (difference < -DX_PI_F)
    {
        difference += DX_TWO_PI_F;
    }
    else if (difference > DX_PI_F)
    {
        difference -= DX_TWO_PI_F;
    }

    // 角度の差が０に近づける
    if (difference > 0.0f)
    {
        // 差がプラスの場合は引く
        difference -= angleSpeed;
        if (difference < 0.0f)
        {
            difference = 0.0f;
        }
    }
    else
    {
        // 差がマイナスの場合は足す
        difference += angleSpeed;
        if (difference > 0.0f)
        {
            difference = 0.0f;
        }
    }

    // モデルの角度を更新
    angle = targetAngle - difference;
    DebugDrawer::Instance().InformationInput_string_float("angle %f\n", angle);
    DebugDrawer::Instance().InformationInput_string_float("rotate_x %f\n", rotate_x);
    MV1SetRotationXYZ(modelHandle, VGet(rotate_x * DX_PI_F / 180.0f, angle + DX_PI_F, 0.0f));
}

void BaseChara::PositionUpdate()
{
    // プレイヤーのモデルの座標を更新する
    MV1SetPosition(modelHandle, position);
}