#include "timer.hpp"
#include <string>
#include <vector>

void Timer::create_timer(const std::string& timer_name) {
  Timer& t = get_singleton();
  t.m_timer_name_to_id[timer_name] = t.m_next_timer_id++;
  t.m_timer_names.push_back(timer_name);
  t.m_beg_time.push_back({});
  t.m_end_time.push_back({});
  t.m_elapsed_time.push_back(unit(0.0));
}

void Timer::start_timer(const std::string& timer_name) {
  Timer& t = get_singleton();
  unsigned timer_id = t.m_timer_name_to_id[timer_name];
  t.m_beg_time[timer_id] = clock::now();
}

void Timer::stop_timer(const std::string& timer_name) {
  Timer& t = get_singleton();
  unsigned timer_id = t.m_timer_name_to_id[timer_name];
  t.m_end_time[timer_id] = clock::now();
  t.m_elapsed_time[timer_id] +=
      unit(t.m_end_time[timer_id] - t.m_beg_time[timer_id]);
}

Timer::unit Timer::time(const std::string& timer_name) {
  Timer& t = get_singleton();
  unsigned timer_id = t.m_timer_name_to_id[timer_name];
  return t.m_elapsed_time[timer_id];
}

const std::vector<std::string>& Timer::existing_timers() {
  return get_singleton().m_timer_names;
}

