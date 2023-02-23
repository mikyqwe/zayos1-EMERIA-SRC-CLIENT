#pragma once

#pragma warning(disable:4702)
#pragma warning(disable:4100)
#pragma warning(disable:4201)
#pragma warning(disable:4511)
#pragma warning(disable:4663)
#pragma warning(disable:4018)
#pragma warning(disable:4245)

#if _MSC_VER >= 1400
//if don't use below, time_t is 64bit
#define _USE_32BIT_TIME_T
#endif
#include <iterator>
#include "../eterLib/StdAfx.h"
#include "../eterPythonLib/StdAfx.h"
#include "../gameLib/StdAfx.h"
#include "../scriptLib/StdAfx.h"
#include "../milesLib/StdAfx.h"
#include "../EffectLib/StdAfx.h"
#include "../PRTerrainLib/StdAfx.h"
#include "../SpeedTreeLib/StdAfx.h"
#include "LoadingWindowMain.h"

#ifndef __D3DRM_H__
#define __D3DRM_H__
#endif

#include <dshow.h>
#include <qedit.h>

#include "Locale.h"
#include "Locale_inc.h"
#include "GameType.h"
extern DWORD __DEFAULT_CODE_PAGE__;

#define APP_NAME	"Metin 2"

enum
{
	POINT_MAX_NUM = 255,
	CHARACTER_NAME_MAX_LEN = 24,
#if defined(LOCALE_SERVICE_JAPAN)
	PLAYER_NAME_MAX_LEN = 16,
#else
	PLAYER_NAME_MAX_LEN = 12,
#endif
	#ifdef ENABLE_MULTI_LANGUAGE_SYSTEM
	LANGUAGE_MAX_LEN = 2,
	LANGUAGE_MAX_NUM = 5,
	#endif
};

void initudp();
void initapp();
void initime();
void initsystemSetting();
void initchr();
void initchrmgr();
void initChat();
void initTextTail();
void initime();
void initItem();
void initNonPlayer();
void initnet();
void initPlayer();
void initSectionDisplayer();
void initServerStateChecker();
#ifdef ENABLE_ANTI_MULTIPLE_FARM
void initAntiMultipleFarmMethods();
#endif
void initTrade();
void initMiniMap();
void initProfiler();
void initEvent();
void initeffect();
void initsnd();
void initeventmgr();
void initBackground();
void initwndMgr();
void initshop();
void initpack();
void initskill();
void initPlayerSettingsModule();
void initfly();
#if defined(GUILD_RANK_SYSTEM)
void initguildranking();
#endif
#if defined(__BL_RANKING__)
void initranking();
#endif
void initquest();
void initsafebox();
#if defined(__BL_MAILBOX__)
void initmail();
#endif
#ifdef ENABLE_WHISPER_RENEWAL
void initWhisper();
#endif
void initguild();
void initMessenger();
#ifdef INGAME_WIKI
void initWiki();
#endif
#if defined(ENABLE_DUNGEON_INFO_SYSTEM)
void initDungeonInfo();
#endif
#ifdef ENABLE_CUBE_RENEWAL_WORLDARD
void intcuberenewal();
#endif
#ifdef ENABLE_CHANGELOOK_SYSTEM
void initChangeLook();
#endif
#ifdef ENABLE_SWITCHBOT
void initSwitchbot();
#endif
#ifdef NEW_PET_SYSTEM
void initskillpet();
#endif
#ifdef ENABLE_ACCE_COSTUME_SYSTEM
void	initAcce();
#endif
#ifdef ENABLE_6_7_BONUS_NEW_SYSTEM
void int67bonus();
#endif
#ifdef RENDER_TARGET
void initRenderTarget();
#endif

extern const std::string& ApplicationStringTable_GetString(DWORD dwID);
extern const std::string& ApplicationStringTable_GetString(DWORD dwID, LPCSTR szKey);

extern const char* ApplicationStringTable_GetStringz(DWORD dwID);
extern const char* ApplicationStringTable_GetStringz(DWORD dwID, LPCSTR szKey);

extern void ApplicationSetErrorString(const char* szErrorString);
