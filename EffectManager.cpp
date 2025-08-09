#include "common.h"
#include <vector>
#include <memory>
#include "EffekseerForDXLib.h"
#include "EffectManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
EffectManager::EffectManager()
{

}

/// <summary>
/// デストラクタ
/// </summary>
EffectManager::~EffectManager()
{
	effectDatas.clear();
}

/// <summary>
/// 要素の追加
/// </summary>
/// <param name="path"></param>
/// <param name="scale"></param>
/// <param name="setPos"></param>
void EffectManager::Add(const char* path, std::string& setTag,
	const float& scale)
{

	std::shared_ptr<EffectData> data = std::make_shared<EffectData>();

	data->resourceHandle = LoadEffekseerEffect(path, scale);
	data->tag = setTag;

	effectDatas.push_back(data);

}

/// <summary>
/// 描画
/// </summary>
void EffectManager::Draw()
{
	DrawEffekseer3D();
}

/// <summary>
/// Effekseerにより再生中のエフェクトを更新する。
/// </summary>
void EffectManager::PlayEffectUpdate()
{
	UpdateEffekseer3D();
}

/// <summary>
/// エフェクトを再生
/// </summary>
/// <param name="tag"></param>
void EffectManager::PlayEffect(const std::string& tag)
{
	for (auto& effect : effectDatas)
	{
		if (effect->tag == tag)
		{
			// エフェクトを再生する。
			effect->playingEffectHandle = PlayEffekseer3DEffect(effect->resourceHandle);
		}
	}
}

/// <summary>
/// エフェクトの停止
/// </summary>
/// <param name="tag"></param>
void EffectManager::StopEffect(const std::string& tag)
{
	for (auto& effect : effectDatas)
	{
		if (effect->tag == tag)
		{
			StopEffekseer3DEffect(effect->playingEffectHandle);
		}
	}
}

/// <summary>
/// ポジションを保存
/// </summary>
/// <param name="setPosition"></param>
void EffectManager::SetPosition(const VECTOR setPosition,
	const std::string& tag)
{
	for (auto& effect : effectDatas)
	{
		if (effect->tag == tag)
		{
			effect->position = setPosition;
			// 再生中のエフェクトを移動する。
			SetPosPlayingEffekseer3DEffect(effect->playingEffectHandle, effect->position.x, effect->position.y, effect->position.z);
		}
	}
}

/// <summary>
/// 再生中のエフェクトの拡大率調整
/// </summary>
/// <param name="scale"></param>
/// <param name="tag"></param>
void EffectManager::SetScale(const VECTOR& scale,
	const std::string& tag)
{
	for (auto& effect : effectDatas)
	{
		if (effect->tag == tag)
		{
			SetScalePlayingEffekseer3DEffect(effect->playingEffectHandle, scale.x, scale.y, scale.z);
		}
	}
}

/// <summary>
///	再生中のエフェクトの再生速度の調整
/// </summary>
/// <param name="speed"></param>
/// <param name="tag"></param>
void EffectManager::SetSpeed(const float& speed, const std::string& tag)
{
	for (auto& effect : effectDatas)
	{
		if (effect->tag == tag)
		{
			SetSpeedPlayingEffekseer3DEffect(effect->playingEffectHandle, speed);
		}
	}
}

/// <summary>
/// 再生中の3D表示のエフェクトの角度を設定する
/// </summary>
/// <param name="setPosition"></param>
/// <param name="tag"></param>
void EffectManager::SetRotation(const VECTOR& setPosition,
	const std::string& tag)
{
	for (auto& effect : effectDatas)
	{
		if (effect->tag == tag)
		{
			SetRotationPlayingEffekseer3DEffect(effect->playingEffectHandle, setPosition.x, setPosition.y, setPosition.z);
		}
	}
}