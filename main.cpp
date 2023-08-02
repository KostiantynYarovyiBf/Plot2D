#include <algorithm>
#include <fstream>
#include <string>
#include <vector>

#include "draw.h"

const size_t winX = 1000;
const size_t winY = 500;

void parseFile(std::vector<std::pair<float, float>> &curve,
               const std::string &path) {
  std::fstream force_curve;
  force_curve.open(path);
  try {
    if (force_curve.is_open()) {
      std::string line;

      while (std::getline(force_curve, line)) {

        std::string Xstr, Ystr;
        Xstr = line.substr(0, line.find(";"));
        Ystr = line.substr(line.find(";") + 1, line.find("\n"));
        if (!Xstr.empty() && !Ystr.empty()) {
          float first = static_cast<float>(std::stod(Xstr));
          float second = static_cast<float>(stod(Ystr));
          curve.emplace_back(std::make_pair(first, second));
        }
      }
    } else {
      force_curve.close();
      throw std::runtime_error("Error: Unable to open the file.");
    }

    force_curve.close();
  } catch (const std::exception &ex) {
    std::cout << "ERROR parseFile " << ex.what() << std::endl;
    std::terminate();
  }
}

auto filterFIR(const std::vector<std::pair<float, float>> &curve,
               std::vector<std::pair<float, float>> &filtered_data,
               size_t chunk) {
  float minX = std::numeric_limits<float>::max();
  float minY = std::numeric_limits<float>::max();
  float maxX = std::numeric_limits<float>::min();
  float maxY = std::numeric_limits<float>::min();

  try {
    for (size_t i = 0; i < curve.size() - chunk; ++i) {
      float sumX = 0.0, sumY = 0.0;
      for (size_t j = i; j < i + chunk; ++j) {
        sumX += (curve[j].first);
        sumY += (curve[j].second);
      }
      float newX = sumX / static_cast<float>(chunk);
      float newY = sumY / static_cast<float>(chunk);
      if (newX < minX) {
        minX = newX;
      }
      if (newX > maxX) {
        maxX = newX;
      }
      if (newY < minY) {
        minY = newY;
      }
      if (newY > maxY) {
        maxY = newY;
      }
      filtered_data.emplace_back(std::make_pair(newX, newY));
    }

  } catch (const std::exception &ex) {
    std::cout << "ERROR filterFIR " << ex.what() << std::endl;
    std::terminate();
  }

  return std::make_tuple(minX, maxX, minY, maxY);
}

int main(int argc, char *argv[]) {
  std::string path = "";
  std::string fontpath = "";

  if (argc <= 1) {
    std::cout << "Please, write path to force_curve.csv" << std::endl;
    std::cin >> path;
    std::cout << "Please, write path to font file" << std::endl;
    std::cin >> fontpath;
  } else if (argc >= 3) {
    path = argv[1];
    fontpath = argv[2];
  }

  std::vector<std::pair<float, float>> curve{};
  std::vector<std::pair<float, float>> filtered_data{};
  size_t chunk = 4;
  sf::RenderWindow window(sf::VideoMode(winX, winY),
                          "Two-dimensional graph plot",
                          sf::Style::Titlebar | sf::Style::Close);
  parseFile(curve, path);

  auto [minX, maxX, minY, maxY] = filterFIR(curve, filtered_data, chunk);

  draw(window, filtered_data, fontpath, minX, maxX, minY, maxY);

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }
  }

  return 0;
}
