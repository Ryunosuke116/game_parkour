#include "common.h"
#include <memory>
#include <vector>
#include <cassert>
#include "SoundPlayer.h"

SoundPlayer::SoundPlayer()
{
	tag = "sound";
}

SoundPlayer::~SoundPlayer()
{
	soundDatas.clear();
}

void SoundPlayer::Create()
{
	for (auto& data : jsonData["list"])
	{
		std::string path = data[0];
		std::string name = data[1];
		bool isLoop		 = data[2].get<bool>();

		int soundHandle = LoadSoundMem(path.c_str());

		soundDatas[name] =
		{
			LoadSoundMem(path.c_str()),
			isLoop
		};
	}

	handle = LoadSoundMem("material/sound/gameBGM.mp3");
}

void SoundPlayer::Play(const std::string& name)
{
	auto it = soundDatas.find(name);

	if (it != soundDatas.end())
	{
		 PlaySoundMem(it->second.handle, DX_PLAYTYPE_LOOP, TRUE);
		 CheckSoundMem(it->second.handle);
		//	it->second.iSLoop ? DX_PLAYTYPE_LOOP : DX_PLAYTYPE_BACK);
	}

	PlaySoundMem(handle, DX_PLAYTYPE_LOOP);

}

void SoundPlayer::Stop(const std::string& name)
{
	StopSoundMem(soundDatas.at(name).handle);
}