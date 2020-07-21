#include "Tetromino.h"
#include <vector>
#include "Point.h"

Tetromino::Tetromino() {
	setShape(TetShape::SHAPE_S);

}

TetColor Tetromino::getColor() const {
	return color;
}

TetShape Tetromino::getShape() const {
	return shape;
}

TetShape Tetromino::getRandomShape() const {
	int shapeIndex = rand() % TetShape::COUNT;
	
	return static_cast<TetShape>(shapeIndex);

}


void Tetromino::setShape(TetShape shape) {
	// set the shape
	//  - clear any blockLocs set previously
	//  - set the blockLocs for the shape
	//  - set the color for the shape
	blockLocs.clear();

	if (shape == TetShape::SHAPE_S) {
		color = TetColor::RED;
		//blockLocs = {Point(0,0), Point(0,1).... }

		blockLocs.push_back(Point(-1, 0));
		blockLocs.push_back(Point(0, 0));
		blockLocs.push_back(Point(0, 1));
		blockLocs.push_back(Point(1, 1));
	}

	if (shape == TetShape::SHAPE_Z) {
		color = TetColor::ORANGE;

		blockLocs.push_back(Point(-1, 1));
		blockLocs.push_back(Point(0, 1));
		blockLocs.push_back(Point(0, 0));
		blockLocs.push_back(Point(1, 0));
	}

	if (shape == TetShape::SHAPE_L) {
		color = TetColor::YELLOW;

		blockLocs.push_back(Point(0, 2));
		blockLocs.push_back(Point(0, 1));
		blockLocs.push_back(Point(0, 0));
		blockLocs.push_back(Point(1, 0));
	}

	if (shape == TetShape::SHAPE_J) {
		color = TetColor::GREEN;

		blockLocs.push_back(Point(-1, -1));
		blockLocs.push_back(Point(0, -1));
		blockLocs.push_back(Point(0, 0));
		blockLocs.push_back(Point(0, 1));
	}

	if (shape == TetShape::SHAPE_O) {
		color = TetColor::BLUE_LIGHT;

		blockLocs.push_back(Point(1, 0));
		blockLocs.push_back(Point(0, 1));
		blockLocs.push_back(Point(0, 0));
		blockLocs.push_back(Point(1, 1));
	}

	if (shape == TetShape::SHAPE_I) {
		color = TetColor::BLUE_DARK;

		blockLocs.push_back(Point(0, 2));
		blockLocs.push_back(Point(0, 1));
		blockLocs.push_back(Point(0, 0));
		blockLocs.push_back(Point(0, -1));
	}

	if (shape == TetShape::SHAPE_T) {
		color = TetColor::PURPLE;

		blockLocs.push_back(Point(-1, 0));
		blockLocs.push_back(Point(0, -1));
		blockLocs.push_back(Point(0, 0));
		blockLocs.push_back(Point(1, 0));
	}
}

void Tetromino::rotateCW() {
	// rotate the shape 90 degrees around [0,0] (clockwise)
	// to do this:
	//  - iterate through blockLocs
	//  - rotate each Point 90 degrees around [0,0]
	// hint: rotate the point [1,2] clockwise around [0,0] and note
	// how the x,y values change. There are 2 functions in the Point
	// class that can be used to accomplish a rotation.	 

	for (int i = 0; i < blockLocs.size(); i++) {
		blockLocs[i].multiplyX(-1);
		blockLocs[i].swapXY();
	}

}

void Tetromino::printToConsole() const {
	

	for (int y = 3; y > -3; y--) {
		for (int x = -4; x < 4; x++) {

			bool match = false;
			// test if one of our points matches the current x,y
			for (int i = 0; i < blockLocs.size(); i++) {
				Point loc = blockLocs[i];

				if (loc.getX() == x && loc.getY() == y) {
					 match = true;
					 break;
				}

			}

			if (match) {
				std::cout << "x";
			}
			else {
				std::cout << ".";
			}
			
		}
		std::cout << '\n';
	}

	// print a grid to display the current shape
	// to do this:
	// print out a “grid” of text to represent a co-ordinate
	// system.  Start at top left [-3,3] go to bottom right [3,-3]
	// (use nested for loops)
	//  for each [x,y] point, loop through the blockLocs and if
	//  the point exists in the list, print an 'x' instead of a '.'
	// You should end up with something like this:
	// (results will vary depending on shape and rotation, eg: this
	//  one shows a T shape rotated clockwise once)
	//  .......
	//  .......
	//  ...x...
	//  ..xx...
	//  ...x...
	//  .......



}