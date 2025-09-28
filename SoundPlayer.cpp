#include "common.h"
#include <memory>
#include <vector>
#include <cassert>
#include "SoundPlayer.h"
#include "JsonManager.h"

SoundPlayer::SoundPlayer()
{

}

SoundPlayer::~SoundPlayer()
{
	for (auto& soundData : soundDatas)
	{
		DeleteSoundMem(soundData.second.handle);
	}
	soundDatas.clear();
}

void SoundPlayer::Shutdown()
{
	for (auto& soundData : soundDatas)
	{
		DeleteSoundMem(soundData.second.handle);
	}
	soundDatas.clear();
}

void SoundPlayer::Create(const std::string& sceneName)
{
	if (soundDatas.size() != 0)
	{
		assert("soundDatas‚Ì‰Šú‰»‚ªs‚í‚ê‚Ä‚¢‚Ü‚¹‚ñ");
	}

	const nlohmann::json soundData = JsonManager::GetInstance().GetJsons("sound");
	for (auto& data : soundData["list"])
	{
		std::string path = data[0].get<std::string>();
		std::string name = data[1].get<std::string>();
		bool isLoop		 = data[2].get<bool>();

		int soundHandle = LoadSoundMem(path.c_str());
		soundDatas[name] =
		{
			LoadSoundMem(path.c_str()),
			isLoop
		};
	}
}

void SoundPlayer::Play(const std::string& name)
{
	auto it = soundDatas.find(name);

	if (it != soundDatas.end())
	{
		 int a = PlaySoundMem(it->second.handle,
			 it->second.iSLoop ? DX_PLAYTYPE_LOOP : DX_PLAYTYPE_BACK);
	}
}

void SoundPlayer::Stop(const std::string& name)
{
	StopSoundMem(soundDatas.at(name).handle);
}