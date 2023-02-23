#include "StdAfx.h"
#include "PythonSystem.h"
#include "PythonApplication.h"

#define DEFAULT_VALUE_ALWAYS_SHOW_NAME		1

void CPythonSystem::SetInterfaceHandler(PyObject * poHandler)
{
// NOTE : ���۷��� ī��Ʈ�� �ٲ��� �ʴ´�. ���۷����� ���� �־� Python���� ������ �������� �ʱ� ����.
//        ��ſ� __del__�� Destroy�� ȣ���� Handler�� NULL�� �����Ѵ�. - [levites]
//	if (m_poInterfaceHandler)
//		Py_DECREF(m_poInterfaceHandler);

	m_poInterfaceHandler = poHandler;

//	if (m_poInterfaceHandler)
//		Py_INCREF(m_poInterfaceHandler);
}

void CPythonSystem::DestroyInterfaceHandler()
{
	m_poInterfaceHandler = NULL;
}

void CPythonSystem::SaveWindowStatus(int iIndex, int iVisible, int iMinimized, int ix, int iy, int iHeight)
{
	m_WindowStatus[iIndex].isVisible	= iVisible;
	m_WindowStatus[iIndex].isMinimized	= iMinimized;
	m_WindowStatus[iIndex].ixPosition	= ix;
	m_WindowStatus[iIndex].iyPosition	= iy;
	m_WindowStatus[iIndex].iHeight		= iHeight;
}

void CPythonSystem::GetDisplaySettings()
{
	memset(m_ResolutionList, 0, sizeof(TResolution) * RESOLUTION_MAX_NUM);
	m_ResolutionCount = 0;

	LPDIRECT3D8 lpD3D = CPythonGraphic::Instance().GetD3D();

	D3DADAPTER_IDENTIFIER8 d3dAdapterIdentifier;
	D3DDISPLAYMODE d3ddmDesktop;

	lpD3D->GetAdapterIdentifier(0, D3DENUM_NO_WHQL_LEVEL, &d3dAdapterIdentifier);
	lpD3D->GetAdapterDisplayMode(0, &d3ddmDesktop);

	// �� ��Ͱ� ������ �ִ� ���÷��� ��尹���� �����Ѵ�..
	DWORD dwNumAdapterModes = lpD3D->GetAdapterModeCount(0);

	for (UINT iMode = 0; iMode < dwNumAdapterModes; iMode++)
	{
		D3DDISPLAYMODE DisplayMode;
		lpD3D->EnumAdapterModes(0, iMode, &DisplayMode);
		DWORD bpp = 0;

		// 800 600 �̻� �ɷ�����.
		if (DisplayMode.Width < 800 || DisplayMode.Height < 600)
			continue;

		// �ϴ� 16bbp �� 32bbp�� �������.
		// 16bbp�� ó���ϰԲ� ���� - [levites]
		if (DisplayMode.Format == D3DFMT_R5G6B5)
			bpp = 16;
		else if (DisplayMode.Format == D3DFMT_X8R8G8B8)
			bpp = 32;
		else
			continue;

		int check_res = false;

		for (int i = 0; !check_res && i < m_ResolutionCount; ++i)
		{
			if (m_ResolutionList[i].bpp != bpp ||
				m_ResolutionList[i].width != DisplayMode.Width ||
				m_ResolutionList[i].height != DisplayMode.Height)
				continue;

			int check_fre = false;

			// �������ø� �ٸ��Ƿ� �������ø� �������ش�.
			for (int j = 0; j < m_ResolutionList[i].frequency_count; ++j)
			{
				if (m_ResolutionList[i].frequency[j] == DisplayMode.RefreshRate)
				{
					check_fre = true;
					break;
				}
			}

			if (!check_fre)
				if (m_ResolutionList[i].frequency_count < FREQUENCY_MAX_NUM)
					m_ResolutionList[i].frequency[m_ResolutionList[i].frequency_count++] = DisplayMode.RefreshRate;

			check_res = true;
		}

		if (!check_res)
		{
			// ���ο� �Ŵϱ� �߰�������.
			if (m_ResolutionCount < RESOLUTION_MAX_NUM)
			{
				m_ResolutionList[m_ResolutionCount].width			= DisplayMode.Width;
				m_ResolutionList[m_ResolutionCount].height			= DisplayMode.Height;
				m_ResolutionList[m_ResolutionCount].bpp				= bpp;
				m_ResolutionList[m_ResolutionCount].frequency[0]	= DisplayMode.RefreshRate;
				m_ResolutionList[m_ResolutionCount].frequency_count	= 1;

				++m_ResolutionCount;
			}
		}
	}
}

