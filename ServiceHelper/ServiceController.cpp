#include "stdafx.h"
#include "ServiceController.h"

using namespace std::chrono;

ServiceController::ServiceController(string settingsFile)
{
	ServiceSettings* serviceSettings = new ServiceSettings(settingsFile);
	for (auto& currentServer : serviceSettings->GetServerList())
	{
		Server* svr = new Server(get<0>(currentServer), get<1>(currentServer));
		servers.push_back(*svr);
	}
	startWaitTime = serviceSettings->GetStartWaitTime();
	serverCount   = servers.size();
	serialStart   = serviceSettings->SerialStart();

	delete serviceSettings;
}

ServiceController::~ServiceController()
{
	for (std::vector<Server>::iterator currentSvr = servers.begin(); currentSvr != servers.end(); ++currentSvr)
	{
		currentSvr->~Server();
	}
}

unsigned int ServiceController::SerialStart()
{
	return (serialStart);
}

void ServiceController::StartServers()
{
	// Start the services.
	auto start = steady_clock::now();
	duration<double> elapsed;
	DWORD currentStatus = 0x00;
	bool* arStatus = new bool[serverCount + 1]();
	bool allStarted = false;

	cout << "Starting services" << endl;

	for (std::vector<Server>::iterator currentSvr = servers.begin(); currentSvr != servers.end(); ++currentSvr)
	{
		currentSvr->Start();
	}

	// Wait for all services to start.
	while (!allStarted)
	{
		elapsed = steady_clock::now() - start;
		TextHelper::SetCursorPosition(0, 6);
		OutputElapsedTime(elapsed);

		if (duration_cast<milliseconds>(elapsed).count() > startWaitTime)
		{
			TextHelper::Red();
			system("CLS");
			cout << endl << endl << "Service(s) did not start before the specified wait time" << endl;
			break;
		}

		count = 0;
		for (std::vector<Server>::iterator currentSvr = servers.begin(); currentSvr != servers.end(); ++currentSvr)
		{
			TextHelper::SetCursorPosition(0, count + 8);
			cout << currentSvr->getServerName() << " - " << currentSvr->getServiceName() << ": ";
			currentStatus = currentSvr->getServiceStatus();
			OutputStatus(currentStatus);
			arStatus[count] = (currentStatus == SERVICE_RUNNING);
			count++;
		}

		// See if all the services stopped.
		for (unsigned int i = 0; i < serverCount; i++)
		{
			if (false == arStatus[i])
			{
				allStarted = false;
				break;
			}
			else
			{
				allStarted = true;
			}
		}

		Sleep(1000);
	}
}

void ServiceController::StartServersSerial()
{
	// Start the services one at a time, waiting for the previous one to complete.
	auto start = steady_clock::now();
	duration<double> elapsed;
	DWORD currentStatus = 0x00;

	cout << "Starting services" << endl;
	count = 0;
	for (std::vector<Server>::iterator currentSvr = servers.begin(); currentSvr != servers.end(); ++currentSvr)
	{
		currentSvr->Start();
		start = steady_clock::now();
		currentStatus = 0x00;

		while (currentStatus != SERVICE_RUNNING)
		{
			elapsed = steady_clock::now() - start;
			TextHelper::SetCursorPosition(0, 6);
			OutputElapsedTime(elapsed);

			if (duration_cast<milliseconds>(elapsed).count() > startWaitTime)
			{
				TextHelper::Red();
				TextHelper::SetCursorPosition(0, count + 8);
				cout << endl << endl << "Service did not start before the specified wait time" << endl;
				break;
			}

			TextHelper::SetCursorPosition(0, count + 8);
			cout << currentSvr->getServerName() << " - " << currentSvr->getServiceName() << ": ";
			currentStatus = currentSvr->getServiceStatus();
			OutputStatus(currentStatus);

			Sleep(1000);
		}
		count++;
	}
}

void ServiceController::StopServers()
{
	auto start = steady_clock::now();
	duration<double> elapsed;
	DWORD currentStatus = 0x00;
	bool* arStatus = new bool[serverCount + 1];
	bool allStopped = false;

	cout << "Stopping services and changing start type to Disabled" << endl;

	// Issue the stop command (also sets the service to disabled).
	for (std::vector<Server>::iterator currentSvr = servers.begin(); currentSvr != servers.end(); ++currentSvr)
	{
		currentSvr->Stop();
	}

	// Wait for them to stop.
	while (!allStopped)
	{
		elapsed = steady_clock::now() - start;
		TextHelper::SetCursorPosition(0, 6);
		OutputElapsedTime(elapsed);

		count = 0;
		for (std::vector<Server>::iterator currentSvr = servers.begin(); currentSvr != servers.end(); ++currentSvr)
		{
			TextHelper::SetCursorPosition(0, count + 8);
			cout << currentSvr->getServerName() << " - " << currentSvr->getServiceName() << ": ";
			currentStatus = currentSvr->getServiceStatus();
			OutputStatus(currentStatus);
			arStatus[count] = (currentStatus == SERVICE_STOPPED);
			count++;
		}

		// See if all the services stopped.
		for (unsigned int i = 0; i < serverCount; i++)
		{
			if (false == arStatus[i])
			{
				allStopped = false;
				break;
			}
			else
			{
				allStopped = true;
			}
		}

		Sleep(1000);
	}
}

void ServiceController::OutputStatus(DWORD status)
{
	switch (status)
	{
	case SERVICE_STOPPED:
		std::cout << "Stopped         ";
		break;
	case SERVICE_START_PENDING:
		std::cout << "Start Pending   ";
		break;
	case SERVICE_STOP_PENDING:
		std::cout << "Stop Pending    ";
		break;
	case SERVICE_RUNNING:
		std::cout << "Running         ";
		break;
	case SERVICE_CONTINUE_PENDING:
		std::cout << "Continue Pending";
		break;
	case SERVICE_PAUSE_PENDING:
		std::cout << "Pause Pending   ";
		break;
	case SERVICE_PAUSED:
		std::cout << "Paused          ";
		break;
	default:
		std::cout << "Unknown         ";
		break;
	}
}

void ServiceController::OutputElapsedTime(duration<double> elapsed)
{
	int elapsedSeconds = (int)duration_cast<seconds>(elapsed).count();
	int elapsedMinutes = elapsedSeconds / 60;

	cout << "Elapsed time: " << int(elapsedMinutes % 60) << ":" << setfill('0') << setw(2) << int(elapsedSeconds % 60) << endl;
}

