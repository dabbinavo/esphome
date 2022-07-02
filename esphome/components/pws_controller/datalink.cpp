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
  last_time = millis();
  
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

bool Datalink::read_write(std::vector<uint8_t> &data, uart::UARTDevice* uart_device) {
  static unsigned long last_time = 0;
  bool res;

  unsigned long time_diff = millis() - last_time;

  if (time_diff < 10) {
    delay(10 - time_diff);
  }
  
  // Clear all data in the buffers
  uart_device->flush();

  // Write data on serial port
  uart_device->write_array(data);

  // Read array back from buffer
  res = uart_device->read_array(data.data(), data.size());
  last_time = millis();
  
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

bool Datalink::write_empty(uart::UARTDevice* uart_device, uint8_t id) {
  auto data = construct_basic_frame(id, 0x00, 0);
  return read_write(data, uart_device);
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

uint16_t Datalink::decode_uint16(const std::vector<uint8_t> &data, uint8_t pos) {
  return ((uint16_t)data[pos + 1] << 8)
       | ((uint16_t)data[pos + 0] << 0);
}

uint32_t Datalink::decode_uint32(const std::vector<uint8_t> &data, uint8_t pos) {
  return ((uint32_t)data[pos + 3] << 24)
       | ((uint32_t)data[pos + 2] << 16)
       | ((uint32_t)data[pos + 1] <<  8)
       | ((uint32_t)data[pos + 0] <<  0);
}

uint64_t Datalink::decode_uint64(const std::vector<uint8_t> &data, uint8_t pos) {
  return ((uint64_t)data[pos + 7] << 56)
       | ((uint64_t)data[pos + 6] << 48)
       | ((uint64_t)data[pos + 5] << 40)
       | ((uint64_t)data[pos + 4] << 32)
       | ((uint64_t)data[pos + 3] << 24)
       | ((uint64_t)data[pos + 2] << 16)
       | ((uint64_t)data[pos + 1] <<  8)
       | ((uint64_t)data[pos + 0] <<  0);
}

void Datalink::encode_uint16(std::vector<uint8_t> &data, uint8_t pos, const uint16_t &value) {
  data[pos + 1] = uint8_t(value >> 8);
  data[pos + 0] = uint8_t(value >> 0);
}

void Datalink::encode_uint32(std::vector<uint8_t> &data, uint8_t pos, const uint32_t &value) {
  data[pos + 3] = uint8_t(value >> 24);
  data[pos + 2] = uint8_t(value >> 16);
  data[pos + 1] = uint8_t(value >> 8);
  data[pos + 0] = uint8_t(value >> 0);
}

void Datalink::encode_uint64(std::vector<uint8_t> &data, uint8_t pos, const uint64_t &value) {
  data[pos + 7] = uint8_t(value >> 56);
  data[pos + 6] = uint8_t(value >> 48);
  data[pos + 5] = uint8_t(value >> 40);
  data[pos + 4] = uint8_t(value >> 32);
  data[pos + 3] = uint8_t(value >> 24);
  data[pos + 2] = uint8_t(value >> 16);
  data[pos + 1] = uint8_t(value >> 8);
  data[pos + 0] = uint8_t(value >> 0);
}


}  // namespace Datalink
}  // namespace esphome
