#!bin/sh

echo -e "Compiling\n"
make

let i=1

echo -e "Testing\n"
for src_file in ./tests/*
do
    echo "Test # $i"
    echo "Encrypting $src_file"
    ./app.exe -e $src_file ${src_file//test_/enc_}
    
    echo "Decrypting ${src_file//test_/enc_}"
    ./app.exe -d ${src_file//test_/enc_} ${src_file//test_/dec_}
    
    let i=i+1
done
