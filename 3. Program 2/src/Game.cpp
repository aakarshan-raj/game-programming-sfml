#include "Game.h"

Game::Game(const std::string &configFile)
{
	init(configFile);
}

void Game::run()
{
	while (m_running)
	{
		m_entities.update();

		if (!m_paused)
		{
			sLifeSpan();
			sSpawner();
			sMovement();
			sCollision();
		}
		sUserInput();
		sRender();

		// Increment the current frame
		++m_currentFrame;
	}
}

void Game::init(const std::string &config)
{
	// Read in variables
	// create window
	// set frame
	// spawn player
}

void Game::setPaused(bool paused)
{
}

void Game::sMovement()
{
	// All movements here
}

void Game::sUserInput()
{
	// handle all user input, close, pause, up,down,left,right, mouse click, only set state here. No movements here
}

void Game::sLifeSpan()
{
}

void Game::sRender()
{
	// Clear
	//  Logic to change position
	// Draw
	// Display
}

void Game::sSpawner()
{
}

void Game::sCollision()
{
	// check for collisions, call other functions
}

void Game::spawnPlayer()
{
	// add properties to player and spawn it.
}
void Game::sEnemySpawner()
{
	// call spawnEnemy here
}
void Game::spawnEnemy()
{
	// spawn enemy
	// take account of the last frame of spanwed enemy, read doc for reason
}

void Game::spawnSmallEnemies(std::shared_ptr<Entity> parent)
{
}

// Spawns a bullet from a given entity to a target location
void Game::spawnBullet(std::shared_ptr<Entity> shooter, const Vec2 &mousePos)
{
	// calculate velocity, set transform, shape
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
}