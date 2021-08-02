#include "gcrypt.h"

int main(int argc, char **argv)
{
    unsigned char hash[43];
    int i;
    char* data2;

    //char data[] = "abc";
    char data[] = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";
    size_t length = strlen(data);
    
    //// One million 'a' characters in a row:
    //static const int MILLION = 1000000;
    //char data[1000000];
    //for (i = 0; i < MILLION; i++)
    //{
    //    data[i] = 'a';
    //}
    //size_t length = MILLION;

    GCRYPT(data, length, hash);
    printf("%s\n", hash);

    data2 = "";
    GCRYPT(data2, strlen(data2), hash);
    printf("%s\n", hash);

    data2 = "aaz";
    GCRYPT(data2, strlen(data2), hash);
    printf("%s\n", hash);

    data2 = "aaa";
    GCRYPT(data2, strlen(data2), hash);
    printf("%s\n", hash);

    data2 = "aaaaaaaa";
    GCRYPT(data2, strlen(data2), hash);
    printf("%s\n", hash);

    data2 = "aaaapone";
    GCRYPT(data2, strlen(data2), hash);
    printf("%s\n", hash);

    data2 = "hashcat";
    GCRYPT(data2, strlen(data2), hash);
    printf("%s\n", hash);
    
    return 0;
}