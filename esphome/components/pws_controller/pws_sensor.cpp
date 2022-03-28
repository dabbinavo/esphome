#include "pws_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace pwscontroller {

static const char *const TAG = "pws_sensor";

PwsSensor::PwsSensor(uint8_t id, uart::UARTDevice* uart_device)
  : Datalink(uart_device)
{
  this->id = id;
}


bool PwsSensor::check_device(void) {
  return write_empty(this->id);
}

bool PwsSensor::write_id(uint8_t id) {
  std::vector<uint8_t> data(1);
  data[0] = id;
  return write_eeprom(this->id, 0x00, data);
}

bool PwsSensor::read_config(void) {
  bool res;
  std::vector<uint8_t> data(0x20);
  res = read_eeprom(this->id, 0x01, data);
  strncpy(this->config.name, (const char*)data.data(), 20);
  this->config.has_valve = byte_2_bool(data[20]);
  this->config.has_pump = byte_2_bool(data[21]);
  this->config.temperature_interval = ((uint16_t)data[22] << 8) | data[23];
  this->config.resistance_interval_running = ((uint16_t)data[24] << 8) | data[25];
  this->config.resistance_interval_stopped = ((uint16_t)data[26] << 8) | data[27];
  return true;
}

bool PwsSensor::write_config(void) {

  return true;
}

bool PwsSensor::read_status(void) {

  return true;
}

bool PwsSensor::read_sensors(void) {

  return true;
}

bool PwsSensor::set_valve_time(uint8_t seconds) {

  return true;
}

bool PwsSensor::set_pump_time(uint8_t seconds) {

  return true;
}

bool PwsSensor::set_state(state_t state) {

  return true;
}

bool PwsSensor::dump_config(void) {
  ESP_LOGI(TAG, "| Sensor Config | Value |");
  ESP_LOGI(TAG, "|:--------------|------:|");
  ESP_LOGI(TAG, "| name          | %.20s |", this->config.name);
  ESP_LOGI(TAG, "| valve         | %s |", this->config.has_valve ? "yes" : "no");
  ESP_LOGI(TAG, "| pump          | %s |", this->config.has_pump ? "yes" : "no");
  ESP_LOGI(TAG, "| temp upd  [s] | %d |", this->config.temperature_interval);
  ESP_LOGI(TAG, "| res upd r [s] | %d |", this->config.resistance_interval_running);
  ESP_LOGI(TAG, "| res upd s [s] | %d |", this->config.resistance_interval_stopped);
}

}  // namespace pwscontroller
}  // namespace esphome
