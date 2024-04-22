#include "GameState_Play.h"
#include "Common.h"
#include "Physics.h"
#include "Assets.h"
#include "GameEngine.h"
#include "Components.h"
#include <cmath>
#include <fstream>

GameState_Play::GameState_Play(GameEngine &game, const std::string &levelPath)
    : GameState(game), m_levelPath(levelPath)
{
    init(m_levelPath);
}

void GameState_Play::init(const std::string &levelPath)
{
    loadLevel(levelPath);

    spawnPlayer();

    // some sample entities
    auto brick = m_entityManager.addEntity("tile");
    brick->addComponent<CTransform>(Vec2(100, 400));
    brick->addComponent<CAnimation>(m_game.getAssets().getAnimation("Brick"), true);

    if (brick->getComponent<CAnimation>()->animation.getName() == "Brick")
    {
        std::cout << "This could be a good way of identifying if a tile is a brick!\n";
    }

    auto block = m_entityManager.addEntity("tile");
    block->addComponent<CTransform>(Vec2(200, 400));
    block->addComponent<CAnimation>(m_game.getAssets().getAnimation("Block"), true);
    // add a bounding box, this will now show up if we press the 'F' key
    block->addComponent<CBoundingBox>(m_game.getAssets().getAnimation("Block").getSize());

    auto question = m_entityManager.addEntity("tile");
    question->addComponent<CTransform>(Vec2(300, 400));
    question->addComponent<CAnimation>(m_game.getAssets().getAnimation("Question"), true);
}

void GameState_Play::loadLevel(const std::string &filename)
{
    // reset the entity manager every time we load a level
    m_entityManager = EntityManager();

    // TODO: read in the level file and add the appropriate entities
    //       use the PlayerConfig struct m_playerConfig to store player properties
    //       this struct is defined at the top of GameState_Play.h
    std::ifstream file;
    file.open(filename);
    std::string word;
    std::string animation_name;
    float x;
    float y;
    while (file >> word)
    {
        if (word == "Dec")
        {
            file >> animation_name >> x >> y;
            auto entity = m_entityManager.addEntity("dec");
            entity->addComponent<CTransform>(Vec2(x, y));
            entity->addComponent<CAnimation>(m_game.getAssets().getAnimation(animation_name), true);
        }
        else if (word == "Tile")
        {
            file >> animation_name >> x >> y;
            auto entity = m_entityManager.addEntity("tile");
            entity->addComponent<CTransform>(Vec2(x, y));
            entity->addComponent<CAnimation>(m_game.getAssets().getAnimation(animation_name), true);
        }
        else if (word == "Player")
        {
            file >> m_playerConfig.X >> m_playerConfig.Y >>
                m_playerConfig.CX >> m_playerConfig.CY >>
                m_playerConfig.SPEED >> m_playerConfig.MAXSPEED >>
                m_playerConfig.JUMP >> m_playerConfig.GRAVITY >> m_playerConfig.WEAPON;
        }
    }
}

void GameState_Play::spawnPlayer()
{
    // here is a sample player entity which you can use to construct other entities
    m_player = m_entityManager.addEntity("player");
    m_player->addComponent<CTransform>(Vec2(m_playerConfig.X, m_playerConfig.Y));
    m_player->addComponent<CBoundingBox>(Vec2(m_playerConfig.CX, m_playerConfig.CY));
    m_player->addComponent<CAnimation>(m_game.getAssets().getAnimation("Stand"), true);
    m_player->addComponent<CInput>();
    m_player->addComponent<CState>("stand");
    m_player->addComponent<CGravity>(m_playerConfig.GRAVITY);

    // TODO: be sure to add the remaining components to the player
}

