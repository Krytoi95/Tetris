#pragma once

#include "Point.h"

Point** createArray(const int& rows, const int& cols) {
	Point** arr = new Point * [rows];
	for (size_t i{ 0 }; i < rows; i++) {
		arr[i] = new Point[cols];
	}
	return arr;
}
void fillArray(Point**& arr, const int& rows, const int& cols) {
	for()
}