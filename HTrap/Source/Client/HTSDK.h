#pragma once
#ifndef HTSDK_H
#define HTSDK_H
#include <Windows.h>
#include <string>
#include <stdint.h>
#include <atomic>
#include <chrono>
#include <map>
#include "HTVersion.h"
/*
	~~~~~~ ADVANCED ~~~~~~

	If you're using some kind of virtualization (CodeVirtualizer, Themida, VMProtect etc...),
	then here's some macro already set up for you, so you don't have modify my code everywhere!

	Just define the following macros in your stdafx.h:

		HACKTRAP_FUNC_MARKER_START YOUR_CUSTOM_MACRO
			and
		HACKTRAP_FUNC_MARKER_END YOUR_CUSTOM_MACRO

	Before including this header and they will be autmatically used at the most critical parts of
	the code. You're done and the code is well protected.

	~~~~~~ ADVANCED ~~~~~~
*/

#ifndef HACKTRAP_FORCEINLINE
	#define HACKTRAP_FORCEINLINE __forceinline
#endif
#ifndef HACKTRAP_FUNC_MARKER_START
	#define HACKTRAP_SYSCALL hlt
	#define HACKTRAP_FUNC_MARKER_START
#else
	#define HACKTRAP_SYSCALL sti
#endif

#ifndef HACKTRAP_FUNC_MARKER_END
	#define HACKTRAP_FUNC_MARKER_END
#endif

/*
	~~~~~~ CUSTOM HACKTRAP-SECURITY CHECKS ~~~~~~
*/

// These are 2 different approaches for the very same thing
// Practically they are equivalent, however it's a bit easier to patch the non "EX" version, because it leaves traces
// The "EX" version leaves much less traces!

// In this case you just have to specify whether you want a FAST or a SLOW scan
#define HACKTRAP_CHECK_INTEGRITY(check_type) Sleep(check_type);

// Here it's a bit more advanced to use. You have to specify the execution speed first - fast or slow?
// Then you have to specify a local DWORD variable and a magic value. If running succeeds
#define HACKTRAP_CHECK_INTEGRITY_EX(check_type, local_variable, target_magic) HackTrap::SecurityCheck(check_type, local_variable, target_magic)

/*
	In both cases either be it fast or slow mode, checks are only running in every 5second. If less time was spent,
	the check is skipped and it returns all good.
	SLOW mode already includes FAST check, so you don't have to call it twice
	It's restricted to make you be able to put less stress on CPU.

	Although I suggest running SLOW even less often.
*/
#define HACKTRAP_INTEGRITY_FAST 0x90959E05			// Best case: 1ms
#define HACKTRAP_INTEGRITY_SLOW 0x1D006B17			// Best case: ~300ms
#define HACKTRAP_INTEGRITY_SEQUENCE 0xAB3D1E6E		// Place the next element into the variable with the seed of the "magic value"
													// And also execute the INTEGRITY_FAST method

// In both case worst case can be random. I've seen 10+sec running time in logs, but it may just be a suspended process
// I can never be sure. But according to my tasting on slower laptops it shouldn't be slower than a few seconds

namespace HackTrap
{
	/*
		-----------------------------------------------------------
			HEARTBEAT RELATED CODE PARTS
		-----------------------------------------------------------
	*/
	enum class Command
	{
		/*	HackTrap calls this once the anticheat is fully initialized and ready to start.
				> Buffer contains a special number which should be verified using IsValidInitNumber()
				> Return value matters, HackTrap terminates if receives FALSE (0) answer
		*/
		INITIALIZE_CALLBACK,

		/*	HackTrap tells the game what version it's running
				> You should use GetVersionNumber() to get the proper number from the message
		*/
		VERSION_CHECK,

		/*	HackTrap queries if server seed has new seed. Such event can happen anytime if your server decides so. I suggest using different each time
			a user chooses a new character or in case of re-logins.
			You can think of this seed, like a website has a "cookie".
				> Buffer & size is nullptr, should not be used
				> Return should be the new SEED. If there's no new seed, then return 0
				> Notes: Seed MUST be >0
		*/
		CHECK_NEW_SEED,

