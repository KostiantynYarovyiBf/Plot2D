# Requirements:
    - C++ 17+
    - SFML
    - CMAKE

# Install SFML: 
    Windows (Into Shell):
        - git clone https://github.com/Microsoft/vcpkg.git> .\vcpkg\bootstrap-vcpkg.bat -disableMetrics
        - .\vcpkg\vcpkg integrate install
        - .\vcpkg\vcpkg install sfml:x64-windows
    Linux (Debian):
        sudo apt install libsfml-dev

# Run:
    Windows:
        mkdir build
        cd build
        cmake ../
        cmake --build . --config Release
        Release\plot.exe "..\force_curve.csv" "..\times_new_roman.ttf"
    Linux(Do not verified graphics):
        mkdir  build_2 && cd build_2
        cmake ../
        cmake --build .
        ./plot "../force_curve.csv" "../times_new_roman.ttf"


