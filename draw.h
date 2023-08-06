#pragma once

#include <array>
#include <iostream>
#include <math.h>

#include <SFML/Graphics.hpp>

class Draw {
private:
  float percentage(float value, float min, float max);

  float getValue(float per, float min, float max);
  float getCoord(float size, float per);

  float getpercentage(float per, float min, float max);

  void drawLine(sf::RenderWindow &window, sf::Color color,
                std::pair<sf::Vector2f, sf::Vector2f> &&pos);

  void drawText(sf::RenderWindow &window, const sf::Font &font,
                std::string &&str, sf::Vector2f &&p, float rotate);

  int calculateDegee(float value);
  std::string parseValueToString(float value);

public:
  void draw(sf::RenderWindow &window,
            const std::vector<std::pair<float, float>> &filtered_data,
            const std::string &fontpath, float minX, float maxX, float minY,
            float maxY);
};
