#include "StdAfx.h"
#include "PingManager.h"
#ifdef ENABLE_PINGTIME
#include <iphlpapi.h>
#include <icmpapi.h>
#include <stdio.h>
#include <thread>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

extern const char *	global_ip;
std::atomic_bool g_shutPing = false;
std::thread* _pThread = nullptr;

void __cdecl CPingManager::Shut() {
	g_shutPing = true;
	if (_pThread && _pThread->joinable())
		_pThread->join();
}

int __cdecl CPingManager::PingTimeRev()  {
	static HANDLE hIcmpFile;
	static unsigned long ipaddr = INADDR_NONE;
	static DWORD dwRetVal = 0;
	static char SendData[32] = "Data Buffer";
	static size_t ReplySize = 0;
	static std::atomic<int> _atomicRes = 0;

	if (!global_ip)
		return 1;

	auto __pingProcess = []() {
		auto get_time = []() { return std::chrono::duration<double>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();};
		auto next_ping = get_time();

		while (g_shutPing == false) {
			if (next_ping < get_time())
			{
				if(ipaddr == INADDR_NONE)
					ipaddr = inet_addr(global_ip);

				if (ipaddr == INADDR_NONE) {
					return;
				}

				hIcmpFile = IcmpCreateFile();

				if (hIcmpFile == INVALID_HANDLE_VALUE) {
					return;
				}

				ReplySize = sizeof(ICMP_ECHO_REPLY) + sizeof(SendData);
				std::unique_ptr<unsigned char[]> ReplyBuffer(std::make_unique<unsigned char[]>(ReplySize));

				if (ReplyBuffer.get() == NULL) {
					return;
				}

				dwRetVal = IcmpSendEcho(hIcmpFile, ipaddr, SendData, sizeof(SendData), NULL, ReplyBuffer.get(), ReplySize, 1000);
				if (dwRetVal != 0) {
					PICMP_ECHO_REPLY pEchoReply = (PICMP_ECHO_REPLY)ReplyBuffer.get();
					_atomicRes = pEchoReply->RoundTripTime;
				}

				next_ping += 5.0;
			}			
			std::this_thread::sleep_for(std::chrono::duration<double>(0.1));
		}
	};

	if (!_pThread) {
		_pThread = new std::thread(__pingProcess);
	}

	return _atomicRes;
}
#endif