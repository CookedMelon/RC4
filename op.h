#ifndef OP_H
#define OP_H
#include <stdint.h>
#include "u512.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void printBytes(uint8_t *bytes, size_t len);
void printBytesInorder(uint8_t *bytes, size_t len);

#endif