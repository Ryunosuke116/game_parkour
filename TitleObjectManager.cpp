#include "common.h"
#include "TitleObjectManager.h"
#include "BackGround.h"

TitleObjectManager::TitleObjectManager()
{

}

TitleObjectManager::~TitleObjectManager()
{

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

}

void TitleObjectManager::Update()
{

}

void TitleObjectManager::Draw()
{
	for (auto& object : objects)
	{
		object->Draw();
	}
}