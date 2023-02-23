#pragma once

#pragma comment(lib , "Shlwapi.lib")
#pragma comment(lib,  "WS2_32")
#pragma comment(lib,  "Wininet.lib")
#pragma comment(lib, "fwpuclnt.lib")
#pragma comment(lib, "ntdsapi.lib")

#ifdef _WIN64
#	ifdef _DEBUG
#define __LINK_ikarusweb2_NAME "ikarusweb2_x64_d.lib"
#	else
#define __LINK_ikarusweb2_NAME "ikarusweb2_x64.lib"
#	endif
#else
#	ifdef _DEBUG
#define __LINK_ikarusweb2_NAME "ikarusweb2(MTd).lib"
#	else
#define __LINK_ikarusweb2_NAME "ikarusweb2(MT).lib"
#	endif
#endif

#pragma comment(lib, __LINK_ikarusweb2_NAME)
#undef __LINK_ikarusweb2_NAME