int	CPythonSystem::GetResolutionCount()
{
	return m_ResolutionCount;
}

int CPythonSystem::GetFrequencyCount(int index)
{
	if (index >= m_ResolutionCount)
		return 0;

    return m_ResolutionList[index].frequency_count;
}

bool CPythonSystem::GetResolution(int index, OUT DWORD *width, OUT DWORD *height, OUT DWORD *bpp)
{
	if (index >= m_ResolutionCount)
		return false;

	*width = m_ResolutionList[index].width;
	*height = m_ResolutionList[index].height;
	*bpp = m_ResolutionList[index].bpp;
	return true;
}

bool CPythonSystem::GetFrequency(int index, int freq_index, OUT DWORD *frequncy)
{
	if (index >= m_ResolutionCount)
		return false;

	if (freq_index >= m_ResolutionList[index].frequency_count)
		return false;

	*frequncy = m_ResolutionList[index].frequency[freq_index];
	return true;
}

int	CPythonSystem::GetResolutionIndex(DWORD width, DWORD height, DWORD bit)
{
	DWORD re_width, re_height, re_bit;
	int i = 0;

	while (GetResolution(i, &re_width, &re_height, &re_bit))
	{
		if (re_width == width)
			if (re_height == height)
				if (re_bit == bit)
					return i;
		i++;
	}

	return 0;
}

int	CPythonSystem::GetFrequencyIndex(int res_index, DWORD frequency)
{
	DWORD re_frequency;
	int i = 0;

	while (GetFrequency(res_index, i, &re_frequency))
	{
		if (re_frequency == frequency)
			return i;

		i++;
	}

	return 0;
}

DWORD CPythonSystem::GetWidth()
{
	return m_Config.width;
}

DWORD CPythonSystem::GetHeight()
{
	return m_Config.height;
}
DWORD CPythonSystem::GetBPP()
{
	return m_Config.bpp;
}
DWORD CPythonSystem::GetFrequency()
{
	return m_Config.frequency;
}

bool CPythonSystem::IsNoSoundCard()
{
	return m_Config.bNoSoundCard;
}

bool CPythonSystem::IsSoftwareCursor()
{
	return m_Config.is_software_cursor;
}

float CPythonSystem::GetMusicVolume()
{
	return m_Config.music_volume;
}

int CPythonSystem::GetSoundVolume()
{
	return m_Config.voice_volume;
}

void CPythonSystem::SetMusicVolume(float fVolume)
{
	m_Config.music_volume = fVolume;
}

void CPythonSystem::SetSoundVolumef(float fVolume)
{
	m_Config.voice_volume = int(5 * fVolume);
}

int CPythonSystem::GetDistance()
{
	return m_Config.iDistance;
}

int CPythonSystem::GetShadowLevel()
{
	return m_Config.iShadowLevel;
}

void CPythonSystem::SetShadowLevel(unsigned int level)
{
	m_Config.iShadowLevel = MIN(level, 5);
	CPythonBackground::instance().RefreshShadowLevel();
}

int CPythonSystem::IsSaveID()
{
	return m_Config.isSaveID;
}

#ifdef ENABLE_SHOPNAMES_RANGE
void CPythonSystem::SetShopNamesRange(float fRange)
{
	m_Config.shopnames_range = fRange;
}

float CPythonSystem::GetShopNamesRange()
{
	return m_Config.shopnames_range;
}
#endif

const char * CPythonSystem::GetSaveID()
{
	return m_Config.SaveID;
}

bool CPythonSystem::isViewCulling()
{
	return m_Config.is_object_culling;
}

void CPythonSystem::SetSaveID(int iValue, const char * c_szSaveID)
{
	if (iValue != 1)
		return;

	m_Config.isSaveID = iValue;
	strncpy(m_Config.SaveID, c_szSaveID, sizeof(m_Config.SaveID) - 1);
}

const char * CPythonSystem::GetTransLangKey()
{
	return m_Config.TransLangKey;
}

void CPythonSystem::SetTransLangKey(const char * c_szTransLangKey)
{
	strncpy(m_Config.TransLangKey, c_szTransLangKey, sizeof(m_Config.TransLangKey) - 1);
}


CPythonSystem::TConfig * CPythonSystem::GetConfig()
{
	return &m_Config;
}

