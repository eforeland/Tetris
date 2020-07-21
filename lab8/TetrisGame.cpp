#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
#include "GridTetromino.h"
#include "TetrisGame.h"
#include "TestSuite.h"
#include "Point.h"

// constructor
//   assign pointers,
//   load font from file: fonts/RedOctober.ttf
//   setup scoreText
//   reset the game

TetrisGame::TetrisGame(sf::RenderWindow* pWindow, sf::Sprite* pBlockSprite, Point gameboardOffset, Point nextShapeOffset) {
	this->pWindow = pWindow;
	this->pBlockSprite = pBlockSprite;
	this->gameboardOffset = gameboardOffset;
	this->nextShapeOffset = nextShapeOffset;

	board.setContent(Gameboard::MAX_X / 2, Gameboard::MAX_Y / 2, 1); // insert locked block for testing.

	TetShape random = currentShape.Tetromino::getRandomShape();
	currentShape.Tetromino::setShape(random);

	const Point spawn = board.Gameboard::getSpawnLoc();
	currentShape.GridTetromino::setGridLoc(spawn);
}


// destructor, set pointers to null
TetrisGame::~TetrisGame() {

	pWindow = nullptr;
	pBlockSprite = nullptr;

}

// draw anything to do with the game,
// includes board, currentShape, nextShape, score
void TetrisGame::draw() {
	drawGameboard();
	drawTetromino(currentShape, gameboardOffset);
}

// Event and game loop processing
// handles keypress events (up, left, right, down, space)
void TetrisGame::onKeyPressed(sf::Event event) {
	if (event.key.code == sf::Keyboard::Up)
		if (attemptRotate(currentShape))
			bool rotate = true;

	if (event.key.code == sf::Keyboard::Right)
		if (attemptMove(currentShape, 0, 1))
			bool right = true;

	if (event.key.code == sf::Keyboard::Left)
		if (attemptMove(currentShape, 0, -1))
			bool left = true;

	if (event.key.code == sf::Keyboard::Space) {
		drop(currentShape);
		lock(currentShape);
		shapePlacedSinceLastGameLoop = true;
	}

	if (event.key.code == sf::Keyboard::Down)
		if (!attemptMove(currentShape, 1, 0)) {
			lock(currentShape);
			shapePlacedSinceLastGameLoop = true;
		}
		
}

// called every game loop to handle ticks & tetromino placement (locking)
void TetrisGame::processGameLoop(float secondsSinceLastLoop) {
	secondsSinceLastTick += secondsSinceLastLoop;
	if (secondsSinceLastTick > secsPerTick) {
		tick();
		secondsSinceLastTick -= secsPerTick;
	}

}

// A tick() forces the currentShape to move (if there were no tick,
// the currentShape would float in position forever). This should
// call attemptMove() on the currentShape.  If not successful, lock() 
// the currentShape (it can move no further), and record the fact that a
// shape was placed (using shapePlacedSinceLastGameLoop)
void TetrisGame::tick() {
	std::cout << secondsSinceLastTick;
	if (!attemptMove(currentShape, 1, 0)) {
		lock(currentShape);
		shapePlacedSinceLastGameLoop = true;
	}
}


// reset everything for a new game (use existing functions) 
//  - setScore to 0
//  - determineSecondsPerTick(),
//  - clear the gameboard,
//  - pick & spawn next shape
//  - pick next shape again
void TetrisGame::reset() {
	score = 0;
	determineSecsPerTick();
	// how to clear gameboard?
	
	pickNextShape();
	spawnNextShape();
	pickNextShape();
}

// assign nextShape.setShape a new random shape  
void TetrisGame::pickNextShape() {
	nextShape.setShape(nextShape.Tetromino::getRandomShape());
}


// copy the nextShape into the currentShape and set 
//   its loc to be the gameboard's spawn loc.
//	 - return true/false based on isPositionLegal()
bool TetrisGame::spawnNextShape() {
	currentShape = nextShape;
	currentShape.setGridLoc(board.Gameboard::getSpawnLoc());
	return isPositionLegal(currentShape);
}



// test if a rotation is legal on the tetromino, 
//   if so, rotate it.
//  To do this:
//	 1) create a (local) temporary copy of the tetromino
//	 2) rotate it (shape.rotateCW())
//	 3) test if temp rotatio was legal (isPositionLegal()), 
//      if so - rotate the original tetromino.
//	 4) return true/false to indicate successful movement
bool TetrisGame::attemptRotate(GridTetromino& shape) {
	GridTetromino temp = shape;
	temp.rotateCW();
	if (isPositionLegal(temp)) {
		shape.rotateCW();
		return true;
	}
	else return false;
}


// test if a move is legal on the tetromino, if so, move it.
//  To do this:
//	 1) create a (local) temporary copy of the current shape
//	 2) move it (temp.move())
//	 3) test if temp move was legal (isPositionLegal(),
//      if so - move the original.
//	 4) return true/false to indicate successful movement
bool TetrisGame::attemptMove(GridTetromino& shape, int x, int y) {
	GridTetromino temp = shape;
	temp.move(x, y);
	if (isPositionLegal(temp)) {
		shape.move(x, y);
		return true;
	}
	else return false;
}


