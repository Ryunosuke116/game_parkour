#pragma once
#include "ISoundPlayer.h"
#include "BaseGameObjectManager.h"

class SoundPlayer : 
	public ISoundPlayer,
	public BaseGameObjectManager
{
public:

	SoundPlayer();
	~SoundPlayer();

	void Create()override;
	void Initialize()override {}
	void Update(ObjectMediator& objectMediator)override {}
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
	int handle;

};

