
#include "utils/system.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <csignal>

#define DEFAULT_VERBOSE -1
#define DEFAULT_DEBUG   -1

namespace utils {

namespace {
void printUsage()
{
  printf("./hyped [args]\n");
  printf(
    "All arguments are optional. To provide an argument with a value, use <argument>=<value>.\n"
    "Default value of verbose flags is 1\n"
    "Default value of debug   flags is 0\n"
    "\n  -v, --verbose[=<bool>]\n"
    "    Set system-wide setting of verbosity. If enabled, prints all INFO messages\n"
    "\n  -d, --debug[=<level>]\n"
    "    Set system-wide debug level. All DBG[n] where n <= level messages are printed.\n"
    "\n  --debug_motor, --debug_nav, --debug_sensor, --debug_state, --debug_cmn\n"
    "    Set module-specific debug level. All DBG[n] where n <= level messages are printed.\n"
    "\n  --fake_imu --fake_proxi --fake_motors --fake_sensors --fake_keyence --fake_batteries\n"
    "    Make the system use the fake data drivers.\n"
    "\n  --fail_dec_imu --fail_acc_imu --fail_motors --miss_keyence --double_keyence\n"
    "    Make the system use the fake data drivers and fail them for testing.\n"
    "\n  --accurate\n"
    "    Make the system use the accurate fake system\n"
    "");
}
}

System::~System()
{
  if (log_) delete log_;
}

System::System(int argc, char* argv[])
    : verbose(false),
      debug(DEFAULT_DEBUG)
{
  int c;
  int option_index = 0;
  while (1) {
    static option long_options[] = {
      {"verbose", optional_argument, 0, 'v'},
      {"debug", optional_argument, 0, 'd'},
      {"help", no_argument, 0, 'h'},
      {0, 0, 0, 0}
    };
    c = getopt_long(argc, argv, "vd::h", long_options, &option_index);

    /* Detect the end of the options. */
    if (c == -1)
      break;

    switch (c) {
      case 'v':
        if (optarg) verbose = atoi(optarg);
        else        verbose = true;
        break;
      case 'h':
        printUsage();
        exit(0);
        break;
      case 'd':
        if (optarg) debug = atoi(optarg);
        else        debug = 0;
        break;
      default:
        printUsage();
        exit(1);
        break;
    }
  }

  log_ = new Logger(verbose, debug);
  system_ = this;
}

System* System::system_ = 0;

void System::parseArgs(int argc, char* argv[])
{
  if (system_) return;

  system_ = new System(argc, argv);
}

System& System::getSystem()
{
  if (system_) return *system_;
  Logger log;
  log.ERR("SYSTEM", "somebody tried to get System"
          " before initialisation, aborting");
  exit(1);
}

Logger& System::getLogger()
{
  System& sys = getSystem();
  return *sys.log_;
}

}  // namespace utils
