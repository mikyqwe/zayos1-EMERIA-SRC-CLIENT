#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <psapi.h>
#include "CShield.h"
#include "CShieldCheckIntegrity.h"

#if defined(ENABLE_CSHIELD) && !defined(_DEBUG)
std::string GetExeFileName()
{
	char buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	return std::string(buffer);
}

std::string GetExePath()
{
	std::string f = GetExeFileName();
	return f.substr(0, f.find_last_of("\\/"));
}

#define STATUS_SUCCESS ((NTSTATUS)0x00000000L)
#define ThreadQuerySetWin32StartAddress 9
#define NtCurrentProcess ((HANDLE)(LONG_PTR)-1)
typedef NTSTATUS(WINAPI* NTQUERYINFOMATIONTHREAD)(HANDLE, LONG, PVOID, ULONG, PULONG);
DWORD WINAPI GetThreadStartAddress(__in HANDLE hThread)
{
	NTSTATUS ntStatus;
	DWORD dwThreadStartAddr = 0;
	HANDLE hPeusdoCurrentProcess, hNewThreadHandle;
	NTQUERYINFOMATIONTHREAD NtQueryInformationThread;

	if ((NtQueryInformationThread = (NTQUERYINFOMATIONTHREAD)GetProcAddress(GetModuleHandle("ntdll.dll"), "NtQueryInformationThread"))) {
		hPeusdoCurrentProcess = GetCurrentProcess();
		if (DuplicateHandle(hPeusdoCurrentProcess, hThread, hPeusdoCurrentProcess, &hNewThreadHandle, THREAD_QUERY_INFORMATION, FALSE, 0)) {
			ntStatus = NtQueryInformationThread(hNewThreadHandle, ThreadQuerySetWin32StartAddress, &dwThreadStartAddr, sizeof(DWORD), NULL);
			CloseHandle(hNewThreadHandle);
			if (ntStatus != STATUS_SUCCESS) return 0;
		}
	}

	return dwThreadStartAddr;
}

BOOL ValidateCShield()
{
	BOOL b_Suspend = false;;
	cProcInfo i_Proc;
	DWORD u32_Error = i_Proc.Capture();

	if (u32_Error)
	{
#ifdef ENABLE_CSHIELD_DEBUG
		ErrorMessage("Error 1");
#endif
		clientCode = 42;
		Close();
	}

	SYSTEM_PROCESS* pk_Proc = i_Proc.FindProcessByPid(GetCurrentProcessId());
	if (!pk_Proc)
	{
#ifdef ENABLE_CSHIELD_DEBUG
		ErrorMessage("Couldnt find PID");
#endif
		clientCode = 42;
		Close();
	}

	SYSTEM_THREAD* pk_Thread = i_Proc.FindThreadByTid(pk_Proc, myCShieldData.dwThreadId);
	if (!pk_Thread)
	{
#ifdef ENABLE_CSHIELD_DEBUG
		ErrorMessage("Couldnt find TID");
#endif
		clientCode = 42;
		Close();
	}

	i_Proc.IsThreadSuspended(pk_Thread, &b_Suspend);
	if (b_Suspend)
	{
#ifdef ENABLE_CSHIELD_DEBUG
		ErrorMessage("Thread is suspended");
#endif
		clientCode = 42;
		Close();
	}

	// new checks
	DWORD dwStartAddress = GetThreadStartAddress(myCShieldData.hThread);
	if (myCShieldData.dwStartAddress != dwStartAddress)
	{
#ifdef ENABLE_CSHIELD_DEBUG
		ErrorMessage("Error 2");
#endif
		clientCode = 42;
		Close();
	}

	MODULEINFO user32ModInfo = { 0 };
	if (GetModuleInformation(NtCurrentProcess, LoadLibraryA("user32"), &user32ModInfo, sizeof(user32ModInfo)))
	{
		DWORD dwUser32Low = (DWORD)user32ModInfo.lpBaseOfDll;
		DWORD dwUser32Hi = (DWORD)user32ModInfo.lpBaseOfDll + user32ModInfo.SizeOfImage;

		if (dwUser32Low != myCShieldData.dwUser32Low || dwUser32Hi != myCShieldData.dwUser32Hi)
		{
#ifdef ENABLE_CSHIELD_DEBUG
			ErrorMessage("Error 3");
#endif
			clientCode = 42;
			Close();
		}
	}

	MEMORY_BASIC_INFORMATION mbi = { 0 };
	if (VirtualQuery((LPCVOID)dwStartAddress, &mbi, sizeof(mbi)))
	{
		DWORD dwMemType = mbi.Type;
		if (myCShieldData.dwMemType != dwMemType)
		{
#ifdef ENABLE_CSHIELD_DEBUG
			ErrorMessage("Error 4");
#endif
			clientCode = 42;
			Close();
		}

		DWORD dwMemAllocationBase = (DWORD)mbi.AllocationBase;
		if (myCShieldData.dwMemAllocationBase != dwMemAllocationBase)
		{
#ifdef ENABLE_CSHIELD_DEBUG
			ErrorMessage("Error 5");
#endif
			clientCode = 42;
			Close();
		}
	}

	return true;
}

#ifdef ENABLE_CSHIELD_DEBUG
void ErrorMessage(std::string error)
{
	std::ofstream myfile("syserr.txt", std::ios::app);
	if (myfile.is_open())
	{
		myfile << error << "\n";
		myfile.close();
	}
}
#endif

inline void Close()
{
	SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)NULL);
	__asm {
		mov eax, 0x001
		jmp eax
	}
}
#endif