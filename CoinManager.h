#pragma once
#include "CoinObject.h"
#include "BaseManager.h"
#include "Player.h"

class CoinManager : public BaseManager
{
private:
	std::vector<std::shared_ptr<CoinObject>> coins;
	int modelHandle;

public:

	CoinManager();
	~CoinManager();

	void Update(const std::shared_ptr<Player>& player, const VECTOR& cameraLookPos);
	bool Draw()override;

	void Add();

	void Initialize() override;
	void Update()override;
	void Create()override;

	int GetModelHandle()const { return modelHandle; }
};

