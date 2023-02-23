#include "StdAfx.h"
#include "PythonApplication.h"
#include "ProcessScanner.h"
#include "PythonExceptionSender.h"
#include "resource.h"
#include "Version.h"
#ifdef ENABLE_CHECK_FILES_MD5
#include "md5.h"
#include <windows.h>
#endif
#ifdef __ENABLE_NEW_PACK_ENCRYPTION__
#include <ikaruspack/ikarus_pack_link.h>
#include <ikarusgl/ikarus_gl_link.h>
#include <ikarusweb2/IkarusWeb2Link.h>
#include <ppl.h>
#endif
//#include "StealthDataArea.h"
//#define STEALTH_AREA_SIZE 0x30000   
//STEALTH_DATA_AREA(STEALTH_AREA_SIZE);

/*
#ifndef VIRTUALIZER_DOLPHIN_WHITE_START
#define VIRTUALIZER_DOLPHIN_WHITE_START \
  __asm _emit 0xEB \
  __asm _emit 0x10 \
  __asm _emit 0x43\
  __asm _emit 0x56\
  __asm _emit 0x20 \
  __asm _emit 0x20 \
  __asm _emit 0x86 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x43\
  __asm _emit 0x56\
  __asm _emit 0x20 \
  __asm _emit 0x20 
#endif
*/
/*
#ifndef VIRTUALIZER_DOLPHIN_WHITE_END
#define VIRTUALIZER_DOLPHIN_WHITE_END \
  __asm _emit 0xEB \
  __asm _emit 0x10 \
  __asm _emit 0x43\
  __asm _emit 0x56\
  __asm _emit 0x20 \
  __asm _emit 0x20 \
  __asm _emit 0x16 \
  __asm _emit 0x02 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x43\
  __asm _emit 0x56\
  __asm _emit 0x20 \
  __asm _emit 0x20 
#endif
*/
#ifdef _DEBUG
#include <crtdbg.h>
#endif

#include "../eterPack/EterPackManager.h"
#include "../eterLib/Util.h"
#ifdef CEF_BROWSER
#include "CefWebBrowser.h"
#else
#include "../CWebBrowser/CWebBrowser.h"
#endif
#include "../eterBase/CPostIt.h"

#include "CheckLatestFiles.h"

#include "Hackshield.h"
#include "NProtectGameGuard.h"
#include "WiseLogicXTrap.h"

//#define HACKTRAP_FUNC_MARKER_START VIRTUALIZER_DOLPHIN_WHITE_START
//#define HACKTRAP_FUNC_MARKER_END VIRTUALIZER_DOLPHIN_WHITE_END
//#define HACKTRAP_PRECALL [](){ CefWebBrowser_Startup(GetModuleHandleW(nullptr)); }()
//#define HACKTRAP_LICENSE "dsaaaaa2aawaacaacsab2aanaaiaaaaaa6aaaaaaaafaagaaaaaaaaaaaeasaaiadsaaaaenanaaafsaaaaaaaaaaaad5d5aaaaaaaaaaaaaa5idaaagq25cnftyu2vvnj5yi2mxpp5yq4vdnt3hc3djnjsyk2mvppsyc3dvpfszgpdbnj3zg4mbqp3yc75dnfuyc65cqtzgq35dhfvgspm2npxvu6t3gs5hi6v4qjxdinv4pf6zgpvjhfwzi6vkni3hq6tugx2gk2tvhbxgs6dkqe5viqd3npzhcpvsns6denvtra6ys5mfpbxdnpv4qftgn2txnjyyu8vunfwdk3mfrftzk6mgqj2ze8twgptyy3vynjthk25kpp5ygqdegt4yg4vkgi6diqmmppsze8twgtxzi33zq24yqpb2q3vgu6t2pa5vn5vvrb3gw35bgp5vk5v2n3wzn6dipxuhc63upi4gq65tq7tys3mcrfwgi4v3gi6dkp52gw3hu45hqtyzs6myg3uds43zq32yg3m2hfudq2vhhb5g67bxns4yyqbwqf5gupd3pfwhe5tznjxvq4vwq3tdu6vbrf2zq2j2qn3zs2mbppwdn8dsnx5gs5vpn33zq632pe4den5che5gk8b2n24vin5xqptzq33ynp4ze2mkne5ye2vsqbszq3dinxvyy8mzqxxdg5vjnjvgk7b3g7uycqmcg66zc7djqt3di7dqpftvg5m3qpyzq3txpb5dk3tyqxxgqqdsnn6hk3dsntugc6myg26vn4d3gp3hq2vshf5y48vtn23y65mpq24zu3t3gi6ve5t2ni5di8mvhbuvnqdmn66yw7vjnpsyk4mzhb6hc7dzgp6zg8dwpx3y48vhpp2zi45mp3xduqbypptzentunj5hcpmdqtugu6myn24hn5mbpjwgc3jupfxvk25hn36dg232rj6zuqdcnx3zq23xpb6gi8bvp35gu8bwnxwyn7dprfxyq7bwnpwgin5bgxuzw432nxwgk6dghfvycn3znfvg6n5gqf5hi654pa6yuqmgg63gu2vzgj5hw8m2ntvye5twrj2vq3tvq3wvu6jugttzg63zrb4gw6mgqn4ge4dhg7yzwptugt4zaqjvpf2vg6muqb2gip5wntxhspv4ne4ykpbunxxve4mwqtsyipv4hf5ynqdtpjtgk5vggx7gg3vxqa3ve8mep33yi65tqe6gipvjqfxvs7vcn32ye2vbq74gc3t2npvhk3mxpj5gq5vgpjtvi5jynj4dq2t3np3hc3mgpx2zi63xnfwha4bvre6ha5vdhb4y63dsg36dk7dhha4ze3jyre6viqjunfuyqnvxqn5zc2jypp5zn6mvgi4gu2vkrjwhq8vgq3ugg5vwn3yzenv3g3szc33wntvzs5v3qs4zg6vqpa5g46den3uy44jynx4vu7dgn3wgwn5qqt2genvipi4ye8m4qw6dq8mkn76y66jwnt2dg4mtpfxye4iaaaaaaaaaaaaaaaaau8dewmhngtvu33fvezmbhmpt3d9542z3n7bymxjjn94jygsmvjraaf9pnpsq9ej5dmj5bnbu3d9gjshked26c7ax62xs55bq84j8q75w7fincyfigx5tz7fvzbhdjmby8jnes8zc6pu29tb4vukc8fxpz9zq4w67qgxbsfuh4bdtyvek5qus543z359swve84mefbyntdj4p6kc27zygv848764c63w3evdtjvukwzzdvdtfxyaz72rc7k35uzpkutf3dnkgidsa5ewiiybpbkfg6qjg2k3crfzdwqji26pjfjf58trnnq24zbwicn98i9zvgdpcg28has82w6icuxq44czd2yz3jeik9x35wr5bqndq29fxystk66pp5fxwykff797ybfwre2fands3juiyui"

//#include "HTDefaultImplementation.h"

