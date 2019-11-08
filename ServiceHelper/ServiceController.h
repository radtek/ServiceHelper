#pragma once
#include "stdafx.h"
#include <string>
#include <chrono>
#include <iostream>
#include <iomanip>

#include "TextHelper.h"
#include "Privileges.h"
#include "ServiceSettings.h"
#include "Server.h"

class ServiceController
{

public:
	ServiceController(string settingsFile);
	~ServiceController();

	void StopServers();
	void StartServers();
	void StartServersSerial();

	unsigned int SerialStart();


private:
	//Fields
	vector<Server> servers;
	DWORD startWaitTime = 0;
	unsigned int serverCount = 0;
	unsigned int count = 0;
	unsigned int serialStart = 0;

	//Methods
	void OutputStatus(DWORD status);
	void OutputElapsedTime(std::chrono::duration<double> elapsed);

};

