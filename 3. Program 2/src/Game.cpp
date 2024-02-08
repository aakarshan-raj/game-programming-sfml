#include "Game.h"
#include <iostream>
#include <random>
#include <math.h>
#include <fstream>

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
			sMovement();
			sCollision();
		}
		sUserInput();
		sRender();
		++m_currentFrame;
		if (m_currentFrame % 100 == 0)
		{
			sEnemySpawner();
		}
	}
}

void Game::init(const std::string &config)
{
	std::ifstream file_handler;
	file_handler.open(config);
	if (file_handler.fail())
	{
		std::cout << "Error reading configurations." << std::endl;
		exit(-1);
	}
	std::string data;
	int WIDTH;
	int HEIGHT;
	int FRAME;
	bool FULL_SCREEN;
	std::string font_path;
	int font_size;
	int R, B, G;
	while (file_handler >> data)
	{
		if (data == "Window")
		{
			file_handler >> WIDTH;
			file_handler >> HEIGHT;
			file_handler >> FRAME;
			file_handler >> FULL_SCREEN;
		}
		else if (data == "Font")
		{
			file_handler >> font_path;
			file_handler >> font_size;
			file_handler >> R;
			file_handler >> G;
			file_handler >> B;
		}
		else if (data == "Player")
		{
			file_handler >> m_playerConfig.SR;
			file_handler >> m_playerConfig.CR;
			file_handler >> m_playerConfig.FR;
			file_handler >> m_playerConfig.FG;
			file_handler >> m_playerConfig.FB;
			file_handler >> m_playerConfig.OR;
			file_handler >> m_playerConfig.OG;
			file_handler >> m_playerConfig.OB;
			file_handler >> m_playerConfig.OT;
			file_handler >> m_playerConfig.V;
			file_handler >> m_playerConfig.S;
		}
		else if (data == "Enemy")
		{
			file_handler >> m_enemyConfig.SR;
			file_handler >> m_enemyConfig.CR;
			file_handler >> m_enemyConfig.OR;
			file_handler >> m_enemyConfig.OG;
			file_handler >> m_enemyConfig.OB;
			file_handler >> m_enemyConfig.OT;
			file_handler >> m_enemyConfig.VMIN;
			file_handler >> m_enemyConfig.VMAX;
			file_handler >> m_enemyConfig.L;
			file_handler >> m_enemyConfig.SI;
			file_handler >> m_enemyConfig.SMIN;
			file_handler >> m_enemyConfig.SMAX;
		}
		else if (data == "Bullet")
		{
			file_handler >> m_bulletConfig.SR;
			file_handler >> m_bulletConfig.CR;
			file_handler >> m_bulletConfig.FR;
			file_handler >> m_bulletConfig.FG;
			file_handler >> m_bulletConfig.FB;
			file_handler >> m_bulletConfig.OR;
			file_handler >> m_bulletConfig.OG;
			file_handler >> m_bulletConfig.OB;
			file_handler >> m_bulletConfig.OT;
			file_handler >> m_bulletConfig.V;
			file_handler >> m_bulletConfig.L;
			file_handler >> m_bulletConfig.SB;
			file_handler >> m_bulletConfig.S;
		}
	}

	m_window.create(sf::VideoMode(WIDTH, HEIGHT), "Game");
	m_window.setFramerateLimit(FRAME);
	spawnPlayer();
	spawnEnemy();
	if (!m_font.loadFromFile(font_path))
	{
		std::cout << "No Fonts Found" << std::endl;
		exit(-1);
	}
	
	m_scoreText.setString("Score: 0");
	m_scoreText.setFont(m_font);
    m_scoreText.setCharacterSize(font_size);
	m_scoreText.setPosition(0,0);
	m_scoreText.setColor(sf::Color(R,G,B));

}

void Game::setPaused(bool paused)
{
}

void Game::sMovement()
{

	if (m_player->cInput->up == true)
	{
		if (m_player->cShape->circle.getPosition().y - m_player->cCollision->radius > 0)
			m_player->cTransform->pos.y -= m_playerConfig.S;
	}
	if (m_player->cInput->down == true)
	{
		if (m_player->cShape->circle.getPosition().y + m_player->cCollision->radius < m_window.getSize().y)
			m_player->cTransform->pos.y += m_playerConfig.S;
	}
	if (m_player->cInput->left == true)
	{
		if (m_player->cShape->circle.getPosition().x - m_player->cCollision->radius > 0)
			m_player->cTransform->pos.x -= m_playerConfig.S;
	}
	if (m_player->cInput->right == true)
	{
		if (m_player->cShape->circle.getPosition().x + m_player->cCollision->radius < m_window.getSize().x)
			m_player->cTransform->pos.x += m_playerConfig.S;
	}
	for (auto &x : m_entities.getEntities("Enemy"))
	{
		if (x->cTransform->pos.x + x->cCollision->radius > m_window.getSize().x || x->cTransform->pos.x - x->cCollision->radius < 0)
			x->cTransform->velocity.x *= -1;

		if (x->cTransform->pos.y + x->cCollision->radius > m_window.getSize().y || x->cTransform->pos.y - x->cCollision->radius < 0)
			x->cTransform->velocity.y *= -1;
	}
	for (auto &x : m_entities.getEntities())
	{
		x->cTransform->pos.x += x->cTransform->velocity.x;
		x->cTransform->pos.y += x->cTransform->velocity.y;
		x->cShape->circle.setPosition(x->cTransform->pos.x + x->cTransform->velocity.x,
									  x->cTransform->pos.y + x->cTransform->velocity.y);

		x->cShape->circle.setRotation(m_player->cShape->circle.getRotation() + m_player->cTransform->angle);
	}
}

