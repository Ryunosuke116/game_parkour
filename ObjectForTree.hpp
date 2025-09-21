#pragma once
#include <iostream>
#include <memory>

template <class T>
class Cell;

template<class T>
class ObjectForTree : public std::enable_shared_from_this<ObjectForTree<T>>
{
public:
	Cell<T>* cellPointer;	//所属している空間のポインタ
	std::weak_ptr<T> objectPointer;

public:
	ObjectForTree()
	{
		cellPointer = NULL;
	}

	~ObjectForTree()
	{
		//処理なし
	}

	/// <summary>
	/// 空間を登録
	/// </summary>
	/// <param name="cellPointer"></param>
	void RegistCell(Cell<T>* cellPointer)
	{
		this->cellPointer = cellPointer;
	}

	bool Remove()
	{
		auto self = this->shared_from_this();
		std::weak_ptr<ObjectForTree<T>> wpOFT = self;

		//すでに空間に存在しない場合は処理終了
		if (!cellPointer)return false;

		//自分を登録している空間に自身を通知
		cellPointer->OnRemove(wpOFT);
		cellPointer = NULL;

		return true;
	}
};