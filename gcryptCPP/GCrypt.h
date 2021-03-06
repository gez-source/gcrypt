#ifndef GCRYPT_H
#define GCRYPT_H

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include <chrono>
#include <thread>
#include <cmath>
#include <ctime>

#include <stdint.h>   // for uint32_t
#include <limits.h>   // for CHAR_BIT
#include <time.h>

typedef uint8_t byte; //typedef unsigned char byte;
typedef uint32_t uint;
//typedef int32_t int;

class GCrypt
{
public:
	std::string charsetLowerUpperAlphaNumeric = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 ";
	std::string charsetLowerAlphaNumeric = "abcdefghijklmnopqrstuvwxyz";
	std::string charset = charsetLowerAlphaNumeric;

	int security_rounds = 40;

	std::string hex_prefix = "0x";

	uint32_t a = 0;
	uint32_t b = 0;
	uint32_t c = 0;
	uint32_t d = 0;
	uint32_t e = 0;

	uint32_t prime = 0;
	uint32_t prime2 = 0;
	uint32_t prime3 = 0;
	uint32_t prime4 = 0;
	uint32_t prime5 = 0;

	std::clock_t start;
public:

	/// <summary>
	/// Hash the specified input string and return the resultant hash as a string.
	/// </summary>
	std::string ghash(std::string inputString)
	{
		initilise_state();
		execute_round();

		for (std::string::size_type i = 0; i < inputString.size(); i++)
		{
			char c = inputString[i];

			process_byte_to_hash(c);
		}

		return state_to_hash();
	}

	std::string ghash_file(std::string fileName)
	{
		initilise_state();
		execute_round();

		start_timer();

		std::ifstream in;

		in.open(fileName, std::ifstream::in | std::ios::binary);
		if (in.fail()) return "";

		in >> std::noskipws; // Disable whitespace skipping behaviour

		while (!in.eof())
		{
			byte byteInput;

			in >> byteInput;

			if (in.fail())
			{
				break;
			}

			process_byte_to_hash(byteInput);
		}

		stop_timer();

		return state_to_hash();
	}

	/// <summary>
	/// Initilises the state of the hash function.
	/// </summary>
	void initilise_state()
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
	/// <summary>
	/// Processand shred the specified byte into hash state.
	/// </summary>
	void process_byte_to_hash(byte byteInput)
	{
		// Combine current byteInput with state.
		a = (a ^ byteInput) * prime;
		b = (b ^ byteInput) * prime2;
		c = (c ^ byteInput) * prime3;
		d = (d ^ byteInput) * prime4;
		e = (e ^ byteInput) * prime5;

		// Execute the specified number of rounds shuffling hash state some more.
		execute_round();
	}

