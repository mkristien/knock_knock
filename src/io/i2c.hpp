
#pragma once

#include <cstdint>

// Forward Declarations
namespace utils { class Logger; }

namespace io {

class I2C {
 public:
  static I2C& getInstance();

  /**
   * @brief Get data from sensor.
   * @param addr  - sensor address
   * @param rx    - pointer to head of read buffer
   * @param len   - number of BYTES to be read, i.e. size of the read buffer
   * @return true - iff transaction was successful
   */
  bool read(uint32_t addr, uint8_t* rx, uint16_t len);

  /**
   * @brief Write data to sensor.
   * @param addr  - sensor address
   * @param rx    - pointer to head of write buffer
   * @param len   - number of BYTES to be written, i.e. size of the write buffer
   * @return true - iff transaction was successful
   */
  bool write(uint32_t addr, uint8_t* tx, uint16_t len);

  /**
   * @brief Write 1 BYTE to sensor.
   * @param addr  - sensor address
   * @param rx    - BYTE to be written
   * @return true - iff transaction was successful
   */
  bool write(uint32_t addr, uint8_t tx);

 private:
  explicit I2C(utils::Logger& log);
  ~I2C();
  void setSensorAddress(uint32_t addr);

 private:
  utils::Logger&  log_;
  int             fd_;
  uint32_t        sensor_addr_;
};

}   // namespace io
