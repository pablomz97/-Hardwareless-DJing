#include "Point.h"

bool Punto::initialized() {
	if (x != -1 && y != -1) {
		return true;
	}
	return false;
}

ostream &operator<<(ostream &os, Punto const &m) {
	return os << "X: " << m.x << " Y: " << m.y << endl;
}