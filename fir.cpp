#include "fir.h"

void Sma::filterFIR(const std::vector<std::pair<float, float>> &curve,
                    std::vector<std::pair<float, float>> &filtered_data,
                    WinSize &_winSize, size_t chunk) {
  try {
    for (size_t i = 0; i < curve.size() - chunk; ++i) {
      float sumX = 0.0, sumY = 0.0;
      for (size_t j = i; j < i + chunk; ++j) {
        sumX += (curve[j].first);
        sumY += (curve[j].second);
      }
      float newX = sumX / static_cast<float>(chunk);
      float newY = sumY / static_cast<float>(chunk);
      if (newX < _winSize.minX) {
        _winSize.minX = newX;
      }
      if (newX > _winSize.maxX) {
        _winSize.maxX = newX;
      }
      if (newY < _winSize.minY) {
        _winSize.minY = newY;
      }
      if (newY > _winSize.maxY) {
        _winSize.maxY = newY;
      }
      filtered_data.emplace_back(std::make_pair(newX, newY));
    }
  } catch (const std::exception &ex) {
    std::cout << "ERROR filterFIR " << ex.what() << std::endl;
    std::terminate();
  }
}
