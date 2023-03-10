
#pragma once

#include <cstdint>
#include <vector>

// Forward declaration
namespace utils { class Logger; }

namespace io {

namespace gpio  {
constexpr uint8_t kBankNum = 4;
enum Direction {
  kIn   = 0,
  kOut  = 1
};
}   // namespace gpio

class GPIO {
  typedef utils::Logger Logger;
 public:
  GPIO(uint32_t pin, gpio::Direction direction);
  GPIO(uint32_t pin, gpio::Direction direction, Logger& log);

  void    set();     // set high
  void    clear();   // set low
  uint8_t read();    // read pin value

  /**
   * @brief Block caller until value of gpio pin has changed
   * @return int8_t the new gpio value, -1 in case of an error
   */
  int8_t wait();

 private:
  GPIO() = delete;

  // GPIO system configuration
  /**
   * @brief Fill in base_mapping_ with pointers to mmap-ed /dev/mem
   * to 4 GPIO banks/ports.
   */
  static void initialise();
  static void uninitialise();
  static bool initialised_;
  static void* base_mapping_[gpio::kBankNum];
  static std::vector<uint32_t> exported_pins;

  /**
   * @brief Tell kernel we are using this, set direction
   */
  void exportGPIO();

  /**
   * @brief Configure device register pointers
   */
  void attachGPIO();

  /**
   * @brief Configure fd_ for wait functionality in case of input GPIO
   */
  void setupWait();

  uint32_t        pin_;
  gpio::Direction direction_;
  Logger&         log_;

  volatile uint32_t* set_;        // set register
  volatile uint32_t* clear_;      // clear register
  volatile uint32_t* data_;       // data register
  uint32_t           pin_mask_;   // mask for register access to this pin
  int                fd_;         // file pointer to /sys/class/gpio/gpioXX/value
};

}   // namespace io

