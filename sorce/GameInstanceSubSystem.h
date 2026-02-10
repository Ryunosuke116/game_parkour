#pragma once
#include <unordered_map>
#include <memory>
#include <typeindex>
#include <string>

class IGameInstanceSubSystem;

class GameInstanceSubSystem
{
public:
	static GameInstanceSubSystem& GetInstance();

	/// <summary>
	/// 管理したいオブジェクトを生成+追加
	/// </summary>
	/// <typeparam name="Type"></typeparam>
	/// <typeparam name="...Argument"></typeparam>
	/// <param name="...argument"></param>
	/// <returns></returns>
	template <typename Type, typename... Argument>
	std::weak_ptr<Type> AddSubSystem(Argument&&... argument)
	{
		//型をチェック
		static_assert(std::is_base_of<IGameInstanceSubSystem, Type>::value, "TypeはIsubSystemクラスではありません");
	
		std::shared_ptr<Type> subSystem = std::make_shared<Type>(std::forward<Argument>(argument)...);
		subSystems[typeid(Type)] = subSystem;

		std::weak_ptr<Type> weakSubSystem = subSystem;
		return weakSubSystem;
	}

	/// <summary>
	/// 管理しているオブジェクトを渡す
	/// </summary>
	/// <typeparam name="Type"></typeparam>
	/// <returns></returns>
	template<typename Type>
	std::weak_ptr<Type> GetSubSystem() const
	{
		auto it = subSystems.find(typeid(Type));

		//指定したオブジェクトがあればそのオブジェクトを返す
		if (it != subSystems.end())
		{
			return std::dynamic_pointer_cast<Type>(it->second);
		}
		return std::weak_ptr<Type>();
	}

	void Create();
	void Load(const std::string& sceneName);
	void Update();
	void Draw();

	void ShutdownAll();

private:
	std::unordered_map<std::type_index, std::shared_ptr<IGameInstanceSubSystem>> subSystems;
};