void CPythonSystem::SetConfig(TConfig * pNewConfig)
{
	m_Config = *pNewConfig;
}

void CPythonSystem::SetDefaultConfig()
{
	memset(&m_Config, 0, sizeof(m_Config));

	m_Config.width				= 1024;
	m_Config.height				= 768;
	m_Config.bpp				= 32;

#if defined( LOCALE_SERVICE_WE_JAPAN )
	m_Config.bWindowed			= true;
#else
	m_Config.bWindowed			= false;
#endif
	m_Config.is_software_cursor	= false;
	m_Config.is_object_culling	= true;
	m_Config.iDistance			= 3;

	m_Config.gamma				= 3;
	m_Config.music_volume		= 1.0f;
	m_Config.voice_volume		= 5;

	m_Config.bDecompressDDS		= 0;
	m_Config.bSoftwareTiling	= 0;
	m_Config.iShadowLevel		= 3;
#if defined(ENABLE_FOV_OPTION)
	m_Config.fFOV = c_fDefaultCameraPerspective;
#endif
#ifdef ENABLE_REFINE_RENEWAL	
	m_Config.bRefineStatus = true;
#endif
	m_Config.bViewChat			= true;
	m_Config.bAlwaysShowName	= DEFAULT_VALUE_ALWAYS_SHOW_NAME;
	m_Config.bShowDamage		= true;
#ifdef ENABLE_FPS
	m_Config.iFps = 1;
#endif
#ifdef ENABLE_HIDE_PET
	m_Config.bHidePets = false;
#endif
#ifdef ENABLE_HIDE_MOUNT
	m_Config.bHideMounts = false;
#endif
#ifdef ENABLE_HIDE_SHOPS
	m_Config.bHideShops = false;
#endif
	m_Config.bShowSalesText		= true;
#ifdef ENABLE_DOG_MODE
	m_Config.bDogMode = false;
#endif
#ifdef ENABLE_ENB_MODE
	m_Config.bENBModeStatus = false;
#endif
#if defined(__BL_HIDE_EFFECT__)
	m_Config.bBuffEffect = true;
	m_Config.bSkillEffect = true;
#endif
#ifdef ENABLE_SHOPNAMES_RANGE
	m_Config.shopnames_range = 0.0f;
#endif
#ifdef ENABLE_FOG_FIX
	m_Config.bFogMode			= false;
#endif
#if defined(WJ_SHOW_MOB_INFO) && defined(ENABLE_SHOW_MOBAIFLAG)
	m_Config.bShowMobAIFlag		= true;
#endif
#if defined(WJ_SHOW_MOB_INFO) && defined(ENABLE_SHOW_MOBLEVEL)
	m_Config.bShowMobLevel		= true;
#endif
#ifdef ULTIMATE_PICKUP
	m_Config.bPickupMode = false;
	m_Config.bPickupAutoMode = false;
	m_Config.bPickupFilter = 0;
#endif

}

bool CPythonSystem::IsWindowed()
{
	return m_Config.bWindowed;
}

bool CPythonSystem::IsViewChat()
{
	return m_Config.bViewChat;
}

void CPythonSystem::SetViewChatFlag(int iFlag)
{
	m_Config.bViewChat = iFlag == 1 ? true : false;
}

int CPythonSystem::IsAlwaysShowName()
{
	return m_Config.bAlwaysShowName;
}

void CPythonSystem::SetAlwaysShowNameFlag(int iFlag)
{
	m_Config.bAlwaysShowName = iFlag;
}

bool CPythonSystem::IsShowDamage()
{
	return m_Config.bShowDamage;
}

void CPythonSystem::SetShowDamageFlag(int iFlag)
{
	m_Config.bShowDamage = iFlag == 1 ? true : false;
}

bool CPythonSystem::IsShowSalesText()
{
	return m_Config.bShowSalesText;
}

void CPythonSystem::SetShowSalesTextFlag(int iFlag)
{
	m_Config.bShowSalesText = iFlag == 1 ? true : false;
}

#ifdef ENABLE_DOG_MODE
void CPythonSystem::SetDogMode(int iFlag)
{
	m_Config.bDogMode = iFlag == 1 ? true : false;

	CPythonCharacterManager& rkChrMgr = CPythonCharacterManager::Instance();
	rkChrMgr.SetDogMode(m_Config.bDogMode);
}

bool CPythonSystem::GetDogMode()
{
	return m_Config.bDogMode;
}
#endif

