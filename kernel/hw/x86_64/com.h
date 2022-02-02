#pragma once 

#define SERIAL_COM1 0x3f8

void serial_puts(char const *s);
void serial_putc(char c);