extern "C"
{
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

extern "C" {
extern int _fltused;
volatile int _AVOID_FLOATING_POINT_LIBRARY_BUG = _fltused;
};

extern "C" { FILE __iob_func[3] = { *stdin,*stdout,*stderr }; }

#pragma comment(linker, "/NODEFAULTLIB:libci.lib")

#pragma comment( lib, "version.lib" )

//#ifdef _DEBUG
//#pragma comment( lib, "python2_d.lib" )
//#else
#pragma comment( lib, "python2.lib" )
//#endif
#pragma comment( lib, "imagehlp.lib" )
#pragma comment( lib, "devil.lib" )

#pragma comment( lib, "granny2.lib" )
#pragma comment( lib, "mss32.lib" )
#pragma comment( lib, "winmm.lib" )
#pragma comment( lib, "imm32.lib" )
#pragma comment( lib, "oldnames.lib" )
#pragma comment( lib, "SpeedTreeRT.lib" )
#pragma comment( lib, "dinput8.lib" )
#pragma comment( lib, "dxguid.lib" )
#pragma comment( lib, "ws2_32.lib" )
#pragma comment( lib, "strmiids.lib" )
#pragma comment( lib, "ddraw.lib" )
#pragma comment( lib, "dmoguids.lib" )
#pragma comment( lib, "lz4.lib" )
#pragma comment( lib, "xxhash.lib" )
//#pragma comment( lib, "CCC.lib" )
//#pragma comment( lib, "wsock32.lib" )
#include <stdlib.h>
#include <cryptopp/cryptoppLibLink.h>
bool __IS_TEST_SERVER_MODE__=false;

// #define __USE_CYTHON__
#ifdef __USE_CYTHON__
// don't include these two files .h .cpp if you're implementing cython via .pyd
// #include "PythonrootlibManager.h"
// it would be better including such file in the project, but this is easier at this moment:
// #include "PythonrootlibManager.cpp"
#endif

// #define __USE_EXTRA_CYTHON__
#ifdef __USE_EXTRA_CYTHON__
// don't include these two files .h .cpp if you're implementing cython via .pyd
// #include "PythonuiscriptlibManager.h"
// it would be better including such file in the project, but this is easier at this moment:
// #include "PythonuiscriptlibManager.cpp"
#endif

#define ENABLE_PYLIB_CHECK
#define ENABLE_MILES_CHECK
// #define ENABLE_DAEMONPROTECTION

#ifdef ENABLE_DAEMONPROTECTION
#define DPDLL_FILENAME	"dpdll.dll"
#define DPDLL_CRC32		0x48104810
#define DPDLL_FILESIZE	1234567+4
#endif
#if defined(ENABLE_CSHIELD) && !defined(_DEBUG)
#pragma comment( lib, "CShield.lib" )
#include "CShield.h"
#include "CShieldCheckIntegrity.h"
CShieldData myCShieldData;
#endif

extern bool SetDefaultCodePage(DWORD codePage);

#ifdef USE_OPENID
extern int openid_test;
#endif

static const char * sc_apszPythonLibraryFilenames[] =
{
	"UserDict.pyc",
	"__future__.pyc",
	"copy_reg.pyc",
	"linecache.pyc",
	"ntpath.pyc",
	"os.pyc",
	"site.pyc",
	"stat.pyc",
	"string.pyc",
	"traceback.pyc",
	"types.pyc",
	"\n",
};

#ifdef ENABLE_CHECK_FILES_MD5
#define MAX_ROWS_MD5 3
static char* ar_szMD5FileNames[MAX_ROWS_MD5][2] =
{
	{ "lib/__future__.pyc", "1dac743debf65b67931cf6b230546d44" },
	{ "lib/os.pyc", "3b1c276267d7d3cfd20cc9c81a328fb2" },
	{ "python27.dll",	"709944c16570e13b705d167f26548e6e" }
};
#endif


#ifdef ENABLE_PYLIB_CHECK
#define PRINT_LEVEL 0
#define PRINTME(level, ...) if(PRINT_LEVEL>=level) TraceError(__VA_ARGS__);
#define PYFOLD "./lib"
// #define PYFORCE

typedef struct PyLibFiles_s
{
	std::string sFileName;
	size_t stSize;
	DWORD dwCRC32;
} PyLibFiles_t;

PyLibFiles_t PyLibFilesTable[] =
{
#if PY_VERSION_HEX==0x020706f0
	{ PYFOLD"/abc.pyc", 6443, 1016035620 },
	{ PYFOLD"/bisect.pyc", 3492, 1989641187 },
	{ PYFOLD"/codecs.pyc", 36978, 2928014693},
	{ PYFOLD"/collections.pyc", 26172, 385366131},
	{ PYFOLD"/copy.pyc", 13208, 1091298715},
	{ PYFOLD"/copy_reg.pyc", 5157, 536292604},
	{ PYFOLD"/encodings/aliases.pyc", 8803, 3888310600},
	{ PYFOLD"/encodings/cp949.pyc", 2009, 1824094431},
	{ PYFOLD"/encodings/__init__.pyc", 4510, 2926961588},
	{ PYFOLD"/fnmatch.pyc", 3732, 4270526278},
	{ PYFOLD"/functools.pyc", 6193, 3257285433},
	{ PYFOLD"/genericpath.pyc", 3303, 1652596334},
	{ PYFOLD"/heapq.pyc", 13896, 2948659214},
	{ PYFOLD"/keyword.pyc", 2169, 2178546341},
	{ PYFOLD"/linecache.pyc", 3235, 4048207604},
	{ PYFOLD"/locale.pyc", 49841, 4114662314},
	{ PYFOLD"/ntpath.pyc", 11961, 2765879465},
	{ PYFOLD"/os.pyc", 25769, 911432770},
	{ PYFOLD"/pyexpat.pyd", 127488, 2778492911},
	{ PYFOLD"/pyexpat_d.pyd", 194560, 2589182738},
	{ PYFOLD"/re.pyc", 13178, 1671609387},
	{ PYFOLD"/shutil.pyc", 19273, 1873281015},
	{ PYFOLD"/site.pyc", 20019, 3897044925},
	{ PYFOLD"/sre_compile.pyc", 11107, 1620746411},
	{ PYFOLD"/sre_constants.pyc", 6108, 3900811275},
	{ PYFOLD"/sre_parse.pyc", 19244, 1459430047},
	{ PYFOLD"/stat.pyc", 2791, 1375966108},
	{ PYFOLD"/string.pyc", 19656, 1066063587},
	{ PYFOLD"/sysconfig.pyc", 17571, 1529083148},
	{ PYFOLD"/traceback.pyc", 11703, 3768933732},
	{ PYFOLD"/types.pyc", 2530, 920695307},
	{ PYFOLD"/UserDict.pyc", 9000, 1431875928},
	{ PYFOLD"/warnings.pyc", 13232, 3752454002},
	{ PYFOLD"/weakref.pyc", 16037, 2124701469},
	{ PYFOLD"/xml/dom/domreg.pyc", 3506, 2127674645},
	{ PYFOLD"/xml/dom/expatbuilder.pyc", 36698, 316034696},
	{ PYFOLD"/xml/dom/minicompat.pyc", 4144, 747596376},
	{ PYFOLD"/xml/dom/minidom.pyc", 74704, 1543233763},
	{ PYFOLD"/xml/dom/nodefilter.pyc", 1243, 3805409468},
	{ PYFOLD"/xml/dom/xmlbuilder.pyc", 18659, 4118801318},
	{ PYFOLD"/xml/dom/__init__.pyc", 7337, 343751384},
	{ PYFOLD"/xml/parsers/expat.pyc", 326, 2425747752},
	{ PYFOLD"/xml/parsers/__init__.pyc", 353, 1691127318},
	{ PYFOLD"/xml/__init__.pyc", 1117, 3531597556},
	{ PYFOLD"/_abcoll.pyc", 22339, 2365844594},
	{ PYFOLD"/_locale.pyc", 49841, 4114662314},
	{ PYFOLD"/_weakrefset.pyc", 10490, 1576811346},
	{ PYFOLD"/__future__.pyc", 4431, 2857792867},
#elif PY_VERSION_HEX==0x020203f0
#else
#error "unknown python version"
#endif
};

bool checkPyLibDir(const std::string szDirName)
{
	bool HasHack = false;

	char szDirNamePath[MAX_PATH];
	sprintf(szDirNamePath, "%s\\*", szDirName.c_str());

	WIN32_FIND_DATA f;
	HANDLE h = FindFirstFile(szDirNamePath, &f);

	if (h == INVALID_HANDLE_VALUE) { return HasHack; }

	do
	{
		if (HasHack)
			break;
		const char * name = f.cFileName;

		if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0) { continue; }

		if (f.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
		{
			char filePath[MAX_PATH];
			sprintf(filePath, "%s%s%s", szDirName.c_str(), "\\", name);
			PRINTME(1, "sub %s", filePath);
            checkPyLibDir(filePath);
		}
		else
		{
			// start processing file
			PRINTME(1, "starting %s", name);
			std::string sName(name);
			std::string sPathName(szDirName+"/"+name);
			// change \\ to /
			std::replace(sPathName.begin(), sPathName.end(), '\\', '/');
			PRINTME(1, "path %s", sPathName.c_str());
			// lower file name
			std::transform(sName.begin(), sName.end(), sName.begin(), ::tolower);
			{
				PRINTME(1, "verify %s", sName.c_str());
				bool isPyLibFound = true;//delete if change back the changes (default is false value)
				//bool isPyLibFound = false;
				//for (PyLibFiles_t *i1=std::begin(PyLibFilesTable), *e1=std::end(PyLibFilesTable); i1<e1; i1++)
				//{
				//	if (!i1->sFileName.compare(sPathName))
				//	{
				//		PRINTME(1, "found %s==%s", i1->sFileName.c_str(), sName.c_str());
				//		DWORD dwCrc32 = GetFileCRC32(sPathName.c_str());
				//		// assert(dwCrc32);
				//		DWORD dwFileSize = f.nFileSizeLow;
				//		if (i1->stSize!=dwFileSize)
				//		{
				//			PRINTME(1, "wrong size %u==%u", i1->stSize, dwFileSize);
				//			HasHack = true;
				//			PRINTME(0, "wrong size %u for %s", dwFileSize, sPathName.c_str());
				//			return HasHack;
				//		}
				//		else if (i1->dwCRC32 != dwCrc32)
				//		{
				//			PRINTME(1, "wrong crc32 %u==%u", i1->dwCRC32, dwCrc32);
				//			HasHack = true;
				//			PRINTME(0, "wrong crc32 %u for %s", dwCrc32, sPathName.c_str());
				//			return HasHack;
				//		}
				//		PRINTME(1, "right size %u==%u", i1->stSize, dwFileSize);
				//		PRINTME(1, "right crc32 %u==%u", i1->dwCRC32, dwCrc32);
				//		PRINTME(2, "{ \"%s\", %u, %u},", sPathName.c_str(), dwFileSize, dwCrc32);
				//		isPyLibFound = true;
				//		break;
				//	}
				//}
				// block ambiguous pyc/d files
				if (!isPyLibFound)
				{
					PRINTME(1, "not found %s", sName.c_str());
#ifdef PYFORCE
					HasHack = true;
					PRINTME(0, "ambiguous file for %s", sPathName.c_str());
					return HasHack;
#endif
				}
				PRINTME(1, "skipping file(%s) hack(%u) found(%u)", sName.c_str(), HasHack, isPyLibFound);
			}
		}

	} while (FindNextFile(h, &f));
	FindClose(h);
	return HasHack;
}

bool __CheckPyLibFiles()
{
	PRINTME(1, "__CheckPyLibFiles processing "PYFOLD);
	if (checkPyLibDir(PYFOLD))
		return false;
	return true;
}
#endif

#ifdef ENABLE_MILES_CHECK
#include <algorithm>
#include "../EterBase/Filename.h"
// #include "../EterBase/CRC32.h"
#define PRINT_LEVEL 0
#define PRINTME(level, ...) if(PRINT_LEVEL>=level) TraceError(__VA_ARGS__);

typedef struct MilesFiles_s
{
	std::string sFileName;
	size_t stSize;
	DWORD dwCRC32;
} MilesFiles_t;

typedef struct MilesExten_s
{
	std::string ExtName;
	bool IsUni;
} MilesExten_t;

MilesExten_t MilesExtenTable[] = {
	{"dll", false},
	{"asi", true},
	{"flt", true},
	{"m3d", true},
	{"mix", true},
};

MilesFiles_t MilesFilesTable[] =
{
	{"mss32.dll", 349696, 1817711331},
	{"mssa3d.m3d", 83456, 1812642892},
	{"mssds3d.m3d", 70656, 2704596484},
	{"mssdsp.flt", 93696, 3364819387},
	{"mssdx7.m3d", 80896, 236402185},
	{"msseax.m3d", 103424, 3195814903},
	{"mssmp3.asi", 125952, 1219814613},
	{"mssrsx.m3d", 354816, 550946743},
	{"msssoft.m3d", 67072, 4284421368},
	{"mssvoice.asi", 197120, 1407967464},
};

bool checkMilesDir(const std::string szDirName)
{
	bool HasHack = false;

	char szDirNamePath[MAX_PATH];
	sprintf(szDirNamePath, "%s\\*", szDirName.c_str());

	WIN32_FIND_DATA f;
	HANDLE h = FindFirstFile(szDirNamePath, &f);

	if (h == INVALID_HANDLE_VALUE) { return HasHack; }

	do
	{
		if (HasHack)
			break;
		const char * name = f.cFileName;

		if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0) { continue; }

		if (f.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
		{
			// do nothing if it's a folder
		}
		else
		{
			// start processing file
			PRINTME(1, "starting %s", name);
			std::string sName(name);
			std::string sPathName(szDirName+"/"+name);
			PRINTME(1, "path %s", sPathName.c_str());
			// lower file name
			std::transform(sName.begin(), sName.end(), sName.begin(), ::tolower);
			// file or symlink; check for asi flt m3d mix exploit
			std::string sNameExt = CFileNameHelper::GetExtension(sName);
			PRINTME(1, "ext %s", sNameExt.c_str());
			// workaround instead of std::find
			bool isMilesFile = true, isMilesUnique = true;//delete if change back the changes (default is false value for both)
			//bool isMilesFile = false, isMilesUnique = false;
			for (MilesExten_t *i1=std::begin(MilesExtenTable), *e1=std::end(MilesExtenTable); i1<e1; i1++)
			{
				PRINTME(1, "is %s ? %s", i1->ExtName.c_str(), sNameExt.c_str());
				if (!sNameExt.compare(0, i1->ExtName.length(), i1->ExtName))
				{
					isMilesFile = true;
					isMilesUnique = i1->IsUni;
				}
			}
			if (isMilesFile)
			{
				PRINTME(1, "verify %s -> %s", sName.c_str(), sNameExt.c_str());
				bool isMilesFound = true; //delete if change back the changes (default is false value)
				//bool isMilesFound = false;
				//for (MilesFiles_t *i1=std::begin(MilesFilesTable), *e1=std::end(MilesFilesTable); i1<e1; i1++)
				//{
				//	if (!i1->sFileName.compare(sName))
				//	{
				//		PRINTME(1, "found %s==%s", i1->sFileName.c_str(), sName.c_str());
				//		DWORD dwCrc32 = GetFileCRC32(sPathName.c_str());
				//		// assert(dwCrc32);
				//		DWORD dwFileSize = f.nFileSizeLow;
				//		if (i1->stSize!=dwFileSize)
				//		{
				//			PRINTME(1, "wrong size %u==%u", i1->stSize, dwFileSize);
				//			HasHack = true;
				//			PRINTME(0, "wrong size %u for %s", dwFileSize, sPathName.c_str());
				//			return HasHack;
				//		}
				//		else if (i1->dwCRC32 != dwCrc32)
				//		{
				//			PRINTME(1, "wrong crc32 %u==%u", i1->dwCRC32, dwCrc32);
				//			HasHack = true;
				//			PRINTME(0, "wrong crc32 %u for %s", dwCrc32, sPathName.c_str());
				//			return HasHack;
				//		}
				//		PRINTME(1, "right size %u==%u", i1->stSize, dwFileSize);
				//		PRINTME(1, "right crc32 %u==%u", i1->dwCRC32, dwCrc32);
				//		isMilesFound = true;
				//		break;
				//	}
				//}
				// only mss32.dll is checked, and there's no need to check the others
				if (!isMilesFound && isMilesUnique)
				{
					PRINTME(1, "not found %s", sName.c_str());
					HasHack = true;
					PRINTME(0, "ambiguous file for %s", sPathName.c_str());
					return HasHack;
				}
				PRINTME(1, "skipping file(%s) hack(%u) found(%u) uni(%u)", sName.c_str(), HasHack, isMilesFound, isMilesUnique);
			}
		}

	} while (FindNextFile(h, &f));
	FindClose(h);
	return HasHack;
}

bool __CheckMilesFiles()
{
	PRINTME(1, "__CheckMilesFiles processing . and .\\miles");
	if (checkMilesDir(".") || checkMilesDir(".\\miles"))
		return false;
	return true;
}
#endif

char gs_szErrorString[512] = "";

void ApplicationSetErrorString(const char* szErrorString)
{
	strcpy(gs_szErrorString, szErrorString);
}

bool CheckPythonLibraryFilenames()
{
	for (int i = 0; *sc_apszPythonLibraryFilenames[i] != '\n'; ++i)
	{
		std::string stFilename = "lib\\";
		stFilename += sc_apszPythonLibraryFilenames[i];

		if (_access(stFilename.c_str(), 0) != 0)
		{
			return false;
		}

		MoveFile(stFilename.c_str(), stFilename.c_str());
	}

	return true;
}

struct ApplicationStringTable
{
	HINSTANCE m_hInstance;
	std::map<DWORD, std::string> m_kMap_dwID_stLocale;
} gs_kAppStrTable;

void ApplicationStringTable_Initialize(HINSTANCE hInstance)
{
	gs_kAppStrTable.m_hInstance=hInstance;
}

const std::string& ApplicationStringTable_GetString(DWORD dwID, LPCSTR szKey)
{
	char szBuffer[512];
	char szIniFileName[256];
	char szLocale[256];

	::GetCurrentDirectory(sizeof(szIniFileName), szIniFileName);
	if(szIniFileName[lstrlen(szIniFileName)-1] != '\\')
		strcat(szIniFileName, "\\");
	strcat(szIniFileName, "metin2client.dat");

	strcpy(szLocale, LocaleService_GetLocalePath());
	if(strnicmp(szLocale, "locale/", strlen("locale/")) == 0)
		strcpy(szLocale, LocaleService_GetLocalePath() + strlen("locale/"));
	::GetPrivateProfileString(szLocale, szKey, NULL, szBuffer, sizeof(szBuffer)-1, szIniFileName);
	if(szBuffer[0] == '\0')
		LoadString(gs_kAppStrTable.m_hInstance, dwID, szBuffer, sizeof(szBuffer)-1);
	if(szBuffer[0] == '\0')
		::GetPrivateProfileString("en", szKey, NULL, szBuffer, sizeof(szBuffer)-1, szIniFileName);
	if(szBuffer[0] == '\0')
		strcpy(szBuffer, szKey);

	std::string& rstLocale=gs_kAppStrTable.m_kMap_dwID_stLocale[dwID];
	rstLocale=szBuffer;

	return rstLocale;
}

const std::string& ApplicationStringTable_GetString(DWORD dwID)
{
	char szBuffer[512];

	LoadString(gs_kAppStrTable.m_hInstance, dwID, szBuffer, sizeof(szBuffer)-1);
	std::string& rstLocale=gs_kAppStrTable.m_kMap_dwID_stLocale[dwID];
	rstLocale=szBuffer;

	return rstLocale;
}

const char* ApplicationStringTable_GetStringz(DWORD dwID, LPCSTR szKey)
{
	return ApplicationStringTable_GetString(dwID, szKey).c_str();
}

const char* ApplicationStringTable_GetStringz(DWORD dwID)
{
	return ApplicationStringTable_GetString(dwID).c_str();
}

////////////////////////////////////////////

int Setup(LPSTR lpCmdLine); // Internal function forward

#ifdef __ENABLE_NEW_PACK_ENCRYPTION__
#include <ikarusgl/ikarus_gl.h>

void create_initialize_pack_interface(ikgl::gui& gui) {
	const auto width = 300;
	const auto height = 35;

	//* creating application window
	ikgl::create_gui_infow_t info;
	ikgl::make_default_create_info(GetModuleHandle(NULL), &info);
	info.height = height;
	info.width = width;
	info.window_name = L"Pack-Initializing";
	info.wc.lpszClassName = L"Pack-Initializing";

	ikgl::widget_size_t desktopsize = ikgl::utilities::get_desktop_size();
	info.x = (desktopsize.width / 2) - (width / 2);
	info.y = (desktopsize.height / 2) - (height / 2);

	if (!gui.create(&info)) {
		::MessageBoxA(NULL, "Error while initializing pack encryption!", "Fatal error", MB_OK);
		exit(0);
	}

	//* creating application interface
	//* creating layer
	auto layer = gui.add_layer("UI");

	//* appling widgets container
	auto window = layer->create_element<ikgl::window>();
	window->set_background({ {0.0,0.0,0.1,1.0} });
	window->set_size(width, height);
	window->set_position(0, 0);


	//* default text font to apply to the widgets
	ikgl::font_t font;
	ikgl::make_default_font(font);
	font.font_family = L"Corbel";
	font.size = 13;


	//* textout
	auto textout = window->create_element<ikgl::text>();
	textout->set_font(font);
	textout->set_size(width, 15);
	textout->set_horizontal_alignment(ikgl::text_halign_t::TEXT_HALIGN_CENTER);
	textout->set_text("Initializing pack encryption ...");
	textout->move_bottom(4);



	//*loading bar
	auto percentage_bar = window->create_element<ikgl::text>();
	percentage_bar->set_background({ RGB(134, 179, 252) });
	percentage_bar->set_position(0, 30);
	percentage_bar->set_size(width, height - 30);
	percentage_bar->set_window_name("percentage-bar");

}



void initialize_pack_process(const std::vector<std::string>& files) {
	ikgl::gui gui;
	create_initialize_pack_interface(gui);

	static std::atomic<int>  g_file_index = 0;
	static std::atomic<int>  g_file_to_open = files.size();
	static std::atomic<int>  g_file_done = 0;
	static std::atomic<bool> g_error = false;
	static std::mutex g_mtx;

	auto perc_bar = gui.get_window_by_name("percentage-bar");
	perc_bar->set_on_update(__ikgl_wevent_lambda_type{
		if (g_file_to_open == 0)
			widget->set_size(0, widget->get_height());

		else {
			const float p = (float)g_file_done / (float)g_file_to_open;
			const auto  s = widget->get_gui()->get_size();
			widget->set_size((ikgl::coord_t)((float)s.width * p) , widget->get_height());

			if (g_file_done == g_file_to_open && g_file_done != 0) {
				widget->get_gui()->quit();
				return ikgl::widget_event_return_t::UNSET_CALLBACK;
			}
		}

		return ikgl::widget_event_return_t::KEEP_CALLBACK;
		});


	auto __get_one = [&files]() ->std::string {
		if (g_file_index == g_file_to_open || g_error) return "";
		std::lock_guard<std::mutex> l(g_mtx);
		return files[g_file_index++];
	};

	auto __new_pack_import = [&__get_one, &gui]() {
		std::string name;
		while ((name = __get_one()).empty() == false) {
			if (!CEterPackManager::Instance().RegisterPack(name.c_str(), "")) {
				g_error = true;
				gui.quit();
			}
			else g_file_done++;
		}
	};

	const auto thcount = ikgl::utilities::minmaxof(1, (int)std::thread::hardware_concurrency(), (int)files.size());
	std::vector<std::thread> ths;
	for (int i = 0; i < thcount; i++) {
		ths.push_back(std::thread(__new_pack_import));
	}

	gui.start_rendering_thread();
	gui.start_updating_thread();
	gui.message_loop(10);
	gui.clear();

	for (auto& th : ths) {
		if (th.joinable())
			th.join();
	} ths.clear();

	if (g_error) {
		::MessageBoxA(NULL, "Cannot initialize correctly pack encryption.", "Fatal Error", MB_OK);
		ikgl::external::sleep(3000);
		exit(0);
	}
}
#endif



bool PackInitialize(const char * c_pszFolder)
{
#ifndef __ENABLE_NEW_PACK_ENCRYPTION__
	NANOBEGIN
		if (_access(c_pszFolder, 0) != 0)
			return true;

	std::string stFolder(c_pszFolder);
	stFolder += "/";

	CTextFileLoader::SetCacheMode();

#if defined(USE_RELATIVE_PATH)
	CEterPackManager::Instance().SetRelativePathMode();
#endif


	CEterPackManager::Instance().SetCacheMode();
	CEterPackManager::Instance().SetSearchMode(CEterPackManager::SEARCH_PACK_FIRST);

	CSoundData::SetPackMode();

	CEterPackManager::Instance().RegisterPack("pack/property", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_67bonus", "*");
	CEterPackManager::Instance().RegisterPack("pack/araba", "*");
	CEterPackManager::Instance().RegisterPack("pack/effect", "*");
	CEterPackManager::Instance().RegisterPack("pack/etc", "*");
	CEterPackManager::Instance().RegisterPack("pack/guild", "*");
	CEterPackManager::Instance().RegisterPack("pack/icon", "*");
	CEterPackManager::Instance().RegisterPack("pack/ash_1", "*");
	CEterPackManager::Instance().RegisterPack("pack/indoorflame", "*");
	CEterPackManager::Instance().RegisterPack("pack/indoorcatacomb", "*");
	CEterPackManager::Instance().RegisterPack("pack/indoordeviltower1", "*");
	CEterPackManager::Instance().RegisterPack("pack/indoormonkeydungeon1", "*");
	CEterPackManager::Instance().RegisterPack("pack/indoorspiderdungeon1", "*");
	CEterPackManager::Instance().RegisterPack("pack/indoorspiderdungeon3", "*");
	CEterPackManager::Instance().RegisterPack("pack/indoormonkeydungeon2", "*");
	CEterPackManager::Instance().RegisterPack("pack/indoormonkeydungeon3", "*");
	CEterPackManager::Instance().RegisterPack("pack/item", "*");
	CEterPackManager::Instance().RegisterPack("pack/locale", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_5th_armor", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_cube_renewal", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_acce", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_attendance", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_costume_soccer", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_dance", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_dawnmist_dungeon", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_dawnmist_dungeon_mob", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_dragon_rock", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_dragon_rock_mobs", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_dragon_rock_mobs_texcache", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_dragon_rock_texcache", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_ds", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_dss_box", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_e1", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_easter1", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_easter2016", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_etc", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_etc_costume1", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_eu2", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_eu3", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_eu4", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_flame", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_flame_dragonlair", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_guildrenewal", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_guildwar", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_guildwar_object", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_halloween", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_info", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_mineral", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_minigame_okey", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_monster_card", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_mundi", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_myshop_deco", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_new_halloween", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_new_halloween_weapon", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_new_pet", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_new_questicon", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_new_select_ui", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_party", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_pc3", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_pc3_m", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_pepsi", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_pet1", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_pet2", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_privateshop", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_public", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_ramadan_costume", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_sd", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_second_guildrenewal", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_snow", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_snow_dungeon", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_summer", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_user_situation", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_valentine_pet", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_w20_etc", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_w20_sound", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_w21_mobs", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_w21_mobs_m", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_xmas", "*");
	CEterPackManager::Instance().RegisterPack("pack/monster", "*");
	CEterPackManager::Instance().RegisterPack("pack/monster2", "*");
	CEterPackManager::Instance().RegisterPack("pack/login_interface_patch", "*");
	CEterPackManager::Instance().RegisterPack("pack/npc", "*");
	CEterPackManager::Instance().RegisterPack("pack/npc2", "*");
	CEterPackManager::Instance().RegisterPack("pack/outdoor", "*");
	CEterPackManager::Instance().RegisterPack("pack/outdoora1", "*");
	CEterPackManager::Instance().RegisterPack("pack/outdoora2", "*");
	CEterPackManager::Instance().RegisterPack("pack/outdoora3", "*");
	CEterPackManager::Instance().RegisterPack("pack/outdoorb1", "*");
	CEterPackManager::Instance().RegisterPack("pack/outdoorb3", "*");
	CEterPackManager::Instance().RegisterPack("pack/outdoorc1", "*");
	CEterPackManager::Instance().RegisterPack("pack/outdoorCV", "*");
	CEterPackManager::Instance().RegisterPack("pack/outdoorc3", "*");
	CEterPackManager::Instance().RegisterPack("pack/outdoordesert1", "*");
	CEterPackManager::Instance().RegisterPack("pack/outdoorduel", "*");
	CEterPackManager::Instance().RegisterPack("pack/outdoorempirebattle1", "*");
	CEterPackManager::Instance().RegisterPack("pack/outdoorfielddungeon1", "*");
	CEterPackManager::Instance().RegisterPack("pack/outdoorflame1", "*");
	CEterPackManager::Instance().RegisterPack("pack/outdoorgmguildbuild", "*");
	CEterPackManager::Instance().RegisterPack("pack/outdoorguild1", "*");
	CEterPackManager::Instance().RegisterPack("pack/outdoorguild2", "*");
	CEterPackManager::Instance().RegisterPack("pack/outdoorguild3", "*");
	CEterPackManager::Instance().RegisterPack("pack/outdoormilgyo1", "*");
	CEterPackManager::Instance().RegisterPack("pack/OutdoorSnow1", "*");
	CEterPackManager::Instance().RegisterPack("pack/outdoort1", "*");
	CEterPackManager::Instance().RegisterPack("pack/outdoort2", "*");
	CEterPackManager::Instance().RegisterPack("pack/outdoort3", "*");
	CEterPackManager::Instance().RegisterPack("pack/outdoort4", "*");
	CEterPackManager::Instance().RegisterPack("pack/outdoort5", "*");
	CEterPackManager::Instance().RegisterPack("pack/outdoort6", "*");
	CEterPackManager::Instance().RegisterPack("pack/outdoortrent", "*");
	CEterPackManager::Instance().RegisterPack("pack/outdoortrent02", "*");
	CEterPackManager::Instance().RegisterPack("pack/outdoorwedding", "*");
	CEterPackManager::Instance().RegisterPack("pack/patch1", "*");
	CEterPackManager::Instance().RegisterPack("pack/patch2", "*");
	CEterPackManager::Instance().RegisterPack("pack/pc", "*");
	CEterPackManager::Instance().RegisterPack("pack/pc2", "*");
	CEterPackManager::Instance().RegisterPack("pack/property", "*");
	CEterPackManager::Instance().RegisterPack("pack/pvp", "*");
	CEterPackManager::Instance().RegisterPack("pack/uiscript", "*");
	CEterPackManager::Instance().RegisterPack("pack/season1", "*");
	CEterPackManager::Instance().RegisterPack("pack/season2", "*");
	CEterPackManager::Instance().RegisterPack("pack/season3_eu", "*");
	CEterPackManager::Instance().RegisterPack("pack/sound", "*");
	CEterPackManager::Instance().RegisterPack("pack/sound_m", "*");
	CEterPackManager::Instance().RegisterPack("pack/sound2", "*");
	CEterPackManager::Instance().RegisterPack("pack/terrain", "*");
	CEterPackManager::Instance().RegisterPack("pack/textureset", "*");
	CEterPackManager::Instance().RegisterPack("pack/tree", "*");
	CEterPackManager::Instance().RegisterPack("pack/uiloading", "*");
	CEterPackManager::Instance().RegisterPack("pack/zone", "*");
	CEterPackManager::Instance().RegisterPack("pack/critical", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_emotion", "*");
	CEterPackManager::Instance().RegisterPack("pack/new_petsystem", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_special_storage", "*");
	CEterPackManager::Instance().RegisterPack("pack/ash_patch1", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_12zi", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_item_buff", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_12zi_mob", "*");
	CEterPackManager::Instance().RegisterPack("pack/metin2_patch_12zi_obj", "*");

	CEterPackManager::Instance().RegisterRootPack((stFolder + std::string("root")).c_str());
	NANOEND
#else
	//topatch
	CTextFileLoader::SetCacheMode();
	CEterPackManager::Instance().SetCacheMode();
	CEterPackManager::Instance().SetSearchMode(true);
	CSoundData::SetPackMode();

	const std::vector<std::string> files = {
		"pack/root.pack",
		"pack/araba.pack",
		"pack/zay_u1.pack",
		"pack/daimao_dun.pack",
		"pack/Effect.pack",
		"pack/ETC.pack",
		"pack/guild.pack",
		"pack/icon.pack",
		"pack/indoorcatacomb.pack",
		"pack/indoordeviltower1.pack",
		"pack/indoorflame.pack",
		"pack/indoormonkeydungeon1.pack",
		"pack/indoormonkeydungeon2.pack",
		"pack/indoormonkeydungeon3.pack",
		"pack/indoorspiderdungeon1.pack",
		"pack/indoorspiderdungeon3.pack",
		"pack/item.pack",
		"pack/locale.pack",
		"pack/metin2_patch_12zi.pack",
		"pack/metin2_patch_12zi_mob.pack",
		"pack/metin2_patch_12zi_obj.pack",
		"pack/metin2_patch_5th_armor.pack",
		"pack/metin2_patch_67bonus.pack",
		"pack/metin2_patch_acce.pack",
		"pack/metin2_patch_attendance.pack",
		"pack/metin2_patch_costume_soccer.pack",
		"pack/metin2_patch_cube_renewal.pack",
		"pack/metin2_patch_dance.pack",
		"pack/metin2_patch_dawnmist_dungeon.pack",
		"pack/metin2_patch_dawnmist_dungeon_mob.pack",
		"pack/metin2_patch_dragon_rock.pack",
		"pack/metin2_patch_dragon_rock_mobs.pack",
		"pack/metin2_patch_dragon_rock_mobs_texcache.pack",
		"pack/metin2_patch_dragon_rock_texcache.pack",
		"pack/metin2_patch_ds.pack",
		"pack/metin2_patch_dss_box.pack",
		"pack/metin2_patch_e1.pack",
		"pack/metin2_patch_easter1.pack",
		"pack/metin2_patch_easter2016.pack",
		"pack/metin2_patch_emotion.pack",
		"pack/metin2_patch_etc.pack",
		"pack/metin2_patch_etc_costume1.pack",
		"pack/metin2_patch_eu2.pack",
		"pack/metin2_patch_eu3.pack",
		"pack/metin2_patch_eu4.pack",
		"pack/metin2_patch_flame.pack",
		"pack/metin2_patch_flame_dragonlair.pack",
		"pack/metin2_patch_guildrenewal.pack",
		"pack/metin2_patch_guildwar.pack",
		"pack/metin2_patch_guildwar_object.pack",
		"pack/metin2_patch_halloween.pack",
		"pack/metin2_patch_info.pack",
		"pack/metin2_patch_item_buff.pack",
		"pack/metin2_patch_mineral.pack",
		"pack/metin2_patch_minigame_okey.pack",
		"pack/metin2_patch_monster_card.pack",
		"pack/metin2_patch_mundi.pack",
		"pack/metin2_patch_myshop_deco.pack",
		"pack/metin2_patch_new_halloween.pack",
		"pack/metin2_patch_new_halloween_weapon.pack",
		"pack/metin2_patch_new_pet.pack",
		"pack/metin2_patch_new_questicon.pack",
		"pack/metin2_patch_new_select_ui.pack",
		"pack/metin2_patch_party.pack",
		"pack/metin2_patch_pc3.pack",
		"pack/metin2_patch_pc3_m.pack",
		"pack/metin2_patch_pepsi.pack",
		"pack/metin2_patch_pet1.pack",
		"pack/metin2_patch_pet2.pack",
		"pack/metin2_patch_privateshop.pack",
		"pack/metin2_patch_public.pack",
		"pack/metin2_patch_ramadan_costume.pack",
		"pack/metin2_patch_sd.pack",
		"pack/metin2_patch_second_guildrenewal.pack",
		"pack/metin2_patch_snow.pack",
		"pack/metin2_patch_snow_dungeon.pack",
		"pack/metin2_patch_summer.pack",
		"pack/metin2_patch_user_situation.pack",
		"pack/metin2_patch_valentine_pet.pack",
		"pack/metin2_patch_w20_etc.pack",
		"pack/metin2_patch_w20_sound.pack",
		"pack/metin2_patch_w21_mobs.pack",
		"pack/metin2_patch_w21_mobs_m.pack",
		"pack/metin2_patch_xmas.pack",
		"pack/metin2_special_storage.pack",
		"pack/Monster.pack",
		"pack/monster2.pack",
		"pack/new_petsystem.pack",
		"pack/NPC.pack",
		"pack/npc2.pack",
		"pack/Outdoor.pack",
		"pack/OutdoorA1.pack",
		"pack/OutdoorA2.pack",
		"pack/OutdoorA3.pack",
		"pack/OutdoorB1.pack",
		"pack/OutdoorB3.pack",
		"pack/OutdoorC1.pack",
		"pack/OutdoorC3.pack",
		"pack/outdoorCV.pack",
		"pack/outdoordesert1.pack",
		"pack/outdoorduel.pack",
		"pack/outdoorempirebattle1.pack",
		"pack/outdoorfielddungeon1.pack",
		"pack/outdoorflame1.pack",
		"pack/outdoorgmguildbuild.pack",
		"pack/outdoorguild1.pack",
		"pack/outdoorguild2.pack",
		"pack/outdoorguild3.pack",
		"pack/outdoormilgyo1.pack",
		"pack/OutdoorSnow1.pack",
		"pack/outdoort1.pack",
		"pack/outdoort2.pack",
		"pack/outdoort3.pack",
		"pack/outdoort4.pack",
		"pack/outdoort5.pack",
		"pack/outdoort6.pack",
		"pack/outdoortrent.pack",
		"pack/outdoortrent02.pack",
		"pack/outdoorwedding.pack",
		"pack/patch1.pack",
		"pack/patch2.pack",
		"pack/PC.pack",
		"pack/pc2.pack",
		"pack/Property.pack",
		"pack/pvp.pack",
		"pack/season1.pack",
		"pack/season2.pack",
		"pack/season3_eu.pack",
		"pack/Sound.pack",
		"pack/sound2.pack",
		"pack/sound_m.pack",
		"pack/Terrain.pack",
		"pack/textureset.pack",
		"pack/Tree.pack",
		"pack/uiloading.pack",
		"pack/uiscript.pack",
		"pack/Zone.pack",
		"pack/patch_login.pack",
		"pack/pvp_pack.pack",
		"pack/dhyd.pack",
		"pack/selectwindow_new.pack",
		"pack/patch_fastequip.pack"
	};
	initialize_pack_process(files);
	sort_archives(files);

	#ifdef __ENABLE_BACKGROUND_FILE_LOADER__
	start_background_file_loading();
	#endif
#endif
	return true;
}

bool RunMainScript(CPythonLauncher& pyLauncher, const char* lpCmdLine)
{
	initpack();
	initdbg();
	initime();
	initgrp();
	initgrpImage();
	initgrpText();
	initwndMgr();
	/////////////////////////////////////////////
	initudp();
	initapp();
	initsystemSetting();
	initchr();
	initchrmgr();
	initPlayer();
	initItem();

	initNonPlayer();
	initTrade();
	initChat();
	initTextTail();
	initnet();
	initMiniMap();
	initProfiler();
	initEvent();
	initeffect();
	initfly();
#if defined(GUILD_RANK_SYSTEM)
	initguildranking();
#endif
#if defined(__BL_RANKING__)
	initranking();
#endif
	initsnd();
	initeventmgr();
	initshop();
	initskill();

	initPlayerSettingsModule();
#ifdef NEW_PET_SYSTEM
	initskillpet();
#endif
	initquest();
	initBackground();
	initMessenger();
#if defined(ENABLE_DUNGEON_INFO_SYSTEM)
	initDungeonInfo();
#endif
#ifdef INGAME_WIKI
	initWiki();
#endif
#ifdef ENABLE_ACCE_COSTUME_SYSTEM
	initAcce();
#endif
#ifdef ENABLE_CHANGELOOK_SYSTEM
	initChangeLook();
#endif

	initsafebox();
#if defined(__BL_MAILBOX__)
	initmail();
#endif
#ifdef ENABLE_WHISPER_RENEWAL
	initWhisper();
#endif
	initguild();
	initServerStateChecker();
#ifdef ENABLE_ANTI_MULTIPLE_FARM
	initAntiMultipleFarmMethods();
#endif

#ifdef ENABLE_6_7_BONUS_NEW_SYSTEM
	int67bonus();
#endif
#ifdef ENABLE_CUBE_RENEWAL_WORLDARD
	intcuberenewal();
#endif	
#ifdef ENABLE_SWITCHBOT
	initSwitchbot();
#endif
#ifdef RENDER_TARGET
	initRenderTarget();	
#endif
#ifdef __USE_CYTHON__
	// don't add this line if you're implementing cython via .pyd:
	// initrootlibManager();
#endif
#ifdef __USE_EXTRA_CYTHON__
	// don't add this line if you're implementing cython via .pyd:
	// inituiscriptlibManager();
#endif
		
	
	NANOBEGIN

    PyObject * builtins = PyImport_ImportModule("__builtin__");
#ifdef NDEBUG // @warme601 _DISTRIBUTE -> NDEBUG
	PyModule_AddIntConstant(builtins, "__DEBUG__", 1);
#else
	PyModule_AddIntConstant(builtins, "__DEBUG__", 0);
#endif
#ifdef __USE_CYTHON__
	PyModule_AddIntConstant(builtins, "__USE_CYTHON__", 1);
#else
	PyModule_AddIntConstant(builtins, "__USE_CYTHON__", 0);
#endif
#ifdef __USE_EXTRA_CYTHON__
	PyModule_AddIntConstant(builtins, "__USE_EXTRA_CYTHON__", 1);
#else
	PyModule_AddIntConstant(builtins, "__USE_EXTRA_CYTHON__", 0);
#endif

	// RegisterCommandLine
	{
		std::string stRegisterCmdLine;

		const char * loginMark = "-cs";
		const char * loginMark_NonEncode = "-ncs";
		const char * seperator = " ";

		std::string stCmdLine;
		const int CmdSize = 3;
		std::vector<std::string> stVec;
		SplitLine(lpCmdLine,seperator,&stVec);
		if (CmdSize == stVec.size() && stVec[0]==loginMark)
		{
			char buf[MAX_PATH];	//TODO 아래 함수 string 형태로 수정
			base64_decode(stVec[2].c_str(),buf);
			stVec[2] = buf;
			string_join(seperator,stVec,&stCmdLine);
		}
		else if (CmdSize <= stVec.size() && stVec[0]==loginMark_NonEncode)
		{
			stVec[0] = loginMark;
			string_join(" ",stVec,&stCmdLine);
		}
		else
			stCmdLine = lpCmdLine;

		PyModule_AddStringConstant(builtins, "__COMMAND_LINE__", stCmdLine.c_str());
	}
	{
		std::vector<std::string> stVec;
		SplitLine(lpCmdLine," " ,&stVec);

		if (stVec.size() != 0 && "--pause-before-create-window" == stVec[0])
		{
#ifdef XTRAP_CLIENT_ENABLE
			if (!XTrap_CheckInit())
				return false;
#endif
			system("pause");
		}
#ifdef ENABLE_DAEMONPROTECTION
		DWORD dwCrc32, dwSize;
		if (!((dwCrc32 = GetFileCRC32(DPDLL_FILENAME))==DPDLL_CRC32))
		{
			TraceError("dpdll wrong crc32 %d", dwCrc32);
			return false;
		}
		if (!((dwSize = GetFileSize(DPDLL_FILENAME))==DPDLL_FILESIZE))
		{
			TraceError("dpdll wrong size %d", dwSize);
			return false;
		}
		if (!LoadLibraryA(DPDLL_FILENAME))
		{
			TraceError("dpdll not loaded");
			return false;
		}
#endif

#ifdef __USE_CYTHON__
		if (!pyLauncher.RunLine("import rootlib\nrootlib.moduleImport('system')"))
#else
		if (!pyLauncher.RunFile("system.py"))
#endif
		{
			TraceError("RunMain Error");
			return false;
		}
	}

	NANOEND
	return true;
}

bool Main(HINSTANCE hInstance, LPSTR lpCmdLine)
{
	//if (strstr(lpCmdLine, "Zyh2XsaLoS17a") == 0)
		//return false;

#ifdef LOCALE_SERVICE_YMIR
	extern bool g_isScreenShotKey;
	g_isScreenShotKey = true;
#endif

	DWORD dwRandSeed=time(NULL)+DWORD(GetCurrentProcess());
	srandom(dwRandSeed);
	srand(random());

	SetLogLevel(1);

#ifdef LOCALE_SERVICE_VIETNAM_MILD
	extern BOOL USE_VIETNAM_CONVERT_WEAPON_VNUM;
	USE_VIETNAM_CONVERT_WEAPON_VNUM = true;
#endif

	if (_access("perf_game_update.txt", 0)==0)
	{
		DeleteFile("perf_game_update.txt");
	}

	if (_access("newpatch.exe", 0)==0)
	{
		system("patchupdater.exe");
		return false;
	}
#ifndef __VTUNE__
	ilInit();
#endif
	if (!Setup(lpCmdLine))
		return false;

#if defined(_DEBUG) || defined(ENABLE_LOG_CONSOLE_WINDOW)
	OpenConsoleWindow();
	OpenLogFile(true); // true == uses syserr.txt and log.txt
#else
	OpenLogFile(false); // false == uses syserr.txt only
#endif

	static CLZO				lzo;
	static CEterPackManager	EterPackManager;

	if (!PackInitialize("pack"))
	{
		LogBox("Pack Initialization failed. Check log.txt file..");
		return false;
	}

	if(LocaleService_LoadGlobal(hInstance))
		SetDefaultCodePage(LocaleService_GetCodePage());

#ifdef ENABLE_PYLIB_CHECK
	if (!__CheckPyLibFiles())
		return false;
#endif
#ifdef ENABLE_MILES_CHECK
	if (!__CheckMilesFiles())
		return false;
#endif

	auto LoadingWindow = std::thread(LoadingWindow::InitializeLibrary);

	std::unique_ptr<CPythonApplication> app = std::make_unique<CPythonApplication>();
	app->Initialize(hInstance);

	bool ret=false;
	{
		CPythonLauncher pyLauncher;
		CPythonExceptionSender pyExceptionSender;
		SetExceptionSender(&pyExceptionSender);

		if (pyLauncher.Create())
		{
			ret=RunMainScript(pyLauncher, lpCmdLine);	//게임 실행중엔 함수가 끝나지 않는다.
		}

	

		//ProcessScanner_ReleaseQuitEvent();

		//게임 종료시.
		app->Clear();

		timeEndPeriod(1);
		pyLauncher.Clear();
	}

	app->Destroy();
	//delete app;

	return ret;
}

HANDLE CreateMetin2GameMutex()
{
	SECURITY_ATTRIBUTES sa;
	ZeroMemory(&sa, sizeof(SECURITY_ATTRIBUTES));
	sa.nLength				= sizeof(sa);
	sa.lpSecurityDescriptor	= NULL;
	sa.bInheritHandle		= FALSE;

	return CreateMutex(&sa, FALSE, "Metin2GameMutex");
}

void DestroyMetin2GameMutex(HANDLE hMutex)
{
	if (hMutex)
	{
		ReleaseMutex(hMutex);
		hMutex = NULL;
	}
}

void __ErrorPythonLibraryIsNotExist()
{
	LogBoxf("FATAL ERROR!! Python Library file not exist!");
}

bool __IsTimeStampOption(LPSTR lpCmdLine)
{
	const char* TIMESTAMP = "/timestamp";
	return (strncmp(lpCmdLine, TIMESTAMP, strlen(TIMESTAMP))==0);
}

void __PrintTimeStamp()
{
#ifdef	_DEBUG
	if (__IS_TEST_SERVER_MODE__)
		LogBoxf("METIN2 BINARY TEST DEBUG VERSION %s  ( MS C++ %d Compiled )", __TIMESTAMP__, _MSC_VER);
	else
		LogBoxf("METIN2 BINARY DEBUG VERSION %s ( MS C++ %d Compiled )", __TIMESTAMP__, _MSC_VER);

#else
	if (__IS_TEST_SERVER_MODE__)
		LogBoxf("METIN2 BINARY TEST VERSION %s  ( MS C++ %d Compiled )", __TIMESTAMP__, _MSC_VER);
	else
		LogBoxf("METIN2 BINARY DISTRIBUTE VERSION %s ( MS C++ %d Compiled )", __TIMESTAMP__, _MSC_VER);
#endif
}

bool __IsLocaleOption(LPSTR lpCmdLine)
{
	return (strcmp(lpCmdLine, "--locale") == 0);
}

bool __IsLocaleVersion(LPSTR lpCmdLine)
{
	return (strcmp(lpCmdLine, "--perforce-revision") == 0);
}

#ifdef ENABLE_CHECK_FILES_MD5
void CheckMD5Filenames()
{
	MD5 md5;
	for (int it = 0; it < _countof(ar_szMD5FileNames); it++)
	{
		if (strcmp(md5.digestFile(ar_szMD5FileNames[it][0]), ar_szMD5FileNames[it][1]))
		{
			char szBuf[512 + 1];
			snprintf(szBuf, sizeof(szBuf), "File %s has been modified, please update from Patcher.exe", ar_szMD5FileNames[it][0]);
			MessageBoxA(NULL, szBuf, "Emeria - Official", NULL);
			exit(0);
		}
	}
}
#endif

#ifdef USE_OPENID
//2012.07.16 김용욱
//일본 OpenID 지원. 인증키 인자 추가
bool __IsOpenIDAuthKeyOption(LPSTR lpCmdLine)
{
	return (strcmp(lpCmdLine, "--openid-authkey") == 0);
}

bool __IsOpenIDTestOption(LPSTR lpCmdLine) //클라이언트에서 로그인이 가능하다.
{
	return (strcmp(lpCmdLine, "--openid-test") == 0);
}
#endif /* USE_OPENID */

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//REFERENCE_STEALTH_DATA_AREA;

#ifdef CEF_BROWSER
	CefWebBrowser_Startup(hInstance);
#else
	WebBrowser_Startup(hInstance);
#endif

#if defined(ENABLE_CSHIELD) && !defined(_DEBUG)
	myCShieldData = InitializeCShield();
	if (myCShieldData.dwThreadId == 0)
	{
#ifdef ENABLE_CSHIELD_DEBUG
		ErrorMessage("Couldnt initialize CShield");
#endif
		Close();
	}

	if (!ValidateCShield())
	{
#ifdef ENABLE_CSHIELD_DEBUG
		ErrorMessage("Couldnt validate CShield");
#endif
		Close();
	}
#endif

	ApplicationStringTable_Initialize(hInstance);

#ifdef ENABLE_CHECK_FILES_MD5
	CheckMD5Filenames();
#endif

	LocaleService_LoadConfig("locale.cfg");
	SetDefaultCodePage(LocaleService_GetCodePage());

#ifdef XTRAP_CLIENT_ENABLE
	if (!XTrap_Init())
		return 0;
#endif

#ifdef USE_AHNLAB_HACKSHIELD
	if (!HackShield_Init())
		return 0;
#endif

#ifdef USE_NPROTECT_GAMEGUARD
	if (!GameGuard_Init())
		return 0;
#endif

#if defined(CHECK_LATEST_DATA_FILES)
	if (!CheckLatestFiles())
		return 0;
#endif

	bool bQuit = false;
	bool bAuthKeyChecked = false;	//OpenID 버전에서 인증키가 들어왔는지 알기 위한 인자.
	int nArgc = 0;
	PCHAR* szArgv = CommandLineToArgv( lpCmdLine, &nArgc );

	for( int i=0; i < nArgc; i++ ) {
		if(szArgv[i] == 0)
			continue;
		if (__IsLocaleVersion(szArgv[i])) // #0000829: [M2EU] 버전 파일이 항상 생기지 않도록 수정
		{
			char szModuleName[MAX_PATH];
			char szVersionPath[MAX_PATH];
			GetModuleFileName(NULL, szModuleName, sizeof(szModuleName));
			sprintf(szVersionPath, "%s.version", szModuleName);
			FILE* fp = fopen(szVersionPath, "wt");
			if (fp)
			{
				extern int METIN2_GET_VERSION();
				fprintf(fp, "r%d\n", METIN2_GET_VERSION());
				fclose(fp);
			}
			bQuit = true;
		} else if (__IsLocaleOption(szArgv[i]))
		{
			FILE* fp=fopen("locale.txt", "wt");
			fprintf(fp, "service[%s] code_page[%d]",
				LocaleService_GetName(), LocaleService_GetCodePage());
			fclose(fp);
			bQuit = true;
		} else if (__IsTimeStampOption(szArgv[i]))
		{
			__PrintTimeStamp();
			bQuit = true;
		} else if ((strcmp(szArgv[i], "--force-set-locale") == 0))
		{
			// locale 설정엔 인자가 두 개 더 필요함 (로케일 명칭, 데이터 경로)
			if (nArgc <= i + 2)
			{
				MessageBox(NULL, "Invalid arguments", ApplicationStringTable_GetStringz(IDS_APP_NAME, "APP_NAME"), MB_ICONSTOP);
				goto Clean;
			}

			const char* localeName = szArgv[++i];
			const char* localePath = szArgv[++i];

			LocaleService_ForceSetLocale(localeName, localePath);
		}
#ifdef USE_OPENID
		else if (__IsOpenIDAuthKeyOption(szArgv[i]))	//2012.07.16 OpenID : 김용욱
		{
			// 인증키 설정엔 인자가 한 개 더 필요함 (인증키)
			if (nArgc <= i + 1)
			{
				MessageBox(NULL, "Invalid arguments", ApplicationStringTable_GetStringz(IDS_APP_NAME, "APP_NAME"), MB_ICONSTOP);
				goto Clean;
			}

			const char* authKey = szArgv[++i];

			//ongoing (2012.07.16)
			//인증키 저장하는 부분
			LocaleService_SetOpenIDAuthKey(authKey);

			bAuthKeyChecked = true;
		}
		else if (__IsOpenIDTestOption(szArgv[i]))
		{
			openid_test = 1;

		}
#endif /* USE_OPENID */
	}

#ifdef USE_OPENID
	//OpenID
	//OpenID 클라이언트의 경우인증키를 받아오지 않을 경우 (웹을 제외하고 실행 시) 클라이언트 종료.

	if (false == bAuthKeyChecked && !openid_test)
	{
		MessageBox(NULL, "Invalid execution", ApplicationStringTable_GetStringz(IDS_APP_NAME, "APP_NAME"), MB_ICONSTOP);
		goto Clean;
	}
#endif /* USE_OPENID */


	if(bQuit)
		goto Clean;

#if defined(NEEDED_COMMAND_ARGUMENT)
	// 옵션이 없으면 비정상 실행으로 간주, 프로그램 종료
	if (strstr(lpCmdLine, NEEDED_COMMAND_ARGUMENT) == 0) {
		MessageBox(NULL, ApplicationStringTable_GetStringz(IDS_ERR_MUST_LAUNCH_FROM_PATCHER, "ERR_MUST_LAUNCH_FROM_PATCHER"), ApplicationStringTable_GetStringz(IDS_APP_NAME, "APP_NAME"), MB_ICONSTOP);
			goto Clean;
	}
#endif

#if defined(NEEDED_COMMAND_CLIPBOARD)
	{
		CHAR szSecKey[256];
		CPostIt cPostIt( "VOLUME1" );

		if( cPostIt.Get( "SEC_KEY", szSecKey, sizeof(szSecKey) ) == FALSE ) {
			MessageBox(NULL, ApplicationStringTable_GetStringz(IDS_ERR_MUST_LAUNCH_FROM_PATCHER, "ERR_MUST_LAUNCH_FROM_PATCHER"), ApplicationStringTable_GetStringz(IDS_APP_NAME, "APP_NAME"), MB_ICONSTOP);
			goto Clean;
		}
		if( strstr(szSecKey, NEEDED_COMMAND_CLIPBOARD) == 0 ) {
			MessageBox(NULL, ApplicationStringTable_GetStringz(IDS_ERR_MUST_LAUNCH_FROM_PATCHER, "ERR_MUST_LAUNCH_FROM_PATCHER"), ApplicationStringTable_GetStringz(IDS_APP_NAME, "APP_NAME"), MB_ICONSTOP);
			goto Clean;
		}
		cPostIt.Empty();
	}
#endif

#ifndef ENABLE_PYLIB_CHECK
	if (!CheckPythonLibraryFilenames())
	{
		__ErrorPythonLibraryIsNotExist();
		goto Clean;
	}
#endif

	Main(hInstance, lpCmdLine);

#ifdef USE_NPROTECT_GAMEGUARD
	GameGuard_NoticeMessage();
#endif

#ifdef CEF_BROWSER
	CefWebBrowser_Cleanup();
#else
	WebBrowser_Cleanup();
#endif

	::CoUninitialize();

	if(gs_szErrorString[0])
		MessageBox(NULL, gs_szErrorString, ApplicationStringTable_GetStringz(IDS_APP_NAME, "APP_NAME"), MB_ICONSTOP);

Clean:
#ifdef USE_AHNLAB_HACKSHIELD
	HackShield_Shutdown();
#endif
	SAFE_FREE_GLOBAL(szArgv);

	return 0;
}

static void GrannyError(granny_log_message_type Type, granny_log_message_origin Origin, char const*  File, granny_int32x Line, char const *Error, void *UserData)
{
	if (Origin==GrannyFileReadingLogMessage || Origin==GrannyControlLogMessage || Origin==GrannyMeshBindingLogMessage)
		return;

	TraceError("GRANNY: %s(%d): ERROR: %s --- [%d] %s --- [%d] %s", File, Line, Error, Type, GrannyGetLogMessageTypeString(Type), Origin, GrannyGetLogMessageOriginString(Origin));
}

int Setup(LPSTR lpCmdLine)
{
	/*
	 *	타이머 정밀도를 올린다.
	 */
	TIMECAPS tc;
	UINT wTimerRes;

	if (timeGetDevCaps(&tc, sizeof(TIMECAPS)) != TIMERR_NOERROR)
		return 0;

	wTimerRes = MINMAX(tc.wPeriodMin, 1, tc.wPeriodMax);
	timeBeginPeriod(wTimerRes);

	/*
	 *	그래니 에러 핸들링
	 */

	granny_log_callback Callback;
    Callback.Function = GrannyError;
    Callback.UserData = 0;
    GrannySetLogCallback(&Callback);
	return 1;
}
