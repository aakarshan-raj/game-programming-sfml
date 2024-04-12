#include "GameEngine.h"

GameEngine::GameEngine(const std::string &path)
{
    init(path);
}

void GameEngine::init(const std::string &path)
{
    m_window.create(sf::VideoMode(700, 800), "Game");

    pushState(std::make_shared<GameState_Menu>(*this));
}

void GameEngine::pushState(std::shared_ptr<GameState> state)
{
    m_states.push_back(state);
}

bool GameEngine::isRunning()
{
    return m_running & m_window.isOpen();
}


sf::RenderWindow &GameEngine::window()
{
    return m_window;
}

void GameEngine::run()
{
    while (isRunning())
    {
        update();
    }
}

void GameEngine::popState()
{
    m_popStates++;
}

void GameEngine::quit()
{
    m_running = false;
}

void GameEngine::update()
{
    if (!isRunning())
    {
        return;
    }

    for (size_t i = 0; i < m_popStates; i++)
    {
        if (!m_states.empty())
            m_states.pop_back();
    }
    m_popStates = 0;

    for (size_t i = 0; i < m_statesToPush.size(); i++)
    {
        if (!m_states.empty())
            m_states.push_back(m_statesToPush[i]);
    }
    m_statesToPush.clear();

    if (!m_states.empty())
    {
        m_states.back()->update();
    }
}