#pragma once

#include "pws_sensor.h"

#include <esphome/core/component.h>
#include <esphome/components/sensor/sensor.h>
#include <esphome/components/mqtt/custom_mqtt_device.h>

namespace esphome {
namespace pwscontroller {

class PwsController : public PollingComponent, public mqtt::CustomMQTTDevice, public uart::UARTDevice {
 public:
  PwsController();

  void setup() override;
  float get_setup_priority() const override;
  void loop() override;
  void update() override;
  void dump_config() override;

private:
  void handle_mqtt_message(const std::string &topic, const std::string &payload);

protected:
  bool read_write(std::vector<uint8_t> &data);
  std::vector<uint8_t> construct_basic_frame(uint8_t id, uint8_t command, uint16_t payload_size);
  bool write_empty(uint8_t id);

private:
  PwsSensor *sensor1;
};

}  // namespace cse7766
}  // namespace esphome
