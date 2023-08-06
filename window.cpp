#include "window.h"

float Window::percentage(float value, float min, float max) {
  return ((value - min) / (max - min)) * 100;
}

float Window::getpercentage(float per, float min, float max) {
  return (min + (per / 100) * (max - min));
}

float Window::getValue(float per, float min, float max) {
  return (min + (per / 100) * (max - min));
}

float Window::getCoord(float size, float per) { return ((size * per) / 100); }

void Window::drawLine(sf::Color color,
                      std::pair<sf::Vector2f, sf::Vector2f> &&pos) {
  sf::Vertex line[2];
  line[0].color = color;
  line[1].color = color;
  line[0].position = pos.first;
  line[1].position = pos.second;
  window->draw(line, 2, sf::Lines);
}

void Window::drawText(const sf::Font &font, std::string &&str, sf::Vector2f &&p,
                      float rotate) {
  sf::Text text;
  text.setFont(font);
  text.setCharacterSize(12);
  text.setFillColor(sf::Color::Black);
  text.setString(std::move(str));
  text.setPosition(std::move(p));
  text.rotate(rotate);
  window->draw(text);
}

int Window::calculateDegee(float value) {
  int degree = 1;
  if (value == 0) {
    return degree;
  }
  while (static_cast<int>(value) == 0) {
    value = value * 10;
    if (static_cast<int>(value) != 0) {
      return degree;
    }
    degree++;
  }

  return degree;
}

std::string Window::parseValueToString(float value) {
  int degree = calculateDegee(value);
  double scale = pow(10, degree);
  return std::move(std::to_string(value * scale) + "*10^(-" +
                   std::to_string(degree) + ")");
}

void Window::display(const std::vector<std::pair<float, float>> &data,
                     const std::string &fontpath, float minX, float maxX,
                     float minY, float maxY) {
  sf::View view(window->getDefaultView());

  while (window->isOpen()) {
    sf::Event event;
    while (window->pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window->close();
      }
      if (event.type == sf::Event::MouseWheelScrolled) {
        if (event.mouseWheelScroll.delta > 0) {
          // Zoom in
          zoomFactor -= zoomIncrement;
        } else if (event.mouseWheelScroll.delta < 0) {
          // Zoom out
          zoomFactor += zoomIncrement;
        }
        zoomFactor = std::max(0.1f, zoomFactor);
      }
      if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Left) {
          moveLeft = true;
        } else if (event.key.code == sf::Keyboard::Right) {
          moveRight = true;
        }
        if (event.key.code == sf::Keyboard::Up) {
          moveUp = true;
        } else if (event.key.code == sf::Keyboard::Down) {
          moveDown = true;
        }
      }

      if (event.type == sf::Event::KeyReleased) {
        if (event.key.code == sf::Keyboard::Left) {
          moveLeft = false;
        } else if (event.key.code == sf::Keyboard::Right) {
          moveRight = false;
        }
        if (event.key.code == sf::Keyboard::Up) {
          moveUp = false;
        } else if (event.key.code == sf::Keyboard::Down) {
          moveDown = false;
        }
      }
    }

    if (moveLeft) {
      view.move(-5.0f, 0.0f);
    }
    if (moveRight) {
      view.move(5.0f, 0.0f);
    }
    if (moveDown) {
      view.move(0.0f, 5.0f);
    }
    if (moveUp) {
      view.move(0.0f, -5.0f);
    }

    draw(data, fontpath, minX, maxX, minY, maxY);
    view.setSize(window->getSize().x / zoomFactor,
                 window->getSize().y / zoomFactor);
    window->setView(view);
    window->display();
    sf::sleep(sf::milliseconds(10));
  }
}

