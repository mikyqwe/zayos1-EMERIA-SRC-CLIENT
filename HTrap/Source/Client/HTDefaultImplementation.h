#pragma once
#define HACKTRAP_BASICHEARTBEAT_ENABLED
#include "HTSDK.h"
#include <array>

#ifndef HACKTRAP_LIBRARY
	#define HACKTRAP_LIBRARY nullptr
#endif

#ifndef HACKTRAP_PRECALL
	#define HACKTRAP_PRECALL
#endif

#ifndef HACKTRAP_DIRECTORY
	#define HACKTRAP_DIRECTORY nullptr
#endif

#define HACKTRAP_MAGIC_VALUE ((HT_BASIC_KEY * 0x6ACD97A7 >> 13) ^ (HT_BASIC_KEY * 0x95B898D5 >> 16))

namespace HackTrapDefaultImplementation
{
#define HACKTRAP_ERROR_CBRECALL		8650166711
#define HACKTRAP_ERROR_CBINVALID	1434033894
#define HACKTRAP_ERROR_INIT			6643478765
#define HACKTRAP_ERROR_START		2705627482
#define HACKTRAP_ERROR_NOINIT		9281210470

	uint32_t CALLBACK DefaultCallback(const HackTrap::Command command, const uint8_t* const message, const uint32_t messageSize)
	{
		switch (command)
		{
		// HackTrap is initialized!
		case HackTrap::Command::INITIALIZE_CALLBACK:
			if (HackTrap::BasicHeartbeat::g_initialized == HackTrap::BasicHeartbeat::HB_STATE::GOOD)
			{
#ifdef HACKTRAP_DEBUG
				MessageBoxA(nullptr, "HackTrap::INITIALIZE_CALLBACK thrown error", "HackTrap", 0);
#endif
				HackTrap::BasicHeartbeat::Terminate(HACKTRAP_ERROR_CBRECALL, HACKTRAP_DIRECTORY);
			}
				

			if (!HackTrap::IsValidInitNumber(message, messageSize))
			{
#ifdef HACKTRAP_DEBUG
				MessageBoxA(nullptr, "HackTrap::IsValidInitNumber thrown error", "HackTrap", 0);
#endif

				HackTrap::BasicHeartbeat::Terminate(HACKTRAP_ERROR_CBINVALID, HACKTRAP_DIRECTORY);
			}

			HackTrap::BasicHeartbeat::HeartbeatRecieved();

			return TRUE;

		// Used to call Heartbeat v2 API
		case HackTrap::Command::BROADCAST_HEARTBEAT2_FUNCTION:
			HackTrap::BasicHeartbeat::Heartbeat2SetUID = HackTrap::GetFunction<HackTrap::Heartbeat2SetUID>(message, messageSize);
			return TRUE;
		
		// Used to call HeartbeatGenerator (soon to be deprecated)
		case HackTrap::Command::BROADCAST_HEARTBEAT_FUNCTION:
			HackTrap::BasicHeartbeat::StaticSequenceGenerator = HackTrap::GetFunction<HackTrap::StaticSequenceGenerator>(message, messageSize);

			{
				uint64_t generatedElement = 0;
				HackTrap::BasicHeartbeat::StaticSequenceGenerator(0, &generatedElement, true, HackTrap::BasicHeartbeat::g_pid);
				HackTrap::BasicHeartbeat::g_confirmed = generatedElement;
			}

			return TRUE;

		// Heartbeat v1 related APIs
		case HackTrap::Command::SEND_HEARTBEAT_MESSAGE:
			HackTrap::BasicHeartbeat::HeartbeatRecieved();
		case HackTrap::Command::CHECK_NEW_SEED:
			return TRUE;

		// HackTrap DLL version check
		case HackTrap::Command::VERSION_CHECK:
			if (HackTrap::GetVersionNumber(message, messageSize) >= HACKTRAP_VERSION)
				return TRUE;
			else
				return FALSE;

		// HackTrap Database version check
		case HackTrap::Command::VERSION_CHECK_DATABASE:
			if (HackTrap::GetVersionNumber(message, messageSize) >= HACKTRAP_DATABASE_VERSION)
				return TRUE;
			else
				return FALSE;

		// Hack/Suspicious detection found
		// Not that you don't HAVE TO do anything here
		// It's only used if you'd like to do EXTRA things apart from just simply showing error message and exit
		case HackTrap::Command::HACK_FOUND:
		case HackTrap::Command::SUSPICIOUS_FOUND:

		// Unknown
		default:
			return FALSE;
		}
	}

