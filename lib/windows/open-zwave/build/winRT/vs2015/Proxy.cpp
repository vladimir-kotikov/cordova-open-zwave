#include "pch.h"
#include "Proxy.h"

#include "Manager.h"
#include "Node.h"

using namespace OZWProxy;
using namespace Platform;

static CRITICAL_SECTION g_criticalSection;

Proxy::Proxy()
{
	//Options::Create("../../../../../../config/", "", "");
	//Options::Get()->AddOptionInt("SaveLogLevel", LogLevel_Detail);
	//Options::Get()->AddOptionInt("QueueLogLevel", LogLevel_Debug);
	//Options::Get()->AddOptionInt("DumpTrigger", LogLevel_Error);
	//Options::Get()->AddOptionInt("PollInterval", 500);
	//Options::Get()->AddOptionBool("IntervalBetweenPolls", true);
	//Options::Get()->AddOptionBool("ValidateValueChanges", true);
	//Options::Get()->Lock();

	InitializeCriticalSection(&g_criticalSection);

	this->manager = Manager::Create();
	//proxyInstance = this;
}

void Proxy::start(String ^ portName, SuccessHandler ^ successCallback, ErrorHandler ^ errorCallback)
{
	std::wstring wsstr(portName->Data());
	std::string port(wsstr.begin(), wsstr.end());

	this->OnSuccess = successCallback;
	this->OnError = errorCallback;
	
	this->manager->AddWatcher(this->OnNotification, NULL);
	this->manager->AddDriver(port);
}

void Proxy::OnNotification(Notification const * _notification, void * proxy)
{
	// Must do this inside a critical section to avoid conflicts with the main thread
	EnterCriticalSection(&g_criticalSection);

	switch (_notification->GetType())
	{
		case Notification::Type_NodeAdded:
		{
			uint32 const homeId = _notification->GetHomeId();
			uint8 const nodeId = _notification->GetNodeId();
			this->OnSuccess("nodeAdded", nullptr, nullptr);
			break;
		}

		case Notification::Type_NodeRemoved:
		{
			uint32 const homeId = _notification->GetHomeId();
			uint8 const nodeId = _notification->GetNodeId();
			this->OnSuccess("nodeRempved", nullptr, nullptr);
			break;
		}

		case Notification::Type_DriverReady:
		{
			this->OnSuccess("driverReady", nullptr, nullptr);
			break;
		}

		case Notification::Type_DriverFailed:
		{
			this->OnError("driverReady");
			break;
		}

		case Notification::Type_ValueAdded:
		case Notification::Type_ValueRemoved:
		case Notification::Type_ValueChanged:
		case Notification::Type_Group:
		case Notification::Type_NodeEvent:
		case Notification::Type_PollingDisabled:
		case Notification::Type_PollingEnabled:

		case Notification::Type_AwakeNodesQueried:
		case Notification::Type_AllNodesQueried:
		case Notification::Type_AllNodesQueriedSomeDead:
		{
			break;
		}

		case Notification::Type_DriverReset:
		case Notification::Type_NodeNaming:
		case Notification::Type_NodeProtocolInfo:
		case Notification::Type_NodeQueriesComplete:
		default:
		{
		}
	}

	LeaveCriticalSection(&g_criticalSection);
}