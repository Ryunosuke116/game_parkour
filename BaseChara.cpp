#include "common.h"
#include "BaseChara.h"
#include "DebugDrawer.h"
#include "Calculation.h"


BaseChara::BaseChara() :
    modelHandle(-1),
    coinCount(-1),
    nowFrameNumber(-1),
    radian(-1),
    rotateX(-1.0f),
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
    float targetRadian;			                        // 目標角度
    float nowDegree = Calculation::RadToDeg(radian);

    // 目標の方向ベクトルから角度値を算出する
    targetRadian = atan2(direction.x, direction.z);

    const float targetDegree = Calculation::RadToDeg(targetRadian);

    nowDegree = Calculation::RotationAngleDegree(targetDegree, nowDegree, kRotationSpeedDegree);
    radian = Calculation::DegToRad(nowDegree);

    DebugDrawer::GetInstance().InformationInput_string_float("radian %f\n", radian);
    DebugDrawer::GetInstance().InformationInput_string_float("rotateX %f\n", rotateX);
    MV1SetRotationXYZ(modelHandle, VGet(rotateX * DX_PI_F / 180.0f, radian + DX_PI_F, 0.0f));
}

void BaseChara::PositionUpdate()
{
    // プレイヤーのモデルの座標を更新する
    MV1SetPosition(modelHandle, position);
}