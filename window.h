#pragma once

#include <array>
#include <iostream>
#include <math.h>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#define ZERO_F 0.0f
#define NINETY_F 90.0f
const std::string name = "Two-dimensional graph plot";

class Window {
private:
  sf::RenderWindow &window;

  float percentage(float value, float min, float max);

  float getValue(float per, float min, float max);
  float getCoord(float size, float per);

  float getpercentage(float per, float min, float max);

  void drawLine(sf::Color color, std::pair<sf::Vector2f, sf::Vector2f> &&pos);

  void drawText(const sf::Font &font, std::string &&str, sf::Vector2f &&p,
                float rotate);

  int calculateDegee(float value);
  std::string parseValueToString(float value);

public:
  void draw(const std::vector<std::pair<float, float>> &filtered_data,
            const std::string &fontpath, float minX, float maxX, float minY,
            float maxY);
  Window(sf::RenderWindow &wd) : window(wd) {}
  ~Window() { window.close(); }
};
