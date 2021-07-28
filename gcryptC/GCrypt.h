#ifndef GCRYPT_H
#define GCRYPT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define FILE_BUFFER_SIZE 8000

const int security_rounds = 40;

const char* hex_prefix = "0x";

unsigned int a = 0;
unsigned int b = 0;
unsigned int c = 0;
unsigned int d = 0;
unsigned int e = 0;

unsigned int prime = 0;
unsigned int prime2 = 0;
unsigned int prime3 = 0;
unsigned int prime4 = 0;
unsigned int prime5 = 0;

/// <summary>
/// Initilises the state of the hash function.
/// </summary>
void gcrypt_initilise_state()
{
	// Primes generated by bigprimes.org.
	prime = 53893699;
	prime2 = 29602757;
	prime3 = 69409363;
	prime4 = 64997063;
	prime5 = 49541567;

	// Hash state information.
	a = 19342607;
	b = 84843461;
	c = 18026741;
	d = 36106927;
	e = 42463199; // More prime numbers.
}

void gcrypt_int_to_hex(unsigned int integer, char** hexOutput)
{
	//char* hex = malloc(sizeof(char) * (8 + 1));

	// Hex value is 8 digits long for a uint.
	char* h = *hexOutput;
	h[8] = '0';

	sprintf(h, "%x", integer);

	*hexOutput = h;
}

// see: https://stackoverflow.com/questions/43354488/c-formatted-string-how-to-add-leading-zeros-to-string-value-using-sprintf
// prepend "0" as needed resulting in a string of _minimal_ width.
void gcrypt_prepend_zeros(char* dest, const char* src, unsigned minimal_width, char padding_char)
{
	int len = strlen(src);
	int zeros = (len > minimal_width) ? 0 : minimal_width - len;
	memset(dest, padding_char, zeros);
	strcpy(dest + zeros, src);
}
// end of code import.

void gcrypt_state_to_hash(char** outputHash, char** padding)
{
	char* hex_string = *outputHash;
	
	memset(hex_string, 0, 43);
	hex_string[42] = '0';

	char* padded = *padding;
	
	memset(padded, '0', 9);

	strcat(hex_string, "0x");

	gcrypt_int_to_hex(a, &padded);
	gcrypt_prepend_zeros(padded, padded, 8, '0');
	strcat(hex_string, padded);

	gcrypt_int_to_hex(b, &padded);
	gcrypt_prepend_zeros(padded, padded, 8, '0');
	strcat(hex_string, padded);

	gcrypt_int_to_hex(c, &padded);
	gcrypt_prepend_zeros(padded, padded, 8, '0');
	strcat(hex_string, padded);

	gcrypt_int_to_hex(d, &padded);
	gcrypt_prepend_zeros(padded, padded, 8, '0');
	strcat(hex_string, padded);

	gcrypt_int_to_hex(e, &padded);
	gcrypt_prepend_zeros(padded, padded, 8, '0');
	strcat(hex_string, padded);

	*padding = padded;

	*outputHash = hex_string;
}

//  see: https://stackoverflow.com/questions/35167/is-there-a-way-to-perform-a-circular-bit-shift-in-c
unsigned int gcrypt_circular_left_shift(unsigned int value, unsigned int n)
{
	return (value << n) | (value >> (32 - n));
}
unsigned int gcrypt_circular_right_shift(unsigned int value, unsigned int n)
{
	return (value >> n) | (value << (32 - n));
}
// end of code import

/// <summary>
/// Performs a round over the state of the hash function.
/// </summary>
void gcrypt_execute_round()
{
	int l_rotate[] = { 5, 2, 1, 4, 7, 12 };
	int r_rotate[] = { 14, 3, 5, 8, 11, 6 };

	for (int roundNum = 0; roundNum < security_rounds; roundNum++)
	{
		if (roundNum % 4 == 0)
		{
			a = gcrypt_circular_right_shift(a, 13);
		}
		if (roundNum % 3 == 0)
		{
			a = gcrypt_circular_left_shift(a, 7);
		}

		//a = gcrypt_circular_left_shift(a, 13);

		a = gcrypt_circular_left_shift(a, l_rotate[roundNum % 6]);
		a = gcrypt_circular_right_shift(a, r_rotate[roundNum % 6]);

		a = (a ^ b);
		a = (a ^ c);
		a = (a ^ d);
		a = (a ^ e);

		b = (b ^ a);
		b = (b ^ c);
		b = (b ^ d);
		b = (b ^ e);

		c = (c ^ a);
		c = (c ^ b);
		c = (c ^ d);
		c = (c ^ e);

		d = (d ^ a);
		d = (d ^ b);
		d = (d ^ c);
		d = (d ^ e);

		e = (e ^ a);
		e = (e ^ b);
		e = (e ^ c);
		e = (e ^ d);
	}
}

/// <summary>
/// Processand shred the specified byte into hash state.
/// </summary>
void gcrypt_process_byte_to_hash(unsigned char byteInput)
{
	// Combine current byteInput with state.
	a = (a ^ byteInput) * prime;
	b = (b ^ byteInput) * prime2;
	c = (c ^ byteInput) * prime3;
	d = (d ^ byteInput) * prime4;
	e = (e ^ byteInput) * prime5;

	// Execute the specified number of rounds shuffling hash state some more.
	gcrypt_execute_round();
}

/// <summary>
/// Hash the specified input string and return the resultant hash as a string.
/// </summary>
void gcrypt_ghash(char* inputString, char** outputHash, char** padded)
{
	gcrypt_initilise_state();
	gcrypt_execute_round();

	for (int i = 0; i < strlen(inputString); i++)
	{
		uint8_t c = inputString[i];

		gcrypt_process_byte_to_hash(c);
	}

	gcrypt_state_to_hash(outputHash, padded);
}

void gcrypt_ghash_file(char* fileName, char** outputHash, char** padded)
{
	gcrypt_initilise_state();
	gcrypt_execute_round();

	uint8_t buffer[FILE_BUFFER_SIZE] = { 0 };
	int bytesRead = 0;
	int i;
	int bufferSize = sizeof(buffer);

	FILE* fp = fopen(fileName, "rb");

	if (!fp) 
	{
		fprintf(stderr, "error: file open failed '%s'.\n", fileName);
		return 1;
	}

	while ((bytesRead = fread(buffer, sizeof * buffer, bufferSize, fp)) == bufferSize)
	{
		for (i = 0; i < bufferSize; i++)
		{
			gcrypt_process_byte_to_hash(buffer[i]);
		}
	}

	for (i = 0; i < bytesRead; i++)
	{
		gcrypt_process_byte_to_hash(buffer[i]);
	}

	if (fp != stdin)
	{
		fclose(fp);
	}

	gcrypt_state_to_hash(outputHash, padded);
}

#endif