#include "Dimension.h"

//constructor
Dimension::Dimension() {
	minX = 0;
	maxX = 0;
	minY = 0;
	maxY = 0;
	centX = 0;
	centY = 0;
}

//constructor
Dimension::Dimension(float minX, float minY, float maxX, float maxY) {
	this->minX = minX;
	this->maxX = maxX;
	this->minY = minY;
	this->maxY = maxY;
	this->centX = (minX + maxX) / 2;
	this->centY = (minY + maxY) / 2;
}

//Returns dimension object \ d
Dimension Dimension::cut(Dimension d) {
	Dimension y;
	y.minX = max(this->minX, d.minX);
	y.maxX = min(this->maxX, d.maxX);
	y.minY = max(this->minY, d.minY);
	y.maxY = min(this->maxY, d.maxY);
	return y;
}

//checks if both dimensions intersect
bool Dimension::intersects(Dimension d) {
	if (this->minX < d.minX && this->maxX > d.minX ||
		this->minX < d.maxX && this->maxX > d.maxX ||
		this->minY < d.minY && this->maxY > d.minY ||
		this->minY < d.maxY && this->maxY > d.maxY) {
		return true;
	}
	return false;
}

//returns true if dimension object is contained inside d
bool Dimension::isContained(Dimension d) {
	if (minX >= d.minX && maxX <= d.maxX && minY >= d.minY && maxY <= d.maxY ) {
		return true;
	}
	return false;
}

//override of == operator
bool Dimension::operator==(Dimension d) {
	if (minX == d.minX && maxX == d.maxX && minY == d.minY && maxY == d.maxY) {
		return true;
	}
	return false;
}