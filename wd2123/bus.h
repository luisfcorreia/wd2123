#pragma once
#include <Arduino.h>

// Call once from setup()
void busBegin();

// Call every loop() — checks MR pin, drives IRQ if needed
void busUpdate();
