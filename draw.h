#include <SFML/Graphics.hpp>
#include <array>
#include <iostream>

#if defined _WIN32
const std::string fontpath = "..\\times new roman.ttf";
#elif defined(__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
const std::string fontpath = "../times new roman.ttf";
#endif

const size_t winX = 1000;
const size_t winY = 500;

float percentage(float value, float min, float max);

void drawLine(sf::RenderWindow &window, sf::Color color,
              std::pair<sf::Vector2f, sf::Vector2f> &&pos);

void drawTest(sf::RenderWindow &window, const sf::Font &font, std::string &&str,
              sf::Vector2f &&p);

void draw(const std::vector<std::pair<float, float>> &filtered_data, float minX,
          float maxX, float minY, float maxY);