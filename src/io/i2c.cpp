
#include "i2c.hpp"

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#include "utils/logger.hpp"
#include "utils/system.hpp"

using utils::Logger;

namespace io    {

namespace i2c   {
inline int readHelper(int fd, uint8_t* buf, uint16_t len)
{
  return read(fd, buf, len);
}
inline int writeHelper(int fd, uint8_t* buf, uint16_t len)
{
  return write(fd, buf, len);
}
}   // namespace i2c


I2C& I2C::getInstance()
{
  static I2C i2c(utils::System::getLogger());
  return i2c;
}

I2C::I2C(Logger& log)
    : log_(log),
      fd_(0),
      sensor_addr_(0)
{
  char device_name[] = "/dev/i2c-2";
  fd_ = open(device_name, O_RDWR, 0);
  if (fd_ < 0) log_.ERR("I2C", "Could not open i2c device");
}

I2C::~I2C()
{
  if (fd_ >= 0) close(fd_);
}

void I2C::setSensorAddress(uint32_t addr)
{
  if (fd_ < 0) return;

  sensor_addr_ = addr;
  int ret = ioctl(fd_, I2C_SLAVE, addr);
  if (ret < 0) log_.ERR("I2C", "Could not set sensor address");
}

bool I2C::read(uint32_t addr, uint8_t* rx, uint16_t len)
{
  if (fd_ < 0) return false;  // early exit if no i2c device present

  if (sensor_addr_ != addr) setSensorAddress(addr);

  int ret = i2c::readHelper(fd_, rx, len);
  return ret == len;
}

bool I2C::write(uint32_t addr, uint8_t* tx, uint16_t len)
{
  if (fd_ < 0) return false;  // early exit if no i2c device present

  if (sensor_addr_ != addr) setSensorAddress(addr);

  int ret = i2c::writeHelper(fd_, tx, len);
  return ret == len;
}

bool I2C::write(uint32_t addr, uint8_t tx)
{
  return write(addr, &tx, 1);
}

}   // namespace io
