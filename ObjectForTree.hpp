#pragma once
#include <iostream>
#include <memory>

template <class Type>
class Cell;

template<class Type>
class ObjectForTree : public std::enable_shared_from_this<ObjectForTree<Type>>
{
public:
	std::weak_ptr<Cell<Type>> cellPointer;	//所属している空間のポインタ
	std::weak_ptr<Type> objectPointer;

public:
	ObjectForTree()
	{
		cellPointer.reset();
	}

	~ObjectForTree()
	{
		//処理なし
	}

	/// <summary>
	/// 空間を登録
	/// </summary>
	/// <param name="cellPointer"></param>
	void RegistCell(std::weak_ptr<Cell<Type>> cellPointer)
	{
		this->cellPointer = cellPointer;
	}

	bool Remove()
	{
		//すでに空間に存在しない場合は処理終了
		if (!cellPointer)return false;

		auto self = this->shared_from_this();
		std::weak_ptr<ObjectForTree<Type>> wpOFT = self;

		std::shared_ptr<Cell<Type>> spCellPointer = cellPointer.lock();

		//自分を登録している空間に自身を通知
		spCellPointer->OnRemove(wpOFT);
		cellPointer = NULL;

		return true;
	}
};