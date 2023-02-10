
#include <stdio.h>

#include "utils/system.hpp"
#include "utils/logger.hpp"

int main(int argc, char* argv[]) {
  printf("hello world\n");

  utils::System::parseArgs(argc, argv);
  utils::Logger& log = utils::System::getLogger();

  log.INFO("MAIN", "Hello logger");
}
