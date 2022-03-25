#pragma once

#include "datalink.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace pwscontroller {

class PwsSensor : public Datalink {
public:
  PwsSensor(uint8_t id);

  typedef struct {
    char name[20];
    bool has_valve;
    bool has_pump;
    uint16_t temperature_interval;
    uint16_t resistance_interval_running;
    uint16_t resistance_interval_stopped;
  } config_t;

  typedef enum {
    Stopped = 0,
    Running,
  } state_t;

  bool check_device(void);
  bool write_id(uint8_t id);
  bool read_config(void);
  bool write_config(void);
  bool read_status(void);
  bool read_sensors(void);
  bool set_valve_time(uint8_t seconds);
  bool set_pump_time(uint8_t seconds);
  bool set_state(state_t state);
  bool dump_config(void);

private:
  uint8_t id;
  state_t state;
  uint16_t valve_time;
  uint16_t pump_time;
  uint64_t resistance;
  int8_t temperature;
  config_t config;
};

}  // namespace pwscontroller
}  // namespace esphome
