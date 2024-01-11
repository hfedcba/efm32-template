/* Copyright 2022-2023 Sensaru GmbH */

#include "em_chip.h"
#include "em_cmu.h"

extern "C" {

[[noreturn]] int main() {
  // Chip errata
  CHIP_Init();

  // Enable clock for the GPIO module; has no effect on xG21
  CMU_ClockEnable(cmuClock_GPIO, true);

  // Configure LED0
  GPIO_PinModeSet(GPIO_PORTA, 4, gpioModePushPull, 0);

  while (true) {
    GPIO_PinOutToggle(GPIO_PORTA, 4);
    for (volatile unsigned int i = 0; i < 100000; i++);
  }

  return 0;
}

}