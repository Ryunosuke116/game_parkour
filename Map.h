#pragma once
class Map : public BaseObject
{
private:

public:
	Map();
	Map(const char* path);
	~Map();
	void Initialize()override;
	void Update()override;
	void Draw()override;

};


