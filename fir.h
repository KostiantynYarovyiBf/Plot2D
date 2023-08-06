#pragma once

#include <algorithm>
#include <iostream>
#include <vector>

struct WinSize {
public:
  float minX;
  float minY;
  float maxX;
  float maxY;
  WinSize() {
    minX = std::numeric_limits<float>::max();
    minY = std::numeric_limits<float>::max();
    maxX = std::numeric_limits<float>::min();
    maxY = std::numeric_limits<float>::min();
  }
  ~WinSize() = default;
};
class Fir {
public:
  Fir() = default;
  virtual ~Fir() {}
  virtual void filterFIR(const std::vector<std::pair<float, float>> &curve,
                         std::vector<std::pair<float, float>> &filtered_data,
                         WinSize &_winSize, size_t chunk) = 0;
};

class Sma : public Fir {
public:
  Sma() = default;
  ~Sma() = default;
  void filterFIR(const std::vector<std::pair<float, float>> &curve,
                 std::vector<std::pair<float, float>> &filtered_data,
                 WinSize &_winSize, size_t chunk) override;
};
