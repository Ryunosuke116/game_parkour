#pragma once
#include "CollisionObjectManager.h"
#include "Camera.h"
#include <unordered_map>
#include <memory>
#include <typeindex>

class IObject;

class WorldSubSystem
{
public:
	static WorldSubSystem& GetInstance();

	/// <summary>
	/// 管理したいオブジェクトを追加
	/// </summary>
	/// <typeparam name="Type"></typeparam>
	/// <param name="system"></param>
	template <typename Type>
	void AddSubSystem(const std::shared_ptr<Type>& system)
	{
		static_assert(std::is_base_of<IObject, Type>::value, "TypeはIObjectクラスではありません");

		subSystems[typeid(Type)] = system;
	}

	/// <summary>
	/// 指定されたオブジェクトを渡す
	/// </summary>
	/// <typeparam name="Type"></typeparam>
	/// <returns></returns>
	template<typename Type>
	std::shared_ptr<Type> GetSubSystem() const
	{
		auto it = subSystems.find(typeid(Type));

		//オブジェクトを管理していない場合はnullptrで返す
		if (it == subSystems.end())
		{
			return nullptr;
		}

		//指定された型にアップキャストした状態で返す
		if (auto object = it->second.lock())
		{
			return std::dynamic_pointer_cast<Type>(object);
		}

		return nullptr;
	}

	void ShutdownAll();

private:
	std::unordered_map<std::type_index, std::weak_ptr<IObject>> subSystems;
};

