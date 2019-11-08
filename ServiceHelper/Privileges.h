#pragma once
#include "stdafx.h"
#include <Windows.h>

/*
Class used to check for elevated privileges on the current process.
*/
class Privileges
{
public:
	Privileges();
	~Privileges();

	static BOOL IsElevated();
};

