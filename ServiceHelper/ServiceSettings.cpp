#include "stdafx.h"
#include "ServiceSettings.h"
#include "comutil.h"
#include <fstream>
#import <msxml6.dll> rename_namespace(_T("MSXML"))

/* ***********************************************
	Constructor.
	Initializes the class and checks the run mode.
 *********************************************** */
ServiceSettings::ServiceSettings(string settingsFile)
{
	
	LoadSettings(settingsFile);
	
}

/* ***********************************************
	Destructor.
	Cleans up the member variables.
*********************************************** */
ServiceSettings::~ServiceSettings()
{
	servers.clear();
}

/* ***********************************************
	Get accessor for the list of servers.
*********************************************** */
vector<tuple<string, string>> ServiceSettings::GetServerList()
{
	return (servers);
}

/* ***********************************************
	Get accessor for the service start wait time.
*********************************************** */
DWORD ServiceSettings::GetStartWaitTime()
{
	return (startWaitTime);
}

/** **********************************************
	Get accessor for the serial start flag.
*********************************************** */
UINT ServiceSettings::SerialStart()
{
	return (serialStart);
}

/* ***********************************************
	Loads the settings from the Settings.xml file
	and populates the appropriate member variables.
*********************************************** */
void ServiceSettings::LoadSettings(const string& settingsFile)
{
	BSTR nameTag = ::SysAllocString(L"Name");
	BSTR serviceTag = ::SysAllocString(L"Service");

	// Make sure the file exists.
	if (false == ((ifstream)(settingsFile.c_str())).good() )
	{
		cout << "Can not find setting file: " <<settingsFile << endl;
		return;
	}

	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (SUCCEEDED(hr)) {
		try {
			MSXML::IXMLDOMDocument2Ptr xmlDoc;
			hr = xmlDoc.CreateInstance(__uuidof(MSXML::DOMDocument60),
				NULL, CLSCTX_INPROC_SERVER);


			if (xmlDoc->load(settingsFile.c_str()) != VARIANT_TRUE) {
				cout << "Unable to load Settings.xml" << endl;
			}
			else {
				cout << "Settings file was successfully loaded" << endl;

				xmlDoc->setProperty("SelectionLanguage", "XPath");
				// Get the list of servers.
				MSXML::IXMLDOMNodeListPtr serverList = xmlDoc->selectNodes("/Settings/Servers/*");
				long listLength = serverList->Getlength();
				cout << "Server list has " << listLength << ((listLength > 1) ? " items" : " item") << endl;
				MSXML::IXMLDOMNodePtr child = NULL;

				while ((child = serverList->nextNode()) != NULL)
				{
					//servers.push_back((LPCWSTR)child->Gettext().Detach());
					IXMLDOMNamedNodeMapPtr attributes = child->Getattributes();
					
					// Get the Name and Service attribute nodes.
					IXMLDOMNode *attName;
					IXMLDOMNode *attService;
					attributes->getNamedItem(nameTag, &attName);
					attributes->getNamedItem(serviceTag, &attService);

					VARIANT nameVal;
					VARIANT serviceVal;
					::VariantInit(&nameVal);
					::VariantInit(&serviceVal);

					// Get the value of the nodes.
					attName->get_nodeValue(&nameVal);
					attService->get_nodeValue(&serviceVal);

					// Convert the values to std:string.
					std::string serverName = (const char*)_bstr_t(V_BSTR(&nameVal));
					std::string serviceName = (const char*)_bstr_t(V_BSTR(&serviceVal));
					
					// Add the pair to the map.
					servers.push_back(make_tuple(serverName, serviceName));
				}
				serverList = NULL;

				// Get the service start wait time.
				MSXML::IXMLDOMNodePtr startWaitNode = xmlDoc->selectSingleNode("/Settings/StartWait/Milliseconds");
				std::wstring wt = startWaitNode->Gettext().Detach();
				startWaitTime = _wtoi(wt.c_str());

				// Get the serial start flag.
				MSXML::IXMLDOMNodePtr serialStartNode = xmlDoc->selectSingleNode("/Settings/SerialStart");
				std::wstring ss = serialStartNode->Gettext().Detach();
				serialStart = _wtoi(ss.c_str());

			}
		}
		catch (_com_error &e)
		{
			cout << "ERROR: " << e.ErrorMessage() << endl;
		}

		CoUninitialize();
		::SysFreeString(nameTag);
		::SysFreeString(serviceTag);
	}
}
