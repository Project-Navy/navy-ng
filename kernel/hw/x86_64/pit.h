#pragma once 

#include <stdint.h>

void pit_init(int hz);
void pit_sleep(uint16_t ms);