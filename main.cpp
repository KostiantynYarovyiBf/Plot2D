#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

std::vector<std::pair<double, double>> curve;

void parseFile() {
  std::fstream force_curve;
  std::string path;

  std::cout << "Write path to force_curve.csv file\n";
  std::cin >> path;
  // /home/kyarovyi/projects/force_curve/force_curve.csv
#if defined _WIN32
  system("cls");
#elif defined(__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
  system("clear");
#elif defined(__APPLE__)
  system("clear");
#endif
  force_curve.open(path);

  try {
    if (force_curve.is_open()) {
      std::string mystr;
      std::string delimiter = ";";
      double first, second = 0.0;
      while (std::getline(force_curve, mystr)) {
        first = std::stod(mystr.substr(0, mystr.find(delimiter)));
        second = stod(
            mystr.substr(mystr.find(delimiter) + 1, mystr.find(delimiter)));
        curve.push_back(std::make_pair(first, second));

        std::cout << first << " substr2 =  " << second << std::endl;
      }
    } else
      throw std::runtime_error("Error: Unable to open the file.");

    force_curve.close();
  } catch (const std::exception &ex) {
    std::cout << ex.what() << std::endl;
  }
}

int main() {

  parseFile();

  return 0;
}
