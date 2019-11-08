#pragma once

/**
*	Enumeration used to indicate the status of the service operations.
*/
enum SHError
{
	PASS                          = 0x00,
	STATUS_CHECK_FAILED           = 0x01,
	CHANGE_SERVICE_CONFIG_FAILED  = 0x02,
	CHANGE_OPTIONAL_CONFIG_FAILED = 0x03,
	SERVICE_START_FAILED          = 0x04,
	SERVICE_STOP_FAILED           = 0x05,
	MGR_HANDLE_NULL               = 0x06,
	SVC_HANDLE_NULL               = 0x07,
	OPEN_SC_MANAGER_FAILED        = 0x08,
	OPEN_SERVICE_FAILED           = 0x09,
	QUERY_STATUS_FAILED           = 0x10
};
