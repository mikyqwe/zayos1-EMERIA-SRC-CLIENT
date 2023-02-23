#include "StdAfx.h"

#include <io.h>
#include <assert.h>
#include <XORstr.h>
#include "EterPackManager.h"
#include "EterPackPolicy_CSHybridCrypt.h"
#include "../eterBase/Debug.h"
#include "../eterBase/CRC32.h"

#define PATH_ABSOLUTE_YMIRWORK1	"d:/ymir work/"
#define PATH_ABSOLUTE_YMIRWORK2	"d:\\ymir work\\"

#ifdef __THEMIDA__
#include <ThemidaSDK.h>
#endif

#ifdef __ENABLE_NEW_PACK_ENCRYPTION__


#ifdef __ENABLE_IKD_DEBUGGER_PREVENT__
bool CheckProcessDebugFlags()
{
	// Much easier in ASM but C/C++ looks so much better
	typedef NTSTATUS(WINAPI* pNtQueryInformationProcess)
		(HANDLE, UINT, PVOID, ULONG, PULONG);

	DWORD NoDebugInherit = 0;
	NTSTATUS Status;

	// Get NtQueryInformationProcess
	pNtQueryInformationProcess NtQIP = (pNtQueryInformationProcess)
		GetProcAddress(GetModuleHandle(TEXT("ntdll.dll")),
			"NtQueryInformationProcess");

	Status = NtQIP(GetCurrentProcess(),
		0x1f, // ProcessDebugFlags
		&NoDebugInherit, 4, NULL);

	if (Status != 0x00000000)
		return false;

	if (NoDebugInherit == FALSE)
		return true;
	else
		return false;
}

