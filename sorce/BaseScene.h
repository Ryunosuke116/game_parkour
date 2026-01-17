#pragma once

class SceneManager;

class BaseScene
{
public:
    BaseScene(SceneManager& manager);
    ~BaseScene();

    virtual void Create();
    virtual void Initialize();
    virtual void Update();
    virtual void Draw();
    virtual void Shutdown();

    void SetCoinCount(const int set) { coinCount = set; }

protected:
    int coinCount;
    void ChangeScene(std::string_view, const int coinCount);

private:
    SceneManager& manager;
};

