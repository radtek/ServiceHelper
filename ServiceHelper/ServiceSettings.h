#pragma once
#include <iostream>
#include "stdafx.h"
#include <tuple>
#include <stdio.h>
#include <string>
#include <tchar.h>
#include <vector>
#include <Windows.h>

using namespace std;

class ServiceSettings
{
public:
	ServiceSettings(string settingsFile);
	~ServiceSettings();

	vector<tuple<string, string>> GetServerList();
	DWORD GetStartWaitTime();
	UINT SerialStart();

private:
	vector<tuple<string, string>> servers;
	DWORD startWaitTime = 0;
	UINT serialStart = 0;
	void LoadSettings(const string& settingsFile);
	
};


