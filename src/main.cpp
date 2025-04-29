#include <algorithm>
#include <cstddef>
#include <fstream>
#include <ftxui/component/component_options.hpp>
#include <memory>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>
#include <vector>

#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/event.hpp"
#include "ftxui/component/mouse.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/color.hpp"

#include "nlohmann/json.hpp"
using json = nlohmann::json;

constexpr size_t MAX_PROBLEM = 40;
constexpr size_t MAX_LINE = 80;
size_t problem_count{0};

/// # Read problems
/// This function reads the problem source files and stores them in an array of
/// vectors. Each vector contains the lines of code for a specific problem. It
/// uses the `ifstream` class to read the files line by line. The function takes
/// a vector of strings as input, which contains the names of the problem source
/// files. It also takes a 2D vector of strings to store the problem code.
///
/// ## Parameters
/// - `problem_source`: A vector of strings containing the names of the problem
///   source files.
/// - `problem_code`: A 2D vector of strings to store the problem code.
/// - `MAX_PROBLEM`: The maximum number of problems.
///
/// ## Example
/// ```cpp
/// std::vector<std::string> problem_source = {"quick-sort.c", "robin-karp.c"};
/// std::vector<std::string> problem_code[MAX_PROBLEM];
/// read_problems(problem_source, problem_code);
/// ```
int read_problems(std::vector<std::string> &problem_source,
                  std::vector<std::string> *problem_code) {
  std::ifstream config("../problems/problems.json");
  if (!config.is_open()) {
    std::cerr << "Error: cannot open problem.json" << std::endl;
    return 1;
  }
  json j = json::parse(config);
  if (j.is_null()) {
    std::cerr << "Error: cannot parse JSON" << std::endl;
    return 1;
  }
  problem_count = j["problems"].size();
  if (problem_count > MAX_PROBLEM) {
    std::cerr << "Error: too many problems" << std::endl;
    return 1;
  }
  if (problem_count == 0) {
    std::cerr << "Error: no problems found" << std::endl;
    return 1;
  }
  for (size_t i = 0; i < problem_count; ++i) {
    problem_source[i] = j["problems"][i];
  }
  for (size_t i = 0; i < problem_count; ++i) {
    std::ifstream file("../problems/" + problem_source[i]);
    std::string line;
    while (std::getline(file, line)) {
      problem_code[i].push_back(line);
    }
    if (problem_code[i].empty() || problem_code[i].size() > MAX_LINE) {
      std::cerr << "Error: problem code empty or too long." << std::endl;
      return 1;
    }
  }
  return 0;
}

/// # Build the components
/// This function builds the components for each problem.
/// It creates a renderer for each problem that displays the problem code.
/// It also initializes a vector of booleans to keep track of which lines are
/// revealed.
///
/// ## Parameters
/// - `tabs`: A vector of components to hold the renderers.
/// - `problem_code`: A 2D vector of strings to hold the problem code.
/// - `reveal_code`: A 2D vector of booleans to keep track of which lines are
///   revealed.
/// - `MAX_PROBLEM`: The maximum number of problems.
void build_components(ftxui::Components &tabs,
                      std::vector<std::string> *problem_code,
                      std::vector<bool> *reveal_code) {
  using namespace ftxui;
  for (size_t i = 0; i < problem_count; ++i) {
    const std::vector<std::string> &problem = problem_code[i];
    std::vector<bool> &reveal = reveal_code[i];
    reveal.resize(problem.size(), false);
    auto renderer = Renderer([&] {
      Elements lines;
      int reveal_count = 0;
      for (size_t j = 0; j < problem.size(); ++j) {
        if (reveal[j]) {
          reveal_count++;
        }
      }
      for (size_t j = 0; j < std::min(problem.size(), static_cast<size_t>(40));
           ++j) {
        if (reveal[j]) {
          lines.push_back(
              hbox(text((j < 9 ? " " : "") + std::to_string(j + 1)) |
                       size(WIDTH, EQUAL, 4),
                   text(problem[j]) | size(WIDTH, EQUAL, 50)));
        } else {
          lines.push_back(hbox(
              text((j < 9 ? " " : "") + std::to_string(j + 1)) |
                  size(WIDTH, EQUAL, 4),
              text("") | size(WIDTH, EQUAL, 50) | bgcolor(Color::GrayDark)));
        }
      }
      double progress = static_cast<double>(reveal_count) /
                        static_cast<double>(problem.size());
      const std::string progress_text =
          " " + std::to_string(static_cast<int>(progress * 100)) + "%";
      if (problem.size() <= 40) {
        return vbox(
            vbox(lines) | border,
            hbox(text("Progress: "), gauge(progress), text(progress_text)));
      } else {
        auto first_col = vbox(lines) | border;
        lines.clear();
        for (size_t j = 40; j < problem.size(); ++j) {
          if (reveal[j]) {
            lines.push_back(
                hbox(text(std::to_string(j + 1)) | size(WIDTH, EQUAL, 4),
                     text(problem[j]) | size(WIDTH, EQUAL, 50)));
          } else {
            lines.push_back(hbox(
                text(std::to_string(j + 1)) | size(WIDTH, EQUAL, 4),
                text("") | size(WIDTH, EQUAL, 50) | bgcolor(Color::GrayDark)));
          }
        }
        auto second_col = vbox(lines) | border;
        auto h = vbox(
            hbox(first_col, second_col),
            hbox(text("Progress: "), gauge(progress), text(progress_text)));
        return h;
      }
    });
    tabs.push_back(renderer);
  }
}

