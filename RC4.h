#ifndef RC4_H
#define RC4_H
// #include<stdio.h>
// #include<stdlib.h>
#include"op.h"

#define tb_length 256
typedef struct table256{
    uint8_t table[tb_length];
}table256;

typedef struct tableNormal{
    uint32_t len;
    uint8_t *table;
}tableNormal;
#endif
