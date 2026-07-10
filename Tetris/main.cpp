#include <iostream>
#include <string>

#include <fcntl.h> // Для _setmode
#include <io.h>    // Для _setmode

#include "GamingArea.h"
#include "Block.h"

#include <random>
#include <Windows.h>
#include <chrono>

constexpr auto FallDelay{ std::chrono::milliseconds(750) };

static int randomNumber(int minRange, int maxRange) {
	std::random_device rd;
	std::mt19937_64 gen(rd());
	std::uniform_int_distribution<int> dist(minRange, maxRange);

	return dist(gen);
}



static unsigned short proccesInput(const HANDLE& hStdin, short bufferSize, INPUT_RECORD* irInBuf) {
	DWORD count{};

	while (PeekConsoleInput(hStdin, irInBuf, bufferSize, &count)) {
		if (count == 0) { break; }

		ReadConsoleInput(hStdin, irInBuf, bufferSize, &count);
		
		for (DWORD i = 0; i < count; i++)
		{
			if (irInBuf[i].EventType == KEY_EVENT)
			{
				auto& key = irInBuf[i].Event.KeyEvent;

				if (key.bKeyDown) {
					return key.wVirtualKeyCode;
				}
			}
		}
	}
	return 0;
}

using Clock = std::chrono::steady_clock;

static VOID ErrorExit(LPCSTR lpszMessage) {
	std::wcout << lpszMessage << L'\n';
	ExitProcess(0);
}



int main() {
	_setmode(_fileno(stdout), _O_U16TEXT);
	system("cls");
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdout == INVALID_HANDLE_VALUE) return -1;
	DWORD dwMode{ 0 };
	if (!GetConsoleMode(hStdout, &dwMode)) return -1;
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

	if (!SetConsoleMode(hStdout, dwMode)) return -1;

	GamingArea gamingArea{};
	BlockRotationsCatalog blockRCatalog{};
	Block blockInTheGame{};
	Block blockOnHold{};

	blockOnHold.setPoint(Point(5, 0));
	blockOnHold.setColor(static_cast<WorkingWithColors::Color>(randomNumber(0, 6)));
	blockOnHold.setRotations(blockRCatalog.getRotations(static_cast<size_t>(randomNumber(0, 6))));
	blockOnHold.setRotationValue(static_cast<size_t>(randomNumber(0, blockOnHold.getSizeRotations() - 1)));

	bool gameOver{ false };
	bool blockInTheGameExist{ false };
	bool blockOnHoldExist{ false };

	HANDLE hStdin{};
	const short bufferSize{ 256 };
	INPUT_RECORD irInBuf[bufferSize];

	hStdin = GetStdHandle(STD_INPUT_HANDLE);
	if (hStdin == INVALID_HANDLE_VALUE) {
		ErrorExit("GetStdHandle");
	}
	unsigned short input{};
	
	auto previous{ Clock::now() };
	std::chrono::duration<double> fallTimer{ 0.0 };

	while (!gameOver) {
		auto current{ Clock::now() };
		auto deltaTime{current - previous};

		previous = current;
		fallTimer += deltaTime;

		COORD topLeftCorner = makeCoord(0, 0);
		SetConsoleCursorPosition(hStdout, topLeftCorner);
		input = proccesInput(hStdin, bufferSize, irInBuf);
		
		if (!blockInTheGameExist) {
			if (blockOnHoldExist) {
				size_t rotationValue{ static_cast<size_t>(blockOnHold.getSizeRotations()-1) };
				const Block::Rotation& rotation{ blockOnHold.getRotation(rotationValue) };
				size_t numberPoints{ rotation.getSize() };
				Point currentPointBlock{ blockOnHold.getCurrentPoint() };

				for (size_t i{ 0 }; i < numberPoints; i++) {
					Point localPointInTheBlock{ rotation.getPoint(i) };
					int posX{ currentPointBlock.getX() + localPointInTheBlock.getX() };
					int posY{ currentPointBlock.getY() + localPointInTheBlock.getY() };

					if (!gamingArea.canPlacePoint(posX, posY)) {
						system("cls");
						std::wcout << L"GAME OVER!" << L'\n';
						gameOver = true;
						break;
					}
				}
			}
			blockInTheGame = blockOnHold;
			blockInTheGameExist = true;
			blockOnHoldExist = false;

			blockOnHold.setPoint(Point(5, 0));
			blockOnHold.setColor(static_cast<WorkingWithColors::Color>(randomNumber(0, 6)));
			blockOnHold.setRotations(blockRCatalog.getRotations(static_cast<size_t>(randomNumber(0, 6))));
			blockOnHold.setRotationValue(static_cast<size_t>(randomNumber(0, blockOnHold.getSizeRotations()-1)));
			blockOnHoldExist = true;
		}
		
		
		if (input != 0) {
			gamingArea.movingBlock(blockInTheGame, input);
			
		}
		
		if (fallTimer >= FallDelay) {
			fallTimer -= FallDelay;

			if (!gamingArea.movingBlock(blockInTheGame, static_cast<unsigned short>(Button::Down))) {
				blockInTheGameExist = false;
			}
		}

		gamingArea.setBlock(blockInTheGame);
		gamingArea.draw(blockOnHold);
		if (blockInTheGameExist) {
			gamingArea.takeBlock(blockInTheGame);
		}
		gamingArea.clearFilledRows();
	};

	wchar_t in{};
	std::wcin >> in;
	return 0;
}