		/*	HackTrap requests the client to send a heartbeat message with the content specified in the buffer. Occurs about once in every ~30 sec
				> The parameter contains a buffer where the message is written. It MAY NOT be null terminated, so always use
				  the size parameter!!! The buffer may contain non-printable characters!
				> Return TRUE (!=0) if sending or adding it to the buffer succeeded.
		*/
		SEND_HEARTBEAT_MESSAGE,

		/*
			HackTrap broadcasts a function pointer so you can call HB generation manually
				> Should use GetHBGenFunction() on the message to get the actual pointer
		*/
		BROADCAST_HEARTBEAT_FUNCTION,

		/*
			If HackTrap finds a hack, it will signal you, so that you can do whatever you want :) 
			Note that this enum is only used when the anticheat is 200% sure that it's an injected cheat. Very low chance of False Positive!
		*/
		HACK_FOUND,

		/*
			If HackTrap finds something supicious, it will signal you, so that you can do whatever you want :)
			Note that this enum may be used for some suspicious tools, digital signature errors, license errors too!
		*/
		SUSPICIOUS_FOUND,

		/*	HackTrap tells the game what DATABASE version it's running
				> You should use GetVersionNumber() to get the proper number from the message
		*/
		VERSION_CHECK_DATABASE,

		/*
			HackTrap broadcasts a function pointer so you can use the Heartbeat v2 API
				> Should use GetHBGenFunction() on the message to get the actual pointer
		*/
		BROADCAST_HEARTBEAT2_FUNCTION,
	};

	/*
		Callback to receive heartbeat messages
		
		In practice it should look like this:
			uint32_t CALLBACK MyCallback(const HackTrap::Command command, const uint8_t* const message, const uint32_t messageSize);
	*/
	typedef uint32_t(CALLBACK* HackTrapCallback)(
		IN	const Command			command,
		IN	const uint8_t* const	message		OPTIONAL,
		IN	const uint32_t			messageSize	OPTIONAL
	);

	/*
		BROADCAST_HEARTBEAT_FUNCTION gives you a function pointer in message, this type is used in that case.

		hbId		-	A unique identifier, which tells which heartbeat generator object should be used
		resetSeed	-	Whether the function should reset the sequence before generating a new element
		seed		-	If "resetSeed" is true, then parameter is used to set the seed to that specified seed after resetting
						If "resetSeed" is true and "seed" is not specified (0) then it's not changing the seed to "0" but it resets the current, whatever it is
					-	If "resetSeed" is false, then parameter is ignored
	*/
	typedef BOOL(WINAPI* StaticSequenceGenerator)(
		IN	const uint32_t	hbId,
		OUT	uint64_t*		outElement,
		IN	const bool		resetSeed	OPTIONAL,
		IN	const uint32_t	seed		OPTIONAL
	);

	/*
		BROADCAST_HEARTBEAT2_FUNCTION gives you a function pointer in message, this type is used in that case.
	*/
	typedef VOID(WINAPI* Heartbeat2SetUID)(
		IN  const char* const uid,
		IN  const uint32_t size
	);

	/*
		-----------------------------------------------------------
			MAIN CORE RELATED CODE PARTS
		-----------------------------------------------------------
	*/

	// Get the last error code
	extern HACKTRAP_FORCEINLINE DWORD GetLastError();
	extern HACKTRAP_FORCEINLINE void SetLastError(DWORD code);

	// Main initialization function
	extern HACKTRAP_FORCEINLINE bool Initialize(const std::string& license, HackTrap::HackTrapCallback callback = nullptr);

	// Main HackTrap load function
	extern HACKTRAP_FORCEINLINE bool Start(const char* library = nullptr, const char* directory = nullptr);

	// Make sure initialization is valid
	extern HACKTRAP_FORCEINLINE bool IsValidInitNumber(const uint8_t* incomingMessage, const uint32_t incomingSize);

	// Retrieve version number from message
	extern HACKTRAP_FORCEINLINE uint64_t GetVersionNumber(const uint8_t* incomingMessage, const uint32_t incomingSize);

