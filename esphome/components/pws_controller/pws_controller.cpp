#include "pws_controller.h"
#include "esphome/core/log.h"

namespace esphome {
namespace pws_controller {

static const char *const TAG = "pws_controller";

void PwsControllerComponent::setup() {

}

float PwsControllerComponent::get_setup_priority() const {
  return setup_priority::DATA;
}

void PwsControllerComponent::loop() {
  ESP_LOGI(TAG, "this is the loooooop...");
}

void PwsControllerComponent::update() {

}

void PwsControllerComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "PwsController has no config");
  LOG_UPDATE_INTERVAL(this);
  this->check_uart_settings(115200);
}

}  // namespace cse7766
}  // namespace esphome
