#include <algorithm>
#include <cstddef>
#include <fstream>
#include <memory>
#include <string>
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
void read_problems(const std::vector<std::string> &problem_source,
                   std::vector<std::string> problem_code[problem_count]) {
  for (size_t i = 0; i < problem_count; ++i) {
    std::ifstream file("../problems/" + problem_source[i]);
    std::string line;
    while (std::getline(file, line)) {
      problem_code[i].push_back(line);
    }
  }
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
  std::ifstream config("../problems/problems.json");
  if (!config.is_open()) {
    std::cerr << "Error opening file." << std::endl;
    return 1;
  }
  json j = json::parse(config);
  if (j.is_null()) {
    std::cerr << "Error parsing JSON." << std::endl;
    return 1;
  }
  problem_count = j["problems"].size();
  std::vector<std::string> problem_source(problem_count);
  for (size_t i = 0; i < problem_count; ++i) {
    problem_source[i] = j["problems"][i];
  }
  std::vector<std::string> problem_code[problem_count];
  read_problems(problem_source, problem_code);

  using namespace ftxui;

  int selected_prob = 0;
  std::vector<std::string> tab_titles;
  for (size_t i = 0; i < problem_count; ++i) {
    tab_titles.push_back("Problem " + std::to_string(i + 1));
  }
  auto tab_toggle = Menu(&tab_titles, &selected_prob);

  int mouse_x = 0;
  int mouse_y = 0;
  bool is_clicked = false;

  Components tabs;
  std::vector<bool> reveal_code[problem_count];
  build_components(tabs, problem_code, reveal_code);

  auto tab = Container::Tab(tabs, &selected_prob);

  // This capture the last mouse position.
  auto tab_with_mouse = CatchEvent(tab, [&](Event e) {
    if (e.is_mouse()) {
      mouse_x = e.mouse().x;
      mouse_y = e.mouse().y - 2;
      if (e.mouse().button == Mouse::Button::Left) {
        if (mouse_x >= 57 && reveal_code[selected_prob].size() <= 40) {
          reveal_code[selected_prob][mouse_y] = true;
        } else {
          reveal_code[selected_prob][mouse_y + 40 * (mouse_x >= 57)] = true;
        }
      }
    }
    return false;
  });

  auto component = Container::Horizontal({
      tab_with_mouse,
      tab_toggle,
  });

  // Add some separator to decorate the whole component:
  auto component_renderer = Renderer(component, [&] {
    return hbox({tab_with_mouse->Render(), separator(), tab_toggle->Render()}) |
           border;
  });

  auto screen = ScreenInteractive::Fullscreen();
  screen.Loop(component_renderer);
}