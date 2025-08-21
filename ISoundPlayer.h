#pragma once
#include <iostream>
#include <string>

class ISoundPlayer
{
public:
	//ƒˆ‰¼‘zŠÖ”	
	virtual void Create()		abstract;
	virtual void Play(const std::string& name)abstract;
	virtual void Stop(const std::string& name)abstract;
	virtual ~ISoundPlayer() = default;
};