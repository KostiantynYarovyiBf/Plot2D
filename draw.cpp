#include "draw.h"

float percentage(float value, float min, float max) {
  return ((value - min) / (max - min)) * 100;
}

float getCoord(float size, float per) { return ((size * per) / 100); }

void drawLine(sf::RenderWindow &window, sf::Color color,
              std::pair<sf::Vector2f, sf::Vector2f> &&pos) {
  sf::Vertex line[2];
  line[0].color = color;
  line[1].color = color;
  line[0].position = pos.first;
  line[1].position = pos.second;
  window.draw(line, 2, sf::Lines);
}

void drawTest(sf::RenderWindow &window, const sf::Font &font, std::string &&str,
              sf::Vector2f &&p, float rotate) {
  sf::Text text;
  text.setFont(font);
  text.setCharacterSize(12);
  text.setFillColor(sf::Color::Black);
  text.setString(std::move(str));
  text.setPosition(std::move(p));
  text.rotate(rotate);
  window.draw(text);
}

void draw(const std::vector<std::pair<float, float>> &filtered_data, float minX,
          float maxX, float minY, float maxY) {
  sf::RenderWindow window(sf::VideoMode(winX, winY), "PLot");
  std::vector<sf::Vector2f> mappedPosize_ts;
  sf::Vertex line[2];
  line[0].color = sf::Color::Red;
  line[1].color = sf::Color::Red;
  window.clear(sf::Color::White);
  sf::CircleShape posize_tShape(2.0);
  posize_tShape.setFillColor(sf::Color::Green);

  float shiftX = (0.10 * winX);
  float shiftY = (0.90 * winY);

  if (minX > 0)
    minX = 0;
  if (minY > 0)
    minY = 0;

  float x = shiftX + getCoord(winX, percentage(0.0, minX, maxX));
  float y = shiftY + getCoord(winY, percentage(0.0, minY, maxY));

  // draw marks by Y axis
  float markY1 = shiftY + getCoord(winY, percentage(0.0, minY, maxY)) - 10;
  float markY2 = shiftY + getCoord(winY, percentage(0.0, minY, maxY)) + 10;
  float markX25 = shiftX + getCoord(winX, 25.0);
  float markX50 = shiftX + getCoord(winX, 50.0);
  float markX75 = shiftX + getCoord(winX, 75.0);

  // draw marks by X axis
  float markX1 = shiftX + getCoord(winX, percentage(0.0, minX, maxX)) - 10;
  float markX2 = shiftX + getCoord(winX, percentage(0.0, minX, maxX)) + 10;
  float markY25 = shiftY - getCoord(winY, 25.0);
  float markY50 = shiftY - getCoord(winY, 50.0);
  float markY75 = shiftY - getCoord(winY, 75.0);

  std::cout << "markX1 " << markX1 << std::endl;
  std::cout << "markX2 " << markX2 << std::endl;
  std::cout << "markY25 " << markY25 << std::endl;
  std::cout << "x " << x << std::endl;

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

  // Draw points
  for (auto it : filtered_data) {
    float x = shiftX + (percentage(it.first, 0.0, maxX) * 0.90 * winX) / 100;
    float y = shiftY +
              (-1) * (percentage(it.second, minY, maxY) * (0.90 * winY)) / 100;

    mappedPosize_ts.emplace_back(x, y);
    posize_tShape.setPosition(x, y);
    window.draw(posize_tShape);
  }

  // Draw axises
  for (auto it : axises) {
    drawLine(window, sf::Color::Black, std::move(it));
  }

  // Draw text
  sf::Font font;
  if (font.loadFromFile(fontpath)) {
    drawTest(window, font, "Meter",
             sf::Vector2f((winX) - (shiftX), (winY - (0.1 * winY))), 0.0);
    drawTest(window, font, "Newton",
             sf::Vector2f(sf::Vector2f((shiftX - 20), (20))), 90.0);
  }

  // Draw line
  for (size_t i = 1; i < mappedPosize_ts.size(); ++i) {
    line[0].position = mappedPosize_ts[i - 1];
    line[1].position = mappedPosize_ts[i];
    window.draw(line, 2, sf::Lines);
  }

  window.display();

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }
  }
}