int main() {
  std::vector<std::string> problem_source(MAX_PROBLEM);
  std::vector<std::string> problem_code[MAX_PROBLEM];
  if (read_problems(problem_source, problem_code) != 0) {
    return -1;
  }

  using namespace ftxui;
  // Check if the terminal size is large enough, use the C method
  // to get the terminal size.
  struct winsize w;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
    std::cerr << "Error: get terminal size." << std::endl;
    return -1;
  }
  if (w.ws_col < 128 || w.ws_row < 45) {
    std::cerr
        << "Error: terminal size is too small. Expect 45 rows and 128 cols."
        << std::endl;
    return -1;
  }

  auto screen = ScreenInteractive::Fullscreen();

  int selected_prob = 0;
  std::vector<std::string> tab_titles;
  for (size_t i = 0; i < problem_count; ++i) {
    tab_titles.push_back("Problem " + std::to_string(i + 1));
  }

  std::string input;
  std::vector<bool> reveal_code[MAX_PROBLEM];
  auto hint = Renderer([&] { return vbox({text("Reveal line(s): ")}); });
  auto input_handler = Input(&input, "type here");
  input_handler = CatchEvent(input_handler, [&](Event event) {
    if (event.is_mouse())
      return false;
    if (event == Event::Return) {
      if (!input.empty()) {
        if (input == "all") {
          for (size_t i = 0; i < problem_code[selected_prob].size(); ++i) {
            reveal_code[selected_prob][i] = true;
          }
        }

        // Use the input to reveal lines, the input can either be a single
        // number or a range of numbers.
        // For example: 1 or 1-3, or 1,2,3

        // Check the input validity
        std::vector<std::string> lines;
        size_t pos = 0;
        while ((pos = input.find(',')) != std::string::npos) {
          lines.push_back(input.substr(0, pos));
          input.erase(0, pos + 1);
        }
        lines.push_back(input);
        input.clear();
        for (auto &line : lines) {
          for (char &c : line) {
            if (!isdigit(c) && c != '-') {
              input.clear();
              return true;
            }
          }
        }

        // For each line, check if it is a range or a single number
        for (const auto &line : lines) {
          size_t dash_pos = line.find('-');
          if (dash_pos != std::string::npos) {
            // Range
            int start = std::stoi(line.substr(0, dash_pos));
            int end = std::stoi(line.substr(dash_pos + 1));
            for (int i = start; i <= end; ++i) {
              if (i > 0 && i <= problem_code[selected_prob].size()) {
                reveal_code[selected_prob][i - 1] = true;
              }
            }
          } else {
            // Single number
            int line_num = std::stoi(line);
            if (line_num > 0 &&
                line_num <= problem_code[selected_prob].size()) {
              reveal_code[selected_prob][line_num - 1] = true;
            }
          }
        }
      }
      return true;
    }
    return false;
  });
  auto input_component = Container::Horizontal({hint, input_handler});

  auto tab_toggle = Container::Vertical({
      Menu(&tab_titles, &selected_prob),
      input_component,
  });

  int mouse_x = 0;
  int mouse_y = 0;
  bool is_clicked = false;

  Components tabs;
  build_components(tabs, problem_code, reveal_code);

  auto tab = Container::Tab(tabs, &selected_prob);
  auto component = Container::Horizontal({
      tab,
      tab_toggle,
  });

  // Add some separator to decorate the whole component:
  auto component_renderer = Renderer(component, [&] {
    return hbox({tab->Render(), separator(), tab_toggle->Render()}) | border;
  });

  screen.Loop(component_renderer);
}