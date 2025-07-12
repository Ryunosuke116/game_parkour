#pragma once
#include "BaseGameObjectManager.h"

class PlayerManager : public BaseGameObjectManager
{
private:
	std::shared_ptr<BaseChara> player = NULL;
	std::shared_ptr<Player> actualPlayer = NULL;
public:

	PlayerManager();
	~PlayerManager();
	
	void Update(int mapHandle,const VECTOR& cameraDirection);

	void Create()		override;
	void Initialize()	override;
	void Update()		override;
	bool Draw()			override;
	void Add()			override;
	

	VECTOR PositionCheck(const VECTOR& hangingPos, const VECTOR& playerPos);

	std::shared_ptr<Player> GetPlayer()const { return actualPlayer; }
};

