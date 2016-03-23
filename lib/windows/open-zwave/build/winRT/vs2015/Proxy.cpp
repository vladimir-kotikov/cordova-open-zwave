#include "pch.h"
#include "Proxy.h"

using namespace OZWProxy;
using namespace Platform;

static CRITICAL_SECTION g_criticalSection;
//static Manager *manager;
//static SuccessHandler ^success;
//static ErrorHandler ^error;

Proxy::Proxy()
{/*
	Options::Create("../../../../../../config/", "", "");
	Options::Get()->AddOptionInt("SaveLogLevel", LogLevel_Detail);
	Options::Get()->AddOptionInt("QueueLogLevel", LogLevel_Debug);
	Options::Get()->AddOptionInt("DumpTrigger", LogLevel_Error);
	Options::Get()->AddOptionInt("PollInterval", 500);
	Options::Get()->AddOptionBool("IntervalBetweenPolls", true);
	Options::Get()->AddOptionBool("ValidateValueChanges", true);
	Options::Get()->Lock();

	InitializeCriticalSection(&g_criticalSection);

	manager = Manager::Create();
	proxyInstance = this;*/
}

//void Proxy::start(String ^ portName, SuccessHandler ^ success, ErrorHandler ^ error)
//{
//	std::wstring wsstr(portName->Data());
//	std::string port(wsstr.begin(), wsstr.end());
//
//
//
//	manager->AddWatcher(this->onNotification, NULL);
//	manager->AddDriver(port);
//}

//void Proxy::OnNotification(Notification const * _notification, void * proxy)
//{
//	// Must do this inside a critical section to avoid conflicts with the main thread
//	EnterCriticalSection(&g_criticalSection);
//
//	switch (_notification->GetType())
//	{
//		case Notification::Type_NodeAdded:
//		{
//			// Add the new node to our list
//			NodeInfo* nodeInfo = new NodeInfo();
//			nodeInfo->m_homeId = _notification->GetHomeId();
//			nodeInfo->m_nodeId = _notification->GetNodeId();
//			nodeInfo->m_polled = false;
//			g_nodes.push_back(nodeInfo);
//			break;
//		}
//
//		case Notification::Type_NodeRemoved:
//		{
//			// Remove the node from our list
//			uint32 const homeId = _notification->GetHomeId();
//			uint8 const nodeId = _notification->GetNodeId();
//			for (list<NodeInfo*>::iterator it = g_nodes.begin(); it != g_nodes.end(); ++it)
//			{
//				NodeInfo* nodeInfo = *it;
//				if ((nodeInfo->m_homeId == homeId) && (nodeInfo->m_nodeId == nodeId))
//				{
//					g_nodes.erase(it);
//					delete nodeInfo;
//					break;
//				}
//			}
//			break;
//		}
//
//		case Notification::Type_DriverReady:
//		{
//			success({ "driverReady" });
//			break;
//		}
//
//		case Notification::Type_DriverFailed:
//		{
//			error({"driverFailed"});
//			break;
//		}
//
//		case Notification::Type_ValueAdded:
//		case Notification::Type_ValueRemoved:
//		case Notification::Type_ValueChanged:
//		case Notification::Type_Group:
//		case Notification::Type_NodeEvent:
//		case Notification::Type_PollingDisabled:
//		case Notification::Type_PollingEnabled:
//
//		case Notification::Type_AwakeNodesQueried:
//		case Notification::Type_AllNodesQueried:
//		case Notification::Type_AllNodesQueriedSomeDead:
//		{
//			g_nodesQueried = true;
//			break;
//		}
//
//		case Notification::Type_DriverReset:
//		case Notification::Type_NodeNaming:
//		case Notification::Type_NodeProtocolInfo:
//		case Notification::Type_NodeQueriesComplete:
//		default:
//		{
//		}
//	}
//
//	LeaveCriticalSection(&g_criticalSection);
//}