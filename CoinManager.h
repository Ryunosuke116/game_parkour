#pragma once
#include "CoinObject.h"
#include "BaseGameObjectManager.h"
#include "Player.h"


class CoinManager : public BaseGameObjectManager
{
private:
	std::vector<std::shared_ptr<CoinObject>> coins;
	int modelHandle;

public:

	CoinManager();
	~CoinManager();

	void Update(const std::shared_ptr<Player>& player, const VECTOR& cameraLookPos);


	void Create()override;
	void Initialize()override;
	void Update()override;
	bool Draw()override;
	void Add()override;

	int GetModelHandle()const { return modelHandle; }
};

