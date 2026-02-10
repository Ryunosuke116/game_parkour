#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include "ObjectForTree.hpp"

template<class Type>
class Cell : public std::enable_shared_from_this<Cell<Type>>
{
public:
	/// <summary>
	/// オブジェクトを空間に登録
	/// </summary>
	/// <param name="wOFT"></param>
	bool Push(std::shared_ptr<ObjectForTree<Type>>& spOFT)
	{
		//無効なオブジェクトは登録しない
		if (spOFT.get() == NULL)return false;

		auto self = this->shared_from_this();

		//二重登録チェック
		if (spOFT->cellPointer.lock() == self)return false;

		//空間に新規登録する
		objectList.push_back(spOFT);

		//この空間に登録されていることを通知
		spOFT->RegistCell(self);

		return true;
	}

	/// <summary>
	/// 削除されるオブジェクトをチェック
	/// </summary>
	/// <param name="RemoveObject"></param>
	/// <returns></returns>
	std::vector<std::shared_ptr<ObjectForTree<Type>>>::iterator OnRemove(
		std::vector<std::shared_ptr<ObjectForTree<Type>>>::iterator& RemoveObject)
	{
		if (RemoveObject != objectList.end())
		{
			return objectList.erase(RemoveObject);
		}
		return ++RemoveObject;
	}

	std::vector<std::shared_ptr<ObjectForTree<Type>>>& GetObjectList() { return objectList; }

private:
	std::vector<std::shared_ptr<ObjectForTree<Type>>> objectList;
};

