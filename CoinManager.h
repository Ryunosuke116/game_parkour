#pragma once
#include "CoinObject.h"
#include "BaseGameObjectManager.h"
#include "Player.h"
#include "CoinObserver.h"

class EffectManager;

class CoinManager :
	public BaseGameObjectManager,
	public std::enable_shared_from_this<CoinManager>
{
public:
	void AddObserver(std::shared_ptr<CoinObserver> observer) { observers.push_back(observer); }
	void RemoveObserver(std::shared_ptr<CoinObserver> observer);
	void NotifyCoinPicked(int amount);

	CoinManager();
	~CoinManager();

	void Create()override;
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void Add()override;
	void ResultInitialize()override;
	void ResultUpdate()override;
	void ResultCreate()override;
	void ResultCreate(int coinCount);

	int GetModelHandle()const { return modelHandle; }
private:
	std::vector<std::shared_ptr<CoinObject>> coins;
	std::vector<std::weak_ptr<CoinObject>> resultUpdateCoins;
	std::vector<std::weak_ptr<CoinObserver>> observers;
	
	int modelHandle;
	int nowCoinCount;
	float timer;

	bool isAddResultUpdateCoin;				//更新するコインを追加するか
	
	static constexpr float maxAddResultUpdateCoinTimer = 30.0f;
	static constexpr int coinValue = 1;		//コインの価値

};

