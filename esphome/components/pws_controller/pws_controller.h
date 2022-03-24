#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace pws_controller {

class PwsControllerComponent : public PollingComponent, public uart::UARTDevice {
 public:

  void setup() override;
  float get_setup_priority() const override;
  void loop() override;
  void update() override;
  void dump_config() override;

};

}  // namespace cse7766
}  // namespace esphome
