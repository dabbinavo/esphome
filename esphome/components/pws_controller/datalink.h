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
  uint16_t decode_uint16(const std::vector<uint8_t> &data, uint8_t pos);
  uint32_t decode_uint32(const std::vector<uint8_t> &data, uint8_t pos);
  uint64_t decode_uint64(const std::vector<uint8_t> &data, uint8_t pos);
  void encode_uint16(std::vector<uint8_t> &data, uint8_t pos, const uint16_t &value);
  void encode_uint32(std::vector<uint8_t> &data, uint8_t pos, const uint32_t &value);
  void encode_uint64(std::vector<uint8_t> &data, uint8_t pos, const uint64_t &value);

private:
  uart::UARTDevice *uart;
};

}  // namespace pwscontroller
}  // namespace esphome
