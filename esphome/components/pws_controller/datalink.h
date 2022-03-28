#pragma once

#include "esphome/components/uart/uart.h"

namespace esphome {
namespace pwscontroller {

class Datalink {
public:
  Datalink(uart::UARTDevice* uart_device);

protected:
  bool read_write(std::vector<uint8_t> &data);
  std::vector<uint8_t> construct_basic_frame(uint8_t id, uint8_t command, uint16_t payload_size);
  bool write_empty(uint8_t id);
  bool read_ram(uint8_t id, uint8_t address, std::vector<uint8_t> &data);
  bool write_ram(uint8_t id, uint8_t address, std::vector<uint8_t> &data);
  bool read_eeprom(uint8_t id, uint8_t address, std::vector<uint8_t> &data);
  bool write_eeprom(uint8_t id, uint8_t address, std::vector<uint8_t> &data);
  bool byte_2_bool(uint8_t value);
  uint8_t bool_2_byte(bool value);

private:
  uart::UARTDevice *uart;
};

}  // namespace pwscontroller
}  // namespace esphome
