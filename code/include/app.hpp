#pragma once

#include <cstdlib>
#include <filesystem>
#include <string>

#include "CLI11.hpp"
#include "random.hpp"

class App {
 public:
  App(std::string app_name) : cli_app(app_name) {}

 public:
  // add argument
  template <typename T>
  auto arg(T& variable, const std::string& name,
           const std::string& description);

  // add argument with check for allowed values (using a list of allowed values)
  template <typename T>
  auto arg(T& variable, const std::string& name, const std::string& description,
           const std::vector<T>& allowed_values);

  // add argument with check for allowed values (using a CLI validator)
  template <typename T>
  auto arg(T& variable, const std::string& name, const std::string& description,
           CLI::Validator validator);

  // specialization for input files
  auto arg(std::filesystem::path& variable, const std::string& name,
           const std::string& description);

  // special header flags
  void arg_header(const std::string& header) {
    m_no_header = false;
    m_header = header;
    cli_app.add_flag("--only-header", m_only_header,
                     "Only print the csv-header.");
    cli_app.add_flag("--no-header", m_no_header,
                     "Only print the results and not the csv-header.");
  }

  // special seed flag
  void arg_seed() {
    m_seed_used = true;
    m_seed = Random::get_seed();
    cli_app.add_option("--seed", m_seed,
                       "Specify a seed to get deterministic behavior.  If not "
                       "specified, a random seed is used.");
  }

  void parse(int argc, char** argv) {
    try {
      cli_app.parse(argc, argv);
    } catch (const CLI ::ParseError& e) {
      exit(cli_app.exit(e));
    };

    if (m_seed_used) {
      Random::set_seed(m_seed);
    }

    if (!m_no_header) {
      std::cout << m_header << std::endl;
    }
    if (m_only_header) {
      exit(0);
    }
  }

  CLI::App cli_app;

 private:
  template <typename T>
  auto base_arg(T& variable, const std::string& name,
                const std::string& description);

  std::string m_header = "";
  bool m_only_header = false;
  bool m_no_header = true;

  bool m_seed_used = false;
  unsigned m_seed;
};

template <typename T>
auto App::base_arg(T& variable, const std::string& name,
                   const std::string& description) {
  auto opt = cli_app.add_option(name, variable, description);
  // if (opt->get_positional()) {
  //   opt->required();
  // }
  return opt;
}

template <typename T>
inline auto App::arg(T& variable, const std::string& name,
              const std::string& description) {
  return base_arg(variable, name, description);
}

template <>
auto App::arg(bool& variable, const std::string& name,
              const std::string& description) {
  return cli_app.add_flag(name, variable, description);
}

template <typename T>
auto App::arg(T& variable, const std::string& name,
              const std::string& description, CLI::Validator validator) {
  return base_arg(variable, name, description)->check(validator);
}

template <typename T>
auto App::arg(T& variable, const std::string& name,
              const std::string& description,
              const std::vector<T>& allowed_values) {
  return base_arg(variable, name, description)
      ->check(CLI::IsMember(allowed_values));
}

auto App::arg(std::filesystem::path& variable, const std::string& name,
              const std::string& description) {
  return base_arg(variable, name, description)->check(CLI::ExistingFile);
}
