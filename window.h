#pragma once

#include <array>
#include <iostream>
#include <math.h>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

const size_t winX = 1000;
const size_t winY = 500;
const std::string name = "Two-dimensional graph plot";

class Window {
private:
  std::unique_ptr<sf::RenderWindow> window;
  float zoomFactor{1.0f};
  float zoomIncrement{0.1f};
  bool moveLeft{false};
  bool moveRight{false};
  bool moveUp{false};
  bool moveDown{false};
  bool mouseMove{false};

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
  void display(const std::vector<std::pair<float, float>> &data,
               const std::string &fontpath, float minX, float maxX, float minY,
               float maxY);
  void draw(const std::vector<std::pair<float, float>> &filtered_data,
            const std::string &fontpath, float minX, float maxX, float minY,
            float maxY);
  Window() {
    window = std::make_unique<sf::RenderWindow>(
        sf::VideoMode(winX, winY), std::move(name),
        sf::Style::Titlebar | sf::Style::Close);
  }
  ~Window() { window->close(); }
};
