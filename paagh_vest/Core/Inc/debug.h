#ifndef INC_DEBUG
#define INC_DEBUG

#include "stdint.h"
#include <stdarg.h>

void send_command(int command, void *message);

char *itoa(int i);

void prints(const char *str);

void printc(char c);

void printi(int32_t i);

char *convert(unsigned int num, int base);

void print(char *format, ...);

#endif /* INC_DEBUG */