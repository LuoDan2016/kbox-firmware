/*
  The MIT License

  Copyright (c) 2016 Thomas Sarlandie thomas@sarlandie.net

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#pragma once

#include <KBoxHardware.h>

#include "MfgTest.h"

struct ADCTestCase {
  String channelName;
  int channelPin;
};

struct ADCTestCase cases[] = {
  { "Supply", supply_analog },
  { "Bat1", adc1_analog },
  { "Bat2", adc2_analog },
  { "Bat3", adc3_analog }
};

class ADCTest : public MfgTest {
  private:
    ADC& adc;
    int channel;

  public:
    ADCTest(int chan) : MfgTest("ADCTest(" + cases[chan].channelName + ")", 10000), adc(KBox.getADC()), channel(chan) {};

    void setup() {
      setInstructions("Apply 12V to " + cases[channel].channelName + " only.");
    };

    float readVoltage(int index) {
      int adcValue = adc.analogRead(cases[index].channelPin, ADC_0);
      return adcValue * analog_max_voltage / adc.getMaxValue();
    };

    void loop() {
      float voltage = readVoltage(channel);
      setMessage("Measured voltage is " + String(voltage));

      // Walk through the three battery channels and make sure there are no shortcuts
      for (int i = 1; i < 4; i++) {
        if (i != channel) {
          float v = readVoltage(i);
          if (readVoltage(i) > 10) {
            fail("Measured " + String(v) + "V on " + cases[i].channelName);
          }
        }
      }
      if (voltage > 11.9 && voltage < 12.1) {
        pass();
      }
    };
};
