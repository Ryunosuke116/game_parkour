#pragma once
#include "BaseScene.h"
#include <unordered_map>

class SceneManager
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

    void Initialize();
    void Update();
    void Draw();

    void ChangeScene(std::string_view name, const int coinCount);

private:
    std::unordered_map<std::string_view, BaseScene*>scenes;

    BaseScene* nowScene;
    std::shared_ptr<BaseScene> nowScenee;
};

