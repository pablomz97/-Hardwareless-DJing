#ifndef DIM_H
#define DIM_H

#include <algorithm>
#include "Point.h"

class Dimension {
public:
	float minX, centX, maxX;
	float minY, centY, maxY;

	Dimension();
	Dimension(float minX, float maxX, float minY, float maxY);
	Dimension cut(Dimension d);
	bool intersects(Dimension d);
	bool isContained(Dimension d);
	bool operator==(Dimension d);
};

#endif
