#include "EffekseerForDXLib.h"
#include "EffectData.h"

EffectDatas::EffectDatas(int effectHandle, std::string& effectTag) :
	resourceHandle(effectHandle),
	tag(effectTag)
{
	//処理なし
}

EffectDatas::~EffectDatas()
{
	DeleteEffekseerEffect(resourceHandle);
}

void EffectDatas::PlayEffect()
{
	EffectInformation effectInformation;
	effectInformation.playingEffectHandle = PlayEffekseer3DEffect(resourceHandle);

	//再生するエフェクトを追加
	umPlayingEffectHandles.emplace(playingCount, 
		effectInformation);

	playingCount++;
}

/// <summary>
/// 再生中の全てのエフェクトを停止する
/// </summary>
void EffectDatas::AllStopEffect()
{
	for (auto& effectInformation : umPlayingEffectHandles)
	{
		StopEffekseer3DEffect(effectInformation.second.playingEffectHandle);
	}

	umPlayingEffectHandles.clear();
}