void Game::sUserInput()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_window.close();
		}
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Q)
			{
				m_window.close();
			}
			if (event.key.code == sf::Keyboard::W)
			{
				m_player->cInput->up = true;
			}
			if (event.key.code == sf::Keyboard::S)
			{
				m_player->cInput->down = true;
			}
			if (event.key.code == sf::Keyboard::A)
			{
				m_player->cInput->left = true;
			}
			if (event.key.code == sf::Keyboard::D)
			{
				m_player->cInput->right = true;
			}
			if (event.key.code == sf::Keyboard::P)
			{
				m_paused = !m_paused;
			}
		}
		if (event.type == sf::Event::KeyReleased)
		{
			if (event.key.code == sf::Keyboard::W)
			{
				m_player->cInput->up = false;
			}
			if (event.key.code == sf::Keyboard::S)
			{
				m_player->cInput->down = false;
			}
			if (event.key.code == sf::Keyboard::A)
			{
				m_player->cInput->left = false;
			}
			if (event.key.code == sf::Keyboard::D)
			{
				m_player->cInput->right = false;
			}
		}
		if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				Vec2 pos(event.mouseButton.x, event.mouseButton.y);
				spawnBullet(m_player, pos);
			}
		}
	}
}

void Game::sLifeSpan()
{
	for (auto &x : m_entities.getEntities("Bullet"))
	{
		if (x->cLifespan->remaining == 0)
		{
			x->destroy();
		}
		else
		{
			x->cLifespan->remaining -= 1;
			auto in_color = x->cShape->circle.getFillColor();
			in_color.a -= 2;
			auto out_color = x->cShape->circle.getOutlineColor();
			out_color.a -= 2;
			x->cShape->circle.setFillColor(in_color);
			x->cShape->circle.setOutlineColor(out_color);
		}
	}
	for (auto &x : m_entities.getEntities("Small Enemy"))
	{
		if (x->cLifespan->remaining == 0)
		{
			x->destroy();
		}
		else
		{
			x->cLifespan->remaining -= 1;
			auto in_color = x->cShape->circle.getFillColor();
			in_color.a -= 20;
			auto out_color = x->cShape->circle.getOutlineColor();
			out_color.a -= 20;
			x->cShape->circle.setFillColor(in_color);
			x->cShape->circle.setOutlineColor(out_color);
		}
	}
}

void Game::sRender()
{
	
	m_scoreText.setString("Score:"+ std::to_string(m_player->cScore->score));
	m_window.clear();
	for (const std::shared_ptr<Entity> &x : m_entities.getEntities())
	{
		m_window.draw(x->cShape->circle);
	}
	m_window.draw(m_scoreText);
	m_window.display();
}

void Game::sCollision()
{
	for (auto &x : m_entities.getEntities("Bullet"))
	{
		for (auto &y : m_entities.getEntities("Enemy"))
		{
			if (sqrt((x->cTransform->pos.x - y->cTransform->pos.x) * (x->cTransform->pos.x - y->cTransform->pos.x) + (x->cTransform->pos.y - y->cTransform->pos.y) * (x->cTransform->pos.y - y->cTransform->pos.y)) <=
				x->cCollision->radius + y->cCollision->radius)
			{
				x->destroy();
				y->destroy();
				spawnSmallEnemies(y);
				m_player->cScore->score++;
			}
		}
	}
	for (auto &x : m_entities.getEntities("Enemy"))
	{
		for (auto &y : m_entities.getEntities("Player"))
		{
			if (sqrt((x->cTransform->pos.x - y->cTransform->pos.x) * (x->cTransform->pos.x - y->cTransform->pos.x) + (x->cTransform->pos.y - y->cTransform->pos.y) * (x->cTransform->pos.y - y->cTransform->pos.y)) <=
				x->cCollision->radius + y->cCollision->radius)
			{
				x->destroy();
				spawnSmallEnemies(x);
				m_player->cScore->score--;
			}
		}
	}
}

