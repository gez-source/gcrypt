#include "GCrypt.h"

#include <stdio.h>

int main()
{
	// Allocate all the needed memory on the stack here before it is used.
	// No dynamic memory allocation is used at all in this project.
	unsigned char hash[43];

	printf("%s\n", "gcrypt version 0.1");

	gcrypt_ghash("The quick brown fox jumps over the lazy dog", hash);
	printf("%s\n", hash);

	gcrypt_ghash("The quick brown fox jumps over the lazy dog.", hash);
	printf("%s\n", hash);

	gcrypt_ghash("the quick brown fox jumps over the lazy dog", hash);
	printf("%s\n", hash);

	gcrypt_ghash("", hash);
	printf("%s\n", hash);

	gcrypt_ghash("cdvpkddo", hash);
	printf("%s\n", hash);

	gcrypt_ghash("sarcanan", hash);
	printf("%s\n", hash);

	gcrypt_ghash("chapderd", hash);
	printf("%s\n", hash);
	
	gcrypt_ghash("chadstan", hash);
	printf("%s\n", hash);

	gcrypt_ghash("chadygin", hash);
	printf("%s\n", hash);

	gcrypt_ghash("chaderin", hash);
	printf("%s\n", hash);

	gcrypt_ghash("chadperd", hash);
	printf("%s\n", hash);

	gcrypt_ghash("aaaaaman", hash);
	printf("%s\n", hash);

	gcrypt_ghash("hashcat", hash);
	printf("%s\n", hash);

	gcrypt_ghash("aaaaqwan", hash);
	printf("%s\n", hash);
	
	gcrypt_ghash("aaaaaaaa", hash);
	printf("%s\n", hash);

	gcrypt_ghash("aaa", hash);
	printf("%s\n", hash);

	return 0;
}