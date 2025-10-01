#pragma once
#include "DxLib.h"
#include <iostream>
#include <vector>
#include <memory>
#include <unordered_map>
#include "ObjectForTree.hpp"
#include "Cell.hpp"
#include "IObject.h"
#include "WorldSubSystem.h"
#include "boundaryRange.h"
#include "DebugDrawer.h"

template <class T>
class Liner8TreeManager :
	public IObject,
	public std::enable_shared_from_this<Liner8TreeManager<T>>
{
public:
	virtual void Initialize()			override {}
	virtual void Update()				override {}
	virtual void Draw()					override {}
	virtual void ResultInitialize()		override {}
	virtual void ResultUpdate()			override {}
	virtual void ResultCreate()			override {}
public:
	Liner8TreeManager()
	{
		cellArray.clear();
	}

	void Create()override
	{
		auto self = this->shared_from_this();

		WorldSubSystem::GetInstance().AddSubSystem<Liner8TreeManager<T>>(self);
	}

	/// <summary>
	/// 線形8分木配列を構築する
	/// </summary>
	/// <param name="spaceLevel"></param>
	/// <param name="min"></param>
	/// <param name="max"></param>
	bool Initialize(uint32_t spaceLevel,
		const VECTOR& min,
		const VECTOR& max)
	{
		//最高レベル以上の空間は作れない
		if (spaceLevel >= maxLevel)return false;

		//各レベルでの空間数を算出
		iPow[0] = 1;
		for (int i = 1; i < maxLevel + 1; i++)
		{
			iPow[i] = iPow[i - 1] * 8;
		}

		//空間の合計数を算出
		cellNumber = (iPow[spaceLevel + 1] - 1) / 7;

		//領域を登録
		regionMin = min;
		regionMax = max;
		regionWidth = VSub(regionMax, regionMin);
		regionSideLength.x = regionWidth.x / ((float)(1 << spaceLevel));
		regionSideLength.y = regionWidth.y / ((float)(1 << spaceLevel));
		regionSideLength.z = regionWidth.z / ((float)(1 << spaceLevel));

		lowestLevel = spaceLevel;

		return true;	//成功
	}

	/// <summary>
	/// オブジェクトを空間セルに登録する
	/// </summary>
	/// <param name="objectMin">オブジェクトの境界範囲の最小座標ベクトル。</param>
	/// <param name="objectMax">オブジェクトの境界範囲の最大座標ベクトル。</param>
	/// <param name="wOFT"></param>
	bool Regist(const VECTOR& objectMin,
		const VECTOR& objectMax,
		std::shared_ptr<ObjectForTree<T>>& spOFT)
	{
		//オブジェクトの境界範囲から登録モートン番号を算出
		uint32_t spaceNumber = GetMortonNumber(objectMin, objectMax);

		//空間の最大数より小さければ登録
		if (spaceNumber < cellNumber)
		{
			//空間が生成されてない場合は新規作成
			CreateNewCell(spaceNumber);

			//登録
			return cellArray.find(spaceNumber)->second->Push(spOFT);
		}

		return false;	// 登録失敗
	}

	/// <summary>
	/// 空間を作成
	/// 親空間が作られてなかったら作成する
	/// </summary>
	/// <param name="spaceNumber"></param>
	bool CreateNewCell(uint32_t spaceNumber)
	{
		//空間がある場合は抜ける
		if (cellArray.find(spaceNumber) != cellArray.end()) return false;

		//存在していないなら空間を新規作成する
		while (cellArray.find(spaceNumber) == cellArray.end())
		{
			//指定の要素番号に空間を新規作成
			cellArray[spaceNumber] = std::make_shared<Cell<T>>();

			//親空間に移動
			spaceNumber = (spaceNumber - 1) >> 3;
			if (spaceNumber >= cellNumber)break;
		}
		return true;
	}

	/// <summary>
	/// どの空間に所属しているかを算出する
	/// </summary>
	/// <param name="min"></param>
	/// <param name="max"></param>
	/// <returns></returns>
	uint32_t GetMortonNumber(const VECTOR& min, const VECTOR& max)
	{
		VECTOR mObjectMin = min;
		VECTOR mObjectMax = max;

		//ルート空間と同じ移動量分、対象オブジェクトにも加算
		mObjectMin = VAdd(mObjectMin, BoundaryRange::parallelVelocity);
		mObjectMax = VAdd(mObjectMax, BoundaryRange::parallelVelocity);

		//最小レベルにおける各軸位置を算出
		uint32_t leftTop = GetPointElem(mObjectMin);
		uint32_t rightBottom = GetPointElem(mObjectMax);

		uint32_t xorNumber = rightBottom ^ leftTop;
		uint32_t spaceIndex = 0;
		uint32_t shift = 0;

		//空間レベル数分、差があるかチェック
		for (unsigned int i = 0; i < lowestLevel; i++)
		{
			uint32_t checkNumber = (xorNumber >> (i * 3));
			//3ビット分マスクして
			if ((checkNumber & 0x7) != 0)
			{
				spaceIndex = (i + 1);
				shift = spaceIndex * 3;
			}
		}

		//上位レベルのどのセルに位置しているかを計算
		uint32_t spaceNumber = rightBottom >> shift;

		//最上位空間から現在のレベル空間の位置までの
		// 通し番号を求める
		uint32_t addNumber = (iPow[lowestLevel - spaceIndex] - 1) / 7;

		spaceNumber += addNumber;

		//計算結果が配列範囲を超える場合はエラー値を返す
		if (spaceNumber >= cellNumber)
			return 0xffffffff;

		return spaceNumber;
	}

	/// <summary>
	/// 空間の番号を求める
	/// </summary>
	/// <param name="point"></param>
	/// <returns></returns>
	uint32_t GetPointElem(const VECTOR& point)
	{
		const int maxIndex = (1 << lowestLevel) - 1;

		//軸ごとの座標からセルインデックスを求める
		auto computeAxisIndex = [&](float axisValue, float axisMin, float cellSize)->uint32_t {
			float cellCoordFloat = (axisValue - axisMin) / cellSize;
			int cellIndex = static_cast<int>(std::floor(cellCoordFloat));
			if (cellIndex < 0) cellIndex = 0;
			if (cellIndex > maxIndex) cellIndex = maxIndex;
			return  static_cast<uint32_t>(cellIndex);
			};

		uint32_t uintX = computeAxisIndex(point.x, regionMin.x, regionSideLength.x);
		uint32_t uintY = computeAxisIndex(point.y, regionMin.y, regionSideLength.y);
		uint32_t uintZ = computeAxisIndex(point.z, regionMin.z, regionSideLength.z);

		return Get3DMortonNumber(
			static_cast<BYTE>(uintX),
			static_cast<BYTE>(uintY),
			static_cast<BYTE>(uintZ)
		);
	}

	/// <summary>
	/// 各所属ビットを3ビットごとに広げる
	/// </summary>
	/// <param name="number"></param>
	/// <returns></returns>
	uint32_t BitSeparateFor3D(BYTE number)
	{
		uint32_t bitNumber = number;

		bitNumber = (bitNumber | bitNumber << 8) & 0x0000f00f;
		bitNumber = (bitNumber | bitNumber << 4) & 0x000c30c3;
		bitNumber = (bitNumber | bitNumber << 2) & 0x00249249;
		return bitNumber;
	}

	/// <summary>
	/// 8分木モートン番号算出
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="z"></param>
	/// <returns></returns>
	uint32_t Get3DMortonNumber(BYTE x, BYTE y, BYTE z)
	{
		return  BitSeparateFor3D(x) |
			(BitSeparateFor3D(y) << 1) |
			(BitSeparateFor3D(z) << 2);
	}

	std::shared_ptr<Cell<T>> GetCell(uint32_t spaceNumber)
	{
		//空間がない場合は空を返す
		if (cellArray.find(spaceNumber) == cellArray.end())
		{
			return std::shared_ptr<Cell<T>>();
		}
		return cellArray.at(spaceNumber);
	}

	/// <summary>
	/// 親番号を計算
	/// </summary>
	/// <param name="spaceNumber"></param>
	/// <returns></returns>
	uint32_t GetParentNumber(uint32_t spaceNumber)
	{
		//現空間が7以下の場合は親空間は0が確定しているので0を返す
		//現空間0の場合も親がないので０を返す
		if (spaceNumber <= 7)
		{
			return 0;
		}

		return (spaceNumber - 1) / 8;
	}

	/// <summary>
	/// 指定された空間からルート空間までの衝突対象を取得
	/// </summary>
	/// <param name="collisionList"></param>
	/// <param name="spaceNumber"></param>
	/// <returns></returns>
	uint32_t GetAllCollisionList(
		std::vector<std::shared_ptr<ObjectForTree<T>>>& collisionList,
		uint32_t spaceNumber)
	{
		//中身を初期化
		collisionList.clear();

		uint32_t nowSpaceNumber = spaceNumber;

		//空間が存在していなければ抜ける
		if (cellArray.size() == 0)
		{
			return 0;
		}

		//現在の空間の子空間を走査
		const int divisionNumber = 8;	//空間の分割数
		uint32_t childSpaceNumber = spaceNumber << 3;

		for (unsigned int i = 0; i < divisionNumber; i++)
		{
			GetCollisionList(collisionList, childSpaceNumber);
			childSpaceNumber++;
		}

		//深度分走査する
		for (unsigned int i = 0; i < lowestLevel; i++)
		{
			GetCollisionList(collisionList, nowSpaceNumber);

			uint32_t prevSpaceNumber = nowSpaceNumber;
			nowSpaceNumber = GetParentNumber(nowSpaceNumber);

			//既にルート空間(0)だった場合は抜ける
			if (nowSpaceNumber == prevSpaceNumber)break;
		}

		return static_cast<uint32_t>(collisionList.size());
	}

	bool GetCollisionList(
		std::vector<std::shared_ptr<ObjectForTree<T>>>& collisionList,
		uint32_t spaceNumber)
	{
		//空間を検索
		std::shared_ptr<Cell<T>> cell = GetCell(spaceNumber);

		//空間が存在しなければ抜ける
		if (cell == nullptr)return false;

		//空間内に衝突対象が1つも無ければ抜ける
		if (cell->GetObjectList().size() == 0)return false;

		//空間内のオブジェクトを衝突対象として追加
		for (auto it = cell->GetObjectList().begin(); it != cell->GetObjectList().end();)
		{
			collisionList.push_back(*it);
			it++;
		}

		return true;
	}

private:
	static constexpr uint32_t maxLevel = 8;	//空間分割の最大レベル

	unsigned int iPow[maxLevel + 1];	//べき乗数値配列
	VECTOR regionWidth;				//領域の幅
	VECTOR regionMin;				//領域の最小値
	VECTOR regionMax;				//領域の最大値
	VECTOR regionSideLength;		//最小領域の辺の長さ
	uint32_t cellNumber;			//空間の数
	unsigned int lowestLevel;		//最下位レベル

	std::unordered_map <uint32_t, std::shared_ptr<Cell<T>>> cellArray;		//空間リスト
};