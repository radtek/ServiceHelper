#include "Privileges.h"


Privileges::Privileges()
{
}


Privileges::~Privileges()
{
}


BOOL Privileges::IsElevated()
{

		BOOL fRet = FALSE;
		HANDLE hToken = NULL;
		if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
			TOKEN_ELEVATION Elevation = { 0 };
			DWORD cbSize = sizeof(TOKEN_ELEVATION);
			if (GetTokenInformation(hToken, TOKEN_INFORMATION_CLASS::TokenElevation, &Elevation, sizeof(Elevation), &cbSize)) {
				fRet = Elevation.TokenIsElevated;
			}
		}
		if (hToken) {
			CloseHandle(hToken);
		}
		return fRet;
}
