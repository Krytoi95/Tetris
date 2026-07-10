#pragma once

#ifndef TETRIS_BLOCK_H
#define TETRIS_BLOCK_H

#include <vector>

#include "WorkingWithColors.h"

class Point {
public:
	Point() = default;
	Point(const int& x, const int& y) {
		this->x = x;
		this->y = y;
	}
	~Point() = default;

	Point& operator=(const Point& otherPoint) {
		if (this == &otherPoint) return *this;

		this->x = otherPoint.x;
		this->y = otherPoint.y;
		
		return *this;
	}

	void setX(const int& x) { this->x = x; }
	void setY(const int& y) { this->y = y; }
	int getX() const { return this->x; }
	int getY() const { return this->y; }
	void setPoint(const int& x, const int& y) {
		this->x = x;
		this->y = y;
	}
private:
	int x{};
	int y{};
};

enum class TypeBlock : int {
	blockL,
	blockJ,
	blockZ,
	blockS,
	blockI,
	blockO,
	blockT
};

class Block {
private:

public: 
	class Rotation {
	public:
		Rotation() = default;
		Rotation(const std::vector<Point>& rotation) {
			this->rotation = rotation;
		}
		~Rotation() = default;

		Point getPoint(const size_t& place) const  {
			return this->rotation[place];
		}
		size_t getSize() const { return this->rotation.size(); }
	private:
		std::vector<Point> rotation{}; //4 points
	};

	Block() {
		this->wc = L'\u2588';
		this->currentPoint.setPoint(5, 0);
		this->color = WorkingWithColors::Color::WHITE;
		this->rotations = std::vector<Block::Rotation>();
		this->rotation = 0;
	}
	Block(const std::vector<Block::Rotation>& rotations, const size_t& rotation) {
		this->wc = L'\u2588';
		this->currentPoint.setPoint(5, 0);
		this->color = WorkingWithColors::Color::WHITE;
		this->rotations = rotations;
		this->rotation = rotation;
	}
	~Block() = default;
	
	Block& operator=(const Block& otherBlock) {
		if (this == &otherBlock) return *this;

		this->color = otherBlock.color;
		this->currentPoint = otherBlock.currentPoint;
		this->rotations = otherBlock.rotations;
		this->rotation = otherBlock.rotation;

		return *this;
	}

	wchar_t getWChar() const { return this->wc; }
	WorkingWithColors::Color getColor() const { return this->color; }
	void setColor(const WorkingWithColors::Color& color) { this->color = color; }
	Point getCurrentPoint() const  { return this->currentPoint; }
	void setPoint(const Point& point) { this->currentPoint = point; }
	size_t getRotationValue() const  { return this->rotation; }
	void setRotationValue(const size_t& newRotation) { this->rotation = newRotation; }
	int getSizeRotations() const { return static_cast<int>(this->rotations.size()); }
	Block::Rotation getRotation(const size_t& placeRotation) const { return this->rotations[placeRotation]; }
	void setRotations(const std::vector<Block::Rotation>& rotations) { this->rotations = rotations; }

private:
	wchar_t wc{};
	WorkingWithColors::Color color{};
	Point currentPoint{};
	size_t rotation{}; // 0 - 3
	std::vector<Block::Rotation> rotations{}; // 2-4 rotations
};

class BlockRotationsCatalog {
public:
	BlockRotationsCatalog() {
		this->templatesRotations = std::vector<std::vector<Block::Rotation>>{
			std::vector<Block::Rotation>
		{
				std::vector<Point>{ Point(0, 0), Point(0, 1), Point(0, 2), Point(1, 2) },
				std::vector<Point>{ Point(-1, 1), Point(0, 1), Point(1, 1), Point(1, 0) },
				std::vector<Point>{ Point(0, 2), Point(0, 1), Point(0, 0), Point(-1, 0) },
				std::vector<Point>{ Point(1, 1), Point(0, 1), Point(-1, 1), Point(-1, 2) }
		}, // block L
			std::vector<Block::Rotation>
		{
				std::vector<Point>{ Point(0, 0), Point(0, 1), Point(0, 2), Point(-1, 2) },
				std::vector<Point>{ Point(-1, 1), Point(0, 1), Point(1, 1), Point(1, 2) },
				std::vector<Point>{ Point(0, 2), Point(0, 1), Point(0, 0), Point(1, 0) },
				std::vector<Point>{ Point(1, 1), Point(0, 1), Point(-1, 1), Point(-1, 0) }
		}, // block J
			std::vector<Block::Rotation>{
				std::vector<Point>{ Point(0, 0), Point(0, 1), Point(1, 1), Point(-1, 0) },
				std::vector<Point>{ Point(-1, 1), Point(0, 1), Point(0, 0), Point(-1, 2) }
		}, // block Z
			std::vector<Block::Rotation>{
				std::vector<Point>{ Point(0, 0), Point(0, 1), Point(-1, 1), Point(1, 0) },
				std::vector<Point>{ Point(-1, 1), Point(0, 1), Point(0, 2), Point(-1, 0) }
		}, // block S
			std::vector<Block::Rotation>{
				std::vector<Point>{ Point(0, 0), Point(0, 1), Point(0, 2), Point(0, 3) },
				std::vector<Point>{ Point(-1, 1), Point(0, 1), Point(1, 1), Point(2, 1) }
		}, // block I
			std::vector<Block::Rotation>{
				std::vector<Point>{ Point(0, 0), Point(0, 1), Point(1, 0), Point(1, 1) }
		}, // block O
			std::vector<Block::Rotation>{
				std::vector<Point>{ Point(0, 0), Point(0, 1), Point(1, 1), Point(-1, 1) },
				std::vector<Point>{ Point(-1, 1), Point(0, 1), Point(0, 0), Point(0, 2) },
				std::vector<Point>{ Point(0, 2), Point(0, 1), Point(-1, 1), Point(1, 1) },
				std::vector<Point>{ Point(1, 1), Point(0, 1), Point(0, 2), Point(0, 0) }
		} // block T
		};
	}
	~BlockRotationsCatalog() = default;
	std::vector<Block::Rotation> getRotations(const size_t& placeRotations) {
		return this->templatesRotations[placeRotations];
	}
private:
	std::vector<std::vector<Block::Rotation>> templatesRotations{};
};

#endif //TETRIS_BLOCK_H