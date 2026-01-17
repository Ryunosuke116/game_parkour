#pragma once
#include "BaseScene.h"
#include "BaseManager.h"
#include <unordered_map>

class SceneManager : public BaseManager
{
public:
    SceneManager();
    ~SceneManager();

    template <class SceneType>
    void Add(const std::string_view name)
    {
        scenes[name] = new SceneType{ *this };

        if (nowScene == nullptr)
        {
            nowScene = scenes[name];
            nowScene->Create();
            nowScene->Initialize();
        }
    }

    void Create()override;
    void Initialize()override;
    void Update()override;
    void Draw()override;

    void ChangeScene(std::string_view name, const int coinCount);

private:
    std::unordered_map<std::string_view, BaseScene*> scenes;

    BaseScene*                  nowScene;
    std::shared_ptr<BaseScene>  nowScenee;
};

