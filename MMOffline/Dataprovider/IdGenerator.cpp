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
	: rngGenerator(seed), randint(0, std::pow(2, shBorder) - 1), shiftBorder(shBorder)
{
}

IdInt IdGenerator::generate()
// generates new GUID
{
	IdInt newId;
	// time sinse epoch obtained
	newId = std::chrono::system_clock::now().time_since_epoch().count();
	// time shifted left until reaches border. More times value is shifted - more space for random number,
	// which increases dispersion in the same time moment, lesser space for time itself, which increases dispersion
	// for all time period
	newId <<= shiftBorder;
	// right null bytes are filled with random number. This increases dispersion in the same ms:
	// because with very high probability numbers will be different with previous generations
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
IdGenerator* IdGenerator::_instanse = new IdGenerator;
IdGenerator* const IdGenerator::instance()
{
	return _instanse;
}

IdInt IdGenerator::generateId()
{
	return _instanse->generate();
}

IdInt IdGenerator::generateId(int shBorder, int seed)
{
	return IdGenerator(shBorder, seed).generate();
}