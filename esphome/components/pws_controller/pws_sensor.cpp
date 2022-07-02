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
  std::vector<uint8_t> data(28);
  res = read_eeprom(this->id, 0x01, data);
  if (!res) {
    return false;
  }
  strncpy(this->config.name, (const char*)data.data(), 20);
  this->config.has_valve = byte_2_bool(data[20]);
  this->config.has_pump = byte_2_bool(data[21]);
  this->config.temperature_interval = decode_uint16(data, 22);
  this->config.resistance_interval_running = decode_uint16(data, 24);
  this->config.resistance_interval_stopped = decode_uint16(data, 26);
  return true;
}

bool PwsSensor::write_config(void) {
  std::vector<uint8_t> data(0x28);
  strncpy((char*)data.data(), this->config.name, 20);
  data[20] = bool_2_byte(this->config.has_valve);
  data[21] = bool_2_byte(this->config.has_pump);
  encode_uint16(data, 22, config.temperature_interval);
  encode_uint16(data, 24, config.resistance_interval_running);
  encode_uint16(data, 26, config.resistance_interval_stopped);
  return write_eeprom(this->id, 0x01, data);
}

bool PwsSensor::read_status(void) {
  bool res;
  std::vector<uint8_t> data(5);
  res = read_ram(this->id, 0x00, data);
  if (!res) {
    return false;
  }
  this->state = (state_t)data[0];
  this->valve_time = decode_uint16(data, 1);
  this->pump_time = decode_uint16(data, 3);
  return true;
}

bool PwsSensor::read_sensors(void) {
  bool res;
  std::vector<uint8_t> data(9);
  res = read_ram(this->id, 0x05, data);
  if (!res) {
    return false;
  }
  this->resistance = decode_uint64(data, 0);
  this->temperature = data[8];
  return true;
}

bool PwsSensor::set_valve_time(uint8_t seconds) {
  std::vector<uint8_t> data(2);
  encode_uint16(data, 0, seconds);
  return write_ram(this->id, 0x01, data);
}

bool PwsSensor::set_pump_time(uint8_t seconds) {
  std::vector<uint8_t> data(2);
  encode_uint16(data, 0, seconds);
  return write_ram(this->id, 0x03, data);
}

bool PwsSensor::set_state(state_t state) {
  std::vector<uint8_t> data(1);
  data[0] = state;
  return write_ram(this->id, 0x00, data);
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


void PwsSensor::dump_sensors(void) {
  ESP_LOGI(TAG, "| Sensor        | Value |");
  ESP_LOGI(TAG, "|:------------|------:|");
  ESP_LOGI(TAG, "| Resistance  | %.9llu |", this->resistance);
  ESP_LOGI(TAG, "| Temperature | %.3u |", this->temperature);
}

}  // namespace pwscontroller
}  // namespace esphome
