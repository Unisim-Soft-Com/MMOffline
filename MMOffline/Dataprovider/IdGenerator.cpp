#include "IdGenerator.h"
#include <chrono>
#include "debugtrace.h"
#include <sstream>
#include <bitset>
IdGenerator::IdGenerator()
	: rngGenerator(std::random_device()()), randint(0, 1023), shiftBorder(10)
{

}

IdGenerator::IdGenerator(int shBorder, int seed)
	: rngGenerator(seed), randint(0, std::pow(2, shBorder)-1), shiftBorder(shBorder)
{
}

IdInt IdGenerator::generate()
{
	IdInt newId;
	newId = std::chrono::system_clock::now().time_since_epoch().count();
	newId <<= shiftBorder;
	unsigned int t = randint(rngGenerator);
	newId += t;
	return newId; 
}

void IdGenerator::seedGenerator(unsigned int seed)
{
	rngGenerator.seed(seed);
}

void IdGenerator::setShiftBorder(int border)
{
	shiftBorder = border;
	randint = std::uniform_int_distribution<int>(0, std::pow(2, border) - 1);
}
IdGenerator mainGenerator;
IdGenerator* const IdGenerator::instance()
{
	return &mainGenerator;
}

IdInt IdGenerator::generateId()
{
	return mainGenerator.generate();
}

IdInt IdGenerator::generateId(int shBorder, int seed)
{
	return IdGenerator(shBorder, seed).generate();
}
