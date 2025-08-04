#pragma once
#include "CoinObject.h"
#include "BaseGameObjectManager.h"
#include "Player.h"
#include "CoinObserver.h"


class CoinManager : public BaseGameObjectManager
{
private:
	std::vector<std::shared_ptr<CoinObject>> coins;
	std::vector<std::weak_ptr<CoinObserver>> observers;
	int modelHandle;
	static constexpr int coinValue = 1;		//ƒRƒCƒ“‚Ì‰¿’l

public:
	void AddObserver(std::shared_ptr<CoinObserver> observer) { observers.push_back(observer); }
	void RemoveObserver(std::shared_ptr<CoinObserver> observer);
	void NotifyCoinPicked(int amount);

	CoinManager();
	~CoinManager();

	void Update(const std::shared_ptr<Player>& player, const VECTOR& cameraLookPos);

	void Create()override;
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void Add()override;

	int GetModelHandle()const { return modelHandle; }
};

