#pragma once

#include <vector>
#include <memory>
#include "GameState.h"
#include "GameState_Menu.h"
#include <SFML/Graphics.hpp>


class GameEngine{

        public:
        std::vector<std::shared_ptr<GameState>> m_states;
        std::vector<std::shared_ptr<GameState>> m_statesToPush;
        size_t m_popStates = 0;
        bool m_running = true;
        sf::RenderWindow m_window;

        GameEngine(const std::string &);
        void init(const std::string &);

        bool isRunning();
        sf::RenderWindow & window();
        void update();
        void pushState(std::shared_ptr<GameState>);
        void run();
        void popState();
        void quit();
    
};