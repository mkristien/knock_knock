

#include "utils/logger.hpp"

#include <stdio.h>
#include <stdarg.h>

#include <chrono>
#include <iomanip>
#include <ctime>

namespace utils {

namespace {

void myPrint(FILE* file, const char* format, va_list args)
{
  vfprintf(file, format, args);
  fprintf(file, "\n");
}

void logHead(FILE* file, const char* title, const char* module)
{
  using namespace std::chrono;
  std::time_t t = std::time(nullptr);
  tm* tt = localtime(&t);
  fprintf(file, "%02d:%02d:%02d",
      tt->tm_hour, tt->tm_min, tt->tm_sec);

  static const bool print_micro = true;
  if (print_micro) {
    auto now_time = high_resolution_clock::now().time_since_epoch();
    duration<int, std::milli> time_span = duration_cast<std::chrono::milliseconds>
          (now_time);
    fprintf(file, ".%03d ", static_cast<uint16_t>(time_span.count()) % 1000);
  } else {
    fprintf(file, " ");
  }
  fprintf(file, "%s[%s]: ", title, module);
}

}

Logger::Logger(bool verbose, int8_t debug)
    : verbose_(verbose),
      debug_(debug)
{ /* EMPTY */ }

void Logger::ERR(const char* module, const char* format, ...)
{
  static FILE* file = stdout;
  logHead(file, "ERR", module);
  va_list args;
  va_start(args, format);
  myPrint(file, format, args);
  va_end(args);
}

void Logger::INFO(const char* module, const char* format, ...)
{
  static FILE* file = stdout;
  if (verbose_) {
    logHead(file, "INFO", module);
    va_list args;
    va_start(args, format);
    myPrint(file, format, args);
    va_end(args);
  }
}

void Logger::DBG(const char* module, const char* format, ...)
{
  static FILE* file = stderr;
  if (debug_ >= 0) {
    logHead(file, "DBG0", module);
    va_list args;
    va_start(args, format);
    myPrint(file, format, args);
    va_end(args);
  }
}

void Logger::DBG1(const char* module, const char* format, ...)
{
  static FILE* file = stderr;
  if (debug_ >= 1) {
    logHead(file, "DBG1", module);
    va_list args;
    va_start(args, format);
    myPrint(file, format, args);
    va_end(args);
  }
}
void Logger::DBG2(const char* module, const char* format, ...)
{
  static FILE* file = stderr;
  if (debug_ >= 2) {
    logHead(file, "DBG2", module);
    va_list args;
    va_start(args, format);
    myPrint(file, format, args);
    va_end(args);
  }
}
void Logger::DBG3(const char* module, const char* format, ...)
{
  static FILE* file = stderr;
  if (debug_ >= 3) {
    logHead(file, "DBG3", module);
    va_list args;
    va_start(args, format);
    myPrint(file, format, args);
    va_end(args);
  }
}
}  // namespace utils

