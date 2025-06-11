#pragma once

class Field : public BaseObject
{
private:
	bool isPush;
	bool isPoly;
	int boxHandle;
	int meshHandle;
public:

	Field(const char* path);
	~Field();

	void Initialize()override;
	void Update()override;
	void Draw()override;

	int GetMeshHandle() { return meshHandle; }

};