#ifdef ENABLE_REMEMBER_LAST_SPLIT
int CPythonSystem::GetLastRememberSplitData()
{
	return m_Config.iRememberSplit;
}
void CPythonSystem::SetLastRemeberSplitData(int iFlag)
{
	m_Config.iRememberSplit = iFlag;
}
#endif

#ifdef ENABLE_CHAT_COLOR_SYSTEM
bool CPythonSystem::GetChatColor()
{
	return m_Config.bChatColor;
}

void CPythonSystem::SetChatColor(int iFlag)
{
	m_Config.bChatColor = iFlag == 1 ? true : false;
}
#endif

#ifdef ENABLE_FOG_FIX
void CPythonSystem::SetFogMode(int iFlag)
{
	m_Config.bFogMode = iFlag == 1 ? true : false;
}

bool CPythonSystem::IsFogMode()
{
	return m_Config.bFogMode;
}
#endif

#if defined(WJ_SHOW_MOB_INFO) && defined(ENABLE_SHOW_MOBAIFLAG)
bool CPythonSystem::IsShowMobAIFlag()
{
	return m_Config.bShowMobAIFlag;
}

void CPythonSystem::SetShowMobAIFlagFlag(int iFlag)
{
	m_Config.bShowMobAIFlag = iFlag == 1 ? true : false;
}
#endif
#if defined(WJ_SHOW_MOB_INFO) && defined(ENABLE_SHOW_MOBLEVEL)
bool CPythonSystem::IsShowMobLevel()
{
	return m_Config.bShowMobLevel;
}

void CPythonSystem::SetShowMobLevelFlag(int iFlag)
{
	m_Config.bShowMobLevel = iFlag == 1 ? true : false;
}
#endif

bool CPythonSystem::IsAutoTiling()
{
	if (m_Config.bSoftwareTiling == 0)
		return true;

	return false;
}

#ifdef ENABLE_ENB_MODE
bool CPythonSystem::IsENBModeStatus()
{
	return m_Config.bENBModeStatus;
}
void CPythonSystem::SetENBModeStatusFlag(int iFlag)
{
	m_Config.bENBModeStatus = iFlag == 1 ? true : false;
}
#endif

#ifdef ENABLE_REFINE_RENEWAL
bool CPythonSystem::IsRefineStatusShow()
{
	return m_Config.bRefineStatus;
}

void CPythonSystem::SetRefineStatus(int iFlag)
{
	m_Config.bRefineStatus = iFlag == 1 ? true : false;
}
#endif

void CPythonSystem::SetSoftwareTiling(bool isEnable)
{
	if (isEnable)
		m_Config.bSoftwareTiling=1;
	else
		m_Config.bSoftwareTiling=2;
}

bool CPythonSystem::IsSoftwareTiling()
{
	if (m_Config.bSoftwareTiling==1)
		return true;

	return false;
}

#if defined(ENABLE_FOV_OPTION)
float CPythonSystem::GetFOV()
{
	return m_Config.fFOV;
}

void CPythonSystem::SetFOV(float fFlag)
{
	m_Config.fFOV = fMINMAX(c_fMinCameraPerspective, fFlag, c_fMaxCameraPerspective);
}
#endif

bool CPythonSystem::IsUseDefaultIME()
{
	return m_Config.bUseDefaultIME;
}