void Window::draw(const std::vector<std::pair<float, float>> &filtered_data,
                  const std::string &fontpath, float minX, float maxX,
                  float minY, float maxY) {

  std::vector<sf::Vector2f> mappedPosize_ts;
  window->clear(sf::Color::White);
  sf::CircleShape posize_tShape(2.0);
  posize_tShape.setFillColor(sf::Color::Green);
  const float winX = static_cast<float>(window->getSize().x);
  const float winY = static_cast<float>(window->getSize().y);

  float shiftX = (0.10f * winX);
  float shiftY = (0.90f * winY);

  float x = (shiftX + getCoord(winX, 0.0f));
  float y = (shiftY + getCoord(winY, 0.0f));

  // draw marks by Y axis
  float markY1 = shiftY + getCoord(winY, percentage(0.0f, minY, maxY)) - 10;
  float markY2 = shiftY + getCoord(winY, percentage(0.0f, minY, maxY)) + 10;
  float markX25 = shiftX + getCoord(winX, 25.0);
  float markX50 = shiftX + getCoord(winX, 50.0);
  float markX75 = shiftX + getCoord(winX, 75.0);

  // draw marks by X axis
  float markX1 = shiftX + getCoord(winX, 0.0f) - 10;
  float markX2 = shiftX + getCoord(winX, 0.0f) + 10;
  float markY25 = shiftY - getCoord(winY, 25.0);
  float markY50 = shiftY - getCoord(winY, 50.0);
  float markY75 = shiftY - getCoord(winY, 75.0);

  std::array<std::pair<sf::Vector2f, sf::Vector2f>, 8> axises{
      std::make_pair(sf::Vector2f(x, 0), sf::Vector2f(x, winY)),
      std::make_pair(sf::Vector2f(0, y), sf::Vector2f(winX, y)),
      std::make_pair(sf::Vector2f(markX25, markY1),
                     sf::Vector2f(markX25, markY2)),
      std::make_pair(sf::Vector2f(markX50, markY1),
                     sf::Vector2f(markX50, markY2)),
      std::make_pair(sf::Vector2f(markX75, markY1),
                     sf::Vector2f(markX75, markY2)),
      std::make_pair(sf::Vector2f(markX1, markY25),
                     sf::Vector2f(markX2, markY25)),
      std::make_pair(sf::Vector2f(markX1, markY50),
                     sf::Vector2f(markX2, markY50)),
      std::make_pair(sf::Vector2f(markX1, markY75),
                     sf::Vector2f(markX2, markY75)),
  };

  // Window points
  for (auto it : filtered_data) {
    float x0 = static_cast<float>(
        shiftX + (percentage(it.first, minX, maxX) * 0.90f * winX) / 100);
    float y0 = static_cast<float>(
        shiftY +
        (-1) * (percentage(it.second, minY, maxY) * (0.90f * winY)) / 100);

    mappedPosize_ts.emplace_back(x0, y0);
    posize_tShape.setPosition(x0, y0);
    window->draw(posize_tShape);
  }

  // Window axises
  for (auto it : axises) {
    drawLine(sf::Color::Black, std::move(it));
  }

  // Window text
  sf::Font font;
  if (font.loadFromFile(fontpath)) {
    const float rotate0 = 0.0f;
    const float rotate90 = 90.0f;

    drawText(font, "Meter",
             sf::Vector2f((winX) - (shiftX), (winY - (0.1f * winY))), rotate0);
    drawText(font, "Newton", sf::Vector2f(sf::Vector2f((shiftX - 20), (20))),
             rotate90);
    for (int i = 0; i < 100; i += 25) {
      drawText(font,
               parseValueToString(getValue(static_cast<float>(i), minX, maxX)),
               sf::Vector2f(shiftX + getCoord(winX, static_cast<float>(i)),
                            markY1 + 20),
               rotate0);
      drawText(font,
               parseValueToString(getValue(static_cast<float>(i), minY, maxY)),
               sf::Vector2f(markX1 - 10,
                            shiftY - getCoord(winY, static_cast<float>(i))),
               rotate90);
    }
  }

  // Window line
  for (size_t i = 1; i < mappedPosize_ts.size(); ++i) {
    drawLine(sf::Color::Red,
             std::make_pair(mappedPosize_ts[i - 1], mappedPosize_ts[i]));
  }
}