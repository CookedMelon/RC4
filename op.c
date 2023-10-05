#include "op.h"

void printBytes(uint8_t *bytes, size_t len){
    for (size_t i = 0; i < len; ++i)
        printf("%02hhx ", i[bytes]);
    printf("\n");
}

void printBytesInorder(uint8_t *bytes, size_t len){
    for (size_t i = 0; i < len; ++i)
        printf("%01hhx%01hhx", i[bytes]&0xf, i[bytes]>>4);
    printf("\n");
}

void processBuffer(unsigned char *buffer, size_t size) {
    for (size_t i = 0; i < size; i++) {
        buffer[i]++;  // Just a simple encoding by adding 1 to each byte
    }
}