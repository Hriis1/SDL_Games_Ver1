#pragma once

class IScene
{
public:
	virtual bool loadMedia() = 0;
	virtual void update() = 0;
	virtual void draw() = 0;
};