bool CheckForBreakPoints()
{
	
	unsigned char* pMem = NULL;
	SYSTEM_INFO sysinfo = { 0 };
	DWORD OldProtect = 0;
	void* pAllocation = NULL; // Get the page size for the system 

	GetSystemInfo(&sysinfo); // Allocate memory 

	pAllocation = VirtualAlloc(NULL, sysinfo.dwPageSize,
		MEM_COMMIT | MEM_RESERVE,
		PAGE_EXECUTE_READWRITE);

	
	if (pAllocation == NULL)
		return false;

	// Write a ret to the buffer (opcode 0xc3)
	pMem = (unsigned char*)pAllocation;
	*pMem = 0xc3;

	// Make the page a guard page         
	if (VirtualProtect(pAllocation, sysinfo.dwPageSize,
		PAGE_EXECUTE_READWRITE | PAGE_GUARD,
		&OldProtect) == 0)
	{
		return false;
	}

	__try
	{
		__asm
		{
			mov eax, pAllocation
			// This is the address we'll return to if we're under a debugger
			push MemBpBeingDebugged
			jmp eax // Exception or execution, which shall it be :D?
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		// The exception occured and no debugger was detected
		VirtualFree(pAllocation, NULL, MEM_RELEASE);
		return false;
	}

	__asm {MemBpBeingDebugged:}
	VirtualFree(pAllocation, NULL, MEM_RELEASE);
	return true;
}

int CheckForHardwareBreakPoints()
{
	unsigned int NumBps = 0;

	// This structure is key to the function and is the 
	// medium for detection and removal
	CONTEXT ctx;
	ZeroMemory(&ctx, sizeof(CONTEXT));

	// The CONTEXT structure is an in/out parameter therefore we have
	// to set the flags so Get/SetThreadContext knows what to set or get.
	ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;

	// Get a handle to our thread
	HANDLE hThread = GetCurrentThread();

	// Get the registers
	if (GetThreadContext(hThread, &ctx) == 0)
		return -1;

	// Now we can check for hardware breakpoints, its not 
	// necessary to check Dr6 and Dr7, however feel free to
	if (ctx.Dr0 != 0)
		++NumBps;
	if (ctx.Dr1 != 0)
		++NumBps;
	if (ctx.Dr2 != 0)
		++NumBps;
	if (ctx.Dr3 != 0)
		++NumBps;

	return NumBps;
}

inline bool HideThreadFromDebugger(HANDLE hThread = NULL)
{
	typedef NTSTATUS(NTAPI* pNtSetInformationThread)
		(HANDLE, UINT, PVOID, ULONG);
	NTSTATUS Status;

	// Get NtSetInformationThread
	pNtSetInformationThread NtSIT = (pNtSetInformationThread)
		GetProcAddress(GetModuleHandle(TEXT("ntdll.dll")),
			"NtSetInformationThread");

	// Shouldn't fail
	if (NtSIT == NULL)
		return false;

	// Set the thread info
	if (hThread == NULL)
		Status = NtSIT(GetCurrentThread(),
			0x11, // HideThreadFromDebugger
			0, 0);
	else
		Status = NtSIT(hThread, 0x11, 0, 0);

	if (Status != 0x00000000)
		return false;
	else
		return true;
}

bool CheckForDebugObject()
{
	// Much easier in ASM but C/C++ looks so much better
	typedef NTSTATUS(WINAPI* pNtQueryInformationProcess)
		(HANDLE, UINT, PVOID, ULONG, PULONG);

	HANDLE hDebugObject = NULL;
	NTSTATUS Status;

	// Get NtQueryInformationProcess
	pNtQueryInformationProcess NtQIP = (pNtQueryInformationProcess)
		GetProcAddress(GetModuleHandle(TEXT("ntdll.dll")),
			"NtQueryInformationProcess");

	Status = NtQIP(GetCurrentProcess(),
		0x1e, // ProcessDebugObjectHandle
		&hDebugObject, 4, NULL);

	if (Status != 0x00000000)
		return false;

	if (hDebugObject)
		return true;
	else
		return false;
}

bool CheckFor2DInstruction()
{
	__try
	{
		__asm
		{
			int 0x2d
			xor eax, eax
			add eax, 2
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		return false;
	}
	return true;
}

bool CheckForDebugger() {
	return (CheckProcessDebugFlags() ||
		CheckForBreakPoints() ||
		CheckForHardwareBreakPoints() != 0 ||
		CheckForDebugObject() ||
		CheckFor2DInstruction());
}

#endif



#include <ikarusweb2/IkarusWeb2.h>
#define ikarusweb ikarusweb2

namespace ikarus {
	void decrypt_url(void* mem, size_t len) {
		ikaruspack::dynamic_buffer buff;
		buff.write(mem, len);

		ikaruspack::__Key key;
		key.set_bytes(std::string("asdaf22f2fwfgfgj23gu823g"));

		ikaruspack::utils_easy_decrypt(buff, key);
		std::memcpy(mem, buff.get_data(), len);
	}
}


char g_local_key[] = { -30, -33, -16, -88, -92, 93, 94, -91, 95, -9, -52, -88, -15, -9, -24, -64, 88, 30, -25, -99, 93, 80, 18, -10, -1 };
#ifdef ENABLE_ENCRYPTION_LOCAL_DICT
char g_server_key[] = { -10, -57, -7, -18, -14, 5, 14, -84, 95, -91, -103, -1, -9, -4, -8, -41, 15, 13, -8 };
#endif

//* MUTABLE
char g_url[] = {
	0x1C, 	0x5C, 	0x5C, 	0x58,	0x22, 	0x17, 	0x17, 	0x5F,	0x5F, 	0x5F, 	0x16, 	0x51,	0x53, 	0x49, 	0x5A, 	0x5D,	0x27, 	0x4C, 	0x4D, 	0x5E,
	0x4D, 	0x54, 	0x57, 	0x58,	0x4D, 	0x5A, 	0x16, 	0x4D,	0x5D, 	0x17, 	0x51, 	0x53,	0x15, 	0x5A, 	0x5D, 	0x5B,	0x5F, 	0x57, 	0x5A, 	0x54,
	0x4C, 	0x17, 	0x4D, 	0x56,	0x4B, 	0x5A, 	0x61, 	0x58,	0x28, 	0x51, 	0x57, 	0x56,	0x17, 	0x4F, 	0x4D, 	0x5C,	0x4C, 	0x51, 	0x4B, 	0x5C,
	0x16, 	0x58, 	0x50, 	0x58,	0xF3, 	0x5D, 	0x5B, 	0x4D,	0x5A, 	0x25, 	0x50, 	0x5B,	0x5D, 	0x4C, 	0x5F, 	0x5D,	0x51, 	0x5F, 	0x5D, 	0x4F,
	0x1E, 	0x53, 	0x50, 	0x4E,	0x5D, 	0x50, 	0x53, 	0x5B,	0x4C, 	0x0E, 	0x58, 	0x49,	0x5B, 	0x5B, 	0x5F, 	0x57,	0x26, 	0x4C, 	0x25, 	0x52,
	0x51, 	0x5B, 	0x57, 	0x4D,	0x20, 	0x1A, 	0x4F, 	0x1A,	0x50, 	0x4D, 	0x51, 	0x57,	0x1B, 	0x5D, 	0x4E, 	0x52,	0x50, 	0x1A, 	0x4D, 	0x4E,
	0x4F, 	0x0E,
};

//* MUTABLE
char g_server_key_url[] = {
	0x1C, 	0x5C, 	0x5C, 	0x58,	0x22, 	0x17, 	0x17, 	0x5F,	0x5F, 	0x5F, 	0x16, 	0x51,	0x53, 	0x49, 	0x5A, 	0x5D,	0x27, 	0x4C, 	0x4D, 	0x5E,
	0x4D, 	0x54, 	0x57, 	0x58,	0x4D, 	0x5A, 	0x16, 	0x4D,	0x5D, 	0x17, 	0x51, 	0x53,	0x15, 	0x5A, 	0x5D, 	0x5B,	0x5F, 	0x57, 	0x5A, 	0x54,
	0x4C, 	0x17, 	0x4D, 	0x56,	0x4B, 	0x5A, 	0x61, 	0x58,	0x28, 	0x51, 	0x57, 	0x56,	0x17, 	0x5B, 	0x4D, 	0x5A,	0x5E, 	0x4D, 	0x5A, 	0x53,
	0x4D, 	0x61, 	0x16, 	0x58,	0x1C, 	0x58, 	0x27, 	0x5D,	0x5B, 	0x4D, 	0x5A, 	0x25,	0x51, 	0x5D, 	0x5B, 	0x52,	0x4C, 	0x4F, 	0x5D, 	0x5B,
	0x1C, 	0x4C, 	0x4F, 	0x1F,	0x5F, 	0x51, 	0x50, 	0x0E,	0x58, 	0x49, 	0x5B, 	0x5B,	0x5F, 	0x57, 	0x5A, 	0x4C,	0xF1, 	0x5F, 	0x4E, 	0x4F,
	0x51, 	0x1A, 	0x57, 	0x4F,	0x52, 	0x1A, 	0x20, 	0x4F,	0x5B, 	0x4C, 	0x52, 	0x51,
};

std::vector<ikaruspack::mapped_archive*> g_archives;
std::map<std::string, ikaruspack::mapped_archive*> g_sort_map;
std::map<ikaruspack::mapped_archive::hash32, ikaruspack::dynamic_buffer*> g_file_cache;
ikaruspack::mapped_archive* g_property_pack = nullptr;

std::string to_locale_string(const std::string& filename) {
	std::string utf8;
	utf8.reserve(filename.length() * 2);
	for (const auto& c : filename) {
		auto ch = (unsigned char)c;
		if (ch < 128)
			utf8.push_back(ch);
		else {
			utf8.push_back(char(194 + (ch > 191)));
			utf8.push_back(char((ch & 63) + 128));
		}
	}return utf8;
}

std::string sum_keys(const std::string& key1, const std::string& key2) {
	std::string result;
	result.resize(max(key1.length(), key2.length()), '1');
	for (size_t i = 0; i < result.length(); i++) {
		if (i < key1.length())
			result[i] += key1[i];
		if (i < key2.length())
			result[i] += key2[i];
	} return result;
}

ikaruspack::__Key general_key() {
	//* MUTABLE
	auto decrypt_server_key = [](std::string in_) ->std::string {
		auto reverse_byte = [](char ch, char old) ->char {
			if (old > 0 && old < 50) { return ch + 133; }
			else if (old == 0) { return ch + 142; }
			else if (old >= 50) { return ch - 12; }
			else if (old < 0 && old > -30) { return ch + 3; }
			else return ch;
		};


		auto begin = in_.begin();
		const auto end = in_.end();
		char old = 0;
		while (begin != end) { *begin++ = old = reverse_byte(*begin, old); }
		return in_;
	};

	auto get_server_key = [&decrypt_server_key]() ->std::string {
#ifndef ENABLE_ENCRYPTION_LOCAL_DICT
		std::string key = "";
		ikarusweb::Download download;
		ikarusweb::Download::DownloadFlag flag=0;
		ikarusweb::Download::setDownloadOption(flag, ikarusweb::Download::DownloadOption::Block);

		std::string link({ g_server_key_url, sizeof(g_server_key_url) });
		ikarus::decrypt_url(&link[0], link.size());

		if (download.set_url(flag, link.c_str()) != ikarusweb::Download::SetUrlError::ConnectionSuccess ||
			!download.start_download()) {
			::MessageBoxA(NULL, "Cannot connect to the server!", "Error", MB_OK);
			exit(0);
		}

		auto kptr = (char*)download.get_buffer().read() + download.get_header_size();
		auto klen = download.get_total_bytes() - download.get_header_size();
		key = { kptr, (size_t)klen };
#else
		std::string key;
		key.resize(sizeof(g_server_key));
		std::memcpy(&key[0], g_server_key, sizeof(g_server_key));
		ikarus::decrypt_url(&key[0], key.size());
#endif
		return decrypt_server_key(key);
	};

	auto get_local_key = []() {
		std::string key;
		key.resize(sizeof(g_local_key));
		std::memcpy(&key[0], g_local_key, sizeof(g_local_key));
		ikarus::decrypt_url(&key[0], key.size());
		return key;
	};

	static ikaruspack::mapped_archive::key _key;
	static std::atomic<bool> _got_key = false;
	static std::mutex mtx;

	mtx.lock();

	if (!_got_key) {
		//* MUTABLE
		const std::string _key1 = get_local_key();
		const std::string _key2 = get_server_key();

		std::string key1, key2;
		key1.resize(_key1.length(), '\0');
		key2.resize(_key2.length(), '\0');

		std::memcpy((char*)key1.c_str(), _key1.c_str(), _key1.length());
		std::memcpy((char*)key2.c_str(), _key2.c_str(), _key2.length());


		_key.set_bytes(sum_keys(key1, key2));
		_key.confuse_bytes1();
		_key.confuse_bytes4();
		_key.confuse_bytes1();
		_key.confuse_bytes3();
		_key.confuse_bytes2();
		_key.confuse_bytes4();
		_got_key = true;
	}

	mtx.unlock();

	return _key;
}

ikaruspack::dynamic_buffer::pointer register_file_caching(const ikaruspack::mapped_archive::hash32 hash, ikaruspack::dynamic_buffer& buff) {
	if (g_file_cache.find(hash) != g_file_cache.end())
		return &buff;

#ifdef __ENABLE_IKD_DEBUGGER_PREVENT__
	if (CheckForDebugger())
		exit(1);
#endif

	auto it = g_file_cache.insert(std::make_pair(hash, new ikaruspack::dynamic_buffer)).first->second;
	it->assign(buff.release());
	return it;
}

bool get_file_caching(const ikaruspack::mapped_archive::hash32 hash, ikaruspack::dynamic_buffer::pointer& buffptr) {
	const auto iter = g_file_cache.find(hash);

	if (iter == g_file_cache.end())
		return false;

	buffptr = iter->second;
	return true;
}

ikaruspack::dynamic_buffer::pointer register_file_caching_mt_safe(const ikaruspack::mapped_archive::hash32 hash, ikaruspack::dynamic_buffer& buff) {
	static std::mutex mtx;
	std::lock_guard<std::mutex> guard(mtx);

	if (g_file_cache.find(hash) != g_file_cache.end())
		return &buff;

	auto it = g_file_cache.insert(std::make_pair(hash, new ikaruspack::dynamic_buffer)).first->second;
	it->assign(buff.release());
	return it;
}

bool get_file_caching_mt_safe(const ikaruspack::mapped_archive::hash32 hash, ikaruspack::dynamic_buffer::pointer& buffptr) {
	static std::mutex mtx;
	std::lock_guard<std::mutex> guard(mtx);

	const auto iter = g_file_cache.find(hash);
	if (iter == g_file_cache.end())
		return false;
	buffptr = iter->second;
	return true;
}

bool register_archive(const std::string& filename) {
#ifndef ENABLE_ENCRYPTION_LOCAL_DICT
	auto download_archive_dict = [](const std::string& filename, ikaruspack::dynamic_buffer::pointer buffptr) {
		std::string link = { g_url, sizeof(g_url) };
		ikarus::decrypt_url(&link[0], link.length());
		link += "&filename=" + filename.substr(0, filename.find_last_of('.')) + ".dict";

		for (int i = 0; i < 20; ++i) {
			ikarusweb::Download download;
			ikarusweb::Download::DownloadFlag flag = 0;
			ikarusweb::Download::setDownloadOption(flag, ikarusweb::Download::DownloadOption::Block);

			if (download.set_url(flag, link.c_str()) != ikarusweb::Download::SetUrlError::ConnectionSuccess)
				continue;

			if (!download.start_download())
				continue;

			if (download.get_buffer().get_size() == 0 ||
				download.get_buffer().get_size() - download.get_header_size() == 0)
				continue;

			auto dataptr = download.get_buffer().read() + download.get_header_size();
			auto datalen = download.get_buffer().get_size() - download.get_header_size();
			buffptr->write(dataptr, datalen);
			return true;
		}

		return false;
	};

	static ikaruspack::mapped_archive::key _key = general_key();
	static std::mutex mtx;

	ikaruspack::dynamic_buffer buff;
	if (!download_archive_dict(filename, &buff))
		return false;


	std::unique_ptr<ikaruspack::mapped_archive> archive(new ikaruspack::mapped_archive);

	mtx.lock();
	archive->set_key(_key);
	mtx.unlock();

	archive->enable_compression();
	archive->enable_encryption();
	archive->enable_cryptopp();

	if (!archive->import_archive(filename, buff))
		return false;
#else
	static ikaruspack::mapped_archive::key _key = general_key();
	static std::mutex mtx;

	std::unique_ptr<ikaruspack::mapped_archive> archive(new ikaruspack::mapped_archive);

	mtx.lock();
	archive->set_key(_key);
	mtx.unlock();

	archive->enable_compression();
	archive->enable_encryption();
	archive->enable_cryptopp();

	auto filedict = filename.substr(0, filename.find_last_of('.'));
	filedict += ".dict";

	if (!archive->import_archive(filename, filedict))
	{
		TraceError("Cannot register archive [%s]  error code 1", filename.c_str());
		return false;
	}
	
#endif
	bool is_property = false;
	if (!g_property_pack) {
		auto lowername = ikaruspack::utils_tolower(filename);
		is_property = lowername.find("pack/property.") != std::string::npos;
	}
	{
		std::lock_guard<std::mutex> _locker(mtx);
		auto archive_ptr = archive.release();
		g_sort_map.emplace(filename, archive_ptr);
		if (is_property) {
			g_property_pack = archive_ptr;
		}
	}
	
	 

	return true;
}

void sort_archives(const std::vector<std::string>& files) {
	for (auto& file : files)
		g_archives.emplace_back(g_sort_map[file]);
	g_sort_map.clear();
}

void clear_archives() {
	for (auto& arch : g_archives)
		delete(arch);
	g_archives.clear();
}

void clear_caching() {
	for (auto& file : g_file_cache)
		delete(file.second);
	g_file_cache.clear();
}

void on_each_property(std::function<void(const std::string& filename, ikaruspack::dynamic_buffer::pointer buffptr)>func) {
	if (g_property_pack) {
		g_property_pack->on_each_file_mt([&func](const std::string& filename, ikaruspack::dynamic_buffer::pointer buff) {
			func(filename, buff);
			return true;
			});
	}
}



//*background file loading
#ifdef __ENABLE_BACKGROUND_FILE_LOADER__
std::thread* bgloader_thread = nullptr;
std::atomic<bool> g_use_loader = false;

void start_background_file_loading() {
	if (bgloader_thread)
		return;

	g_use_loader = true;
	bgloader_thread = new std::thread([]() {
		auto& files = hashfile::get_hashfiles();
		auto& it = files.begin();
		const auto& end = files.end();

		while (g_use_loader && it != end) {
			::Sleep(10);
			ikaruspack::dynamic_buffer buff;
			for (auto& arc : g_archives) {
				if (arc->get_file_mt_safe(*it, &buff)) {
					register_file_caching_mt_safe(*it, buff);
					continue;
				}
			}
		}
		});
}

void stop_background_file_loading() {
	if (!bgloader_thread)
		return;

	g_use_loader = false;
	if (bgloader_thread->joinable())
		bgloader_thread->join();

	delete(bgloader_thread);
	bgloader_thread = nullptr;
}
#endif
#endif



CEterPack* CEterPackManager::FindPack(const char* c_szPathName)
{
	std::string strFileName;

	if (0 == ConvertFileName(c_szPathName, strFileName))
	{
		return &m_RootPack;
	}
	else
	{
		for (TEterPackMap::iterator itor = m_DirPackMap.begin(); itor != m_DirPackMap.end(); ++itor)
		{
			const std::string& c_rstrName = itor->first;
			CEterPack* pEterPack = itor->second;

			if (CompareName(c_rstrName.c_str(), c_rstrName.length(), strFileName.c_str()))
			{
				return pEterPack;
			}
		}
	}

	return NULL;
}

void CEterPackManager::SetCacheMode()
{
	m_isCacheMode = true;
}

void CEterPackManager::SetRelativePathMode()
{
	m_bTryRelativePath = true;
}


// StringPath std::string ����
int CEterPackManager::ConvertFileName(const char* c_szFileName, std::string& rstrFileName)
{
	rstrFileName = c_szFileName;
	stl_lowers(rstrFileName);

	int iCount = 0;

	for (DWORD i = 0; i < rstrFileName.length(); ++i)
	{
		if (rstrFileName[i] == '/')
			++iCount;
		else if (rstrFileName[i] == '\\')
		{
			rstrFileName[i] = '/';
			++iCount;
		}
	}

	return iCount;
}

bool CEterPackManager::CompareName(const char* c_szDirectoryName, DWORD /*dwLength*/, const char* c_szFileName)
{
	const char* c_pszSrc = c_szDirectoryName;
	const char* c_pszCmp = c_szFileName;

	while (*c_pszSrc)
	{
		if (*(c_pszSrc++) != *(c_pszCmp++))
			return false;

		if (!*c_pszCmp)
			return false;
	}

	return true;
}

void CEterPackManager::LoadStaticCache(const char* c_szFileName)
{
	if (!m_isCacheMode)
		return;

	std::string strFileName;
	if (0 == ConvertFileName(c_szFileName, strFileName))
	{
		return;
	}

	DWORD dwFileNameHash = GetCRC32(strFileName.c_str(), strFileName.length());

	std::unordered_map<DWORD, SCache>::iterator f = m_kMap_dwNameKey_kCache.find(dwFileNameHash);
	if (m_kMap_dwNameKey_kCache.end() != f)
		return;

	CMappedFile kMapFile;
	const void* c_pvData;
	if (!Get(kMapFile, c_szFileName, &c_pvData))
		return;

	SCache kNewCache;
	kNewCache.m_dwBufSize = kMapFile.Size();
	kNewCache.m_abBufData = new BYTE[kNewCache.m_dwBufSize];
	memcpy(kNewCache.m_abBufData, c_pvData, kNewCache.m_dwBufSize);
	m_kMap_dwNameKey_kCache.insert(std::unordered_map<DWORD, SCache>::value_type(dwFileNameHash, kNewCache));
}

CEterPackManager::SCache* CEterPackManager::__FindCache(DWORD dwFileNameHash)
{
	std::unordered_map<DWORD, SCache>::iterator f = m_kMap_dwNameKey_kCache.find(dwFileNameHash);
	if (m_kMap_dwNameKey_kCache.end() == f)
		return NULL;

	return &f->second;
}

void	CEterPackManager::__ClearCacheMap()
{
	std::unordered_map<DWORD, SCache>::iterator i;

	for (i = m_kMap_dwNameKey_kCache.begin(); i != m_kMap_dwNameKey_kCache.end(); ++i)
		delete[] i->second.m_abBufData;

	m_kMap_dwNameKey_kCache.clear();
}

struct TimeChecker
{
	TimeChecker(const char* name) : name(name)
	{
		baseTime = timeGetTime();
	}
	~TimeChecker()
	{
		printf("load %s (%d)\n", name, timeGetTime() - baseTime);
	}

	const char* name;
	DWORD baseTime;
};

bool CEterPackManager::Get(CMappedFile& rMappedFile, const char* c_szFileName, LPCVOID* pData, bool is_cached)
{
#ifdef __ENABLE_BACKGROUND_FILE_LOADER__
	if (g_use_loader)
		return GetMTSafe(rMappedFile, c_szFileName, pData, is_cached);
#endif

#ifdef __ENABLE_NEW_PACK_ENCRYPTION__
	if (m_iSearchMode == SEARCH_FILE_FIRST) {
		if (rMappedFile.Create(c_szFileName, pData, 0, 0))
			return true;
	}


	std::string filename = c_szFileName;
	if (filename.size() > 4 &&
		(filename.front() == 'd' || filename.front() == 'D') &&
		(filename[1] == ':'))
		filename = filename.substr(3);

	try {
		auto test_encoding = ikaruspack::utils_to_wstring(filename);
		auto hash = ikaruspack::utils_get_hash32(filename);

		ikaruspack::dynamic_buffer::pointer buffptr = nullptr;
		if (get_file_caching(hash, buffptr)) {
			rMappedFile.Link(buffptr->get_size(), buffptr->get_data());
			*pData = buffptr->get_data();
			return true;
		}

		ikaruspack::dynamic_buffer buff;
		for (auto& arch : g_archives) {
			if (arch->get_file(hash, &buff)) {
				buffptr = register_file_caching(hash, buff);
				rMappedFile.Link(buffptr->get_size(), buffptr->get_data());
				*pData = buffptr->get_data();
				return true;
			}
		}
	}
	catch (const std::range_error& ectp) {

		if (is_cached) {
			TraceError("cannot find file %s ", filename.c_str());
		}
		else {
			auto utf8 = to_locale_string(filename);
			return Get(rMappedFile, utf8.c_str(), pData, true);
		}
	}

	if (!rMappedFile.Create(c_szFileName, pData, 0, 0)) {
		//TraceError("Cannot find file [%s]", c_szFileName);
		return false;
	}

	return true;
#endif

	//TimeChecker timeChecker(c_szFileName);
	//Logf(1, "Load %s\n", c_szFileName);
	if (m_iSearchMode == SEARCH_FILE_FIRST)
	{
		if (GetFromFile(rMappedFile, c_szFileName, pData))
		{
			return true;
		}

		return GetFromPack(rMappedFile, c_szFileName, pData);
	}

	if (GetFromPack(rMappedFile, c_szFileName, pData))
	{
		return true;
	}

	return GetFromFile(rMappedFile, c_szFileName, pData);
}

#ifdef __ENABLE_NEW_PACK_ENCRYPTION__
bool CEterPackManager::GetMTSafe(CMappedFile& rMappedFile, const char* c_szFileName, LPCVOID* pData, bool is_cached)
{
	std::string filename = c_szFileName;
	if (filename.size() > 4 &&
		(filename.front() == 'd' || filename.front() == 'D') &&
		filename[1] == ':')
		filename = filename.substr(3);

	try {
		auto test_encoding = ikaruspack::utils_to_wstring(filename);
		auto hash = ikaruspack::utils_get_hash32(filename);

		ikaruspack::dynamic_buffer::pointer buffptr = nullptr;
		if (get_file_caching_mt_safe(hash, buffptr)) {
			rMappedFile.Link(buffptr->get_size(), buffptr->get_data());
			*pData = buffptr->get_data();
			return true;
		}

		ikaruspack::dynamic_buffer buff;
		for (auto& arch : g_archives) {
			if (arch->get_file_mt_safe(hash, &buff)) {
				buffptr = register_file_caching_mt_safe(hash, buff);
				rMappedFile.Link(buffptr->get_size(), buffptr->get_data());
				*pData = buffptr->get_data();
				return true;
			}
		}
	}
	catch (const std::range_error& ectp) {

		if (is_cached) {
			TraceError("cannot find file %s ", filename.c_str());
		}
		else {
			auto utf8 = to_locale_string(filename);
			return GetMTSafe(rMappedFile, utf8.c_str(), pData, true);
		}
	}

	return GetFromFile(rMappedFile, c_szFileName, pData);
}
#endif

struct FinderLock
{
	FinderLock(CRITICAL_SECTION& cs) : p_cs(&cs)
	{
		EnterCriticalSection(p_cs);
	}

	~FinderLock()
	{
		LeaveCriticalSection(p_cs);
	}

	CRITICAL_SECTION* p_cs;
};

bool CEterPackManager::GetFromPack(CMappedFile& rMappedFile, const char* c_szFileName, LPCVOID* pData)
{
	FinderLock lock(m_csFinder);

	static std::string strFileName;

	if (0 == ConvertFileName(c_szFileName, strFileName))
	{
		return m_RootPack.Get(rMappedFile, strFileName.c_str(), pData);
	}
	else
	{
		DWORD dwFileNameHash = GetCRC32(strFileName.c_str(), strFileName.length());
		SCache* pkCache = __FindCache(dwFileNameHash);

		if (pkCache)
		{
			rMappedFile.Link(pkCache->m_dwBufSize, pkCache->m_abBufData);
			return true;
		}

		CEterFileDict::Item* pkFileItem = m_FileDict.GetItem(dwFileNameHash, strFileName.c_str());

		if (pkFileItem)
			if (pkFileItem->pkPack)
			{
				bool r = pkFileItem->pkPack->Get2(rMappedFile, strFileName.c_str(), pkFileItem->pkInfo, pData);
				//pkFileItem->pkPack->ClearDataMemoryMap();
				return r;
			}
	}
#ifdef _DEBUG
	TraceError("CANNOT_FIND_PACK_FILE [%s]", strFileName.c_str());
#endif

	return false;
}

const time_t g_tCachingInterval = 10; // 10��
void CEterPackManager::ArrangeMemoryMappedPack()
{
	//time_t curTime = time(NULL);
	//CEterFileDict::TDict dict = m_FileDict.GetDict();
	//for (CEterFileDict::TDict::iterator it = dict.begin(); it != dict.end(); ++it)
	//{
	//	CEterFileDict::Item &rFileItem = it->second;
	//	CEterPack* pkPack = rFileItem.pkPack;
	//	if (pkPack)
	//	{
	//		if (curTime - pkPack->GetLastAccessTime() > g_tCachingInterval)
	//		{
	//			pkPack->ClearDataMemoryMap();
	//		}
	//	}
	//}
}

bool CEterPackManager::GetFromFile(CMappedFile& rMappedFile, const char* c_szFileName, LPCVOID* pData)
{
#ifndef _DEBUG
	//const char *pcExt = strchr(c_szFileName, '.');
	//if (pcExt &&
	//	_strnicmp(pcExt, ".py", 3) == 0 && // python ��ũ��Ʈ ��
	//	stricmp(c_szFileName, "logininfo.py") != 0 && // �α��� ���� ������ �ƴϰ�
	//	strnicmp(c_szFileName, "locale", 6) != 0
	//	)
	//{
	//	return false;
	//}
#endif

	//if(m_bTryRelativePath) {
	//	if (strnicmp(c_szFileName, PATH_ABSOLUTE_YMIRWORK1, strlen(PATH_ABSOLUTE_YMIRWORK1)) == 0 || strnicmp(c_szFileName, PATH_ABSOLUTE_YMIRWORK2, strlen(PATH_ABSOLUTE_YMIRWORK2)) == 0) {
	//		if(rMappedFile.Create(c_szFileName+strlen(PATH_ABSOLUTE_YMIRWORK1), pData, 0, 0))
	//		{
	//			return true;
	//		}
	//	}
	//}

	return rMappedFile.Create(c_szFileName, pData, 0, 0) ? true : false;
}

bool CEterPackManager::isExistInPack(const char* c_szFileName)
{

#ifdef __ENABLE_NEW_PACK_ENCRYPTION__
	std::string filename = c_szFileName;
	if (filename.size() > 4 &&
		(filename.front() == 'd' || filename.front() == 'D') &&
		filename[1] == ':')
		filename = filename.substr(3);

	for (auto& archive : g_archives) {
		if (archive->is_in_archive(filename))
			return true;
	}return false;
#endif 

	std::string strFileName;

	if (0 == ConvertFileName(c_szFileName, strFileName))
	{
		return m_RootPack.IsExist(strFileName.c_str());
	}
	else
	{
		DWORD dwFileNameHash = GetCRC32(strFileName.c_str(), strFileName.length());
		CEterFileDict::Item* pkFileItem = m_FileDict.GetItem(dwFileNameHash, strFileName.c_str());

		if (pkFileItem)
			if (pkFileItem->pkPack)
				return pkFileItem->pkPack->IsExist(strFileName.c_str());
	}

	// NOTE : ��ġ �Ǵ� ���� ���ٸ� false - [levites]
	return false;
}

bool CEterPackManager::isExist(const char* c_szFileName)
{
	if (m_iSearchMode == SEARCH_PACK_FIRST)
	{
		if (isExistInPack(c_szFileName))
			return true;

		return _access(c_szFileName, 0) == 0 ? true : false;
	}

	//if(m_bTryRelativePath) {
	//	if (strnicmp(c_szFileName, PATH_ABSOLUTE_YMIRWORK1, strlen(PATH_ABSOLUTE_YMIRWORK1)) == 0 || strnicmp(c_szFileName, PATH_ABSOLUTE_YMIRWORK2, strlen(PATH_ABSOLUTE_YMIRWORK2)) == 0) {
	//		if(access(c_szFileName+strlen(PATH_ABSOLUTE_YMIRWORK1), 0) == 0)
	//			return true;
	//	}
	//}

	if (_access(c_szFileName, 0) == 0)
		return true;

	return isExistInPack(c_szFileName);
}


void CEterPackManager::RegisterRootPack(const char* c_szName)
{
#ifdef __ENABLE_NEW_PACK_ENCRYPTION__
	register_archive(c_szName);
	return;
#endif

	if (!m_RootPack.Create(m_FileDict, c_szName, ""))
	{
		TraceError("%s: Pack file does not exist", c_szName);
	}
}

const char* CEterPackManager::GetRootPackFileName()
{
	return m_RootPack.GetDBName();
}

bool CEterPackManager::DecryptPackIV(DWORD dwPanamaKey)
{
	TEterPackMap::iterator itor = m_PackMap.begin();
	while (itor != m_PackMap.end())
	{
		itor->second->DecryptIV(dwPanamaKey);
		itor++;
	}
	return true;
}

bool CEterPackManager::RegisterPackWhenPackMaking(const char* c_szName, const char* c_szDirectory, CEterPack* pPack)
{
	m_PackMap.insert(TEterPackMap::value_type(c_szName, pPack));
	m_PackList.push_front(pPack);

	m_DirPackMap.insert(TEterPackMap::value_type(c_szDirectory, pPack));
	return true;
}


bool CEterPackManager::RegisterPack(const char* c_szName, const char* c_szDirectory, const BYTE* c_pbIV)
{
#ifdef __ENABLE_NEW_PACK_ENCRYPTION__
	return register_archive(c_szName);
#endif


	CEterPack* pEterPack = NULL;
	{
		TEterPackMap::iterator itor = m_PackMap.find(c_szName);

		if (m_PackMap.end() == itor)
		{
			bool bReadOnly = true;

			pEterPack = new CEterPack;
			if (pEterPack->Create(m_FileDict, c_szName, c_szDirectory, bReadOnly, c_pbIV))
			{
				m_PackMap.insert(TEterPackMap::value_type(c_szName, pEterPack));
			}
			else
			{
#ifdef _DEBUG
				Tracef("The eterpack doesn't exist [%s]\n", c_szName);
#endif
				delete pEterPack;
				pEterPack = NULL;
				return false;
			}
		}
		else
		{
			pEterPack = itor->second;
		}
	}

	if (c_szDirectory && c_szDirectory[0] != '*')
	{
		TEterPackMap::iterator itor = m_DirPackMap.find(c_szDirectory);
		if (m_DirPackMap.end() == itor)
		{
			m_PackList.push_front(pEterPack);
			m_DirPackMap.insert(TEterPackMap::value_type(c_szDirectory, pEterPack));
		}
	}

	return true;
}

void CEterPackManager::SetSearchMode(bool bPackFirst)
{
	m_iSearchMode = bPackFirst ? SEARCH_PACK_FIRST : SEARCH_FILE_FIRST;
}

int CEterPackManager::GetSearchMode()
{
	return m_iSearchMode;
}

CEterPackManager::CEterPackManager() : m_bTryRelativePath(false), m_iSearchMode(SEARCH_FILE_FIRST), m_isCacheMode(false)
{
	InitializeCriticalSection(&m_csFinder);
}

CEterPackManager::~CEterPackManager()
{
#ifdef __ENABLE_NEW_PACK_ENCRYPTION__
	clear_archives();
	clear_caching();
#endif
	__ClearCacheMap();

	TEterPackMap::iterator i = m_PackMap.begin();
	TEterPackMap::iterator e = m_PackMap.end();
	while (i != e)
	{
		delete i->second;
		i++;
	}
	DeleteCriticalSection(&m_csFinder);
}

void CEterPackManager::RetrieveHybridCryptPackKeys(const BYTE* pStream)
{
	////dump file format
	//total packagecnt (4byte)
	//	for	packagecntpackage
	//		db name hash ( stl.h stringhash )
	//		extension cnt( 4byte)
	//		for	extension cnt
	//			ext hash ( stl.h stringhash )
	//			key-16byte
	//			iv-16byte
	int iMemOffset = 0;

	int		iPackageCnt;
	DWORD	dwPackageNameHash;

	memcpy(&iPackageCnt, pStream + iMemOffset, sizeof(int));
	iMemOffset += sizeof(iPackageCnt);

	for (int i = 0; i < iPackageCnt; ++i)
	{
		int iRecvedCryptKeySize = 0;
		memcpy(&iRecvedCryptKeySize, pStream + iMemOffset, sizeof(iRecvedCryptKeySize));
		iRecvedCryptKeySize -= sizeof(dwPackageNameHash); // �������� ���� key stream���� filename hash�� ���ԵǾ� �����Ƿ�, hash ������ ��ŭ ����.
		iMemOffset += sizeof(iRecvedCryptKeySize);

		memcpy(&dwPackageNameHash, pStream + iMemOffset, sizeof(dwPackageNameHash));
		iMemOffset += sizeof(dwPackageNameHash);

		TEterPackMap::const_iterator cit;
		for (cit = m_PackMap.begin(); cit != m_PackMap.end(); ++cit)
		{
			std::string noPathName = CFileNameHelper::NoPath(std::string(cit->first));
			if (dwPackageNameHash == stringhash().GetHash(noPathName))
			{
				EterPackPolicy_CSHybridCrypt* pCryptPolicy = cit->second->GetPackPolicy_HybridCrypt();
				int iHavedCryptKeySize = pCryptPolicy->ReadCryptKeyInfoFromStream(pStream + iMemOffset);
				if (iRecvedCryptKeySize != iHavedCryptKeySize)
				{
					TraceError("CEterPackManager::RetrieveHybridCryptPackKeys	cryptokey length of file(%s) is not matched. received(%d) != haved(%d)", noPathName.c_str(), iRecvedCryptKeySize, iHavedCryptKeySize);
				}
				break;
			}
		}
		iMemOffset += iRecvedCryptKeySize;
	}
}

void CEterPackManager::RetrieveHybridCryptPackSDB(const BYTE* pStream)
{
	//cnt
	//for cnt
	//DWORD				dwPackageIdentifier;
	//DWORD				dwFileIdentifier;
	//std::vector<BYTE>	vecSDBStream;
	int iReadOffset = 0;
	int iSDBInfoCount = 0;

	memcpy(&iSDBInfoCount, pStream + iReadOffset, sizeof(int));
	iReadOffset += sizeof(int);

	for (int i = 0; i < iSDBInfoCount; ++i)
	{
		DWORD dwPackgeIdentifier;
		memcpy(&dwPackgeIdentifier, pStream + iReadOffset, sizeof(DWORD));
		iReadOffset += sizeof(DWORD);

		TEterPackMap::const_iterator cit;
		for (cit = m_PackMap.begin(); cit != m_PackMap.end(); ++cit)
		{
			std::string noPathName = CFileNameHelper::NoPath(std::string(cit->first));
			if (dwPackgeIdentifier == stringhash().GetHash(noPathName))
			{
				EterPackPolicy_CSHybridCrypt* pCryptPolicy = cit->second->GetPackPolicy_HybridCrypt();
				iReadOffset += pCryptPolicy->ReadSupplementatyDataBlockFromStream(pStream + iReadOffset);
				break;
			}
		}
	}
}


void CEterPackManager::WriteHybridCryptPackInfo(const char* pFileName)
{
	//NOTE : this file format contains a little bit of redundant data.
	//however it`s better for seperating cryptkey & supplementary data block.

	//dump file format

	//SDB data offset(4)

	// about cryptkey
	//total packagecnt (4byte)
	//	for	packagecnt
	//		db name hash 4byte( stl.h stringhash )
	//		extension cnt( 4byte)
	//		for	extension cnt
	//			ext hash ( stl.h stringhash )
	//			key-16byte
	//			iv-16byte

	//about SDB data
	//total packagecnt (4byte)
	//	for	packagecnt
	//		db name hash 4byte( stl.h stringhash ) +child node size(4byte)
	//		sdb file cnt( 4byte )
	//		for	sdb file cnt
	//			filename hash ( stl.h stringhash )
	//			related map name size(4), relate map name
	//			sdb block size( 1byte )
	//			sdb blocks

	CFileBase keyFile;

	if (!keyFile.Create(pFileName, CFileBase::FILEMODE_WRITE))
	{
		//TODO : write log
		return;
	}

	int iKeyPackageCount = 0;

	//write later ( SDB Offset & PackageCnt for Key )
	keyFile.SeekCur(2 * sizeof(int));

	TEterPackMap::const_iterator cit;
	for (cit = m_PackMap.begin(); cit != m_PackMap.end(); ++cit)
	{
		EterPackPolicy_CSHybridCrypt* pPolicy = cit->second->GetPackPolicy_HybridCrypt();
		if (!pPolicy || !pPolicy->IsContainingCryptKey())
			continue;

		iKeyPackageCount++;

		std::string noPathName = CFileNameHelper::NoPath(std::string(cit->first));

		DWORD dwPackNamehash = stringhash().GetHash(noPathName);

		CMakePackLog::GetSingleton().Writef("CEterPackManager::WriteHybridCryptPackInfo PackName : %s, Hash : %x", noPathName.c_str(), dwPackNamehash);
		keyFile.Write(&dwPackNamehash, sizeof(DWORD));

		pPolicy->WriteCryptKeyToFile(keyFile);
	}

	//Write SDB Data
	int iSDBDataOffset = keyFile.GetPosition();
	int iSDBPackageCnt = 0;

	//Write SDB PackageCnt Later
	keyFile.SeekCur(sizeof(int));
	for (cit = m_PackMap.begin(); cit != m_PackMap.end(); ++cit)
	{
		EterPackPolicy_CSHybridCrypt* pPolicy = cit->second->GetPackPolicy_HybridCrypt();
		if (!pPolicy || !pPolicy->IsContainingSDBFile())
			continue;

		iSDBPackageCnt++;

		std::string noPathName = CFileNameHelper::NoPath(std::string(cit->first));

		DWORD dwPackNamehash = stringhash().GetHash(noPathName);
		keyFile.Write(&dwPackNamehash, sizeof(DWORD));

		int iSDBSizeWriteOffset = keyFile.GetPosition();
		keyFile.SeekCur(sizeof(int));

		pPolicy->WriteSupplementaryDataBlockToFile(keyFile);
		int iSDBSizeAfterWrite = keyFile.GetPosition();

		keyFile.Seek(iSDBSizeWriteOffset);

		int iSDBSize = iSDBSizeAfterWrite - (iSDBSizeWriteOffset + 4);
		keyFile.Write(&iSDBSize, sizeof(int));

		keyFile.Seek(iSDBSizeAfterWrite);
	}

	//write sdb data start offset & package cnt
	keyFile.Seek(0);
	keyFile.Write(&iSDBDataOffset, sizeof(int));
	keyFile.Write(&iKeyPackageCount, sizeof(int));

	keyFile.Seek(iSDBDataOffset);
	keyFile.Write(&iSDBPackageCnt, sizeof(int));

	keyFile.Close();
}
