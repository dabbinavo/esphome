#include "pws_controller.h"
#include "esphome/core/log.h"

namespace esphome {
namespace pwscontroller {

static const char *const TAG = "pws_controller";

PwsController::PwsController() : PollingComponent(1000) {

}

void PwsController::setup() {

}

float PwsController::get_setup_priority() const {
  return setup_priority::DATA;
}

void PwsController::loop() {
}

void PwsController::update() {
  write_empty(1);
  ESP_LOGI(TAG, "this is the loooooop...");
}

void PwsController::dump_config() {
  ESP_LOGCONFIG(TAG, "PwsController has no config");
  LOG_UPDATE_INTERVAL(this);
  this->check_uart_settings(4800);
}

bool PwsController::read_write(std::vector<uint8_t> &data) {
  bool res;
  
  // Clear all data in the buffers
  flush();

  // Write data on serial port
  write_array(data);

  // Read array back from buffer
  res = read_array(data.data(), data.size());
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

std::vector<uint8_t> PwsController::construct_basic_frame(uint8_t id, uint8_t command, uint16_t payload_size) {
  std::vector<uint8_t> ret(payload_size + 3, 0);
  ret[0] = id << 1 | 0x01;
  ret[2] = command;
  return ret;
}

bool PwsController::write_empty(uint8_t id) {
  auto data = construct_basic_frame(id, 0x00, 0);
  return read_write(data);
}


}  // namespace pwscontroller
}  // namespace esphome
