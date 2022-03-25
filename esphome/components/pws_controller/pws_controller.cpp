#include "pws_controller.h"

#include "esphome/core/log.h"

namespace esphome {
namespace pwscontroller {

static const char *const TAG = "pws_controller";

PwsController::PwsController() : PollingComponent(1000) {
  sensor1 = new PwsSensor(0x01);
}

void PwsController::setup() {

}

float PwsController::get_setup_priority() const {
  return setup_priority::DATA;
}

void PwsController::loop() {
}

void PwsController::update() {
  bool res;
  res = sensor1->check_device();
  if (res) {
    ESP_LOGI(TAG, "sensor 1 responded!");
  }
  else {
    ESP_LOGW(TAG, "sensor 1 not detected...");
  }
  res = sensor1->read_config();
  if (res) {
    ESP_LOGI(TAG, "successfully read config");
    sensor1->dump_config();
  }
  else {
    ESP_LOGW(TAG, "unable to read config...");
  }
}

void PwsController::dump_config() {
  ESP_LOGCONFIG(TAG, "PwsController has no config");
  LOG_UPDATE_INTERVAL(this);
  this->check_uart_settings(4800);
}

}  // namespace pwscontroller
}  // namespace esphome
