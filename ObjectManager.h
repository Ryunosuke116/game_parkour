#pragma once
#include "BaseManager.h"
class ObjectManager : public BaseManager
{
public:
	ObjectManager();
	~ObjectManager();

	void Create() override;
	void Initialize()override;
	void Update()override;
	void Draw()override;

private:
	std::shared_ptr<BaseObject> map = NULL;
	std::shared_ptr<BaseObject> field = NULL;
	std::shared_ptr<BaseObject> coin = NULL;
	std::shared_ptr<BaseChara> player = NULL;
	std::shared_ptr<Camera> camera = NULL;
	std::shared_ptr<PlayerManager> playerManager = NULL;
	std::shared_ptr<BaseObject> fieldMesh = NULL;

	std::shared_ptr<Player> actualPlayer = NULL;
	std::shared_ptr<CoinObject> actualCoin = NULL;
};