bool CPythonSystem::LoadConfig()
{
	FILE * fp = NULL;

	if (NULL == (fp = fopen("metin2.cfg", "rt")))
		return false;

	char buf[256];
	char command[256];
	char value[256];

	while (fgets(buf, 256, fp))
	{
		if (sscanf(buf, " %s %s\n", command, value) == EOF)
			break;

		if (!stricmp(command, "WIDTH"))
			m_Config.width = atoi(value);
		else if (!stricmp(command, "HEIGHT"))
			m_Config.height = atoi(value);
		else if (!stricmp(command, "BPP"))
			m_Config.bpp = atoi(value);
		else if (!stricmp(command, "FREQUENCY"))
			m_Config.frequency = atoi(value);
		else if (!stricmp(command, "SOFTWARE_CURSOR"))
			m_Config.is_software_cursor = atoi(value) ? true : false;
		else if (!stricmp(command, "OBJECT_CULLING"))
			m_Config.is_object_culling = atoi(value) ? true : false;
		else if (!stricmp(command, "VISIBILITY"))
			m_Config.iDistance = atoi(value);
		else if (!stricmp(command, "MUSIC_VOLUME")) {
			if (strchr(value, '.') == 0) { // Old compatiability
				m_Config.music_volume = pow(10.0f, (-1.0f + (((float)atoi(value)) / 5.0f)));
				if (atoi(value) == 0)
					m_Config.music_volume = 0.0f;
			}
			else
				m_Config.music_volume = atof(value);
		}
		else if (!stricmp(command, "VOICE_VOLUME"))
			m_Config.voice_volume = (char)atoi(value);
		else if (!stricmp(command, "GAMMA"))
			m_Config.gamma = atoi(value);
		else if (!stricmp(command, "IS_SAVE_ID"))
			m_Config.isSaveID = atoi(value);
		else if (!stricmp(command, "SAVE_ID"))
			strncpy(m_Config.SaveID, value, 20);
		else if (!stricmp(command, "PRE_LOADING_DELAY_TIME"))
			g_iLoadingDelayTime = atoi(value);
		else if (!stricmp(command, "WINDOWED"))
		{
			m_Config.bWindowed = atoi(value) == 1 ? true : false;
		}
		else if (!stricmp(command, "USE_DEFAULT_IME"))
			m_Config.bUseDefaultIME = atoi(value) == 1 ? true : false;
		else if (!stricmp(command, "SOFTWARE_TILING"))
			m_Config.bSoftwareTiling = atoi(value);
#if defined(ENABLE_FOV_OPTION)
		else if (!stricmp(command, "FOV"))
		{
			m_Config.fFOV = atof(value);
		}
#endif
#ifdef ENABLE_FPS
		else if (!stricmp(command, "FRAME_VALUE"))
			m_Config.iFps = atoi(value);
#endif
		else if (!stricmp(command, "SHADOW_LEVEL"))
			m_Config.iShadowLevel = atoi(value);
		else if (!stricmp(command, "DECOMPRESSED_TEXTURE"))
			m_Config.bDecompressDDS = atoi(value) == 1 ? true : false;
		else if (!stricmp(command, "NO_SOUND_CARD"))
			m_Config.bNoSoundCard = atoi(value) == 1 ? true : false;
		else if (!stricmp(command, "VIEW_CHAT"))
			m_Config.bViewChat = atoi(value) == 1 ? true : false;
		else if (!stricmp(command, "ALWAYS_VIEW_NAME"))
			m_Config.bAlwaysShowName = atoi(value);
		else if (!stricmp(command, "SHOW_DAMAGE"))
			m_Config.bShowDamage = atoi(value) == 1 ? true : false;
		else if (!stricmp(command, "SHOW_SALESTEXT"))
			m_Config.bShowSalesText = atoi(value) == 1 ? true : false;
#ifdef ENABLE_DOG_MODE
		else if (!stricmp(command, "DOG_MODE_ON"))
			m_Config.bDogMode = atoi(value) == 1 ? true : false;
#endif
#ifdef ENABLE_CHAT_COLOR_SYSTEM
		else if (!stricmp(command, "MAVIRUH_CHAT_COLOR"))
			m_Config.bChatColor = atoi(value) == 1 ? true : false;
#endif
#ifdef ENABLE_ENB_MODE
		else if (!stricmp(command, "ENB_MODE_STATUS"))
			m_Config.bENBModeStatus = atoi(value) == 1 ? true : false;
#endif

#ifdef ENABLE_REMEMBER_LAST_SPLIT
		else if (!stricmp(command, "SPLIT_LAST_DATA"))
			m_Config.iRememberSplit = atoi(value);
#endif
#if defined(__BL_HIDE_EFFECT__)
		else if (!stricmp(command, "BUFF_EFFECT"))
			SetBLEffectOption(0, (atoi(value) == 1));
		else if (!stricmp(command, "SKILL_EFFECT"))
			SetBLEffectOption(1, (atoi(value) == 1));
#endif
#ifdef ENABLE_HIDE_PET
		else if (!stricmp(command, "HIDE_PETS"))
			m_Config.bHidePets = atoi(value) == 1 ? true : false;
#endif
#ifdef ENABLE_HIDE_MOUNT
		else if (!stricmp(command, "HIDE_MOUNTS"))
			m_Config.bHideMounts = atoi(value) == 1 ? true : false;
#endif
#ifdef ENABLE_HIDE_SHOPS
		else if (!stricmp(command, "HIDE_SHOPS"))
			m_Config.bHideShops = atoi(value) == 1 ? true : false;
#endif
		else if (!stricmp(command, "TRANS_LANG_KEY"))
			strncpy(m_Config.TransLangKey, value, 5);
#ifdef ENABLE_SHOPNAMES_RANGE
		else if (!stricmp(command, "SHOPNAMES_RANGE"))
		{
			if (strchr(value, '.') == 0)
			{
				m_Config.shopnames_range = pow(10.0f, (-1.0f + (((float)atoi(value)) / 5.0f)));
				if (atoi(value) == 0)
					m_Config.shopnames_range = 0.0f;
			}
			else
				m_Config.shopnames_range = atof(value);
		}
#endif
#ifdef ENABLE_FOG_FIX
		else if (!stricmp(command, "FOG_MODE_ON"))
			m_Config.bFogMode = atoi(value) == 1 ? true : false;
#endif
#ifdef ENABLE_REFINE_RENEWAL
		else if (!stricmp(command, "SHOW_REFINE_DIALOG"))
			m_Config.bRefineStatus = atoi(value) == 1 ? true : false;
#endif
#if defined(WJ_SHOW_MOB_INFO) && defined(ENABLE_SHOW_MOBAIFLAG)
		else if (!stricmp(command, "SHOW_MOBAIFLAG"))
			m_Config.bShowMobAIFlag = atoi(value) == 1 ? true : false;
#endif
#if defined(WJ_SHOW_MOB_INFO) && defined(ENABLE_SHOW_MOBLEVEL)
		else if (!stricmp(command, "SHOW_MOBLEVEL"))
			m_Config.bShowMobLevel = atoi(value) == 1 ? true : false;
#endif
#ifdef ULTIMATE_PICKUP
		else if (!stricmp(command, "PICKUP_MODE"))
			m_Config.bPickupMode = atoi(value) == 1 ? true : false;
		else if (!stricmp(command, "PICKUP_AUTO_MODE"))
			m_Config.bPickupAutoMode = atoi(value) == 1 ? true : false;
		else if (!stricmp(command, "PICKUP_FLAG"))
			m_Config.bPickupFilter = atoi(value);
#endif
	}

	if (m_Config.bWindowed)
	{
		unsigned screen_width = GetSystemMetrics(SM_CXFULLSCREEN);
		unsigned screen_height = GetSystemMetrics(SM_CYFULLSCREEN);

		if (m_Config.width >= screen_width)
		{
			m_Config.width = screen_width;
		}
		if (m_Config.height >= screen_height)
		{
			m_Config.height = screen_height;
		}
	}

	m_OldConfig = m_Config;

	fclose(fp);

//	Tracef("LoadConfig: Resolution: %dx%d %dBPP %dHZ Software Cursor: %d, Music/Voice Volume: %d/%d Gamma: %d\n",
//		m_Config.width,
//		m_Config.height,
//		m_Config.bpp,
//		m_Config.frequency,
//		m_Config.is_software_cursor,
//		m_Config.music_volume,
//		m_Config.voice_volume,
//		m_Config.gamma);

	return true;
}

