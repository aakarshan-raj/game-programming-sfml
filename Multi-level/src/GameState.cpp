#include "GameState.h"

GameState::GameState(GameEngine &game) : m_game(game)
{
}

void GameState::setPaused(bool pause)
{
    paused = pause;
}