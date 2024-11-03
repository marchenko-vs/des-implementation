# DES implementation

## Implementation of Data Encryption Standard with CBC mode in C

You can download **des.c** and **des.h** files and use them in your projects for 
encrypting and decrypting data.

## How to use

```console
$ make
$ ./app.exe -e [origin_file] [encrypted_file]
$ ./app.exe -d [encrypted_file] [decrypted_file]
$ make clean
```

You can encrypt and decrypt any files you want.
It doesn't matter if it's just a simple text or a 2-hour movie.
But if an input file is big enough, you will have to wait patiently.

## Description

Basically, the program consists of the following stages.
1. Generating 64-bit key and initialization vector using random generator.
2. Saving down the key and the initialization vector in files.
3. Generating sixteen 48-bit subkeys.
4. Encrypting 64-bit block using subkeys.

In order to decrypt data, you need to reverse the order of subkeys. 
Everything else remains the same.

## Useful links

[An awesome explanation of the whole DES algorithm with an example](https://page.math.tu-berlin.de/~kant/teaching/hess/krypto-ws2006/des.htm#:~:text=For%20example%2C%20if%20we%20take,the%20original%20plaintext%20%228787878787878787%22)

[An explanation of CBC mode](https://en.wikipedia.org/wiki/Block_cipher_mode_of_operation)
