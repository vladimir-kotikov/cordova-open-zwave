#pragma once

using namespace Platform;

#include "Notification.h"

using namespace OpenZWave;

namespace OZWProxy
{
	public delegate void SuccessHandler(String^ status, String^ nodeId, String^ homeId);
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
		void ReportSuccess(String^ status, String^ nodeId, String^ homeId);
		void ReportError(String^ message);
    };
}
