#pragma once

#include <chrono>
#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>

// Timer provides an arbitrary number of globally available named timers,
// measuring run time in milliseconds.  Once created, each  named timer can be
// started and stopped independently.  A timer can be started and stopped
// multiple times.  Each timer measures the total elapsed time between starts
// and stops in milliseconds.
//
// Starting an already started timer or stopping a non-running timer results in
// undefined behavior.  Also, don't try to use a timer that was not created
// before.
class Timer {
  using clock = std::chrono::high_resolution_clock;
  using unit = std::chrono::duration<double, std::milli>;

 public:
  // Create a new timer with the given name. If timer_name is already in used, a
  // new timer is created and the old one is discarded.
  static void create_timer(const std::string& timer_name);

  // Start the timer with the given name (it has to be created an not running).
  static void start_timer(const std::string& timer_name);

  // Stop the timer with the given name (it has to be created and running).
  static void stop_timer(const std::string& timer_name);

  // Get the total time the timer ran (it has to be created and not running).
  static unit time(const std::string& timer_name);

  // Get the names of all existing timers.
  static const std::vector<std::string>& existing_timers();

 private:
  // The actual timers.
  std::unordered_map<std::string, unsigned> m_timer_name_to_id;
  std::vector<std::string> m_timer_names;
  unsigned m_next_timer_id = 0;

  std::vector<clock::time_point> m_beg_time;
  std::vector<clock::time_point> m_end_time;
  std::vector<unit> m_elapsed_time;

  // Implementation of the singleton pattern.
  Timer() {}
  static Timer& get_singleton() {
    static Timer instance;
    return instance;
  }

 public:
  Timer(const Timer&) = delete;
  void operator=(const Timer&) = delete;
};
