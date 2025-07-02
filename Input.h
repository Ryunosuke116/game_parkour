#pragma once
class Input
{
public:
	
	~Input() {};

	virtual void Update()abstract;

	virtual int GetNowFrameInput() const abstract;
	virtual int GetNowFrameNewInput() const abstract;

//private:
//	int		nowFrameInput;				// 現在のフレームで押されているボタン
//	int		nowFrameNewInput;			// 現在のフレームで新たに押されたボタン
};

