#include <algorithm>
#include <fstream>
#include <string>
#include <vector>

#include "draw.h"

#if defined _WIN32
const std::string path = "..\\force_curve.csv";
#elif defined(__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
const std::string path = "../force_curve.csv";
#endif

void parseFile(std::vector<std::pair<float, float>> &curve) {
  std::fstream force_curve;
  force_curve.open(path);

  try {
    if (force_curve.is_open()) {
      std::string line;

      while (std::getline(force_curve, line)) {
        float first, second = 0.0;
        std::string Xstr, Ystr;
        Xstr = line.substr(0, line.find(";"));
        Ystr = line.substr(line.find(";") + 1, line.find("\n"));
        if (!Xstr.empty() && !Ystr.empty()) {
          first = std::stod(Xstr);
          second = stod(Ystr);
          curve.emplace_back(std::make_pair(first, second));
        }
      }
    } else
      throw std::runtime_error("Error: Unable to open the file.");

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
        sumX += curve[j].first;
        sumY += curve[j].second;
      }
      float newX = sumX / chunk;
      float newY = sumY / chunk;
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

int main() {
  std::vector<std::pair<float, float>> curve{};
  std::vector<std::pair<float, float>> filtered_data{};
  size_t chunk = 10;

  parseFile(curve);

  auto [minX, maxX, minY, maxY] = filterFIR(curve, filtered_data, chunk);
  draw(filtered_data, minX, maxX, minY, maxY);
  return 0;
}