bool CPythonSystem::SaveConfig()
{
	FILE *fp;

	if (NULL == (fp = fopen("metin2.cfg", "wt")))
		return false;

	fprintf(fp, "WIDTH						%d\n"
		"HEIGHT						%d\n"
		"BPP						%d\n"
		"FREQUENCY					%d\n"
		"SOFTWARE_CURSOR			%d\n"
		"OBJECT_CULLING				%d\n"
		"VISIBILITY					%d\n"
		"MUSIC_VOLUME				%.3f\n"
		"VOICE_VOLUME				%d\n"
		"GAMMA						%d\n"
		"IS_SAVE_ID					%d\n"
		"SAVE_ID					%s\n"
		"TRANS_LANG_KEY				%s\n"
		"PRE_LOADING_DELAY_TIME		%d\n"
		"DECOMPRESSED_TEXTURE		%d\n"
#ifdef ENABLE_SHOPNAMES_RANGE
		"SHOPNAMES_RANGE		%.3f\n",
#endif
		m_Config.width,
		m_Config.height,
		m_Config.bpp,
		m_Config.frequency,
		m_Config.is_software_cursor,
		m_Config.is_object_culling,
		m_Config.iDistance,
		m_Config.music_volume,
		m_Config.voice_volume,
		m_Config.gamma,
		m_Config.isSaveID,
		m_Config.SaveID,
		m_Config.TransLangKey,
		g_iLoadingDelayTime,
		m_Config.bDecompressDDS,
#ifdef ENABLE_SHOPNAMES_RANGE
		m_Config.shopnames_range);
#endif

	if (m_Config.bWindowed == 1)
		fprintf(fp, "WINDOWED				%d\n", m_Config.bWindowed);
	if (m_Config.bViewChat == 0)
		fprintf(fp, "VIEW_CHAT				%d\n", m_Config.bViewChat);
	fprintf(fp, "ALWAYS_VIEW_NAME		%d\n", m_Config.bAlwaysShowName);
	if (m_Config.bShowDamage == 0)
		fprintf(fp, "SHOW_DAMAGE		%d\n", m_Config.bShowDamage);
	if (m_Config.bShowSalesText == 0)
		fprintf(fp, "SHOW_SALESTEXT		%d\n", m_Config.bShowSalesText);
#ifdef ENABLE_DOG_MODE
	fprintf(fp, "DOG_MODE_ON			%d\n", m_Config.bDogMode);
#endif
#ifdef ENABLE_CHAT_COLOR_SYSTEM
	fprintf(fp, "MAVIRUH_CHAT_COLOR			%d\n", m_Config.bChatColor);
#endif
#ifdef ENABLE_HIDE_PET
	fprintf(fp, "HIDE_PETS				%d\n", m_Config.bHidePets);
#endif
#ifdef ENABLE_HIDE_MOUNT
	fprintf(fp, "HIDE_MOUNTS			%d\n", m_Config.bHideMounts);
#endif
#ifdef ENABLE_HIDE_SHOPS
	fprintf(fp, "HIDE_SHOPS				%d\n", m_Config.bHideShops);
#endif
#ifdef ENABLE_FOG_FIX
	if (m_Config.bFogMode == 0)
		fprintf(fp, "FOG_MODE_ON		%d\n", m_Config.bFogMode);
#endif
#ifdef ENABLE_REMEMBER_LAST_SPLIT
	if (m_Config.iRememberSplit == 0)
		fprintf(fp, "LAST_SPLIT_DATA		%d\n", m_Config.iRememberSplit);
#endif
#ifdef ENABLE_REFINE_RENEWAL
	fprintf(fp, "SHOW_REFINE_DIALOG		%d\n", m_Config.bRefineStatus);
#endif
#if defined(WJ_SHOW_MOB_INFO) && defined(ENABLE_SHOW_MOBAIFLAG)
	if (m_Config.bShowMobAIFlag == 0)
		fprintf(fp, "SHOW_MOBAIFLAG		%d\n", m_Config.bShowMobAIFlag);
#endif
#if defined(WJ_SHOW_MOB_INFO) && defined(ENABLE_SHOW_MOBLEVEL)
	if (m_Config.bShowMobLevel == 0)
		fprintf(fp, "SHOW_MOBLEVEL		%d\n", m_Config.bShowMobLevel);
#endif
	fprintf(fp, "USE_DEFAULT_IME		%d\n", m_Config.bUseDefaultIME);
#if defined(__BL_HIDE_EFFECT__)
	fprintf(fp, "BUFF_EFFECT		%d\n", m_Config.bBuffEffect);
	fprintf(fp, "SKILL_EFFECT		%d\n", m_Config.bSkillEffect);
#endif
	fprintf(fp, "SOFTWARE_TILING		%d\n", m_Config.bSoftwareTiling);
#ifdef ENABLE_FPS
	fprintf(fp, "FRAME_VALUE			%d\n", m_Config.iFps);
#endif
	fprintf(fp, "SHADOW_LEVEL			%d\n", m_Config.iShadowLevel);
#ifdef ENABLE_ENB_MODE
	fprintf(fp, "ENB_MODE_STATUS			%d\n", m_Config.bENBModeStatus);
#endif
#ifdef ULTIMATE_PICKUP
	fprintf(fp, "PICKUP_MODE			%d\n", m_Config.bPickupMode);
	fprintf(fp, "PICKUP_AUTO_MODE			%d\n", m_Config.bPickupAutoMode);
	fprintf(fp, "PICKUP_FLAG			%d\n", m_Config.bPickupFilter);
#endif
#if defined(ENABLE_FOV_OPTION)
	fprintf(fp, "FOV						%.2f\n", m_Config.fFOV);
#endif

	fprintf(fp, "\n");

	fclose(fp);
	return true;
}

