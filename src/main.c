#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>
#include <string.h>

#include "des.h"

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        printf("Error: incorrect number of arguments.\n");
        return -1;
    }

    FILE *f_in = fopen(argv[2], "rb");
    FILE *f_out = fopen(argv[3], "wb");
    
    if (strcmp(argv[1], "-e") == 0)
    {
        srand(time(NULL)); // for generating key and initialization vector

        FILE *f_key = fopen("key.bin", "wb");
        uint64_t key = generate_key();
        fwrite_uint64(f_key, key);
        
        FILE *f_iv = fopen("iv.bin", "wb");
        uint64_t iv = generate_key();
        fwrite_uint64(f_iv, iv);

        uint64_t key_buffer[16];
        generate_subkeys(key, key_buffer);

        uint64_t block = 0;
        uint64_t xored_block = 0;
        uint64_t ciphered_block = 0;
        
        while (fread(&block, sizeof(uint8_t), 8, f_in))
        {   
            xored_block = block ^ iv;
            ciphered_block = encrypt_block(key_buffer, xored_block);
            iv = ciphered_block;
            
            fwrite(&ciphered_block, sizeof(uint64_t), 1, f_out);

            block = 0;
        }

        fclose(f_iv);
        fclose(f_key);
    }
    else if (strcmp(argv[1], "-d") == 0)
    {
        FILE *f_key = fopen("key.bin", "rb");
        uint64_t key = 0;
        fread(&key, sizeof(uint64_t), 1, f_key);
        
        FILE *f_iv = fopen("iv.bin", "rb");
        uint64_t iv = 0;
        fread(&iv, sizeof(uint64_t), 1, f_iv);

        uint64_t key_buffer[16];
        generate_subkeys(key, key_buffer);
        reverse_key_buffer(key_buffer);

        uint64_t ciphered_block = 0;
        uint64_t xored_block = 0;
        uint64_t block = 0;

        while (fread(&ciphered_block, sizeof(uint8_t), 8, f_in))
        {   
            block = encrypt_block(key_buffer, ciphered_block);
            xored_block = iv ^ block;
            iv = ciphered_block;
            fwrite(&xored_block, sizeof(uint64_t), 1, f_out);
            
            ciphered_block = 0;
        }

        fclose(f_iv);
        fclose(f_key);
    }
    else
    {
        printf("Error: incorrect option.\n");
    }

    fclose(f_out);
    fclose(f_in);

    return 0;
}
