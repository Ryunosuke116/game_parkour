#pragma once
#include "BaseManager.h"
#include <vector>
#include <memory>
class IObject;

class TitleObjectManager :
    public BaseManager
{
public:
	TitleObjectManager();
	~TitleObjectManager();

	void Create()override;
	void Initialize()override;
	void Update()override;
	void Draw()override;
private:
	std::vector<std::shared_ptr<IObject>> objects;
};