bool CPythonSystem::LoadInterfaceStatus()
{
	FILE * File;
	File = fopen("interface.cfg", "rb");

	if (!File)
		return false;

	fread(m_WindowStatus, 1, sizeof(TWindowStatus) * WINDOW_MAX_NUM, File);
	fclose(File);
	return true;
}

void CPythonSystem::SaveInterfaceStatus()
{
	if (!m_poInterfaceHandler)
		return;

	PyCallClassMemberFunc(m_poInterfaceHandler, "OnSaveInterfaceStatus", Py_BuildValue("()"));

	FILE * File;

	File = fopen("interface.cfg", "wb");

	if (!File)
	{
		TraceError("Cannot open interface.cfg");
		return;
	}

	fwrite(m_WindowStatus, 1, sizeof(TWindowStatus) * WINDOW_MAX_NUM, File);
	fclose(File);
}

bool CPythonSystem::isInterfaceConfig()
{
	return m_isInterfaceConfig;
}

const CPythonSystem::TWindowStatus & CPythonSystem::GetWindowStatusReference(int iIndex)
{
	return m_WindowStatus[iIndex];
}

void CPythonSystem::ApplyConfig() // ���� ������ ���� ������ ���ؼ� �ٲ� ������ ���� �Ѵ�.
{
	if (m_OldConfig.gamma != m_Config.gamma)
	{
		float val = 1.0f;

		switch (m_Config.gamma)
		{
			case 0: val = 0.4f;	break;
			case 1: val = 0.7f; break;
			case 2: val = 1.0f; break;
			case 3: val = 1.2f; break;
			case 4: val = 1.4f; break;
		}

		CPythonGraphic::Instance().SetGamma(val);
	}

	if (m_OldConfig.is_software_cursor != m_Config.is_software_cursor)
	{
		if (m_Config.is_software_cursor)
			CPythonApplication::Instance().SetCursorMode(CPythonApplication::CURSOR_MODE_SOFTWARE);
		else
			CPythonApplication::Instance().SetCursorMode(CPythonApplication::CURSOR_MODE_HARDWARE);
	}

	m_OldConfig = m_Config;

	ChangeSystem();
}

