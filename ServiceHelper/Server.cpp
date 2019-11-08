#include "stdafx.h"
#include "Server.h"
#include "SHError.h"


Server::Server(string serverName, string serviceName)
{
	_serverName = serverName;
	_serviceName = serviceName;
}

Server::~Server()
{
	CloseService();
}

void Server::CloseService()
{
	try
	{
		CloseServiceHandle(_hSvc);
		CloseServiceHandle(_hMgr);
		_hSvc = NULL;
		_hMgr = NULL;
	}
	catch (const std::exception)
	{
	}
}

int Server::Open()
{
	return (GetHandle());
}

string Server::getServerName()
{
	return (_serverName);
}

string Server::getServiceName()
{
	return (_serviceName);
}


DWORD Server::getServiceStatus()
{
	if (CheckStatus() == SHError::PASS)
	{
		return (_status.dwCurrentState);
	}
	else
	{
		return (SHError::STATUS_CHECK_FAILED);
	}
}

int Server::Start()
{
	Open();

	// Starting
	SERVICE_DELAYED_AUTO_START_INFO startInfo;
	startInfo.fDelayedAutostart = true;
	if (ChangeServiceConfig(_hSvc, SERVICE_NO_CHANGE, SERVICE_AUTO_START, SERVICE_NO_CHANGE, NULL, NULL, NULL, NULL, NULL, NULL, NULL) == 0)
	{
		CloseService();
		return(SHError::CHANGE_SERVICE_CONFIG_FAILED);
	}
	else
	{
		if (ChangeServiceConfig2(_hSvc, SERVICE_CONFIG_DELAYED_AUTO_START_INFO, &startInfo) == 0)
		{
			CloseService();
			return(SHError::CHANGE_OPTIONAL_CONFIG_FAILED);
		}
	}

	if (StartService(_hSvc, 0, NULL) != 0)
	{
		CloseService();
		return (SHError::PASS); // Pass
	}
	else
	{
		CloseService();
		return(SHError::SERVICE_START_FAILED);
	}
}

int Server::Stop()
{
	SERVICE_STATUS status = { 0 };

	Open();

	// Stop the service and change the start type to disabled.
	if (ControlService(_hSvc, SERVICE_CONTROL_STOP, &status) == 0)
	{
		return (SHError::SERVICE_STOP_FAILED);
	}
	else
	{
		if (ChangeServiceConfig(_hSvc, SERVICE_NO_CHANGE, SERVICE_DISABLED, SERVICE_NO_CHANGE, NULL, NULL, NULL, NULL, NULL, NULL, NULL) == 0)
		{
			CloseService();
			return(SHError::CHANGE_SERVICE_CONFIG_FAILED);
		}
	}
	CloseService();
	return (SHError::PASS);
}

int Server::GetHandle()
{
	// If the service manager already has a handle, don't try and open another.
	if (_hMgr != NULL)
	{
		return (SHError::MGR_HANDLE_NULL);
	}

	// Open the service manager and service.
	wstring svrName = std::wstring(_serverName.begin(), _serverName.end());
	LPCWSTR server = svrName.c_str();

	wstring svcName = std::wstring(_serviceName.begin(), _serviceName.end());
	LPCWSTR service = svcName.c_str();

	// A blank string will connect to localhost.
	if (svrName.compare(L"localhost") == 0) { server = '\0'; }

	_hMgr = OpenSCManager(server, SERVICES_ACTIVE_DATABASE, SC_MANAGER_ALL_ACCESS);
	if (_hMgr == NULL)
	{
		return (SHError::OPEN_SC_MANAGER_FAILED);
	}
	else
	{
		// Now open the service.
		_hSvc = OpenService(_hMgr, service, SERVICE_START | SERVICE_STOP | SERVICE_CHANGE_CONFIG | SERVICE_QUERY_STATUS);
		if (_hSvc == NULL)
		{
			return (SHError::OPEN_SERVICE_FAILED);
		}
		else
		{
			return (SHError::PASS);
		}
	}
}

int Server::CheckStatus()
{
	Open();
	if (_hSvc != NULL)
	{
		DWORD dwBytesNeeded = 0;

		if (QueryServiceStatusEx(
			_hSvc,                           // handle to service 
			SC_STATUS_PROCESS_INFO,          // info level
			(LPBYTE)& _status,               // address of structure
			sizeof(SERVICE_STATUS_PROCESS),  // size of structure
			&dwBytesNeeded) != 0)            // if buffer too small
		{
			return (SHError::PASS);
		}
		else
		{
			CloseService();
			return (SHError::QUERY_STATUS_FAILED);
		}
	}
	else
	{
		CloseService();
		return (SHError::SVC_HANDLE_NULL);
	}
}


