#ifndef GCRYPT_H
#define GCRYPT_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>		 // For uint32_t.
//#include <sys/types.h> // For uint32_t.

#define GCRYPT_CIRCULAR_LEFT_SHIFT(value,n) (value << n) | (value >> (32 - n))
#define GCRYPT_CIRCULAR_RIGHT_SHIFT(value,n) (value >> n) | (value << (32 - n))

#define GCRYPT_ROUNDS 5

#define GCRYPT_PRIME0 53893699
#define GCRYPT_PRIME1 29602757
#define GCRYPT_PRIME2 69409363
#define GCRYPT_PRIME3 64997063
#define GCRYPT_PRIME4 49541567

struct GCRYPT_CONTEXT_t;
typedef struct GCRYPT_CONTEXT_T
{
	uint32_t state[5];
	uint32_t count[2];
	uint8_t buffer[64];
} GCRYPT_CONTEXT_T;

typedef union {
	unsigned char blockBuffer[64];
	uint32_t blockInput[16];
} GCRYPT_BLOCK_T;

static const int gcrypt_l_rotate[] = { 5, 2, 1, 4, 7, 12 };
static const int gcrypt_r_rotate[] = { 14, 3, 5, 8, 11, 6 };

void gcrypt_int_to_hex(unsigned int integer, unsigned char hexOutput[8]);
void gcrypt_prepend_zeros(char* dest, const char* src, unsigned minimalWidth, char paddingChar);
void gcrypt_state_to_hash(GCRYPT_CONTEXT_T* context, unsigned char outputHash[43]);

void GCRYPT_Transform(uint32_t state[5], const unsigned char buffer[64]);
void GCRYPT_Init(GCRYPT_CONTEXT_T* context);
void GCRYPT_Update(GCRYPT_CONTEXT_T* context, const unsigned char* data, uint32_t len);
void GCRYPT_Final(unsigned char digest[43], GCRYPT_CONTEXT_T* context);
void GCRYPT(const unsigned char* data, unsigned long length, unsigned char digest[43]);


void gcrypt_int_to_hex(unsigned int integer, unsigned char hexOutput[8])
{
	// Hex value is 8 digits long for a uint.
	hexOutput[8] = '0';

	sprintf(hexOutput, "%08x", integer);
}

// see: https://stackoverflow.com/questions/43354488/c-formatted-string-how-to-add-leading-zeros-to-string-value-using-sprintf
// prepend "0" as needed resulting in a string of _minimal_ width.
void gcrypt_prepend_zeros(char* dest, const char* src, unsigned minimalWidth, char paddingChar)
{
	int len = strlen(src);
	int zeros = (len > minimalWidth) ? 0 : minimalWidth - len;
	memset(dest, paddingChar, zeros);
	strcpy(dest + zeros, src);
}
// end of code import.

void gcrypt_state_to_hash(GCRYPT_CONTEXT_T* context, unsigned char outputHash[43])
{
	unsigned char padding[9];

	memset(outputHash, 0, 43);

	memset(padding, '0', 9);
	outputHash[0] = '0';
	outputHash[1] = 'x';

	gcrypt_int_to_hex(context->state[0], padding);
	strcat(outputHash, padding);

	gcrypt_int_to_hex(context->state[1], padding);
	strcat(outputHash, padding);

	gcrypt_int_to_hex(context->state[2], padding);
	strcat(outputHash, padding);

	gcrypt_int_to_hex(context->state[3], padding);
	strcat(outputHash, padding);

	gcrypt_int_to_hex(context->state[4], padding);
	strcat(outputHash, padding);
}

