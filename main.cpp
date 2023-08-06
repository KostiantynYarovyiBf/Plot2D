#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include "draw.h"
#include "fir.h"

const size_t winX = 1000;
const size_t winY = 500;
const std::string name = "Two-dimensional graph plot";
enum class SupportFir {
  SMA,
};
struct WinSize;

class Plot {
private:
  std::vector<std::pair<float, float>> curve;
  std::vector<std::pair<float, float>> filtered_data;
  size_t chunk;
  std::string path;
  std::string fontpath;
  std::unique_ptr<Fir> fir;
  WinSize _winSize;

  void setFilterType(SupportFir sp) {
    switch (sp) {
    case SupportFir::SMA: {
      fir = std::make_unique<Sma>();
      break;
    }
    default:
      fir = std::make_unique<Sma>();
      break;
    }
  }

public:
  Plot() = default;
  ~Plot() = default;
  Plot(int argc, char *argv[], size_t ch = 4)
      : curve{}, filtered_data{}, chunk{ch} {
    if (argc <= 1) {
      std::cout << "Please, write path to force_curve.csv" << std::endl;
      std::cin >> path;
      std::cout << "Please, write path to font file" << std::endl;
      std::cin >> fontpath;
    } else if (argc >= 3) {
      path = argv[1];
      fontpath = argv[2];
    }
  }

  std::string &getFont() { return this->fontpath; }
  auto &getFillterData() { return this->filtered_data; }
  auto &getData() { return this->curve; }
  auto &getWinSize() { return this->_winSize; }

  void parseFile() {
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

  void fillterData(SupportFir sp) {
    setFilterType(sp);
    this->fir.get()->filterFIR(this->curve, this->filtered_data, _winSize,
                               this->chunk);
  }

  void draw(sf::RenderWindow &window) {

    try {
      Draw draw;
      draw.draw(window, this->filtered_data, this->fontpath, _winSize.minX,
                _winSize.maxX, _winSize.minY, _winSize.maxY);
    } catch (const std::exception &ex) {
      std::cout << "ERROR: Plot Draw " << ex.what() << std::endl;
      window.close();
    }
  }
};

int main(int argc, char *argv[]) {

  sf::RenderWindow window(sf::VideoMode(winX, winY), std::move(name),
                          sf::Style::Titlebar | sf::Style::Close);

  Plot plot(argc, argv);
  plot.parseFile();
  plot.fillterData(SupportFir::SMA);
  // auto &_winSize = plot.getWinSize();
  plot.draw(window);

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
