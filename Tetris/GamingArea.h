#pragma once

#ifndef TETRIS_GAMINGAREA_H
#define TETRIS_GAMINGAREA_H

#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>


#include "Block.h"
#include "WorkingWithColors.h"

class Cell {
public:
	Cell() {
		this->wc = EMPTY;
		this->color = WorkingWithColors::Color::WHITE;
		this->empty = true;
	}
	wchar_t getWChar() const { return this->wc; }
	WorkingWithColors::Color getColor() const { return this->color; }
	bool isEmpty() const { return this->empty; }
	void setWChar(const wchar_t& wc) {
		if (wc == EMPTY) {
			this->empty = true;
		}
		else {
			this->empty = false;
		}
		this->wc = wc;
	}
	void setColor(const WorkingWithColors::Color& color) { this->color = color; }
private:
	wchar_t wc{};
	WorkingWithColors::Color color{};
	bool empty{};
};

enum class Button : unsigned short
{
	Left = 'A',
	Right = 'D',
	Down = 'S',
	Rotate = 'F'
};

static COORD makeCoord(int x, int y) {
	COORD coord = { (SHORT)x, (SHORT)y };
	return coord;
}

class GamingArea {
public:
	GamingArea() {
		gamingArea = std::vector<std::vector<Cell>>(MAX_LENGTH, std::vector<Cell>(MAX_WIDTH));
		areaForBlocksOnHold = std::vector<std::vector<Cell>>(4, std::vector<Cell>(4));
		setWCharsForArea(EMPTY);
	}
	~GamingArea() = default;
	
