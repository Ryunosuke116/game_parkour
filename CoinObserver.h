#pragma once
class CoinObserver
{
public:
	virtual void OnCoinPicked(int amount)abstract;
	virtual ~CoinObserver() = default;


};

