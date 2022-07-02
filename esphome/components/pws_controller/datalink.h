#pragma once

#include "esphome/components/uart/uart.h"

namespace esphome {
namespace pwscontroller {

class Datalink {
public:
  Datalink(uart::UARTDevice* uart_device);
  static bool write_empty(uart::UARTDevice* uart_device, uint8_t id);

protected:
  static std::vector<uint8_t> construct_basic_frame(uint8_t id, uint8_t command, uint16_t payload_size);

  bool write_empty(uint8_t id);

  bool read_ram(uint8_t id, uint8_t address, std::vector<uint8_t> &data);
  bool write_ram(uint8_t id, uint8_t address, std::vector<uint8_t> &data);
  bool read_eeprom(uint8_t id, uint8_t address, std::vector<uint8_t> &data);
  bool write_eeprom(uint8_t id, uint8_t address, std::vector<uint8_t> &data);

  static bool byte_2_bool(uint8_t value);
  static uint8_t bool_2_byte(bool value);
  static uint16_t decode_uint16(const std::vector<uint8_t> &data, uint8_t pos);
  static uint32_t decode_uint32(const std::vector<uint8_t> &data, uint8_t pos);
  static uint64_t decode_uint64(const std::vector<uint8_t> &data, uint8_t pos);
  static void encode_uint16(std::vector<uint8_t> &data, uint8_t pos, const uint16_t &value);
  static void encode_uint32(std::vector<uint8_t> &data, uint8_t pos, const uint32_t &value);
  static void encode_uint64(std::vector<uint8_t> &data, uint8_t pos, const uint64_t &value);

private:
  bool read_write(std::vector<uint8_t> &data);
  static bool read_write(std::vector<uint8_t> &data, uart::UARTDevice* uart_device);

private:
  uart::UARTDevice *uart;
};

}  // namespace pwscontroller
}  // namespace esphome
