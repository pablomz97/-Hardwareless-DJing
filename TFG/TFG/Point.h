#ifndef POINT_H
#define POINT_H

#include <iostream>
#include <string>

using namespace std;

class Punto {
public:
	float x;
	float y;
	unsigned int(*pFunc)(string, int);

	Punto(float x = 0, float y = 0, unsigned int(*func)(string, int) = nullptr) : x(x), y(y), pFunc(func) {};

	bool initialized();
};

ostream &operator<<(ostream &os, Punto const &m);

#endif
