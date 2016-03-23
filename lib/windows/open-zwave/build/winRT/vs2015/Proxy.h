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
		void OnNotification(Notification const* _notification, void* _context);
		SuccessHandler^ OnSuccess;
		ErrorHandler^ OnError;
    public:
        Proxy();
		void start(String^ portName, SuccessHandler^ successCallback, ErrorHandler^ errorCallback);
    };
}
