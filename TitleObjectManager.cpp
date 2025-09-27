#include "common.h"
#include "TitleObjectManager.h"
#include "BackGround.h"

TitleObjectManager::TitleObjectManager()
{

}

TitleObjectManager::~TitleObjectManager()
{
	objects.clear();
}

void TitleObjectManager::Create()
{
	//-----------------------------------------//
	// 2D•`‰æ‚·‚éobject‚ğ’Ç‰Á‚·‚éê‡
	// æ‚É•`‰æ‚µ‚½‚¢‡‚ÉƒŠƒXƒg‚É’Ç‰Á‚·‚é
	//----------------------------------------//

	objects.push_back(std::make_shared<BackGround>());

	for (auto& object : objects)
	{
		object->Create();
	}
}

void TitleObjectManager::Initialize()
{
	for (auto& object : objects)
	{
		object->Initialize();
	}
}

void TitleObjectManager::Update()
{
	for (auto& object : objects)
	{
		object->Update();
	}
}

void TitleObjectManager::Draw()
{
	for (auto& object : objects)
	{
		object->Draw();
	}
}