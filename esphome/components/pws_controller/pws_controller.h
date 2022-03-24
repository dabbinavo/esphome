#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace pwscontroller {

class PwsController : public PollingComponent, public uart::UARTDevice {
 public:
  PwsController();

  void setup() override;
  float get_setup_priority() const override;
  void loop() override;
  void update() override;
  void dump_config() override;

protected:
  bool read_write(std::vector<uint8_t> &data);
  std::vector<uint8_t> construct_basic_frame(uint8_t id, uint8_t command, uint16_t payload_size);
  bool write_empty(uint8_t id);

};

}  // namespace cse7766
}  // namespace esphome
