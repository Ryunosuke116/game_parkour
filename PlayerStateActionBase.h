#pragma once
class PlayerStateActionBase
{
public:
	PlayerStateActionBase();
	~PlayerStateActionBase() {};

	void MotionUpdate();

protected:
	int modelHandle;

};

