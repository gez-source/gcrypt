#include "gcrypt.h"
#include <string>
#include <iostream>
#include <fstream>

std::string charsetLowerUpperAlphaNumeric = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 ";
std::string charsetLowerAlphaNumeric = "abcdefghijklmnopqrstuvwxyz";
std::string charset = charsetLowerAlphaNumeric;

void GcryptHash(std::string strInput, char digest[43], GCRYPT_CONTEXT_T* ctx)
{
    GCRYPT_Init(ctx);

    GCRYPT_Update(ctx, (const unsigned char*)strInput.c_str(), strInput.length());

    GCRYPT_Final(digest, ctx);
}

std::string Gcrypt(std::string strInput, GCRYPT_CONTEXT_T* ctx)
{
	char digest[43];

	GcryptHash(strInput, digest, ctx);

	return std::string(digest);
}

/// <summary>
/// Execute a simple bruteforce from a - zzzzzzzz.
/// </summary>
void bruteforce_simple_log()
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

	//const double percentInc = 1.0 / cs / cs / cs / cs / cs / cs / cs / cs;
	const double percentInc = 1.0 / cs / cs / cs / cs;
	double percentage = 0;

	time_t start;
	start = time(0);

	std::ofstream myfile;
	myfile.open("gcrypt.csv");
	myfile << "a,b,c,d,e,hashBase16,hashBase10,\n";
	//for (std::string::size_type e = 0; e < charset.size(); e++)
	//{
	//	char t = charset[e];

	//	for (std::string::size_type f = 0; f < charset.size(); f++)
	//	{
	//		char u = charset[f];

	//		for (std::string::size_type g = 0; g < charset.size(); g++)
	//		{
	//			char v = charset[g];

	//			for (std::string::size_type h = 0; h < charset.size(); h++)
	//			{
					//char w = charset[h];

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

									//if (completed_seq_t)
									//{
									//	plaintext += t;
									//}
									//if (completed_seq_u)
									//{
									//	plaintext += u;
									//}
									//if (completed_seq_v)
									//{
									//	plaintext += v;
									//}
									//if (completed_seq_w)
									//{
									//	plaintext += w;
									//}
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

									GCRYPT_CONTEXT_T ctx;

									hash = Gcrypt(plaintext, &ctx);

									uint32_t _a = ctx.state[0];
									uint32_t _b = ctx.state[1];
									uint32_t _c = ctx.state[2];
									uint32_t _d = ctx.state[3];
									uint32_t _e = ctx.state[4];

									//myfile << "a,b,c,d,e,hashBase16,hashBase10,\n";
									std::string hashBase10 = std::to_string(_a) + std::to_string(_b) + std::to_string(_c) + std::to_string(_d) + std::to_string(_e);

									myfile << _a << "," << _b << "," << _c << "," << _d << "," << _e << "," << hash << "," << hashBase10 << ",\n";

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
	//				completed_seq_w = true;
	//			}
	//			completed_seq_v = true;
	//		}
	//		completed_seq_u = true;
	//	}
	//	completed_seq_t = true;
	//}

	myfile.close();

	std::cout << plaintext << " " << hash << std::endl;
	std::cout << "Done." << std::endl;
}


int main(int argc, char **argv)
{
    char hash[43];
    int i;
    const char* data2;

    //char data[] = "abc";
    const char data[] = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";
    size_t length = strlen(data);
    
    //// One million 'a' characters in a row:
    //static const int MILLION = 1000000;
    //char data[1000000];
    //for (i = 0; i < MILLION; i++)
    //{
    //    data[i] = 'a';
    //}
    //size_t length = MILLION;

    //GCRYPT(data, length, hash);
    //printf("%s\n", hash);

    //data2 = "";
    //GCRYPT(data2, strlen(data2), hash);
    //printf("%s\n", hash);

    //data2 = "aaz";
    //GCRYPT(data2, strlen(data2), hash);
    //printf("%s\n", hash);

    //data2 = "aaa";
    //GCRYPT(data2, strlen(data2), hash);
    //printf("%s\n", hash);

    //data2 = "aaaaaaaa";
    //GCRYPT(data2, strlen(data2), hash);
    //printf("%s\n", hash);

    //data2 = "aaaapone";
    //GCRYPT(data2, strlen(data2), hash);
    //printf("%s\n", hash);

    //data2 = "hashcat";
    //GCRYPT(data2, strlen(data2), hash);
    //printf("%s\n", hash);
    
	bruteforce_simple_log();

    return 0;
}