void GCRYPT_Transform(uint32_t state[5], const unsigned char buffer[64])
{
	// Hash a 512-bit block 512 bits is 64 bytes.
	uint32_t a; 
	uint32_t b; 
	uint32_t c; 
	uint32_t d; 
	uint32_t e;
	int roundNum;
	int blockIndex;
	uint32_t blockInput;

	GCRYPT_BLOCK_T block[1];
	memcpy(block, buffer, 64);
	
	a = state[0];
	b = state[1];
	c = state[2];
	d = state[3];
	e = state[4];

	for (blockIndex = 0; blockIndex < 14; blockIndex++)
	{
		blockInput = block[0].blockInput[blockIndex];

		// Combine current blockInput with state.
		a = (a ^ blockInput) * GCRYPT_PRIME0;
		b = (b ^ blockInput) * GCRYPT_PRIME1;
		c = (c ^ blockInput) * GCRYPT_PRIME2;
		d = (d ^ blockInput) * GCRYPT_PRIME3;
		e = (e ^ blockInput) * GCRYPT_PRIME4;

		for (roundNum = 0; roundNum < GCRYPT_ROUNDS; roundNum++)
		{
			// Shuffle state some more:
			if (roundNum % 4 == 0)
			{
				a = GCRYPT_CIRCULAR_RIGHT_SHIFT(a, 13);
			}
			if (roundNum % 3 == 0)
			{
				a = GCRYPT_CIRCULAR_LEFT_SHIFT(a, 7);
			}

			a = GCRYPT_CIRCULAR_LEFT_SHIFT(a, gcrypt_l_rotate[roundNum % 6]);
			a = GCRYPT_CIRCULAR_RIGHT_SHIFT(a, gcrypt_r_rotate[roundNum % 6]);

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

	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;
	state[4] += e;

	// Clear hash state information.
	a = 0;
	b = 0;
	c = 0;
	d = 0;
	e = 0;
	blockInput = 0;
	roundNum = 0;

	memset(block, 0, sizeof(GCRYPT_BLOCK_T));
}

void GCRYPT_Init(GCRYPT_CONTEXT_T* context)
{
	context->state[0] = 19342607; // 127250F
	context->state[1] = 84843461; // 50E9BC5
	context->state[2] = 18026741; // 11310F5
	context->state[3] = 36106927; // 226F2AF
	context->state[4] = 42463199; // 287EFDF
	context->count[0] = 0;
	context->count[1] = 0;
}


void GCRYPT_Update(GCRYPT_CONTEXT_T* context, const unsigned char* data, uint32_t len)
{
	uint32_t i;
	uint32_t j;

	j = context->count[0];
	if ((context->count[0] += len << 3) < j)
	{
		context->count[1]++;
	}
	context->count[1] += (len >> 29);
	j = (j >> 3) & 63;
	if ((j + len) > 63) 
	{
		memcpy(&context->buffer[j], data, (i = 64 - j));
		GCRYPT_Transform(context->state, context->buffer);
		for (; i + 63 < len; i += 64) 
		{
			GCRYPT_Transform(context->state, &data[i]);
		}
		j = 0;
	}
	else i = 0;
	memcpy(&context->buffer[j], &data[i], len - i);
}

void GCRYPT_Final(unsigned char digest[43], GCRYPT_CONTEXT_T* context)
{
	unsigned i;
	unsigned char finalcount[8];
	unsigned char c;

	for (i = 0; i < 8; i++) 
	{
		finalcount[i] = (unsigned char)((context->count[(i >= 4 ? 0 : 1)] >> ((3 - (i & 3)) * 8)) & 255);
	}

	c = 0200;
	GCRYPT_Update(context, &c, 1);
	while ((context->count[0] & 504) != 448) 
	{
		c = 0000;
		GCRYPT_Update(context, &c, 1);
	}
	GCRYPT_Update(context, finalcount, 8);

	gcrypt_state_to_hash(context, digest);

	// Clear hash state information.
	memset(context, 0, sizeof(GCRYPT_CONTEXT_T));
	memset(&finalcount, 0, sizeof(finalcount));
}

void GCRYPT(const unsigned char* data, unsigned long length, unsigned char digest[43])
{
	GCRYPT_CONTEXT_T ctx;
	//unsigned int ii;

	GCRYPT_Init(&ctx);

	// Can update all at once.
	GCRYPT_Update(&ctx, data, length);

	// Can update byte at a time.
	//for (ii = 0; ii < length; ii += 1)
	//{
	//    GCRYPT_Update(&ctx, (const unsigned char*)data + ii, 1);
	//}

	GCRYPT_Final(digest, &ctx);
}

#endif