#pragma once

using namespace Platform;

#include "Notification.h"

using namespace OpenZWave;

namespace OZWProxy
{
	public value struct NodeInfo {
		String^ manufacturer;
		String^ manufacturerid;
		String^ product;
		String^ producttype;
		String^ productid;
		String^ type;
		String^ name;
		String^ loc;
	};

	public delegate void SuccessHandler(String^ status, String^ nodeId, String^ homeId, NodeInfo info);
	public delegate void ErrorHandler(String^ message);

	public ref class Proxy sealed
    {
	private:
		Manager *manager;
		SuccessHandler^ OnSuccess;
		ErrorHandler^ OnError;
    public:
        Proxy();
		void start(String^ portName, SuccessHandler^ successCallback, ErrorHandler^ errorCallback);
		void Destroy();

		// TODO: this should NOT be exposed
		void ReportSuccess(String^ status, String^ nodeId, String^ homeId);
		void ReportSuccess(String^ status, String^ nodeId, String^ homeId, NodeInfo info);
		void ReportError(String^ message);
    };
}
