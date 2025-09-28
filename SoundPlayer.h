#pragma once
#include "BaseGameObjectManager.h"
#include "IWorldSubSystem.h"

class SoundPlayer : 
	public IWorldSubSystem
{
public:

	SoundPlayer();
	~SoundPlayer();

	void Create(const std::string& sceneName)override;
	void Shutdown()override;
	void Update()override {}
	void Draw()override{}
	void Play(const std::string& name);
	void Stop(const std::string& name);

private:

	struct SoundData
	{
		int handle;
		bool iSLoop;
	};
	std::unordered_map<std::string, SoundData> soundDatas;
	int handle;

};

