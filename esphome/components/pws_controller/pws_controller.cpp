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
  ESP_LOGI(TAG, "this is the loooooop...");
}

void PwsController::update() {

}

void PwsController::dump_config() {
  ESP_LOGCONFIG(TAG, "PwsController has no config");
  LOG_UPDATE_INTERVAL(this);
  this->check_uart_settings(115200);
}

}  // namespace pwscontroller
}  // namespace esphome