void Game::spawnPlayer()
{

	m_player = m_entities.addEntity("Player");
	m_player->cTransform = std::make_shared<CTransform>(Vec2(400, 200), Vec2(0, 0), 3);
	m_player->cCollision = std::make_shared<CCollision>(m_playerConfig.CR);
	m_player->cShape = std::make_shared<CShape>(m_playerConfig.SR, m_playerConfig.V, sf::Color(m_playerConfig.FR, m_playerConfig.FG, m_playerConfig.FB), sf::Color(m_playerConfig.OR, m_playerConfig.OG, m_playerConfig.OB), m_playerConfig.OT);
	m_player->cShape->circle.setPosition(m_player->cTransform->pos.x, m_player->cTransform->pos.y);
	m_player->cInput = std::make_shared<CInput>();
	m_player->cScore = std::make_shared<CScore>(0);
}
void Game::sEnemySpawner()
{
	spawnEnemy();
}
void Game::spawnEnemy()
{
	auto enemy = m_entities.addEntity("Enemy");

	std::random_device dev;
	std::mt19937 mt(dev());
	enemy->cCollision = std::make_shared<CCollision>(m_enemyConfig.CR);

	std::uniform_real_distribution<float> x(0 + enemy->cCollision->radius, m_window.getSize().x - enemy->cCollision->radius);
	std::uniform_real_distribution<float> y(0 + enemy->cCollision->radius, m_window.getSize().y - enemy->cCollision->radius);

	std::uniform_real_distribution<float> x_speed(m_enemyConfig.SMIN, m_enemyConfig.SMAX);
	std::uniform_real_distribution<float> y_speed(m_enemyConfig.SMIN, m_enemyConfig.SMAX);

	std::uniform_int_distribution<int> colorx(0, 256);
	std::uniform_int_distribution<int> sizex(m_enemyConfig.VMIN, m_enemyConfig.VMAX);

	enemy->cTransform = std::make_shared<CTransform>(Vec2(x(mt), y(mt)), Vec2(x_speed(mt), y_speed(mt)), 3);
	enemy->cShape = std::make_shared<CShape>(m_enemyConfig.SR, sizex(mt), sf::Color(colorx(mt), colorx(mt), colorx(mt)), sf::Color(m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB), m_enemyConfig.OT);
	enemy->cShape->circle.setPosition(enemy->cTransform->pos.x, enemy->cTransform->pos.y);
}

void Game::spawnSmallEnemies(std::shared_ptr<Entity> parent)
{
	Vec2 pos;
	Vec2 Velocity;
	int sides = parent->cShape->circle.getPointCount();
	float degree = 360 / sides;

	for (int i = 1; i <= sides; i++)
	{
		float angle = (degree * (3.14 / 180)) * i;
		float vx = cos(angle);
		float vy = sin(angle);
		float z = sqrt(parent->cTransform->pos.x * parent->cTransform->pos.x +
					   parent->cTransform->pos.y + parent->cTransform->pos.y);
		vx = vx / z;
		vy = vy / z;

		float velocity = 3000;
		vx *= velocity;
		vy *= velocity;

		auto small = m_entities.addEntity("Small Enemy");
		small->cTransform = std::make_shared<CTransform>(Vec2(parent->cTransform->pos.x, parent->cTransform->pos.y),
														 Vec2(vx, vy), 10);
		small->cShape = std::make_shared<CShape>(10, parent->cShape->circle.getPointCount(), sf::Color(parent->cShape->circle.getFillColor()), sf::Color(parent->cShape->circle.getOutlineColor()), m_enemyConfig.OT);
		small->cLifespan = std::make_shared<CLifeSpan>(m_enemyConfig.L);
	}
}

void Game::spawnBullet(std::shared_ptr<Entity> shooter, const Vec2 &mousePos)
{
	float a = mousePos.x - shooter->cTransform->pos.x;
	float b = mousePos.y - shooter->cTransform->pos.y;

	float hypo = sqrt(a * a + b * b);
	float x = a / hypo;
	float y = b / hypo;
	float velocity = m_bulletConfig.S;

	x *= velocity;
	y *= velocity;
	auto bullet = m_entities.addEntity("Bullet");
	bullet->cCollision = std::make_shared<CCollision>(m_bulletConfig.CR);
	bullet->cTransform = std::make_shared<CTransform>(Vec2(shooter->cTransform->pos.x,
														   shooter->cTransform->pos.y),
													  Vec2(x, y), 10);
	bullet->cShape = std::make_shared<CShape>(m_bulletConfig.SR, m_bulletConfig.V, sf::Color(m_bulletConfig.FR, m_bulletConfig.FG, m_bulletConfig.FB), sf::Color(m_bulletConfig.OR, m_bulletConfig.OG, m_bulletConfig.OB), m_bulletConfig.OT);
	bullet->cLifespan = std::make_shared<CLifeSpan>(m_bulletConfig.L);
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
}
