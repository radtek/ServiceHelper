#include <Windows.h>
#include <string>

using namespace std;

class Server
{
public:
	Server(string serverName, string serviceName);
	~Server();

	string getServerName();
	string getServiceName();
	DWORD  getServiceStatus();

	int Stop();
	int Start();

private:
	string _serverName;
	string _serviceName;

	SC_HANDLE _hSvc = NULL;
	SC_HANDLE _hMgr = NULL;
	SERVICE_STATUS_PROCESS _status = { 0 };

	void CloseService();
	int Open();
	int GetHandle();
	int CheckStatus();
	

};
