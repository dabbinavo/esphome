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

PwsController::PwsController()
  : PollingComponent(1000)
{

}

void PwsController::setup() {
  subscribe("pws/sensor/+/set/#", &PwsController::handle_mqtt_message);
  discover_devices();
  ESP_LOGI(TAG, "init finished. found %u active sensors.", sensors.size());
}

float PwsController::get_setup_priority() const {
  return setup_priority::DATA;
}

void PwsController::loop() {
}

void PwsController::update() {
  if (sensors.size() == 0) {
    discover_devices();
  }
  for (const auto& sensor : sensors) {
    sensor.second->dump_config();
    sensor.second->read_sensors();
    sensor.second->dump_sensors();
  }
}

void PwsController::dump_config() {
  ESP_LOGCONFIG(TAG, "PwsController has no config");
  LOG_UPDATE_INTERVAL(this);
  this->check_uart_settings(4800);
}

void PwsController::discover_devices() {
    for (int id = 1; id < 5; id++) {
    bool res = Datalink::write_empty(this, id);
    if (res) {
      ESP_LOGI(TAG, "found sensor with id %u", id);
      sensors[id] = new PwsSensor(id, this);
      res = sensors[id]->read_config();
      if (!res) {
        ESP_LOGW(TAG, "unable to read config...");
        continue;
      }
      sensors[id]->dump_config();
    }
  }
  ESP_LOGI(TAG, "found %u active sensors.", sensors.size());
}

void PwsController::handle_mqtt_message(const std::string &topic, const std::string &payload) {
  ESP_LOGI(TAG, "received mqtt");
  int id;
  char parameter[20];
  
  sscanf(topic.c_str(), "pws/sensor/%d/set/%s", &id, parameter);
  
  ESP_LOGI(TAG, "received message for id(%d): parameter(%s) payload(%s)", id, parameter, payload.c_str());

  auto sensor = sensors.find(id);
  if (sensor == sensors.end()) {
    ESP_LOGW(TAG, "cannot perform operation as sensor(%u) is not available!", id);
    return;
  }

  if (strcmp(parameter, "config/name") == 0) {
    strncpy(sensor->second->config.name, payload.c_str(), 20);
    sensor->second->write_config();
  }
  else if(strcmp(parameter, "config/valve") == 0) {
    sensor->second->config.has_valve = payload.at(0) == '1' ? true : false;
    sensor->second->write_config();
  }
  else if(strcmp(parameter, "config/pump") == 0) {
    sensor->second->config.has_pump = payload.at(0) == '1' ? true : false;
    sensor->second->write_config();
  }
  else if(strcmp(parameter, "config/temperature") == 0) {
    sensor->second->config.temperature_interval = atoi(payload.c_str());
    sensor->second->write_config();
  }
  else if(strcmp(parameter, "config/resistance1") == 0) {
    sensor->second->config.resistance_interval_running = atoi(payload.c_str());
    sensor->second->write_config();
  }
  else if(strcmp(parameter, "config/resistance2") == 0) {
    sensor->second->config.resistance_interval_stopped = atoi(payload.c_str());
    sensor->second->write_config();
  }
}

}  // namespace pwscontroller
}  // namespace esphome
