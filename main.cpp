#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include "fir.h"
#include "window.h"

const size_t defaultChunk = 20;
const float moveStep = 5.0f;

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
  WinSize winSizeFiltered;
  WinSize winSize;

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
  Plot(int argc, char *argv[], size_t ch = defaultChunk)
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
  auto &getWinSizeFil() { return this->winSizeFiltered; }
  auto &getWinSize() { return this->winSize; }

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
            if (first < winSize.minX) {
              winSize.minX = first;
            }
            if (first > winSize.maxX) {
              winSize.maxX = first;
            }
            if (second < winSize.minY) {
              winSize.minY = second;
            }
            if (second > winSize.maxY) {
              winSize.maxY = second;
            }
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
    this->fir.get()->filterFIR(this->curve, this->filtered_data,
                               winSizeFiltered, this->chunk);
  }

  void draw() {}
};

int main(int argc, char *argv[]) {
  const size_t winX = 1000;
  const size_t winY = 500;
  float zoomFactor{1.0f};
  float zoomIncrement{0.1f};
  bool moveLeft{false};
  bool moveRight{false};
  bool moveUp{false};
  bool moveDown{false};
  bool toggleState = true;

  Plot plot(argc, argv);
  plot.parseFile();
  plot.fillterData(SupportFir::SMA);
  sf::RenderWindow window(sf::VideoMode(winX, winY), std::move(name),
                          sf::Style::Titlebar | sf::Style::Close);

  Window win(window);
  sf::View view(window.getDefaultView());

  WinSize winSize = plot.getWinSize();

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
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
        view.setSize(static_cast<float>(window.getSize().x) / zoomFactor,
                     static_cast<float>(window.getSize().y) / zoomFactor);
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

        if (event.key.code == sf::Keyboard::Space) {
          toggleState = !toggleState;
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
      view.move((-1) * moveStep, ZERO_F);
    }
    if (moveRight) {
      view.move(moveStep, ZERO_F);
    }
    if (moveDown) {
      view.move(ZERO_F, moveStep);
    }
    if (moveUp) {
      view.move(ZERO_F, (-1) * moveStep);
    }

    window.setView(view);
    window.clear(sf::Color::White);
    if (toggleState)
      win.draw(plot.getFillterData(), plot.getFont(), winSize.minX,
               winSize.maxX, winSize.minY, winSize.maxY);
    else
      win.draw(plot.getData(), plot.getFont(), winSize.minX, winSize.maxX,
               winSize.minY, winSize.maxY);

    window.display();
    sf::sleep(sf::milliseconds(10));
  }

  return 0;
}
