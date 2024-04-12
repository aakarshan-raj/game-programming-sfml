#pragma once
class GameEngine;

class GameState
{
public:
    GameEngine &m_game;
    bool paused = false;

    GameState(GameEngine &game);

    virtual void update() = 0;
    virtual void setPaused(bool pause);
};