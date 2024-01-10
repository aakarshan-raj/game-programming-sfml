#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <memory>
#include <map>

const int width = 1200;
const int height = 800;
const int FrameRate = 1000;
const char *Title = "Bound It!";
const char *BottomText = "Sample Text";

struct font
{
    sf::Font f;
    float size;
    int R, G, B;
};

void read_file(char *,
               std::map<std::string,
                        std::pair<std::pair<
                                      std::shared_ptr<sf::Shape>,
                                      std::shared_ptr<sf::Text>>,
                                  std::pair<float, float>>> &,
               std::shared_ptr<sf::RenderWindow> &,
               font &);

int main()
{
    std::shared_ptr<sf::RenderWindow> window = std::make_shared<sf::RenderWindow>();
    std::map<std::string, std::pair<std::pair<std::shared_ptr<sf::Shape>, std::shared_ptr<sf::Text>>, std::pair<float, float>>> Stuff;
    font f1;
    read_file("config/config", Stuff, window, f1);

    window->setFramerateLimit(FrameRate);

    while (window->isOpen())
    {
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window->close();
            }
        }

        window->clear();
        for (auto &[key, value] : Stuff)
        {
            window->draw(*(value.first.first));
            value.first.first->move(value.second.first, value.second.second);

            if ((value.first.first)->getPosition().x < 0)
            {
                value.second.first *= -1;
            }
            if ((value.first.first)->getPosition().y < 0)
            {
                value.second.second *= -1;
            }
            if ((value.first.first)->getPosition().x + value.first.first->getLocalBounds().width > window->getSize().x)
            {
                value.second.first *= -1;
            }
            if ((value.first.first)->getPosition().y + value.first.first->getLocalBounds().height > window->getSize().y)
            {
                value.second.second *= -1;
            }
            value.first.second->setPosition(value.first.first->getPosition().x+(value.first.first->getLocalBounds().width/2)-value.first.second->getLocalBounds().width/2,
                                            value.first.first->getPosition().y+(value.first.first->getLocalBounds().height/2)-value.first.second->getLocalBounds().height/2);

            window->draw(*value.first.second);
        }

        window->display();
    }
    return 0;
}

void read_file(char *filepath, std::map<std::string, std::pair<std::pair<std::shared_ptr<sf::Shape>, std::shared_ptr<sf::Text>>, std::pair<float, float>>> &Stuff,
               std::shared_ptr<sf::RenderWindow> &window, font &font)
{
    std::ifstream file_handler;
    file_handler.open(filepath);
    std::string line;
    while (file_handler >> line)
    {
        if (line == "Window")
        {
            float width, height;
            file_handler >> width >> height;
            window->create(sf::VideoMode(width, height), "bounce");
        }
        else if (line == "Font")
        {
            std::string font_path;
            float size;
            int R;
            int G;
            int B;
            file_handler >> font_path >> size >> R >> G >> B;
            if (!font.f.loadFromFile(font_path))
            {
                std::cout << "error in importing font" << std::endl;
                exit(-1);
            }
            font.size = size;
            font.R = R;
            font.G = G;
            font.B = B;
        }
        else if (line == "Circle")
        {
            std::string name;
            float x, y, x_speed, y_speed, radius;
            int R, G, B;
            file_handler >> name >> x >> y >> x_speed >> y_speed >> R >> G >> B >> radius;
            std::shared_ptr<sf::Shape> c1 = std::make_shared<sf::CircleShape>(radius);
            c1->setPosition(x, y);
            c1->setFillColor(sf::Color(R, G, B));
            std::shared_ptr<sf::Text> f1 = std::make_shared<sf::Text>(name, font.f);
            f1->setColor(sf::Color(font.R, font.G, font.B));
            f1->setCharacterSize(font.size);
            Stuff.insert(std::make_pair(name, std::pair(std::make_pair(c1, f1), std::pair(x_speed, y_speed))));
        }
        else if (line == "Rectangle")
        {

            std::string name;
            float x, y, x_speed, y_speed, width, height;
            int R, G, B;
            file_handler >> name >> x >> y >> x_speed >> y_speed >> R >> G >> B >> width >> height;
            std::shared_ptr<sf::Shape> r1 = std::make_shared<sf::RectangleShape>(sf::Vector2f(width, height));
            r1->setPosition(x, y);
            r1->setFillColor(sf::Color(R, G, B));
            std::shared_ptr<sf::Text> f1 = std::make_shared<sf::Text>(name, font.f);
            f1->setColor(sf::Color(font.R, font.G, font.B));
            f1->setCharacterSize(font.size);
            Stuff.insert(std::make_pair(name, std::pair(std::make_pair(r1, f1), std::pair(x_speed, y_speed))));
        }
        else
        {
            std::cout << "Undefined Structure" << std::endl;
            exit(-1);
        }
    }
}
//Probaby the worst way to do it