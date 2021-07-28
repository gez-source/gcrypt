#include "GCrypt.h"

#include <stdio.h>

int main()
{
	// Allocate all the needed memory here once before it is used.
	char* hash = (char*)malloc(sizeof(char) * 43);
	char* padded = (char*)malloc(sizeof(char) * (9));

	printf("%s\n", "gcrypt version 0.1");

	gcrypt_ghash("The quick brown fox jumps over the lazy dog", &hash, &padded);
	printf("%s\n", hash);

	gcrypt_ghash("The quick brown fox jumps over the lazy dog.", &hash, &padded);
	printf("%s\n", hash);

	gcrypt_ghash("the quick brown fox jumps over the lazy dog", &hash, &padded);
	printf("%s\n", hash);

	gcrypt_ghash("", &hash, &padded);
	printf("%s\n", hash);

	free(hash);
	free(padded);

	return 0;
}