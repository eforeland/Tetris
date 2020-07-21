#include "Point.h"
#include <iostream>
#include <string>

Point::Point() {
	x = 0;
	y = 0;
}

Point::Point(int newX, int newY) {
	x = newX;
	y = newY;
}

int Point::getX() const{
	return this->x;
}

int Point::getY() const{
	return this->y;
}

void Point::setX(int x) {
	this->x = x;
}
void Point::setY(int y) {
	this->y = y;
}
void Point::setXY(int x, int y) {
	this->x = x;
	this->y = y;
}
void Point::swapXY() { //swap x and y
	int temp;
	temp = this->x;
	this->x = this->y;
	this->y = temp;
}

void Point::multiplyX(int factor) { // multiply x by some factor
	(this->x) *= factor;
}
void Point::multiplyY(int factor) { // multiply y by some factor
	(this->y) *= factor;
}

std::string Point::toString() const {
// return a string in the form "[x,y]" to represent the state of the Point instance (for debugging) Use stringstream
	std::string x = std::to_string(this->x);
	std::string y = std::to_string(this->y);

	return "[" + x + "," + y + "]";
}