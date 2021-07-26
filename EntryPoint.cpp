#include "GCrypt.h"

int main()
{
	//std::cout << "gcrypt version 0.1" << std::endl;

	GCrypt g;
	std::cout << g.ghash("The quick brown fox jumps over the lazy dog") << std::endl;
	std::cout << g.ghash("The quick brown fox jumps over the lazy dog.") << std::endl;
	std::cout << g.ghash("the quick brown fox jumps over the lazy dog") << std::endl;
	std::cout << g.ghash("") << std::endl;

	//g.collision_test();

	g.bruteforce_simple();

	return 0;
}