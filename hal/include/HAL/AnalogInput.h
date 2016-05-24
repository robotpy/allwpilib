#pragma once
#include <stdint.h>

extern "C" {
// Analog input functions
void* initializeAnalogInputPort(void* port_pointer, int32_t* status);
void freeAnalogInputPort(void* analog_port_pointer);
bool checkAnalogModule(uint8_t module);
bool checkAnalogInputChannel(uint32_t pin);

void setAnalogSampleRate(double samplesPerSecond, int32_t* status);
float getAnalogSampleRate(int32_t* status);
void setAnalogAverageBits(void* analog_port_pointer, uint32_t bits,
                          int32_t* status);
uint32_t getAnalogAverageBits(void* analog_port_pointer, int32_t* status);
void setAnalogOversampleBits(void* analog_port_pointer, uint32_t bits,
                             int32_t* status);
uint32_t getAnalogOversampleBits(void* analog_port_pointer, int32_t* status);
int16_t getAnalogValue(void* analog_port_pointer, int32_t* status);
int32_t getAnalogAverageValue(void* analog_port_pointer, int32_t* status);
int32_t getAnalogVoltsToValue(void* analog_port_pointer, double voltage,
                              int32_t* status);
float getAnalogVoltage(void* analog_port_pointer, int32_t* status);
float getAnalogAverageVoltage(void* analog_port_pointer, int32_t* status);
uint32_t getAnalogLSBWeight(void* analog_port_pointer, int32_t* status);
int32_t getAnalogOffset(void* analog_port_pointer, int32_t* status);
}
