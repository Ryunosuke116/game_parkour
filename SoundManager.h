#pragma once
#include "ISoundPlayer.h"
#include "BaseGameObjectManager.h"

class SoundManager : 
	public ISoundPlayer,
	public BaseGameObjectManager
{
public:

	SoundManager();
	~SoundManager();

	void Create()override;
	void Initialize()override {}
	void Update()override {}
	void Draw()override{}
	void Add()override {}
	void Play(const std::string& name)override;
	void Stop(const std::string& name)override;

private:

	struct SoundData
	{
		~SoundData()
		{
			DeleteSoundMem(handle);
		}
		int handle;
		bool iSLoop;
	};
	std::unordered_map<std::string, SoundData> soundDatas;

};

