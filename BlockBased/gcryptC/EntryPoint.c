#include "gcrypt.h"

int main(int argc, char **argv)
{
    unsigned char hash[43];
    int i;

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

    return 0;
}