	int HACKTRAP_FORCEINLINE StartHackTrap()
	{
		HackTrap::SetLastError(0);

#ifdef HACKTRAP_DEBUG
		MessageBoxA(nullptr, "This is a DEBUG enabled HackTrap SDK. Never release this to users.", "!!! WARNING !!!", 0);
#endif

		HACKTRAP_PRECALL;

		bool initialized = HackTrap::Initialize(HACKTRAP_LICENSE, DefaultCallback);
		if (!initialized)
		{
			// Some error occured. You can message the user or log into syserr or anything you want
			DWORD errorCode = HackTrap::GetLastError();

#ifdef HACKTRAP_DEBUG
			MessageBoxA(nullptr, "HackTrap::Initialize thrown error", "HackTrap", 0);
#endif

			HackTrap::BasicHeartbeat::Terminate(HACKTRAP_ERROR_INIT, HACKTRAP_DIRECTORY);
		}

		bool started = HackTrap::Start(HACKTRAP_LIBRARY, HACKTRAP_DIRECTORY);
		if (!started)
		{
			// Some error occured. You can message the user or log into syserr or anything you want
			DWORD errorCode = HackTrap::GetLastError();

#ifdef HACKTRAP_DEBUG
			MessageBoxA(nullptr, "HackTrap::Start thrown error", "HackTrap", 0);
#endif

			HackTrap::BasicHeartbeat::Terminate(HACKTRAP_ERROR_START, HACKTRAP_DIRECTORY);
		}

		// Initialize client-only heartbeat if server-sided is not available
		HackTrap::BasicHeartbeat::BeginProtection();

		HACKTRAP_FUNC_MARKER_START;
		size_t delayer = 1;
		while (HackTrap::BasicHeartbeat::g_initialized != HackTrap::BasicHeartbeat::HB_STATE::GOOD)
			Sleep(delayer++);

		while (!HackTrap::BasicHeartbeat::Heartbeat2SetUID || !HackTrap::BasicHeartbeat::StaticSequenceGenerator)
			Sleep(delayer++);

		// Do an integrity check
		HACKTRAP_CHECK_INTEGRITY(HACKTRAP_INTEGRITY_SLOW);

		DWORD local = 0;
		HACKTRAP_CHECK_INTEGRITY_EX(HACKTRAP_INTEGRITY_SLOW, &local, HACKTRAP_MAGIC_VALUE);

		if (!HACKTRAP_INITIALIZED || local != HACKTRAP_MAGIC_VALUE) {
#ifdef HACKTRAP_DEBUG
			MessageBoxA(nullptr, "HACKTRAP_INITIALIZED Failed", "HackTrap", 0);
#endif

			HackTrap::BasicHeartbeat::g_initialized = HackTrap::BasicHeartbeat::HB_STATE::BAD;
			HackTrap::BasicHeartbeat::Terminate(HACKTRAP_ERROR_NOINIT, HACKTRAP_DIRECTORY);
		}

		HACKTRAP_FUNC_MARKER_END;

#ifdef HACKTRAP_DEBUG
		MessageBoxA(nullptr, "HackTrap Started successfully! Press ok to continue.", "HackTrap", 0);
#endif

		return delayer;
	}

	static volatile int skeletonInitializer = StartHackTrap();
}