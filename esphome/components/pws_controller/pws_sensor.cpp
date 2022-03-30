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
  if (!res) {
    return false;
  }
  strncpy(this->config.name, (const char*)data.data(), 20);
  this->config.has_valve = byte_2_bool(data[20]);
  this->config.has_pump = byte_2_bool(data[21]);
  this->config.temperature_interval = ((uint16_t)data[23] << 8) | data[22];
  this->config.resistance_interval_running = ((uint16_t)data[25] << 8) | data[24];
  this->config.resistance_interval_stopped = ((uint16_t)data[27] << 8) | data[26];
  return true;
}

bool PwsSensor::write_config(void) {

  return true;
}

bool PwsSensor::read_status(void) {
  bool res;
  std::vector<uint8_t> data(5);
  res = read_ram(this->id, 0x00, data);
  if (!res) {
    return false;
  }
  this->state = (state_t)data[0];
  this->valve_time = get_uint16(data, 1);
  this->pump_time = get_uint16(data, 3);
  return true;
}

bool PwsSensor::read_sensors(void) {
  bool res;
  std::vector<uint8_t> data(9);
  res = read_ram(this->id, 0x05, data);
  if (!res) {
    return false;
  }
  this->resistance = get_uint64(data, 0);
  this->temperature = data[8];
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

void PwsSensor::dump_config(void) {
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