	// Execute a security check without calling any external APIs
	extern void HACKTRAP_FORCEINLINE SecurityCheck(const DWORD executionType, DWORD* const localVariable, const DWORD targetMagic);

	template<typename T>
	HACKTRAP_FORCEINLINE T GetFunction(const uint8_t* incomingMessage, const uint32_t incomingSize)
	{
		UNREFERENCED_PARAMETER(incomingSize);
		static_assert(std::is_same<T, HackTrap::StaticSequenceGenerator>::value || std::is_same<T, HackTrap::Heartbeat2SetUID>::value, "Invalid GetFunction");

		return reinterpret_cast<T>(incomingMessage);
	}

	/*
		----------------------------------------------------------------------------------------------------------------------
			Error codes which may be returned from the HackTrap::GetLastError() function call
		----------------------------------------------------------------------------------------------------------------------

		Numbers can be either one of:
		https://docs.microsoft.com/en-us/windows/win32/debug/system-error-codes

		or one of the ones defined here
	*/

	#define HACKTRAP_ERROR_GENERIC			20001
	#define HACKTRAP_ERROR_SECURITY			20002
	#define HACKTRAP_ERROR_INVALID_DLL		20003
	#define HACKTRAP_ERROR_EXCEPTION		20004
	#define HACKTRAP_ERROR_RECONF			20005
	#define HACKTRAP_ERROR_DLL_NO_LOAD		20006
	#define HACKTRAP_ERROR_INVALID_SIZE		20007

	/*
		-----------------------------------------------------------
			This is a very basic CLIENT ONLY heartbeat implementation. Should not blindly rely on this,
			especially not if you're going for the premium experience.
		-----------------------------------------------------------
	*/

	namespace BasicHeartbeat
	{
#define HT_BASIC_KEY   (DWORD_PTR(__TIME__[0] + __TIME__[1] + __TIME__[3] + __TIME__[4] + __TIME__[6] + __TIME__[7]))
#define HT_ROLLING_KEY (DWORD_PTR(__TIME__[0] + __TIME__[1] + __TIME__[3] + __TIME__[4] + __TIME__[6] + __TIME__[7] + __COUNTER__))

		enum class HB_STATE : uint32_t
		{
			NEUTRAL,
			BAD,
			GOOD
		};

		extern HackTrap::Heartbeat2SetUID Heartbeat2SetUID;
		extern HackTrap::StaticSequenceGenerator StaticSequenceGenerator;
		extern std::atomic<HackTrap::BasicHeartbeat::HB_STATE> g_initialized;
		extern std::atomic<bool> g_armed;
		extern std::atomic<uint64_t> g_confirmed;
		extern std::atomic<DWORD> g_pid;

		// Call this after the HackTrap::Start()
		extern HACKTRAP_FORCEINLINE bool BeginProtection();

		// Call this in your callback
		extern HACKTRAP_FORCEINLINE void HeartbeatRecieved();

		// Security-incident termination which calls error message and closes the game
		// Calls HTShowErrorMessage with the provided error code from the 'directory' specified
		extern HACKTRAP_FORCEINLINE void Terminate(const uint64_t errorCode, const char* directory = nullptr);

		// Just a security check
		// Only use it if you're using the BasicHeartbeat namespace at all! Else it's useless!
		/*
			Sample usage:

			if(!HACKTRAP_INITIALIZED) {
				... terminate ...
			}
		*/
		#define HACKTRAP_INITIALIZED (																												\
			(																																		\
				HackTrap::BasicHeartbeat::g_initialized == HackTrap::BasicHeartbeat::HB_STATE::GOOD													\
			)																																		\
			&&																																		\
			(																																		\
				[]() -> bool {																														\
					uint64_t generatedElement = 0;																									\
					HackTrap::BasicHeartbeat::StaticSequenceGenerator(0, &generatedElement, true, HackTrap::BasicHeartbeat::g_pid);					\
					return HackTrap::BasicHeartbeat::g_confirmed == generatedElement;																\
				}()																																	\
			)																																		\
		)
	}
}
#endif