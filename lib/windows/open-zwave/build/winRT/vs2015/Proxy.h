#pragma once

#include "Windows.h"
#include "Options.h"
#include "Manager.h"
#include "Driver.h"
#include "Node.h"
#include "Group.h"
#include "Notification.h"
#include "value_classes/ValueStore.h"
#include "value_classes/Value.h"
#include "value_classes/ValueBool.h"
#include "platform/Log.h"

using namespace OpenZWave;

typedef struct {
	String^ status;
	String^ nodeId;
	String^ homeId;
} SuccessEventArgs;

typedef struct {
	String^ status;
	String^ message;
} ErrorEventArgs;

namespace OZWProxy
{
    public ref class Proxy sealed
    {
	private:
		void onNotification(Notification const* _notification, void* _context);
    public:
        Proxy();
		void start(String^ portName, SuccessHandler^ success, ErrorHandler^ error);
    };

	public delegate void SuccessHandler(SuccessEventArgs evt);
	public delegate void ErrorHandler(ErrorEventArgs evt);
}
