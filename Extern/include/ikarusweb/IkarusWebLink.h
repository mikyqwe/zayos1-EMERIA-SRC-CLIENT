#pragma once

#pragma comment(lib , "Shlwapi.lib")
#pragma comment(lib,  "WS2_32")
#pragma comment(lib,  "Wininet.lib")
#pragma comment(lib, "fwpuclnt.lib")
#pragma comment(lib, "ntdsapi.lib")

#ifdef _WIN64
#	ifdef _DEBUG
#define __LINK_IKARUSWEB_NAME "ikarusweb_x64_d.lib"
#	else
#define __LINK_IKARUSWEB_NAME "ikarusweb_x64.lib"
#	endif
#else
#	ifdef _DEBUG
#define __LINK_IKARUSWEB_NAME "ikarusweb(MTd).lib"
#	else
#define __LINK_IKARUSWEB_NAME "ikarusweb(MT).lib"
#	endif
#endif

#pragma comment(lib, __LINK_IKARUSWEB_NAME)
#undef __LINK_IKARUSWEB_NAME