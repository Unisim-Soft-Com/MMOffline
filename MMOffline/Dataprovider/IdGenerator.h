#pragma once
#include <random>

/* 
	This file contains GUID generator, which is used for assigning unique id to new elements
	without collisions. This is achieved with standard GUID generation, which guaranties ~ 95/100 unique values within 
	one millisecond and 100% unique with one ms delay. Random guaranties that you can create more than one
	generators with still unique IDs.

	Singletone pattern, you don't need two instanses.
*/

// typedef for normal id int
typedef long long int IdInt;

class IdGenerator
{
	std::mt19937 rngGenerator; // random generator engine
	std::uniform_int_distribution<int> randint;	// distribution used in the generator
	int shiftBorder;	//	border of shift. Larger value means larger time independense and smaller period
	// of non-repeated header

	static IdGenerator* _instanse;

public:
	IdGenerator();
	IdGenerator(int shBorder, int seed);
	IdInt generate();
	// reloads generator with the new seed
	void seedGenerator(unsigned int seed);
	// moves shift border
	void setShiftBorder(int border);

	// makes instance if it does not exists. Singleton template
	static IdGenerator* const instance();
	// equal to IdGenerator::instance()->generate()
	static IdInt generateId();
	// creates proxy generator with new border and seed values, then generates ID
	static IdInt generateId(int shBorder, int seed);
};