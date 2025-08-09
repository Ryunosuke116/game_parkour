#include "common.h"
#include <string>
#include "EffekseerForDXLib.h"
#include "Effect.h"

/// <summary>
/// コンストラクタ
/// </summary>
Effect::Effect()
{
    position = VGet(0, 0, 0);
    time = 0;
    playingEffectHandle = -1;
    resourceHandle = -1;
}

/// <summary>
/// デストラクタ
/// </summary>
Effect::~Effect()
{

}

/// <summary>
/// 情報読み込み
/// </summary>
/// <param name="path"></param>
/// <param name="scale"></param>
/// <param name="setPos"></param>
void Effect::Load(const char* path, std::string& setTag,
    const float& scale, const VECTOR& setPos)
{
    resourceHandle = LoadEffekseerEffect(path, scale);
    position = setPos;
    tag = setTag;
}

/// <summary>
/// 座標の更新
/// </summary>
/// <param name="objectPosition"></param>
void Effect::PositionUpdate(const VECTOR& setPos)
{
    position = setPos;

    // 再生中のエフェクトを移動する。
    SetPosPlayingEffekseer3DEffect(playingEffectHandle, position.x, position.y, position.z);
}

/// <summary>
///  エフェクトを再生
/// </summary>
void Effect::PlayEffect()
{
    // エフェクトを再生する。
    playingEffectHandle = PlayEffekseer3DEffect(resourceHandle);
}

/// <summary>
/// 再生中のモーションを停止する
/// </summary>
void Effect::StopEffect()
{
    StopEffekseer3DEffect(playingEffectHandle);
}


/// <summary>
/// 拡大率
/// </summary>
/// <param name="scale"></param>
void Effect::SetScale(VECTOR scale)
{
    SetScalePlayingEffekseer3DEffect(playingEffectHandle, scale.x, scale.y, scale.z);
}

/// <summary>
/// 再生速度の変更
/// </summary>
/// <param name="speed"></param>
void Effect::SetSpeed(float speed)
{
    SetSpeedPlayingEffekseer3DEffect(playingEffectHandle, speed);
}

