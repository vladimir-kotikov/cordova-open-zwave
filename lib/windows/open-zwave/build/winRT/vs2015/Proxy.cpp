#include "pch.h"
#include "Proxy.h"

#include "Manager.h"
#include "Node.h"
#include "..\..\..\..\src\Options.h"
#include "..\..\..\..\src\platform\Log.h"

using namespace OZWProxy;
using namespace Platform;

static CRITICAL_SECTION g_criticalSection;
static Proxy^ _proxyInstance;

String^ toStr(string const input)
{
	wstring intermediate(input.begin(), input.end());
	return ref new String(intermediate.c_str());
}

void OnNotification(Notification const * _notification, void * proxy) {

	// Must do this inside a critical section to avoid conflicts with the main thread
	EnterCriticalSection(&g_criticalSection);

	// test success call
	// uint32 const homeId = 1234;
	// uint8 const nodeId = 56;
	// _proxyInstance->ReportSuccess("nodeAdded", nodeId.ToString(), homeId.ToString());
	
	switch (_notification->GetType())
	{
		case Notification::Type_NodeAdded:
		{
			uint32 const homeId = _notification->GetHomeId();
			uint8 const nodeId = _notification->GetNodeId();
			
			_proxyInstance->ReportSuccess("nodeAdded", nodeId.ToString(), homeId.ToString());
			break;
		}

		case Notification::Type_NodeRemoved:
		{
			uint32 const homeId = _notification->GetHomeId();
			uint8 const nodeId = _notification->GetNodeId();

			_proxyInstance->ReportSuccess("nodeRemoved", nodeId.ToString(), homeId.ToString());
			break;
		}

		case Notification::Type_DriverReady:
		{
			_proxyInstance->ReportSuccess("driverReady", nullptr, nullptr);
			break;
		}

		case Notification::Type_DriverFailed:
		{
			_proxyInstance->ReportError("Driver failed to start");
			break;
		}

		case Notification::Type_NodeQueriesComplete:
		{
			uint32 const homeId = _notification->GetHomeId();
			uint8 const nodeId = _notification->GetNodeId();

			Manager *manager = Manager::Get();

			NodeInfo nodeInfo({
				toStr(manager->GetNodeManufacturerName(homeId, nodeId)),   /* manufacturer */
				toStr(manager->GetNodeManufacturerId(homeId, nodeId)),     /* manufacturerid */
				toStr(manager->GetNodeProductName(homeId, nodeId)),        /* product */
				toStr(manager->GetNodeProductType(homeId, nodeId)),        /* producttype */
				toStr(manager->GetNodeProductId(homeId, nodeId)),          /* productid */
				toStr(manager->GetNodeType(homeId, nodeId)),               /* type */
				toStr(manager->GetNodeName(homeId, nodeId)),               /* name */
				toStr(manager->GetNodeLocation(homeId, nodeId))            /* loc */
			});

			_proxyInstance->ReportSuccess("nodeInfo", nodeId.ToString(), homeId.ToString(), nodeInfo);

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
		default:
		{
		}
	}

	LeaveCriticalSection(&g_criticalSection);
}

Proxy::Proxy()
{
	Options::Create("./", "", "");
	Options::Get()->AddOptionInt("SaveLogLevel", LogLevel_Detail);
	Options::Get()->AddOptionInt("QueueLogLevel", LogLevel_Debug);
	Options::Get()->AddOptionInt("DumpTrigger", LogLevel_Error);
	Options::Get()->AddOptionInt("DriverMaxAttempts", 3);
	Options::Get()->AddOptionInt("PollInterval", 500);
	Options::Get()->AddOptionBool("IntervalBetweenPolls", true);
	Options::Get()->AddOptionBool("ValidateValueChanges", true);
	Options::Get()->Lock();
	
	InitializeCriticalSection(&g_criticalSection);

	this->manager = Manager::Create();
	_proxyInstance = this;
}

void Proxy::start(String ^ portName, SuccessHandler ^ successCallback, ErrorHandler ^ errorCallback)
{
	std::wstring wsstr(portName->Data());
	std::string port(wsstr.begin(), wsstr.end());

	this->OnSuccess = successCallback;
	this->OnError = errorCallback;
	
	// TODO: pass instace as context to get rid of _proxyInstance
	this->manager->AddWatcher(OnNotification, NULL);
	this->manager->AddDriver(port);
}

void Proxy::Destroy() {
	Manager::Destroy();
	this->manager = nullptr;
	this->OnSuccess = nullptr;
	this->OnError= nullptr;
}

void Proxy::ReportSuccess(String^ status, String^ nodeId, String^ homeId)
{
	this->ReportSuccess(status, nodeId, homeId, {});
}

void OZWProxy::Proxy::ReportSuccess(String ^ status, String ^ nodeId, String ^ homeId, NodeInfo info)
{
	Windows::UI::Core::CoreDispatcher^ dispatcher = Windows::ApplicationModel::Core::CoreApplication::MainView->CoreWindow->Dispatcher;

	dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal,
		ref new Windows::UI::Core::DispatchedHandler([=]() -> void
	{
		this->OnSuccess(status, nodeId, homeId, info);
	}));
}

void Proxy::ReportError(String^ message)
{
	Windows::UI::Core::CoreDispatcher^ dispatcher = Windows::ApplicationModel::Core::CoreApplication::MainView->CoreWindow->Dispatcher;

	dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal,
		ref new Windows::UI::Core::DispatchedHandler([=]() -> void
	{
		this->OnError(message);
	}));
}
