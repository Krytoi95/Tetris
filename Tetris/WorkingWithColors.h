#pragma once

#include <string>

constexpr int MAX_LENGTH{ 20 };
constexpr int MAX_WIDTH{ 10 };
constexpr wchar_t EMPTY{ L' ' };

namespace WorkingWithColors {
	//constexpr std::wstring_view RED{ L"\033[31m" };
	//constexpr std::wstring_view GREEN{ L"\033[32m" };
	//constexpr std::wstring_view YELLOW{ L"\033[33m" };
	//constexpr std::wstring_view BLUE{ L"\033[34m" };
	//constexpr std::wstring_view PURPLE{ L"\033[35m" };
	//constexpr std::wstring_view CEAN{ L"\033[36m" };
	//constexpr std::wstring_view WHITE{ L"\033[37m" };

	static constexpr std::wstring_view colors[7]{
		L"\033[37m", L"\033[31m", L"\033[32m", L"\033[33m", L"\033[34m", L"\033[35m", L"\033[36m"
	};

	enum class Color : int {
		WHITE,
		RED,
		GREEN,
		YELLOW,
		BLUE,
		PURPLE,
		CEAN
	};
}