#include "StdAfx.h"
#define HACKTRAP_BASICHEARTBEAT_ENABLED
#include "HTSDK.h"
#include <thread>
#include <array>
#include <atomic>
#include <intrin.h>
#include <io.h>
#include <string.h>

// These are just internal helper functions to generate the required string constant for several settings
// Do not use them by hand, leave it to the HackTrap dev!

#define HACKTRAP_MAGIC_VALUE (DWORD_PTR((HT_BASIC_KEY * 0x156FC407 >> 13) ^ (HT_BASIC_KEY * 0x072187C1  >> 16)))

namespace HackTrap
{
	namespace Internals
	{
#pragma pack(push,1)
		struct InternalLicenseConfig
		{
			char* license = nullptr;
			void* callback = nullptr;
			LicenseDistributionMethod method;
		};
#pragma pack(pop)

		bool g_configured = false;
		DWORD g_lastError = 0;

		const HACKTRAP_FORCEINLINE std::string GetDefaultDirectory()
		{
			// HackTrap
			static volatile std::atomic<uint64_t> defaultDirectory = 0x9C861A0A7628C119;

			// Decrypt the encrypted text
			uint64_t directory = defaultDirectory ^ 0xECE7685E1D4BA051;

			// Append the decrypted bytes as text
			std::string directoryString(reinterpret_cast<const char*>(&directory), reinterpret_cast<const char*>(&directory) + sizeof(directory));

			return directoryString;
		}

		const HACKTRAP_FORCEINLINE std::string GetDefaultModule()
		{
			std::string library;

			// HackTrap.dll
			static std::array<std::atomic<uint64_t>, 2> defaultModule = {
				0x9C861A0A7628C119,
				0x7603DD070C9CFDAA
			};

			// Decrypt the encrypted text
			uint64_t lib1 = defaultModule[0] ^ 0xECE7685E1D4BA051, lib2 = defaultModule[1] ^ 0x7603DD0760F09984;

			// Append the decrypted bytes as text
			library.append(reinterpret_cast<const char*>(&lib1), reinterpret_cast<const char*>(&lib1) + sizeof(lib1));
			library.append(reinterpret_cast<const char*>(&lib2), reinterpret_cast<const char*>(&lib2) + sizeof(lib2));

			return library;
		}

		uint64_t HACKTRAP_FORCEINLINE MurmurOAAT64(const char* key)
		{
			uint64_t h(525201411107845655ull);
			for (; *key; ++key) {
				h ^= *key;
				h *= 0x5bd1e9955bd1e995;
				h ^= h >> 47;
			}

			return h;
		}

		uint64_t HACKTRAP_FORCEINLINE MurmurOAAT64(const char* key, const size_t length)
		{
			uint64_t h(525201411107845656ull);
			for (size_t i = 0; i < length; i++) {
				h ^= key[i];
				h *= 0x5bd1e9955bd1e996;
				h ^= h >> 47;
			}

			return h;
		}

		bool HACKTRAP_FORCEINLINE SaveConfig(const std::string& license, void* callback, const LicenseDistributionMethod method)
		{
			HACKTRAP_FUNC_MARKER_START;
			static InternalLicenseConfig g_config = {};

			// Save current settings 
			g_config.callback = callback;
			g_config.license = _strdup(license.c_str());
			g_config.method = method;

			// Publish the config
			std::wstring envName = std::to_wstring(GetCurrentProcessId());
			envName.append(1, L'H');
			envName.append(1, L'T');

			std::wstring confAddress = std::to_wstring(reinterpret_cast<uintptr_t>(&g_config));

			BOOL status = SetEnvironmentVariableW(envName.c_str(), confAddress.c_str());

			if (!status)
				g_lastError = ::GetLastError();
			else
				g_configured = true;
			HACKTRAP_FUNC_MARKER_END;

			// Make sure everything is all right and succeed!
			return status;
		}

