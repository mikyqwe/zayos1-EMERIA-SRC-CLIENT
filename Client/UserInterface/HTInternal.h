#pragma once
#ifndef HTINTERNAL_H
#define HTINTERNAL_H
#include <Windows.h>
#include <string>
#include <thread>
#include <array>
#include <atomic>

// These are just internal helper functions to generate the required string constant for several settings
// Do not use them by hand, leave it to the HackTrap dev!

#ifndef HACKTRAP_FORCEINLINE
	#define HACKTRAP_FORCEINLINE __forceinline
#endif
#ifndef HACKTRAP_FUNC_MARKER_START
	#define HACKTRAP_FUNC_MARKER_START
#endif
#ifndef HACKTRAP_FUNC_MARKER_END
	#define HACKTRAP_FUNC_MARKER_END
#endif

#define HACKTRAP_BASE GetCurrentProcessId()

namespace HackTrap
{
	namespace Internals
	{
#pragma pack(push,1)
		struct InternalLicenseConfig
		{
			char* license = nullptr;
			void* callback = nullptr;
		};
#pragma pack(pop)

		static InternalLicenseConfig g_config = {};
		static HANDLE g_semaphoreHandle = nullptr;
		static DWORD g_lastError = 0;
		static std::array<std::atomic_uint64_t, 5> magic = {
			0x9C861A0A7628C119,
			0x7603DD070C9CFDAA,
			0x0000000000000000,
			0xECE7685E1D4BA051,
			0x7603DD0760F09984
		};

		const HACKTRAP_FORCEINLINE char* GetDefaultModule()
		{
			if (magic[0] == 0x9C861A0A7628C119)
			{
				magic[0] ^= magic[3];
				magic[1] ^= magic[4];
			}

			return reinterpret_cast<const char*>(magic.data());
		}

		void HACKTRAP_FORCEINLINE DestroyDefaultModule()
		{
			for (auto& it : magic)
			{
				it = 0;
			}
		}

		unsigned long HACKTRAP_FORCEINLINE GetRandom(const unsigned long seed)
		{
			unsigned long h = 0xC613FC14;

			for (unsigned char i = 0; i < sizeof(unsigned long); i++) {
				h ^= reinterpret_cast<const unsigned char*>(&seed)[i];
				h *= 0x5BD1E994;
				h ^= h >> 15;
			}

			return h;
		}

		char HACKTRAP_FORCEINLINE GetCharacter(const unsigned long i) {
			if (i % 2) {
				return 65 + (i % 26);
			}
			else {
				return 48 + (i % 9);
			}
		}

		std::string HACKTRAP_FORCEINLINE GenerateGUID(unsigned long next) {
			HACKTRAP_FUNC_MARKER_START;
			std::string out;
			unsigned char i = 0;

			for (i = 0; i < 8; i++) {
				next = GetRandom(next);
				out += GetCharacter(next);
			}

			out += '-';

			for (i = 0; i < 3; i++) {
				for (int k = 0; k < 4; k++) {
					next = GetRandom(next);
					out += GetCharacter(next);
				}

				out += '-';
			}

			for (i = 0; i < 12; i++) {
				next = GetRandom(next);
				out += GetCharacter(next);
			}
			HACKTRAP_FUNC_MARKER_END;

			return out;
		}

		struct NormalizedValue {
			LONG normalizeNeeded = 0;
			LONG normalized = 0;
		};

		NormalizedValue HACKTRAP_FORCEINLINE NormalizeAddress(const void* value) {
			HACKTRAP_FUNC_MARKER_START;
			NormalizedValue norm;
			norm.normalizeNeeded = ((DWORD(value) & 0x80000000) != 0);
			norm.normalized = DWORD(value) & 0x7FFFFFFF;
			HACKTRAP_FUNC_MARKER_END;

			return norm;
		}

		void HACKTRAP_FORCEINLINE CleanUp()
		{
			HACKTRAP_FUNC_MARKER_START;
			if (Internals::g_semaphoreHandle) {
				CloseHandle(Internals::g_semaphoreHandle);
				Internals::g_semaphoreHandle = nullptr;
			}

			if (Internals::g_config.license) {
				ZeroMemory(Internals::g_config.license, strlen(Internals::g_config.license));
				delete[] Internals::g_config.license;
				Internals::g_config.license = nullptr;
			}

			Internals::g_config.callback = nullptr;
			HACKTRAP_FUNC_MARKER_END;
		}

		bool HACKTRAP_FORCEINLINE SaveConfig(const std::string& license, void* callback = nullptr)
		{
			// Save current settings 
			HACKTRAP_FUNC_MARKER_START;
			DWORD seed = HACKTRAP_BASE;
			size_t length = license.length();
			Internals::g_config.callback = callback;
			Internals::g_config.license = new char[length + 1];
			memcpy(Internals::g_config.license, license.c_str(), length);
			Internals::g_config.license[length] = 0;

			// This is the only edge case which I can't handle
			// Pretty 0 chance that this would ever happen
			if (reinterpret_cast<DWORD>(&Internals::g_config) == 0x80000000)
			{
				g_lastError = 1234567890; // Arbitrary custom error code
				return false;
			}
				
			HACKTRAP_FUNC_MARKER_END;

			// Generate name for our config
			const Internals::NormalizedValue nmd = Internals::NormalizeAddress(&Internals::g_config);
			const std::string guid = Internals::GenerateGUID(seed);

			// Publish the config for the HackTrap
			HACKTRAP_FUNC_MARKER_START;
			Internals::g_semaphoreHandle = CreateSemaphoreA(nullptr, nmd.normalizeNeeded, nmd.normalized, guid.c_str());
			g_lastError = GetLastError();

			// Start cleaning up after a minute
			std::thread cleanUp([]() {
				Sleep(120000);
				CleanUp();
			});
			cleanUp.detach();
			HACKTRAP_FUNC_MARKER_END;

			// Make sure everything is all right and succeed!
			return Internals::g_semaphoreHandle != nullptr && g_lastError != ERROR_INVALID_HANDLE && Internals::g_semaphoreHandle != INVALID_HANDLE_VALUE;
		}
	}
}
#endif
