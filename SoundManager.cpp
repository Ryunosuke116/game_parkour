#include "common.h"
#include <memory>
#include <vector>
#include <cassert>
#include "SoundManager.h"

SoundManager::SoundManager()
{
	tag = "sound";
}

SoundManager::~SoundManager()
{
	soundDatas.clear();
}

void SoundManager::Create()
{
	for (auto& data : jsonData["list"])
	{
		std::string path = data[0];
		std::string name = data[1];
		bool isLoop		 = data[2].get<bool>();

		soundDatas[name] =
		{
			LoadSoundMem(path.c_str()),
			isLoop
		};
	}
}

void SoundManager::Play(const std::string& name)
{
	auto it = soundDatas.find(name);

	if (it != soundDatas.end())
	{
		PlaySoundMem(it->second.handle,
			it->second.iSLoop ? DX_PLAYTYPE_LOOP : DX_PLAYTYPE_BACK);
	}

}

void SoundManager::Stop(const std::string& name)
{
	StopSoundMem(soundDatas.at(name).handle);
}