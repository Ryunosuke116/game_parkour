#pragma once
#include "BaseGameObjectManager.h"
#include "BaseObject.h"

class CollisionObjectManager : 
	public BaseGameObjectManager,
	public std::enable_shared_from_this<CollisionObjectManager>
{
public:
	CollisionObjectManager();
	~CollisionObjectManager();

	void Create()			override;
	void Initialize()		override;
	void Update()			override;
	void Draw()				override;
	void Add()				override{}
	void ResultCreate()		override;
	void ResultInitialize()	override;
	void ResultUpdate()		override;

	std::vector<std::weak_ptr<BaseObject>> const GetCollisionObjects()
	{
		std::vector<std::weak_ptr<BaseObject>> weakCollisionObjects;
		weakCollisionObjects.reserve(collisionObjects.size());
		for (auto& collisionObject : collisionObjects)
		{
			weakCollisionObjects.emplace_back(collisionObject);
		}

		return weakCollisionObjects;
	}

	/// <summary>
	/// 引数で指定されたオブジェクトを返す
	/// </summary>
	/// <param name="objectTag"></param>
	/// <returns></returns>
	std::shared_ptr<BaseObject> const GetCollisionObject(const std::string& objectTag)
	{
		for (auto& collisionObject : collisionObjects)
		{
			if (collisionObject->GetTag() == objectTag)
			{
				return collisionObject;
			}
		}

		return NULL;
	}

private:

	std::vector<std::shared_ptr<BaseObject>> collisionObjects;
};