		bool HACKTRAP_FORCEINLINE BasicSecurityCheck()
		{
			static const std::array<const uint8_t* const, 3> g_securityCheckedFunctions = {
				0, 0, 0
				/*reinterpret_cast<const uint8_t* const>(&LoadLibraryExA),
				reinterpret_cast<const uint8_t* const>(&LoadLibraryA),
				reinterpret_cast<const uint8_t* const>(&Sleep),*/
			};

			for (const auto& ptr : g_securityCheckedFunctions)
			{
				if (ptr && ptr[0] == 0x55 && ptr[1] == 0xE9)
				{
#ifdef HACKTRAP_DEBUG
						MessageBoxA(nullptr, "BasicSecurityCheck Failed", "HackTrap", 0);
#endif
					Internals::g_lastError = HACKTRAP_ERROR_HOOKED;

					return false;
				}
			}
			

			return true;
		}

		bool BasicModuleCheck(const HMODULE moduleBase)
		{
			__try
			{
				auto read = reinterpret_cast<const uint16_t* const>(moduleBase);
				if ((*read ^ 0xE210) != 0xB85D)
				{
					Internals::g_lastError = HACKTRAP_ERROR_INVALID_DLL;
					return false;
				}
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				Internals::g_lastError = HACKTRAP_ERROR_EXCEPTION;
				return false;
			}

			return true;
		}
	}
}

DWORD HackTrap::GetLastError()
{
	return Internals::g_lastError;
}

void HackTrap::SetLastError(DWORD code)
{
	Internals::g_lastError = code;
}

bool HackTrap::Initialize(const std::string& license, HackTrap::HackTrapCallback callback, const LicenseDistributionMethod method)
{
	// Make sure to only initialize once
	if (Internals::g_configured)
		return false;

	// Set the config
	return Internals::SaveConfig(license, reinterpret_cast<void*>(callback), method);
}

bool HackTrap::Start(const char* library, const char* directory)
{
	HACKTRAP_FUNC_MARKER_START;
	// If Initialization didn't happen, then don't even run
	if (!Internals::g_configured)
	{
		Internals::g_lastError = HACKTRAP_ERROR_RECONF;

#ifdef HACKTRAP_DEBUG
		MessageBoxA(nullptr, "Initialization didn't happen", "HackTrap", 0);
#endif

		return false;
	}

	// Default to HackTrap.dll
	std::string lib = directory ? directory : Internals::GetDefaultDirectory();
	if(lib.length())
		lib.append("/");
	lib.append(library ? library : Internals::GetDefaultModule());

	if (!Internals::BasicSecurityCheck())
	{
#ifdef HACKTRAP_DEBUG
		MessageBoxA(nullptr, "Security check failed", "HackTrap", 0);
#endif

		return false;
	}

	HMODULE baseAddress = LoadLibraryA(lib.c_str());

	if (!baseAddress)
	{
#ifdef HACKTRAP_DEBUG
		MessageBoxA(nullptr, "Loading module failed!", lib.c_str(), 0);
#endif

		Internals::g_lastError = ::GetLastError();
		return false;
	}

	if (baseAddress)
	{
		if (!Internals::BasicModuleCheck(baseAddress))
		{
#ifdef HACKTRAP_DEBUG
				MessageBoxA(nullptr, "BasicModuleCheck Failed #1", "HackTrap", 0);
#endif

			return false;
		}

		if (GetModuleHandleA(lib.c_str()) != baseAddress)
		{
#ifdef HACKTRAP_DEBUG
			MessageBoxA(nullptr, "BasicModuleCheck Failed #2", "HackTrap", 0);
#endif

			Internals::g_lastError = HACKTRAP_ERROR_DLL_NO_LOAD;
			return false;
		}
	}

	HACKTRAP_FUNC_MARKER_END;

	return true;
}