void GameState_Play::spawnBullet(std::shared_ptr<Entity> entity)
{
    // TODO: this should spawn a bullet at the given entity, going in the direction the entity is facing
    auto bullet = m_entityManager.addEntity("bullet");
    float direction = 11.0f;
    if (m_player->getComponent<CTransform>()->scale.x < 0)
    {
        direction = -11.0f;
    }

    bullet->addComponent<CAnimation>(m_game.getAssets().getAnimation(m_playerConfig.WEAPON), true);
    bullet->addComponent<CTransform>(Vec2(entity->getComponent<CTransform>()->pos.x, entity->getComponent<CTransform>()->pos.y),
                                     Vec2(direction, 0.0f),
                                     Vec2(1.0f, 1.0f),
                                     0.0f);
    bullet->addComponent<CLifeSpan>(40);
    bullet->addComponent<CBoundingBox>(Vec2(m_game.getAssets().getAnimation(m_playerConfig.WEAPON).getSize().x,
                                            m_game.getAssets().getAnimation(m_playerConfig.WEAPON).getSize().y));
}

void GameState_Play::update()
{
    m_entityManager.update();

    // TODO: implement pause functionality

    sMovement();
    sLifespan();
    sCollision();
    sAnimation();
    sUserInput();
    sRender();
}

void GameState_Play::sMovement()
{
    // TODO: Implement player movement / jumping based on its CInput component
    // TODO: Implement gravity's effect on the player
    // TODO: Implement the maxmimum player speed in both X and Y directions
    if (m_player->getComponent<CInput>()->up == true)
    {
        m_player->getComponent<CTransform>()->pos.y += m_playerConfig.JUMP;
    }
    else if (m_player->getComponent<CInput>()->left == true)
    {
        m_player->getComponent<CTransform>()->pos.x -= m_playerConfig.SPEED;
    }
    else if (m_player->getComponent<CInput>()->right == true)
    {
        m_player->getComponent<CTransform>()->pos.x += m_playerConfig.SPEED;
    }
}

void GameState_Play::sLifespan()
{
    // TODO: Check lifespawn of entities that have them, and destroy them if they go over
    for (auto &e : m_entityManager.getEntities())
    {
        if (e->hasComponent<CLifeSpan>())
        {
            if (e->getComponent<CLifeSpan>()->lifespan < 0)
            {
                e->destroy();
            }
            else
            {
                e->getComponent<CLifeSpan>()->lifespan -= 1;
            }
        }
    }
}

void GameState_Play::sCollision()
{
    // TODO: Implement Physics::GetOverlap() function, use it inside this function

    // TODO: Implement bullet / tile collisions
    //       Destroy the tile if it has a Brick animation
    // TODO: Implement player / tile collisions and resolutions
    //       Update the CState component of the player to store whether
    //       it is currently on the ground or in the air. This will be
    //       used by the Animation system
    // TODO: Check to see if the player has fallen down a hole (y < 0)
    // TODO: Don't let the player walk off the left side of the map
}

void GameState_Play::sUserInput()
{
    // TODO: implement the rest of the player input

    sf::Event event;
    while (m_game.window().pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            m_game.quit();
        }

        if (event.type == sf::Event::KeyPressed)
        {
            switch (event.key.code)
            {
            case sf::Keyboard::Escape:
            {
                m_game.popState();
                break;
            }
            case sf::Keyboard::Z:
            {
                init(m_levelPath);
                break;
            }
            case sf::Keyboard::R:
            {
                m_drawTextures = !m_drawTextures;
                break;
            }
            case sf::Keyboard::F:
            {
                m_drawCollision = !m_drawCollision;
                break;
            }
            case sf::Keyboard::P:
            {
                setPaused(!m_paused);
                break;
            }
            case sf::Keyboard::W:
            {
                m_player->getComponent<CInput>()->up = true;
                m_player->getComponent<CState>()->state = "Air";
                break;
            }
            case sf::Keyboard::A:
            {
                m_player->getComponent<CInput>()->left = true;
                m_player->getComponent<CState>()->state = "Run";
                m_player->getComponent<CTransform>()->scale = {-1.0f, 1};
                break;
            }
            case sf::Keyboard::D:
            {
                m_player->getComponent<CInput>()->right = true;
                m_player->getComponent<CState>()->state = "Run";
                m_player->getComponent<CTransform>()->scale = {1, 1};
                break;
            }
            case sf::Keyboard::Space:
            {
                spawnBullet(m_player);
                break;
            }
            }
        }

        if (event.type == sf::Event::KeyReleased)
        {
            switch (event.key.code)
            {
            case sf::Keyboard::Escape:
            {
                break;
            }
            case sf::Keyboard::W:
            {
                m_player->getComponent<CInput>()->up = false;
                m_player->getComponent<CState>()->state = "Stand";
                break;
            }
            case sf::Keyboard::A:
            {
                m_player->getComponent<CInput>()->left = false;
                m_player->getComponent<CState>()->state = "Stand";
                break;
            }
            case sf::Keyboard::D:
            {
                m_player->getComponent<CInput>()->right = false;
                m_player->getComponent<CState>()->state = "Stand";
                break;
            }
            }
        }
    }
}

