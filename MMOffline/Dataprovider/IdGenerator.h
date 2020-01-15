#pragma once
#include <random>

typedef long long int IdInt;

class IdGenerator
{
	std::mt19937 rngGenerator;
	std::uniform_int_distribution<int> randint;
	int shiftBorder;

public:
	IdGenerator();
	IdGenerator(int shBorder, int seed);
	IdInt generate();
	void seedGenerator(unsigned int seed);
	void setShiftBorder(int border);

	static IdGenerator* const instance();
	static IdInt generateId();
	static IdInt generateId(int shBorder, int seed);
};