uint64_t HackTrap::GetVersionNumber(const uint8_t* incomingMessage, const uint32_t incomingSize)
{
	UNREFERENCED_PARAMETER(incomingSize);

	HACKTRAP_FUNC_MARKER_START;
	const uint64_t* verPtr = reinterpret_cast<const uint64_t*>(incomingMessage);
	const uint32_t encodedVersion = static_cast<uint32_t>(*verPtr);
	uint32_t version = _rotl(encodedVersion, sizeof(encodedVersion) / 2);
	HACKTRAP_FUNC_MARKER_END;

	return version;
}

void HACKTRAP_FORCEINLINE HackTrap::SecurityCheck(const DWORD executionType, DWORD* const localVariable, const DWORD targetMagic)
{
	__try
	{
		_asm {
			push eax
			push ebx
			push ecx

			mov eax, localVariable
			mov ebx, targetMagic
			mov ecx, executionType

			HACKTRAP_SYSCALL

			pop ecx
			pop ebx
			pop eax
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
	}
}

bool HackTrap::IsValidInitNumber(const uint8_t* incomingMessage, const uint32_t incomingSize)
{
	HACKTRAP_FUNC_MARKER_START;
	static std::atomic<DWORD_PTR> pid = GetCurrentProcessId();

	DWORD_PTR currentPid = pid;

	if (incomingSize != 16)
	{
		Internals::g_lastError = HACKTRAP_ERROR_INVALID_SIZE;
		return false;
	}

	const uint64_t* messageNumber = reinterpret_cast<const uint64_t*>(incomingMessage);

	bool valid = Internals::MurmurOAAT64(reinterpret_cast<char*>(&currentPid), sizeof(currentPid)) == *messageNumber;
	HACKTRAP_FUNC_MARKER_END;

	return valid;
}

// Global externally defined variables
std::atomic<bool>								HackTrap::BasicHeartbeat::g_armed = false;
std::atomic<DWORD>								HackTrap::BasicHeartbeat::g_pid = GetCurrentProcessId();
std::atomic<uint64_t>							HackTrap::BasicHeartbeat::g_confirmed = 0;
HackTrap::Heartbeat2SetUID						HackTrap::BasicHeartbeat::Heartbeat2SetUID = nullptr;
HackTrap::StaticSequenceGenerator				HackTrap::BasicHeartbeat::StaticSequenceGenerator = nullptr;
std::atomic<HackTrap::BasicHeartbeat::HB_STATE>	HackTrap::BasicHeartbeat::g_initialized = HackTrap::BasicHeartbeat::HB_STATE::NEUTRAL;
std::atomic<uint64_t>							HackTrap::BasicHeartbeat::g_versionVerified = 0;
DWORD											HackTrap::BasicHeartbeat::g_securityThreadId = 0;

void HACKTRAP_FORCEINLINE HackTrap::BasicHeartbeat::Terminate(const uint64_t errorCode, const char* directory)
{
	HACKTRAP_FUNC_MARKER_START;

	// Local string constants used as low-key encrypted
	static volatile std::atomic<uint64_t> start = 0x41809D2E01E1FA9C;

	static volatile std::array<std::atomic<uint64_t>, 3> errorDisplayer = {
		0x7085CA351BD3DAA7,
		0xB2C113953A08E839,
		0xCF62A124CF886255
	};

	// 'start'
	uint64_t decStart = start ^ 0x41809D5A73808EEF;
	std::string decryptedStart = reinterpret_cast<const char*>(&decStart);

	// 'HackTrap'
	std::string decryptedDirectory = directory  ? directory : HackTrap::Internals::GetDefaultDirectory();

	// HTShowMessage.exe
	std::array<std::atomic<uint64_t>, 3> errorProcess = {
		errorDisplayer._Elems[0] ^ 0x15C8BD5A73808EEF,
		errorDisplayer._Elems[1] ^ 0xCAA43DF05D699B4A,
		errorDisplayer._Elems[2] ^ 0x03AE6DE80344FC30
	};

	std::string decryptedShowMessage;

	decryptedShowMessage.append(reinterpret_cast<const char*>(&errorProcess[0]), reinterpret_cast<const char*>(&errorProcess[0]) + sizeof(errorProcess[0]));
	decryptedShowMessage.append(reinterpret_cast<const char*>(&errorProcess[1]), reinterpret_cast<const char*>(&errorProcess[1]) + sizeof(errorProcess[1]));
	decryptedShowMessage.append(reinterpret_cast<const char*>(&errorProcess[2]), reinterpret_cast<const char*>(&errorProcess[2]) + 1);

	std::string targetProcess;
	if (decryptedDirectory.length())
		targetProcess = decryptedDirectory + '\\';

	targetProcess += decryptedShowMessage;

	// Target error message
	std::string message = decryptedStart + ' ' + targetProcess + ' ' + std::to_string(17) + ' ' + std::to_string(errorCode) + '-' + std::to_string(HackTrap::GetLastError());

#ifdef HACKTRAP_DEBUG
	std::string debugMessage = std::string("Error happened, trying to execute:\n\n") + decryptedStart + ' ' + targetProcess + ' ' + std::to_string(17) + ' ' + std::to_string(errorCode) + '-' + std::to_string(HackTrap::GetLastError());
	MessageBoxA(nullptr, debugMessage.c_str(), "HackTrap", 0);
#endif

	// Check if file exists & accessible
	if (_access_s(targetProcess.c_str(), 0) == 0)
	{
		std::system(message.c_str());
	}
	else
	{
		char targetBuffer[512] = { 0 };
		sprintf_s(targetBuffer, "start mshta javascript:alert(\"HT Error:\\n\\n%s\");close();", message.c_str());

		std::system(targetBuffer);
	}

	std::thread finalMurder([]() {
		Sleep(1000);
		std::terminate();
	});

	ExitProcess(0);
	HACKTRAP_FUNC_MARKER_END;
}

bool HackTrap::BasicHeartbeat::BeginProtection()
{
	HACKTRAP_FUNC_MARKER_START;
	std::thread guardThread([]() {
		DWORD local = HACKTRAP_MAGIC_VALUE;

		while (true)
		{
			Sleep(60000);

			g_initialized = g_armed ? HackTrap::BasicHeartbeat::HB_STATE::GOOD : HackTrap::BasicHeartbeat::HB_STATE::BAD;

			if (!g_armed)
			{
#ifdef HACKTRAP_DEBUG
				MessageBoxA(nullptr, "Armed failure!", "HackTrap", 0);
#endif

				Terminate(HACKTRAP_ERROR_BASICHB);
			}

			if (local != HACKTRAP_MAGIC_VALUE)
			{
#ifdef HACKTRAP_DEBUG
				MessageBoxA(nullptr, "Security check fired!", "HackTrap", 0);
#endif

				SetLastError(local);
				Terminate(HACKTRAP_ERROR_SECURITY);
			}

			HACKTRAP_CHECK_INTEGRITY(HACKTRAP_INTEGRITY_FAST);
			g_armed = false;
			local = 0;
			HACKTRAP_CHECK_INTEGRITY_EX(HACKTRAP_INTEGRITY_FAST, &local, HACKTRAP_MAGIC_VALUE);
		}
	});

	g_securityThreadId = GetThreadId(guardThread.native_handle());

	guardThread.detach();
	HACKTRAP_FUNC_MARKER_END;

	return guardThread.joinable();
}

void HackTrap::BasicHeartbeat::HeartbeatRecieved()
{
	HACKTRAP_FUNC_MARKER_START;
	if(HackTrap::BasicHeartbeat::g_versionVerified >= HACKTRAP_VERSION)
		g_armed = true;

	g_initialized = HackTrap::BasicHeartbeat::HB_STATE::GOOD;
	HACKTRAP_FUNC_MARKER_END;
}
