#pragma once

#ifndef _WINSOCK_DEPRECATED_NO_WARNINGS
#	define _WINSOCK_DEPRECATED_NO_WARNINGS
#	define __WAS_UNDEF_WINSOCK_NO_WARNINGS
#endif



//* microsoft api
#include <winsock2.h>
#include <Windows.h>
#include <shlwapi.h>
#include <wininet.h>
#include <mstcpip.h>
#include <ws2tcpip.h>
#include <rpc.h>
#include <ntdsapi.h>

//* std
#include <thread>
#include <mutex>
#include <vector>
#include <algorithm>
#include <string>
#include <chrono>
#include <string.h>
#include <functional>

//* ikarusweb
#include "DynamicBuffer.h"
#include "Download.h"
#include "Interface.h"
#include "Utils.h"

#define CriticalScope(mutex_) std::lock_guard<std::mutex> lock(mutex_);



#ifdef __WAS_UNDEF_WINSOCK_NO_WARNINGS
#undef _WINSOCK_DEPRECATED_NO_WARNINGS
#endif