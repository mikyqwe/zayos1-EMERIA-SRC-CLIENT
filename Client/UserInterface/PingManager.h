#pragma once
#include "StdAfx.h"

#ifdef ENABLE_PINGTIME
class CPingManager : public CSingleton<CPingManager>
{

public:
	int __cdecl CPingManager::PingTimeRev();
	void __cdecl CPingManager::Shut();
};
#endif