// drops the tetromino vertically as far as it can 
//   legally go.  Use attemptMove(). This can be done in 1 line.
void TetrisGame::drop(GridTetromino& shape) {
	attemptMove(shape, 0, Gameboard::MAX_Y);
}

// copy the contents of the tetromino's mapped block locs to the grid.
//	 1) get current blockshape locs via tetromino.getBlockLocsMappedToGrid()
//	 2) iterate on the mapped block locs and copy the contents (color) 
//      of each to the grid (via gameboard.setGridContent()) 
void TetrisGame::lock(const GridTetromino& shape) {
	std::vector<Point>mappedLocs = shape.GridTetromino::getBlockLocsMappedToGrid();
	int c = static_cast<int>(shape.getColor());
	for (Point element : mappedLocs) {
		int x = element.getX();
		int y = element.getY();
		board.setContent(x, y, c);
	}
}

// Graphics methods ==============================================

// draw a tetris block sprite on the canvas		
// x,y are meant to be block offsets (not pixels), which means you
// will need to multiply them by BLOCK_WIDTH & BLOCK_HEIGHT
//  for help: www.sfml-dev.org/tutorials/2.5/graphics-sprite.php
//	 1) set the block color using pBlockSprite->setTextureRect()
//   2) set the block loc using pBlockSprite->setPosition()   
//	 3) draw the block using pWindow.draw()
// (pointers to window and sprite were passed into the constructor)
void TetrisGame::drawBlock(int x, int y, TetColor color, Point origin) {
	pBlockSprite->setTextureRect(sf::IntRect(static_cast<int>(color)* BLOCK_WIDTH, 0, BLOCK_WIDTH, BLOCK_HEIGHT));
	pBlockSprite->setPosition((origin.getX() + (BLOCK_WIDTH * x)), (origin.getY() + BLOCK_HEIGHT * y));
	pWindow->draw(*pBlockSprite);
}

// draw the gameboard blocks on the window
//   iterate through each row & col, use drawBlock() to 
//   draw a block if it it isn't empty.
void TetrisGame::drawGameboard() {
	for (int x = 0; x < Gameboard::MAX_X; x++) {
		for (int y = 0; y < Gameboard::MAX_Y; y++) {
			if (board.getContent(x, y) != Gameboard::EMPTY_BLOCK) {
				drawBlock(x, y, static_cast<TetColor>(board.getContent(x, y)), gameboardOffset);
			}
		}
	}
}

// draw a tetromino on the window
//	 iterate through each mapped loc & drawBlock() for each.
//   the origin determines a 'base point' from which to calculate block offsets
//   If the Tetromino is on the gameboard: use gameboardOffset (otherwise you 
//   can specify another point as the origin - for the nextShape)
void TetrisGame::drawTetromino(GridTetromino tetromino, Point origin) {
	std::vector<Point>mappedLocs = currentShape.GridTetromino::getBlockLocsMappedToGrid();

	for (Point& element : mappedLocs) {
		int x = element.getX();
		int y = element.getY();
		TetColor color = tetromino.getColor();
		drawBlock(x, y, color, origin);
	}
}

// update the score display
// form a string "score: ##" to display the current score
// user scoreText.setString() to display it.
void TetrisGame::updateScoreDisplay() {

}

// State & gameplay/logic methods ================================

// return true if shape is within borders (isShapeWithinBorders())
//	 and does NOT intersect locked blocks (!doesShapeIntersectLockedBlocks())
bool TetrisGame::isPositionLegal(const GridTetromino& shape) {
	if ((isShapeWithinBorders(shape)) && (doesShapeIntersectLockedBlocks(shape)))
		return true;
	else return false;
}

// return true if the shape is within the left, right,
//	 and lower border of the grid. (false otherwise)
//   All of a shape's blocks must be on the gameboard to be within borders
bool TetrisGame::isShapeWithinBorders(const GridTetromino& shape) {
	std::vector<Point>shapePoints = shape.GridTetromino::getBlockLocsMappedToGrid();

	for (Point& element : shapePoints) {
		int x = element.getX();
		int y = element.getY();
		if (x >= Gameboard::MAX_X || x < 0 || y >= Gameboard::MAX_Y)
			return true;
	}
	return false;
}

// return true if the shape passed in intersects with content on the gameboard.
//   Use Gameboard's areLocsEmpty() for this, and pass it the shape's mapped locs.
bool TetrisGame::doesShapeIntersectLockedBlocks(const GridTetromino& shape) {
	std::vector<Point>shapePoints = shape.GridTetromino::getBlockLocsMappedToGrid();

	if (board.Gameboard::areLocsEmpty(shapePoints))
		return false;

	return true;
}

// set secsPerTick 
//   - basic: use MAX_SECS_PER_TICK
//   - advanced: base it on score (higher score results in lower secsPerTick)
void TetrisGame::determineSecsPerTick() {

}