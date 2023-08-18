#pragma once

enum class GameState
{
	INVALID = 0, RUNNING, GAME_LOST, GAME_WON, GO_TO_NEXT_SCENE
};

class IScene
{
public:
	virtual bool init() = 0;
	virtual bool loadMedia() = 0;
	virtual void update() = 0;
	virtual void handleEvents() = 0;
	virtual void draw() = 0;

	GameState getGameState() const
	{
		return _gameState;
	}

protected:
	GameState _gameState = GameState::RUNNING;

};