	/// <summary>
	/// Performs a round over the state of the hash function.
	/// </summary>
	void execute_round()
	{
		int l_rotate[] = { 5, 2, 1, 4, 7, 12 };
		int r_rotate[] = { 14, 3, 5, 8, 11, 6 };

		for (int roundNum = 0; roundNum < security_rounds; roundNum++)
		{
			if (roundNum % 4 == 0)
			{
				a = circular_right_shift(a, 13);
			}
			if (roundNum % 3 == 0)
			{
				a = circular_left_shift(a, 7);
			}

			//a = circular_left_shift(a, 13);

			a = circular_left_shift(a, l_rotate[roundNum % 6]);
			a = circular_right_shift(a, r_rotate[roundNum % 6]);

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

	std::string state_to_hash()
	{
		std::string hex_string = int_to_hex(a) + int_to_hex(b) + int_to_hex(c)
			+ int_to_hex(d) + int_to_hex(e);

		hex_string = pad_left(hex_string, 40, '0');

		return hex_prefix + hex_string;
	}

	//  see: https://stackoverflow.com/questions/35167/is-there-a-way-to-perform-a-circular-bit-shift-in-c
	uint32_t circular_left_shift(uint32_t value, uint32_t n)
	{
		return (value << n) | (value >> (32 - n));
	}
	uint32_t circular_right_shift(uint32_t value, uint32_t n)
	{
		return (value >> n) | (value << (32 - n));
	}
	// end of code import

	std::string int_to_hex(uint integer)
	{
		std::stringstream stream;
		stream << std::hex << integer;
		std::string result(stream.str());
		return result;
	}

	std::string pad_left(std::string inputString, uint paddingTimes, char paddingCharacter)
	{
		std::string padding = std::string(paddingTimes - inputString.length(), paddingCharacter);
		std::string result = padding + inputString;

		return result;
	}

	std::string get_safe_string(std::string unsafeStr)
	{
		for (int i = 0; i < unsafeStr.length(); i++)
		{
			byte c = unsafeStr[i];

			if (c < 32 || c > 126)
			{
				unsafeStr[i] = '.';
			}
		}

		return unsafeStr;
	}

	void start_timer()
	{
		start = std::clock();
	}

	void stop_timer()
	{
		auto totalTime = (std::clock() - start);
		int ms = totalTime / (double)(CLOCKS_PER_SEC / 1000);
		int secs = ms / 1000;

		std::cout << "Finished in " << secs << "secs." << std::endl;
	}

	/// <summary>
	/// Execute a simple bruteforce from a - zzzz.
	/// </summary>
	void bruteforce_simple()
	{
		bool completed_seq_t = false;
		bool completed_seq_u = false;
		bool completed_seq_v = false;
		bool completed_seq_w = false;
		bool completed_seq_x = false;
		bool completed_seq_y = false;
		bool completed_seq_z = false;

		std::string plaintext;
		std::string hash;
		int cs = charset.size();

		const double percentInc = 1.0 / cs / cs / cs / cs / cs / cs / cs / cs;
		double percentage = 0;

		time_t start;
		start = time(0);

		start_timer();

		for (std::string::size_type e = 0; e < charset.size(); e++)
		{
			char t = charset[e];

			for (std::string::size_type f = 0; f < charset.size(); f++)
			{
				char u = charset[f];

				for (std::string::size_type g = 0; g < charset.size(); g++)
				{
					char v = charset[g];

					for (std::string::size_type h = 0; h < charset.size(); h++)
					{
						char w = charset[h];

						for (std::string::size_type i = 0; i < charset.size(); i++)
						{
							char x = charset[i];

							for (std::string::size_type j = 0; j < charset.size(); j++)
							{
								char y = charset[j];

								for (std::string::size_type k = 0; k < charset.size(); k++)
								{
									char z = charset[k];

									for (std::string::size_type l = 0; l < charset.size(); l++)
									{
										char a = charset[l];

										plaintext = "";

										if (completed_seq_t)
										{
											plaintext += t;
										}
										if (completed_seq_u)
										{
											plaintext += u;
										}
										if (completed_seq_v)
										{
											plaintext += v;
										}
										if (completed_seq_w)
										{
											plaintext += w;
										}
										if (completed_seq_x)
										{
											plaintext += x;
										}
										if (completed_seq_y)
										{
											plaintext += y;
										}
										if (completed_seq_z)
										{
											plaintext += z;
										}

										plaintext += a;

										hash = ghash(plaintext);

										percentage += percentInc;

										//std::cout << plaintext << " " << hash << " " << (percentage * 100) << "%" << std::endl;

										int nSeconds = 1;

										if (time(0) - start > nSeconds)
										{
											std::cout << plaintext << " " << hash << " " << (percentage * 100) << "%" << std::endl;

											start = start + nSeconds;
										}
									}
									completed_seq_z = true;
								}
								completed_seq_y = true;
							}
							completed_seq_x = true;
						}
						completed_seq_w = true;
					}
					completed_seq_v = true;
				}
				completed_seq_u = true;
			}
			completed_seq_t = true;
		}
		std::cout << plaintext << " " << hash << std::endl;
		stop_timer();
		std::cout << "Done." << std::endl;
	}

	void collision_test()
	{
		// Toggle the nth bit in the string.
		std::string collision_target = ghash("the quick brown fox jumps over the lazy dog");
		std::string test = "the quick brown fox jumps over the lazy dog";
		int numCollisions = 0;
		int sizeOfByte = 256;

		const double percentInc = 1.0 / sizeOfByte / sizeOfByte / sizeOfByte / sizeOfByte;
		double percentage = 0;

		time_t start;
		start = time(0);

		for (int i = 0; i < sizeOfByte; i++)
		{
			for (int j = 0; j < sizeOfByte; j++)
			{
				for (int k = 0; k < sizeOfByte; k++)
				{
					for (int l = 0; l < sizeOfByte; l++)
					{
						test = "the quick brown fox jumps over the lazy dog";

						char* c = (char*)test.c_str();

						c[2] = (byte)i;
						c[2] = (byte)j;
						c[1] = (byte)k;
						c[0] = (byte)l;

						std::string hash = ghash(test);

						percentage += percentInc;

						if (collision_target == hash)
						{
							numCollisions++;

							//std::cout << GetSafeString(test) << " [" << hash << "] collisions: " << numCollisions << " " << (percentage * 100) << "%" << std::endl;
						}
						else
						{
							//std::cout << GetSafeString(test) << " " << hash << " collisions: " << numCollisions << " " << (percentage * 100) << "%" << std::endl;
						}

						int nSeconds = 1;

						if (time(0) - start > nSeconds)
						{
							std::cout << get_safe_string(test) << " " << hash << " collisions: " << numCollisions << " " << (percentage * 100) << "%" << std::endl;


							start = start + nSeconds;
						}
					}
				}
			}
		}

		std::cout << "Done. Collisions: " << numCollisions << std::endl;
	}
};


#endif