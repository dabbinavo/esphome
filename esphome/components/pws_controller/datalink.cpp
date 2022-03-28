#include "datalink.h"
#include "esphome/core/log.h"

namespace esphome {
namespace pwscontroller {

static const char *const TAG = "pws_datalink";

Datalink::Datalink() {
}

bool Datalink::read_write(std::vector<uint8_t> &data) {
  bool res;
  ESP_LOGI(TAG, "read_write enter");
  
  // Clear all data in the buffers
  //flush();

  // Write data on serial port
  ESP_LOGI(TAG, "before write_array");
  write_array(data);
  ESP_LOGI(TAG, "after write_array");

  // Read array back from buffer
  res = read_array(data.data(), data.size());
  if (!res) {
    ESP_LOGE(TAG, "read_array error");
  }

  // Check if device responded
  ESP_LOGI(TAG, "before write_array");
  if (data[1] != ((data[0] >> 1) & 0x7F)) {
    ESP_LOGW(TAG, "device not available!");
    return false;
  }

  return true;
}

std::vector<uint8_t> Datalink::construct_basic_frame(uint8_t id, uint8_t command, uint16_t payload_size) {
  ESP_LOGI(TAG, "before vector");
  std::vector<uint8_t> ret(payload_size + 3, 0);
  ESP_LOGI(TAG, "after vector");
  ret[0] = id << 1 | 0x01;
  ESP_LOGI(TAG, "after vector 2");
  ret[2] = command;
  ESP_LOGI(TAG, "after vector 3");
  return ret;
}

bool Datalink::write_empty(uint8_t id) {
  ESP_LOGI(TAG, "before basic frame");
  auto data = construct_basic_frame(id, 0x00, 0);
  ESP_LOGI(TAG, "after basic frame");
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
