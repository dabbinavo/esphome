#include "pws_controller.h"

#include <esphome/core/log.h>

#include <iostream>
#include <sstream>

template <char C>
std::istream& expect(std::istream& in)
{
    if ((in >> std::ws).peek() == C) {
        in.ignore();
    }
    else {
        in.setstate(std::ios_base::failbit);
    }
    return in;
}

namespace esphome {
namespace pwscontroller {

static const char *const TAG = "pws_controller";

PwsController::PwsController() : PollingComponent(1000) {
  sensor1 = new PwsSensor(0x01, this);
}

void PwsController::setup() {
  subscribe("pws/sensor/+/set/+", &PwsController::handle_mqtt_message);
}

float PwsController::get_setup_priority() const {
  return setup_priority::DATA;
}

void PwsController::loop() {
}

void PwsController::update() {
  bool res;
  ESP_LOGI(TAG, "update loop");
  res = sensor1->check_device();
  if (res) {
    ESP_LOGI(TAG, "sensor 1 responded!");
    res = sensor1->read_config();
    if (res) {
      ESP_LOGI(TAG, "successfully read config");
      sensor1->dump_config();
    }
    else {
      ESP_LOGW(TAG, "unable to read config...");
    }
  }
  else {
    ESP_LOGW(TAG, "sensor 1 not detected...");
  }
}

void PwsController::dump_config() {
  ESP_LOGCONFIG(TAG, "PwsController has no config");
  LOG_UPDATE_INTERVAL(this);
  this->check_uart_settings(4800);
}

void PwsController::handle_mqtt_message(const std::string &topic, const std::string &payload) {
  int id;
  char parameter[20];
  
  sscanf(topic.c_str(), "pws/sensor/%d/set/%s", &id, &parameter);

  ESP_LOGI(TAG, "received message for id(%d) and parameter(%s)", id, parameter);
}

}  // namespace pwscontroller
}  // namespace esphome