void CPythonSystem::ChangeSystem()
{
	// Shadow
	/*
	if (m_Config.is_shadow)
		CScreen::SetShadowFlag(true);
	else
		CScreen::SetShadowFlag(false);
	*/
	CSoundManager& rkSndMgr = CSoundManager::Instance();
	/*
	float fMusicVolume;
	if (0 == m_Config.music_volume)
		fMusicVolume = 0.0f;
	else
		fMusicVolume= (float)pow(10.0f, (-1.0f + (float)m_Config.music_volume / 5.0f));
		*/
	rkSndMgr.SetMusicVolume(m_Config.music_volume);

	/*
	float fVoiceVolume;
	if (0 == m_Config.voice_volume)
		fVoiceVolume = 0.0f;
	else
		fVoiceVolume = (float)pow(10.0f, (-1.0f + (float)m_Config.voice_volume / 5.0f));
	*/
	rkSndMgr.SetSoundVolumeGrade(m_Config.voice_volume);
}

void CPythonSystem::Clear()
{
	SetInterfaceHandler(NULL);
}

#ifdef ENABLE_FPS
void CPythonSystem::SetFPS(BYTE iFlag)
{
	m_Config.iFps = iFlag;
	CPythonApplication::Instance().SetAppFPS(iFlag);
} //#MISSING
#endif

#if defined(__BL_HIDE_EFFECT__)
bool CPythonSystem::GetBLEffectOption(std::uint8_t idx)
{
	switch (idx)
	{
	case 0: // BUFF
		return m_Config.bBuffEffect;
	case 1: // SKILL
		return m_Config.bSkillEffect;
	default:
		return false;
	}
}

void CPythonSystem::SetBLEffectOption(std::uint8_t idx, bool ret)
{
	switch (idx)
	{
	case 0: // BUFF
		m_Config.bBuffEffect = ret;
		break;
	case 1: // SKILL
		m_Config.bSkillEffect = ret;
		break;
	default:
		return;
	}
	CEffectManager::Instance().HideOnSomeEffects(idx, ret);
}
#endif


CPythonSystem::CPythonSystem()
{
	memset(&m_Config, 0, sizeof(TConfig));

	m_poInterfaceHandler = NULL;

	SetDefaultConfig();

	LoadConfig();

	ChangeSystem();

	if (LoadInterfaceStatus())
		m_isInterfaceConfig = true;
	else
		m_isInterfaceConfig = false;
}

CPythonSystem::~CPythonSystem()
{
	assert(m_poInterfaceHandler==NULL && "CPythonSystem MUST CLEAR!");
}