void GameState_Play::sAnimation()
{
    // TODO: set the animation of the player based on its CState component
    // TODO: for each entity with an animation, call entity->getComponent<CAnimation>()->animation.update()
    //       if the animation is not repeated, and it has ended, destroy the entity
    if (m_player->getComponent<CState>()->state == m_player->getComponent<CAnimation>()->animation.getName())
    {
    }
    else
    {
        if (m_player->getComponent<CState>()->state == "Stand")
        {
            m_player->getComponent<CAnimation>()->animation = m_game.getAssets().getAnimation("Stand");
        }
        else if (m_player->getComponent<CState>()->state == "Air")
        {
            m_player->getComponent<CAnimation>()->animation = m_game.getAssets().getAnimation("Air");
        }
        else if (m_player->getComponent<CState>()->state == "Run")
        {
            m_player->getComponent<CAnimation>()->animation = m_game.getAssets().getAnimation("Run");
        }
    }
    for (auto &e : m_entityManager.getEntities())
    {
        if (e->hasComponent<CLifeSpan>())
        {
            if (e->getComponent<CLifeSpan>()->lifespan == 5)
            {
                e->getComponent<CAnimation>()->animation = m_game.getAssets().getAnimation("Explosion");
            }
        }
        e->getComponent<CAnimation>()->animation.update();
    }
}

// this function has been completed for you
void GameState_Play::sRender()
{
    // color the background darker so you know that the game is paused
    if (!m_paused)
    {
        m_game.window().clear(sf::Color(100, 100, 255));
    }
    else
    {
        m_game.window().clear(sf::Color(50, 50, 150));
    }

    // set the viewport of the window to be centered on the player if it's far enough right
    auto pPos = m_player->getComponent<CTransform>()->pos;
    float windowCenterX = fmax(m_game.window().getSize().x / 2.0f, pPos.x);
    sf::View view = m_game.window().getView();
    view.setCenter(windowCenterX, m_game.window().getSize().y - view.getCenter().y);
    m_game.window().setView(view);

    // draw all Entity textures / animations
    if (m_drawTextures)
    {
        for (auto e : m_entityManager.getEntities())
        {
            auto transform = e->getComponent<CTransform>();

            transform->pos.x = transform->pos.x + transform->speed.x;
            transform->pos.y = transform->pos.y + transform->speed.y;

            if (e->hasComponent<CAnimation>())
            {
                auto animation = e->getComponent<CAnimation>()->animation;
                animation.getSprite().setRotation(transform->angle);
                animation.getSprite().setPosition(transform->pos.x, m_game.window().getSize().y - transform->pos.y); // Reverse SFML y-co-ordinate system
                animation.getSprite().setScale(transform->scale.x, transform->scale.y);
                m_game.window().draw(animation.getSprite());
            }
        }
    }

    // draw all Entity collision bounding boxes with a rectangleshape
    if (m_drawCollision)
    {
        for (auto e : m_entityManager.getEntities())
        {
            if (e->hasComponent<CBoundingBox>())
            {
                auto box = e->getComponent<CBoundingBox>();
                auto transform = e->getComponent<CTransform>();
                sf::RectangleShape rect;
                rect.setSize(sf::Vector2f(box->size.x - 1, box->size.y - 1));
                rect.setOrigin(sf::Vector2f(box->halfSize.x, box->halfSize.y));
                rect.setPosition(transform->pos.x, m_game.window().getSize().y - transform->pos.y);
                rect.setFillColor(sf::Color(0, 0, 0, 0));
                rect.setOutlineColor(sf::Color(255, 255, 255, 255));
                rect.setOutlineThickness(1);
                m_game.window().draw(rect);
            }
        }
    }

    m_game.window().display();
}
