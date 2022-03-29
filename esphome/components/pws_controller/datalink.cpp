#include "datalink.h"
#include "esphome/core/log.h"

namespace esphome {
namespace pwscontroller {

static const char *const TAG = "pws_datalink";

Datalink::Datalink(uart::UARTDevice* uart_device) {
  this->uart = uart_device;
}

bool Datalink::read_write(std::vector<uint8_t> &data) {
  static unsigned long last_time = 0;
  bool res;

  unsigned long time_diff = millis() - last_time;

  if (time_diff < 10) {
    delay(10 - time_diff);
  }
  
  // Clear all data in the buffers
  uart->flush();

  // Write data on serial port
  uart->write_array(data);

  // Read array back from buffer
  res = uart->read_array(data.data(), data.size());
  if (!res) {
    ESP_LOGE(TAG, "read_array error");
  }

  // Check if device responded
  if (data[1] != ((data[0] >> 1) & 0x7F)) {
    ESP_LOGW(TAG, "device not available!");
    return false;
  }

  return true;
}

std::vector<uint8_t> Datalink::construct_basic_frame(uint8_t id, uint8_t command, uint16_t payload_size) {
  std::vector<uint8_t> ret(payload_size + 3, 0);
  ret[0] = id << 1 | 0x01;
  ret[2] = command;
  return ret;
}

bool Datalink::write_empty(uint8_t id) {
  auto data = construct_basic_frame(id, 0x00, 0);
  return read_write(data);
}


bool Datalink::read_ram(uint8_t id, uint8_t address, std::vector<uint8_t> &data) {
  bool res;
  auto buffer = construct_basic_frame(id, 0x10, 2 + data.size());
  buffer[3] = address;
  buffer[4] = data.size();
  res = read_write(buffer);
  if (!res) {
    return false;
  }
  std::copy(buffer.begin() + 5, buffer.end(), data.begin());
  return true;
}

bool Datalink::write_ram(uint8_t id, uint8_t address, std::vector<uint8_t> &data) {
  bool res;
  auto buffer = construct_basic_frame(id, 0x11, 2 + data.size());
  buffer[3] = address;
  buffer[4] = data.size();
  std::copy(data.begin(), data.end(), buffer.begin() + 5);
  res = read_write(buffer);
  if (!res) {
    return false;
  }
  return true;
}

bool Datalink::read_eeprom(uint8_t id, uint8_t address, std::vector<uint8_t> &data) {
  bool res;
  auto buffer = construct_basic_frame(id, 0x20, 2 + data.size());
  buffer[3] = address;
  buffer[4] = data.size();
  res = read_write(buffer);
  if (!res) {
    return false;
  }
  std::copy(buffer.begin() + 5, buffer.end(), data.begin());
  return true;
}

bool Datalink::write_eeprom(uint8_t id, uint8_t address, std::vector<uint8_t> &data) {
  bool res;
  auto buffer = construct_basic_frame(id, 0x21, 2 + data.size());
  buffer[3] = address;
  buffer[4] = data.size();
  std::copy(data.begin(), data.end(), buffer.begin() + 5);
  res = read_write(buffer);
  if (!res) {
    return false;
  }
  return true;
}

bool Datalink::byte_2_bool(uint8_t value) {
  return value != 0x00;
}

uint8_t Datalink::bool_2_byte(bool value) {
  return value ? 0x01 : 0x00;
}


}  // namespace Datalink
}  // namespace esphome
