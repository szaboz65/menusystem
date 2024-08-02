#include "../my_config.h"
#include "MenuCmds.h"
#include "CmdRenderer.h"
#include "esp_adc_cal.h"

#include <Arduino.h>

BatteryTask::BatteryTask(CmdRenderer *renderer)
  : Task(1000)
  , _renderer(renderer)
{

}

void BatteryTask::setrenderer(CmdRenderer *renderer) {
  _renderer = renderer;
}

void BatteryTask::enter() {
  setVref();
  _lastvoltage = -1.0;
  run();
  start();
}

void BatteryTask::exit() {
  stop();
}

void BatteryTask::render() {
  if( _lastvoltage != _voltage) {
    _lastvoltage = _voltage;
    if (_renderer) _renderer->render(*this);
  }
}

void BatteryTask::run() {
  readAdc();
  render();
}

void BatteryTask::readAdc() {
  uint16_t v = analogRead(ADC_PIN);
  _voltage = ((float)v / 4095.0) * 2.0 * 3.3 * (_vref / 1000.0);
}

void BatteryTask::setVref() {
  esp_adc_cal_characteristics_t adc_chars;
  esp_adc_cal_value_t val_type = esp_adc_cal_characterize((adc_unit_t)ADC_UNIT_1, (adc_atten_t)ADC1_CHANNEL_6, (adc_bits_width_t)ADC_WIDTH_BIT_12, 1100, &adc_chars);
  //Check type of calibration value used to characterize ADC
  if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
    //Serial.printf("eFuse Vref:%u mV", adc_chars.vref);
    _vref = adc_chars.vref;
  } else if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
    //Serial.printf("Two Point --> coeff_a:%umV coeff_b:%umV\n", adc_chars.coeff_a, adc_chars.coeff_b);
    _vref = 1100;
  } else {
    //Serial.println("Default Vref: 1100mV");
    _vref = 1100;
  }
}