	void draw(const Block& blockOnHold) {
		std::wcout << std::wstring(7, L'\t') << std::wstring(MAX_WIDTH + 2, L'@') << L'\n';
		for (size_t i{ 0 }; i < MAX_LENGTH; i++) {
			std::wcout << std::wstring(7, L'\t') << L'@';
			for (size_t j{ 0 }; j < MAX_WIDTH; j++) {
				std::wcout << WorkingWithColors::colors[static_cast<size_t>(gamingArea[i][j].getColor())]
					<< gamingArea[i][j].getWChar() 
					<< WorkingWithColors::colors[static_cast<size_t>(WorkingWithColors::Color::WHITE)];
			}
			std::wcout << L'@' << L'\n';
		}
		std::wcout << std::wstring(7, L'\t') << std::wstring(MAX_WIDTH + 2, L'@') << L'\n';
		setBlockAreaForOnHold(blockOnHold);
		drawAreaForBlockOnHold();
		takeBlockAreaForOnHold(blockOnHold);
	} // !!
	void drawAreaForBlockOnHold() {
		std::wcout << std::wstring(7, L'\t') << std::wstring(6, L'@') << L'\n';
		for (size_t i{ 0 }; i < 4; i++) {
			std::wcout << std::wstring(7, L'\t') << L'@';
			for (size_t j{ 0 }; j < 4; j++) {
				std::wcout << WorkingWithColors::colors[static_cast<size_t>(areaForBlocksOnHold[i][j].getColor())]
					<< areaForBlocksOnHold[i][j].getWChar()
					<< WorkingWithColors::colors[static_cast<size_t>(WorkingWithColors::Color::WHITE)];
			}
			std::wcout << L'@' << L'\n';
		}
		std::wcout << std::wstring(7, L'\t') << std::wstring(6, L'@') << L" Count: " << this->countFilledRows << L'\n';
	}
	void setWCharsForArea(const wchar_t& wc) {
		for (size_t i{ 0 }; i < MAX_LENGTH; i++) {
			for (size_t j{ 0 }; j < MAX_WIDTH; j++) {
				gamingArea[i][j].setWChar(wc);
			}
		}
	}
	void setBlock(const Block& block) {
		size_t numberPoints{ block.getRotation(block.getRotationValue()).getSize() };

		Point currentPointBlock{ block.getCurrentPoint() };
		for (size_t i{ 0 }; i < numberPoints; i++) {
			Point otherPointInTheBlock{ block.getRotation(block.getRotationValue()).getPoint(i) };
			int posX{ currentPointBlock.getX() + otherPointInTheBlock.getX() };
			int posY{ currentPointBlock.getY() + otherPointInTheBlock.getY() };

			this->gamingArea[posY][posX].setWChar(block.getWChar());
			this->gamingArea[posY][posX].setColor(block.getColor());
		}
	}
	void setBlockAreaForOnHold(const Block& block) {
		size_t numberPoints{ block.getRotation(block.getRotationValue()).getSize() };

		Point currentPointBlock{ 1, 0 };
		for (size_t i{ 0 }; i < numberPoints; i++) {
			Point otherPointInTheBlock{ block.getRotation(block.getRotationValue()).getPoint(i) };
			int posX{ currentPointBlock.getX() + otherPointInTheBlock.getX() };
			int posY{ currentPointBlock.getY() + otherPointInTheBlock.getY() };

			this->areaForBlocksOnHold[posY][posX].setWChar(block.getWChar());
			this->areaForBlocksOnHold[posY][posX].setColor(block.getColor());
		}
	}
	void takeBlockAreaForOnHold(const Block& block) {
		size_t numberPoints{ block.getRotation(block.getRotationValue()).getSize() };

		Point currentPointBlock{ 1, 0 };
		for (size_t i{ 0 }; i < numberPoints; i++) {
			Point otherPointInTheBlock{ block.getRotation(block.getRotationValue()).getPoint(i) };
			int posX{ currentPointBlock.getX() + otherPointInTheBlock.getX() };
			int posY{ currentPointBlock.getY() + otherPointInTheBlock.getY() };

			this->areaForBlocksOnHold[posY][posX].setWChar(EMPTY);
			this->areaForBlocksOnHold[posY][posX].setColor(WorkingWithColors::Color::WHITE);
		}
	}
	void takeBlock(const Block& block) {
		size_t numberPoints{ block.getRotation(block.getRotationValue()).getSize() };

		Point currentPointBlock{ block.getCurrentPoint() };
		for (size_t i{ 0 }; i < numberPoints; i++) {
			Point otherPointInTheBlock{ block.getRotation(block.getRotationValue()).getPoint(i) };
			int posX{ currentPointBlock.getX() + otherPointInTheBlock.getX() };
			int posY{ currentPointBlock.getY() + otherPointInTheBlock.getY() };

			this->gamingArea[posY][posX].setWChar(EMPTY);
			this->gamingArea[posY][posX].setColor(WorkingWithColors::Color::WHITE);
		}
	}
	bool movingBlock(Block& block, unsigned short button ) {
		if (button == static_cast<unsigned short>(Button::Left)) {
			return moveBlock(block, -1, 0, 0);
		}	
		if (button == static_cast<unsigned short>(Button::Right)) {
			return moveBlock(block, 1, 0, 0);
		}
		if (button == static_cast<unsigned short>(Button::Down)) {
			return moveBlock(block, 0, 1, 0);
		}
		if (button == static_cast<unsigned short>(Button::Rotate)) {
			return moveBlock(block, 0, 0, 1);
		}
		return true;
	}
	bool canPlacePoint(const int newX, const int newY) {
		if (newX < 0) {
			return false;
		}
		if (newX >= MAX_WIDTH) {
			return false;
		}
		if (newY >= MAX_LENGTH) {
			return false;
		}
		if (!this->gamingArea[newY][newX].isEmpty()) {
			return false;
		}
		return true;
	}
	void clearArea() {
		setWCharsForArea(EMPTY);
	}
	void clearRow(const int& lY) {
		for (size_t i{ 0 }; i < MAX_WIDTH; i++) {
			gamingArea[lY][i].setWChar(EMPTY);
		}
	}
	bool isRowFilled(const int& lY) {
		for (size_t i{ 0 }; i < MAX_WIDTH; i++) {
			if (gamingArea[lY][i].isEmpty()) {
				return false;
			}
		}
		return true;
	}
	void clearFilledRows() {
		int write{ MAX_LENGTH - 1 };
		for (int read{ MAX_LENGTH - 1 }; read >= 0; --read) {
			if (!isRowFilled(read)) {
				gamingArea[write] = gamingArea[read];
				if (write != read) {
					clearRow(read);
				}
				--write;
			}
			else {
				clearRow(read);
				++countFilledRows;
			}
		}
	}
	
private:
	std::vector<std::vector<Cell>> gamingArea{};
	std::vector<std::vector<Cell>> areaForBlocksOnHold{};
	int countFilledRows{};
	bool moveBlock(Block& block, const int& dx, const int& dy, const size_t& rotationV) {
		size_t rotationValue{ (block.getRotationValue() + rotationV) % block.getSizeRotations() };
		const Block::Rotation& rotation{ block.getRotation(rotationValue) };
		size_t numberPoints{ rotation.getSize() };
		Point currentPointBlock{ block.getCurrentPoint() };

		for (size_t i{ 0 }; i < numberPoints; i++) {
			Point localPointInTheBlock{ rotation.getPoint(i) };
			int posX{ currentPointBlock.getX() + dx + localPointInTheBlock.getX() };
			int posY{ currentPointBlock.getY() + dy + localPointInTheBlock.getY() };

			if (!canPlacePoint(posX, posY)) {
				return false;
			}
		}

		currentPointBlock.setX(currentPointBlock.getX() + dx);
		currentPointBlock.setY(currentPointBlock.getY() + dy);
		block.setRotationValue(rotationValue);
		block.setPoint(currentPointBlock);
		return true;
	}
};

#endif // TETRIS_GAMINGAREA_H