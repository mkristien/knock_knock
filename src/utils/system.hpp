
#pragma once

#include <cstdint>

#include "utils/logger.hpp"

namespace utils {

class System {
 public:
  static void parseArgs(int argc, char* argv[]);
  static System& getSystem();
  static Logger& getLogger();

  /**
   * Register custom signal handler for CTRL+C to make system exit gracefully
   */
  static bool setExitFunction();

  // runtime arguments to configure the whole system
  int8_t verbose;
  int8_t debug;

 private:
  Logger* log_;
  System() = delete;
  System(int argc, char* argv[]);
  ~System();
  static System* system_;
};

}  // namespace utils

