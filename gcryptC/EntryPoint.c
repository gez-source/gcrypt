#include "GCrypt.h"

#include <stdio.h>

int main()
{
	char* h;

	printf("%s\n", "gcrypt version 0.1");

	h = gcrypt_ghash("The quick brown fox jumps over the lazy dog");
	printf("%s\n", h);
	free(h);

	h = gcrypt_ghash("The quick brown fox jumps over the lazy dog.");
	printf("%s\n", h);
	free(h);

	h = gcrypt_ghash("the quick brown fox jumps over the lazy dog");
	printf("%s\n", h);
	free(h);

	h = gcrypt_ghash("");
	printf("%s\n", h);
	free(h);

	return 0;
}