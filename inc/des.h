#ifndef _DES_H_
#define _DES_H_

#include <stdio.h>
#include <inttypes.h>

uint64_t generate_key(void);
void fwrite_uint64(FILE *f, const uint64_t num);
void reverse_key_buffer(uint64_t key_buffer[16]);
void generate_subkeys(const uint64_t key_64_bits, uint64_t key_buffer[16]);
uint64_t encrypt_block(uint64_t key_buffer[16], uint64_t block);

#endif // _DES_H_
