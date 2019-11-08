#pragma once
#include <Windows.h>

/*
Class used to change the text color in the console.
*/
class TextHelper
{
public:
	TextHelper();
	~TextHelper();

	static void Red();
	static void White();
	static void Blue();
	static void Gray();
	static void YellowHighlight();

	static void SetCursorPosition(int x, int y);
};