# Guess Code

A terminal-based interactive application for exploring and guessing code implementations. This project uses the FTXUI library to create a user-friendly terminal interface where users can progressively reveal and understand code implementations of various algorithms.

## Project Overview

Guess Code is an educational tool designed to help users learn programming algorithms through an interactive "reveal" approach. The application presents several classic algorithms with their implementations initially hidden. Users can gradually reveal lines of code by clicking on them, encouraging them to think about and predict how the algorithm works before seeing the full implementation.

## Features

- Interactive terminal-based UI powered by FTXUI
- Multiple algorithm implementations to explore
- Progressive code revelation with mouse interaction
- Real-time progress tracking for each algorithm
- Split-view for larger code files

## How to Use

1. Navigate the problems using the tab menu on the right
2. Click on hidden (dark) lines of code to reveal them
3. Track your progress with the progress bar at the bottom
4. Try to understand the algorithm before revealing all lines
5. Make sure your terminal is large enough (at least 45 rows and 128 columns) to view the code properly

## Building the Project

### Prerequisites
- C++ compiler with C++17 support
- CMake (version 3.22 or higher)
- FTXUI library (automatically fetched by CMake)

### Build Instructions

```bash
mkdir build
cd build
cmake ..
make
```

After building, run the executable:

```bash
./guess-code
```

## Implementation Details

The application is structured around several key components:

- **Problem Loading**: Source code files are read from the `problems/` directory and stored in memory
- **Interactive UI**: FTXUI components create a responsive terminal interface
- **Reveal Mechanism**: Mouse events are tracked to determine which lines to reveal
- **Progress Tracking**: The application tracks and displays how much of each algorithm has been revealed

## License

See the LICENSE file for details.

## Contributing

Contributions are welcome! Feel free to submit pull requests or open issues for new features, bug fixes, or additional algorithms to include.

## Credits

- Built using the [FTXUI](https://github.com/ArthurSonzogni/FTXUI) library for terminal user interfaces.
- Using [nlohmann/json](https://github.com/nlohmann/json) for JSON parsing.
