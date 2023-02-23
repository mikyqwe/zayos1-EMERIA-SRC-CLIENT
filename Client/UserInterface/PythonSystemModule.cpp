#include "StdAfx.h"
#include "PythonSystem.h"

PyObject * systemGetWidth(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().GetWidth());
}

PyObject * systemGetHeight(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().GetHeight());
}

PyObject * systemSetInterfaceHandler(PyObject* poSelf, PyObject* poArgs)
{
	PyObject* poHandler;
	if (!PyTuple_GetObject(poArgs, 0, &poHandler))
		return Py_BuildException();

	CPythonSystem::Instance().SetInterfaceHandler(poHandler);
	return Py_BuildNone();
}

PyObject * systemDestroyInterfaceHandler(PyObject* poSelf, PyObject* poArgs)
{
	CPythonSystem::Instance().DestroyInterfaceHandler();
	return Py_BuildNone();
}

PyObject * systemReserveResource(PyObject* poSelf, PyObject* poArgs)
{
	std::set<std::string> ResourceSet;
	CResourceManager::Instance().PushBackgroundLoadingSet(ResourceSet);
	return Py_BuildNone();
}

PyObject * systemisInterfaceConfig(PyObject* poSelf, PyObject* poArgs)
{
	int isInterfaceConfig = CPythonSystem::Instance().isInterfaceConfig();
	return Py_BuildValue("i", isInterfaceConfig);
}

PyObject * systemSaveWindowStatus(PyObject* poSelf, PyObject* poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildException();

	int iVisible;
	if (!PyTuple_GetInteger(poArgs, 1, &iVisible))
		return Py_BuildException();

	int iMinimized;
	if (!PyTuple_GetInteger(poArgs, 2, &iMinimized))
		return Py_BuildException();

	int ix;
	if (!PyTuple_GetInteger(poArgs, 3, &ix))
		return Py_BuildException();

	int iy;
	if (!PyTuple_GetInteger(poArgs, 4, &iy))
		return Py_BuildException();

	int iHeight;
	if (!PyTuple_GetInteger(poArgs, 5, &iHeight))
		return Py_BuildException();

	CPythonSystem::Instance().SaveWindowStatus(iIndex, iVisible, iMinimized, ix, iy, iHeight);
	return Py_BuildNone();
}

PyObject * systemGetWindowStatus(PyObject* poSelf, PyObject* poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildException();

	const CPythonSystem::TWindowStatus & c_rWindowStatus = CPythonSystem::Instance().GetWindowStatusReference(iIndex);
	return Py_BuildValue("iiiii", c_rWindowStatus.isVisible,
								  c_rWindowStatus.isMinimized,
								  c_rWindowStatus.ixPosition,
								  c_rWindowStatus.iyPosition,
								  c_rWindowStatus.iHeight);
}

PyObject * systemGetConfig(PyObject * poSelf, PyObject * poArgs)
{
	CPythonSystem::TConfig *tmp = CPythonSystem::Instance().GetConfig();

	int iRes = CPythonSystem::Instance().GetResolutionIndex(tmp->width, tmp->height, tmp->bpp);
	int iFrequency = CPythonSystem::Instance().GetFrequencyIndex(iRes, tmp->frequency);

	return Py_BuildValue("iiiiiiiii",  iRes,
									  iFrequency,
									  tmp->is_software_cursor,
									  tmp->is_object_culling,
									  tmp->music_volume,
									  tmp->voice_volume,
									  tmp->gamma,
									  tmp->iDistance,
									  tmp->shopnames_range);
}

PyObject * systemSetSaveID(PyObject * poSelf, PyObject * poArgs)
{
	int iValue;
	if (!PyTuple_GetInteger(poArgs, 0, &iValue))
		return Py_BuildException();

	char * szSaveID;
	if (!PyTuple_GetString(poArgs, 1, &szSaveID))
		return Py_BuildException();

	CPythonSystem::Instance().SetSaveID(iValue, szSaveID);
	return Py_BuildNone();
}

PyObject * systemisSaveID(PyObject * poSelf, PyObject * poArgs)
{
	int value = CPythonSystem::Instance().IsSaveID();
	return Py_BuildValue("i", value);
}

PyObject * systemGetSaveID(PyObject * poSelf, PyObject * poArgs)
{
	const char * c_szSaveID = CPythonSystem::Instance().GetSaveID();
	return Py_BuildValue("s", c_szSaveID);
}

PyObject * systemSetTransLangKey(PyObject * poSelf, PyObject * poArgs)
{
	char * szTransLangKey;
	if (!PyTuple_GetString(poArgs, 0, &szTransLangKey))
		return Py_BuildException();

	CPythonSystem::Instance().SetTransLangKey(szTransLangKey);
	return Py_BuildNone();
}

PyObject * systemGetTransLangKey(PyObject * poSelf, PyObject * poArgs)
{
	const char * c_szTransLangKey = CPythonSystem::Instance().GetTransLangKey();
	return Py_BuildValue("s", c_szTransLangKey);
}

PyObject * systemGetMusicVolume(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("f", CPythonSystem::Instance().GetMusicVolume());
}

PyObject * systemGetSoundVolume(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().GetSoundVolume());
}

PyObject * systemSetMusicVolume(PyObject * poSelf, PyObject * poArgs)
{
	float fVolume;
	if (!PyTuple_GetFloat(poArgs, 0, &fVolume))
		return Py_BuildException();

	CPythonSystem::Instance().SetMusicVolume(fVolume);
	return Py_BuildNone();
}

PyObject * systemSetSoundVolumef(PyObject * poSelf, PyObject * poArgs)
{
	float fVolume;
	if (!PyTuple_GetFloat(poArgs, 0, &fVolume))
		return Py_BuildException();

	CPythonSystem::Instance().SetSoundVolumef(fVolume);
	return Py_BuildNone();
}

PyObject * systemIsSoftwareCursor(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().IsSoftwareCursor());
}

PyObject * systemSetViewChatFlag(PyObject * poSelf, PyObject * poArgs)
{
	int iFlag;
	if (!PyTuple_GetInteger(poArgs, 0, &iFlag))
		return Py_BuildException();

	CPythonSystem::Instance().SetViewChatFlag(iFlag);

	return Py_BuildNone();
}

PyObject * systemIsViewChat(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().IsViewChat());
}

PyObject * systemSetAlwaysShowNameFlag(PyObject * poSelf, PyObject * poArgs)
{
	int iFlag;
	if (!PyTuple_GetInteger(poArgs, 0, &iFlag))
		return Py_BuildException();

	CPythonSystem::Instance().SetAlwaysShowNameFlag(iFlag);

	return Py_BuildNone();
}

PyObject * systemSetShowDamageFlag(PyObject * poSelf, PyObject * poArgs)
{
	int iFlag;
	if (!PyTuple_GetInteger(poArgs, 0, &iFlag))
		return Py_BuildException();

	CPythonSystem::Instance().SetShowDamageFlag(iFlag);

	return Py_BuildNone();
}

PyObject * systemSetShowSalesTextFlag(PyObject * poSelf, PyObject * poArgs)
{
	int iFlag;
	if (!PyTuple_GetInteger(poArgs, 0, &iFlag))
		return Py_BuildException();

	CPythonSystem::Instance().SetShowSalesTextFlag(iFlag);

	return Py_BuildNone();
}

#ifdef WJ_SHOW_MOB_INFO
#include "PythonCharacterManager.h"
#include "PythonNonPlayer.h"
#include "PythonSystem.h"
#if defined(WJ_SHOW_MOB_INFO) && defined(ENABLE_SHOW_MOBAIFLAG)
PyObject * systemIsShowMobAIFlag(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().IsShowMobAIFlag());
}
PyObject * systemSetShowMobAIFlag(PyObject * poSelf, PyObject * poArgs)
{
	int iFlag;
	if (!PyTuple_GetInteger(poArgs, 0, &iFlag))
		return Py_BuildException();

	CPythonSystem::Instance().SetShowMobAIFlagFlag(iFlag);

	for (CPythonCharacterManager::CharacterIterator it=CPythonCharacterManager::Instance().CharacterInstanceBegin(), ti=CPythonCharacterManager::Instance().CharacterInstanceEnd();
			it!=ti;
			++it)
	{
		CInstanceBase * pkInst = *it;
		if (pkInst && pkInst->IsEnemy())
			pkInst->MobInfoAiFlagRefresh();
	}
	return Py_BuildNone();
}
#endif
#if defined(WJ_SHOW_MOB_INFO) && defined(ENABLE_SHOW_MOBLEVEL)
PyObject * systemIsShowMobLevel(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().IsShowMobLevel());
}
PyObject * systemSetShowMobLevel(PyObject * poSelf, PyObject * poArgs)
{
	int iFlag;
	if (!PyTuple_GetInteger(poArgs, 0, &iFlag))
		return Py_BuildException();

	CPythonSystem::Instance().SetShowMobLevelFlag(iFlag);

	for (CPythonCharacterManager::CharacterIterator it=CPythonCharacterManager::Instance().CharacterInstanceBegin(), ti=CPythonCharacterManager::Instance().CharacterInstanceEnd();
			it!=ti;
			++it)
	{
		CInstanceBase * pkInst = *it;
		if (pkInst && pkInst->IsEnemy())
			pkInst->MobInfoLevelRefresh();
	}
	return Py_BuildNone();
}
#endif
#endif

PyObject * systemIsAlwaysShowName(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().IsAlwaysShowName());
}

PyObject * systemIsShowDamage(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().IsShowDamage());
}

PyObject * systemIsShowSalesText(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().IsShowSalesText());
}

#ifdef ENABLE_CHAT_COLOR_SYSTEM
PyObject* systemGetChatColor(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().GetChatColor());
}

PyObject* systemSetChatColor(PyObject* poSelf, PyObject* poArgs)
{
	int iFlag;
	if (!PyTuple_GetInteger(poArgs, 0, &iFlag))
		return Py_BuildException();

	CPythonSystem::Instance().SetChatColor(iFlag);

	return Py_BuildNone();
}
#endif

PyObject * systemSetConfig(PyObject * poSelf, PyObject * poArgs)
{
	int res_index;
	int width;
	int height;
	int bpp;
	int frequency_index;
	int frequency;
	int software_cursor;
	int shadow;
	int object_culling;
	int music_volume;
	int voice_volume;
	int gamma;
	int distance;
	int shopnames_range;

	if (!PyTuple_GetInteger(poArgs, 0, &res_index))
		return Py_BuildException();

	if (!PyTuple_GetInteger(poArgs, 1, &frequency_index))
		return Py_BuildException();

	if (!PyTuple_GetInteger(poArgs, 2, &software_cursor))
		return Py_BuildException();

	if (!PyTuple_GetInteger(poArgs, 3, &shadow))
		return Py_BuildException();

	if (!PyTuple_GetInteger(poArgs, 4, &object_culling))
		return Py_BuildException();

	if (!PyTuple_GetInteger(poArgs, 5, &music_volume))
		return Py_BuildException();

	if (!PyTuple_GetInteger(poArgs, 6, &voice_volume))
		return Py_BuildException();

	if (!PyTuple_GetInteger(poArgs, 7, &gamma))
		return Py_BuildException();

	if (!PyTuple_GetInteger(poArgs, 8, &distance))
		return Py_BuildException();

#ifdef ENABLE_SHOPNAMES_RANGE
	if (!PyTuple_GetInteger(poArgs, 9, &shopnames_range))
		return Py_BuildException();
#endif

	if (!CPythonSystem::Instance().GetResolution(res_index, (DWORD *) &width, (DWORD *) &height, (DWORD *) &bpp))
		return Py_BuildNone();

	if (!CPythonSystem::Instance().GetFrequency(res_index,frequency_index, (DWORD *) &frequency))
		return Py_BuildNone();

	CPythonSystem::TConfig tmp;

	memcpy(&tmp, CPythonSystem::Instance().GetConfig(), sizeof(tmp));

	tmp.width				= width;
	tmp.height				= height;
	tmp.bpp					= bpp;
	tmp.frequency			= frequency;
	tmp.is_software_cursor	= software_cursor ? true : false;
	tmp.is_object_culling	= object_culling ? true : false;
	tmp.music_volume		= (char) music_volume;
	tmp.voice_volume		= (char) voice_volume;
	tmp.gamma				= gamma;
	tmp.iDistance			= distance;
#ifdef ENABLE_SHOPNAMES_RANGE
	tmp.shopnames_range = (char)shopnames_range;
#endif
	CPythonSystem::Instance().SetConfig(&tmp);
	return Py_BuildNone();
}

PyObject * systemApplyConfig(PyObject * poSelf, PyObject * poArgs)
{
	CPythonSystem::Instance().ApplyConfig();
	return Py_BuildNone();
}

PyObject * systemSaveConfig(PyObject * poSelf, PyObject * poArgs)
{
	int ret = CPythonSystem::Instance().SaveConfig();
	return Py_BuildValue("i", ret);
}

PyObject * systemGetResolutionCount(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().GetResolutionCount());
}

PyObject * systemGetFrequencyCount(PyObject * poSelf, PyObject * poArgs)
{
	int	index;

	if (!PyTuple_GetInteger(poArgs, 0, &index))
		return Py_BuildException();

	return Py_BuildValue("i", CPythonSystem::Instance().GetFrequencyCount(index));
}

PyObject * systemGetResolution(PyObject * poSelf, PyObject * poArgs)
{
	int	index;
	DWORD width = 0, height = 0, bpp = 0;

	if (!PyTuple_GetInteger(poArgs, 0, &index))
		return Py_BuildException();

	CPythonSystem::Instance().GetResolution(index, &width, &height, &bpp);
	return Py_BuildValue("iii", width, height, bpp);
}

PyObject * systemGetCurrentResolution(PyObject * poSelf, PyObject *poArgs)
{
	CPythonSystem::TConfig *tmp = CPythonSystem::Instance().GetConfig();
	return Py_BuildValue("iii", tmp->width, tmp->height, tmp->bpp);
}

PyObject * systemGetFrequency(PyObject * poSelf, PyObject * poArgs)
{
	int	index, frequency_index;
	DWORD frequency = 0;

	if (!PyTuple_GetInteger(poArgs, 0, &index))
		return Py_BuildException();

	if (!PyTuple_GetInteger(poArgs, 1, &frequency_index))
		return Py_BuildException();

	CPythonSystem::Instance().GetFrequency(index, frequency_index, &frequency);
	return Py_BuildValue("i", frequency);
}

PyObject * systemGetShadowLevel(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().GetShadowLevel());
}

#ifdef ENABLE_REFINE_RENEWAL
PyObject * systemSetRefineStatus(PyObject * poSelf, PyObject * poArgs)
{
	int iFlag;
	if (!PyTuple_GetInteger(poArgs, 0, &iFlag))
		return Py_BuildException();

	CPythonSystem::Instance().SetRefineStatus(iFlag);

	return Py_BuildNone();
}

PyObject * systemIsRefineStatusShow(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().IsRefineStatusShow());
}
#endif

PyObject * systemSetShadowLevel(PyObject * poSelf, PyObject * poArgs)
{
	int level;

	if (!PyTuple_GetInteger(poArgs, 0, &level))
		return Py_BuildException();

	if (level > 0)
		CPythonSystem::Instance().SetShadowLevel(level);

	return Py_BuildNone();
}

#ifdef ENABLE_DOG_MODE
PyObject* systemSetDogMode(PyObject* poSelf, PyObject* poArgs)
{
	int iFlag;
	if (!PyTuple_GetInteger(poArgs, 0, &iFlag))
		return Py_BuildException();

	CPythonSystem::Instance().SetDogMode(iFlag);

	return Py_BuildNone();
}

PyObject* systemGetDogMode(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().GetDogMode());
}
#endif

#ifdef ENABLE_HIDE_PET
PyObject * systemSetHidePets(PyObject * poSelf, PyObject * poArgs)
{
	int iFlag;
	if (!PyTuple_GetInteger(poArgs, 0, &iFlag))
		return Py_BuildException();

	CPythonSystem::Instance().SetHidePets(iFlag);

	return Py_BuildNone();
}
PyObject * systemIsHidePets(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().IsHidePets());
}
#endif
#ifdef ENABLE_HIDE_MOUNT
PyObject * systemSetHideMounts(PyObject * poSelf, PyObject * poArgs)
{
	int iFlag;
	if (!PyTuple_GetInteger(poArgs, 0, &iFlag))
		return Py_BuildException();

	CPythonSystem::Instance().SetHideMounts(iFlag);

	return Py_BuildNone();
}
PyObject * systemIsHideMounts(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().IsHideMounts());
}
#endif
#ifdef ENABLE_HIDE_SHOPS
PyObject * systemSetHideShops(PyObject * poSelf, PyObject * poArgs)
{
	int iFlag;
	if (!PyTuple_GetInteger(poArgs, 0, &iFlag))
		return Py_BuildException();

	CPythonSystem::Instance().SetHideShops(iFlag);

	return Py_BuildNone();
}
PyObject * systemIsHideShops(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().IsHideShops());
}
#endif


#ifdef ENABLE_FOG_FIX
PyObject * systemSetFogMode(PyObject * poSelf, PyObject * poArgs)
{
	int iFlag;
	if (!PyTuple_GetInteger(poArgs, 0, &iFlag))
		return Py_BuildException();

	CPythonSystem::Instance().SetFogMode(iFlag);

	return Py_BuildNone();
}

PyObject * systemIsFogMode(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().IsFogMode());
}
#endif

#ifdef ENABLE_SHOPNAMES_RANGE
PyObject * systemSetShopNamesRange(PyObject * poSelf, PyObject * poArgs)
{
	float fRange;
	if (!PyTuple_GetFloat(poArgs, 0, &fRange))
		return Py_BuildException();

	CPythonSystem::Instance().SetShopNamesRange(fRange);
	return Py_BuildNone();
}

PyObject * systemGetShopNamesRange(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("f", CPythonSystem::Instance().GetShopNamesRange());
}
#endif

#if defined(__BL_HIDE_EFFECT__)
PyObject* systemGetBLEffectOption(PyObject* poSelf, PyObject* poArgs)
{
	std::uint8_t idx;
	if (!PyTuple_GetInteger(poArgs, 0, &idx))
		return Py_BuildException();

	return Py_BuildValue("b", CPythonSystem::Instance().GetBLEffectOption(idx));
}

PyObject* systemSetBLEffectOption(PyObject* poSelf, PyObject* poArgs)
{
	std::uint8_t idx; bool ret;
	if (!PyTuple_GetInteger(poArgs, 0, &idx) || !PyTuple_GetBoolean(poArgs, 1, &ret))
		return Py_BuildException();

	CPythonSystem::Instance().SetBLEffectOption(idx, ret);
	return Py_BuildNone();
}
#endif

#ifdef ULTIMATE_PICKUP
PyObject * systemSetPickupMode(PyObject * poSelf, PyObject * poArgs)
{
	int iFlag;
	if (!PyTuple_GetInteger(poArgs, 0, &iFlag))
		return Py_BuildException();
	if (iFlag == 1)
		CPythonSystem::Instance().SetPickupMode(TRUE);
	else
		CPythonSystem::Instance().SetPickupMode(FALSE);
	return Py_BuildNone();
}
PyObject * systemGetPickupMode(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().GetPickupMode());
}
PyObject * systemSetPickupAutoMode(PyObject * poSelf, PyObject * poArgs)
{
	int iFlag;
	if (!PyTuple_GetInteger(poArgs, 0, &iFlag))
		return Py_BuildException();
	if (iFlag == 1)
		CPythonSystem::Instance().SetPickupAutoMode(TRUE);
	else
		CPythonSystem::Instance().SetPickupAutoMode(FALSE);
	return Py_BuildNone();
}
PyObject * systemGetPickupAutoMode(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().GetPickupAutoMode());
}
PyObject * systemSetPickupFilter(PyObject * poSelf, PyObject * poArgs)
{
	int iFlag;
	if (!PyTuple_GetInteger(poArgs, 0, &iFlag))
		return Py_BuildException();
	CPythonSystem::Instance().SetPickupFilter((DWORD)iFlag);
	return Py_BuildNone();
}
PyObject * systemGetPickupFilter(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().GetPickupFilter());
}

PyObject * systemIsSet(PyObject * poSelf, PyObject * poArgs)
{
	int iFlag;
	if (!PyTuple_GetInteger(poArgs, 0, &iFlag))
		return Py_BuildException();
	int iSecondFlag;
	if (!PyTuple_GetInteger(poArgs, 1, &iSecondFlag))
		return Py_BuildException();
	if (IS_SET((DWORD)iFlag, iSecondFlag))
		return Py_BuildValue("i", 1);
	return Py_BuildValue("i", 0);
}

PyObject * systemSetBit(PyObject * poSelf, PyObject * poArgs)
{
	int iFlag;
	if (!PyTuple_GetInteger(poArgs, 0, &iFlag))
		return Py_BuildException();
	int iSecondFlag;
	if (!PyTuple_GetInteger(poArgs, 1, &iSecondFlag))
		return Py_BuildException();
	SET_BIT(iFlag, iSecondFlag);
	return Py_BuildValue("i", iFlag);
}
PyObject * systemRemoveBit(PyObject * poSelf, PyObject * poArgs)
{
	int iFlag;
	if (!PyTuple_GetInteger(poArgs, 0, &iFlag))
		return Py_BuildException();
	int iSecondFlag;
	if (!PyTuple_GetInteger(poArgs, 1, &iSecondFlag))
		return Py_BuildException();
	REMOVE_BIT(iFlag, iSecondFlag);
	return Py_BuildValue("i", iFlag);
}
#endif

#ifdef ENABLE_ENB_MODE
PyObject* systemSetENBModeStatusFlag(PyObject* poSelf, PyObject* poArgs)
{
	int iFlag;
	if (!PyTuple_GetInteger(poArgs, 0, &iFlag))
		return Py_BuildException();
	CPythonSystem::Instance().SetENBModeStatusFlag(iFlag);
	return Py_BuildNone();
}

PyObject* systemIsENBModeStatus(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().IsENBModeStatus());
}
#endif

#ifdef ENABLE_REMEMBER_LAST_SPLIT
PyObject* systemSetLastRememberData(PyObject* poSelf, PyObject* poArgs)
{
	int iFlag;
	if (!PyTuple_GetInteger(poArgs, 0, &iFlag))
		return Py_BuildException();
	CPythonSystem::Instance().SetLastRemeberSplitData(iFlag);
	return Py_BuildNone();
}

PyObject* systemGetLastRememberData(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().GetLastRememberSplitData());
}
#endif

#ifdef ENABLE_FPS
PyObject* systemSetFPS(PyObject* poSelf, PyObject* poArgs)
{
	int iFlag;
	if (!PyTuple_GetInteger(poArgs, 0, &iFlag))
		return Py_BuildException();
	CPythonSystem::Instance().SetFPS(iFlag);
	return Py_BuildNone();
}
PyObject* systemGetFPS(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().GetFPS());
}
#endif

#if defined(ENABLE_FOV_OPTION)
PyObject* systemSetFOV(PyObject* poSelf, PyObject* poArgs)
{
	float fValue;
	if (!PyTuple_GetFloat(poArgs, 0, &fValue))
		return Py_BuildException();

	CPythonSystem::Instance().SetFOV(fValue);
	return Py_BuildNone();
}

PyObject* systemGetFOV(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("f", CPythonSystem::Instance().GetFOV());
}
#endif

void initsystemSetting()
{
	static PyMethodDef s_methods[] =
	{
		{ "GetWidth",					systemGetWidth,					METH_VARARGS },
		{ "GetHeight",					systemGetHeight,				METH_VARARGS },

		{ "SetInterfaceHandler",		systemSetInterfaceHandler,		METH_VARARGS },
		{ "DestroyInterfaceHandler",	systemDestroyInterfaceHandler,	METH_VARARGS },
		{ "ReserveResource",			systemReserveResource,			METH_VARARGS },

		{ "isInterfaceConfig",			systemisInterfaceConfig,		METH_VARARGS },
		{ "SaveWindowStatus",			systemSaveWindowStatus,			METH_VARARGS },
		{ "GetWindowStatus",			systemGetWindowStatus,			METH_VARARGS },

		{ "GetResolutionCount",			systemGetResolutionCount,		METH_VARARGS },
		{ "GetFrequencyCount",			systemGetFrequencyCount,		METH_VARARGS },

		{ "GetCurrentResolution",		systemGetCurrentResolution,		METH_VARARGS },

		{ "GetResolution",				systemGetResolution,			METH_VARARGS },
		{ "GetFrequency",				systemGetFrequency,				METH_VARARGS },

		{ "ApplyConfig",				systemApplyConfig,				METH_VARARGS },
		{ "SetConfig",					systemSetConfig,				METH_VARARGS },
		{ "SaveConfig",					systemSaveConfig,				METH_VARARGS },
		{ "GetConfig",					systemGetConfig,				METH_VARARGS },

		{ "SetSaveID",					systemSetSaveID,				METH_VARARGS },
		{ "isSaveID",					systemisSaveID,					METH_VARARGS },
		{ "GetSaveID",					systemGetSaveID,				METH_VARARGS },
		{ "SetTransLangKey",			systemSetTransLangKey,			METH_VARARGS },
		{ "GetTransLangKey",			systemGetTransLangKey,			METH_VARARGS },

		{ "GetMusicVolume",				systemGetMusicVolume,			METH_VARARGS },
		{ "GetSoundVolume",				systemGetSoundVolume,			METH_VARARGS },

		{ "SetMusicVolume",				systemSetMusicVolume,			METH_VARARGS },
		{ "SetSoundVolumef",			systemSetSoundVolumef,			METH_VARARGS },
		{ "IsSoftwareCursor",			systemIsSoftwareCursor,			METH_VARARGS },

		{ "SetViewChatFlag",			systemSetViewChatFlag,			METH_VARARGS },
		{ "IsViewChat",					systemIsViewChat,				METH_VARARGS },

		{ "SetAlwaysShowNameFlag",		systemSetAlwaysShowNameFlag,	METH_VARARGS },
		{ "IsAlwaysShowName",			systemIsAlwaysShowName,			METH_VARARGS },


		{ "SetShowDamageFlag",			systemSetShowDamageFlag,		METH_VARARGS },
		{ "IsShowDamage",				systemIsShowDamage,				METH_VARARGS },
#ifdef ENABLE_REFINE_RENEWAL
		{ "SetRefineStatus", systemSetRefineStatus, METH_VARARGS },
		{ "IsRefineShow", systemIsRefineStatusShow, METH_VARARGS },
#endif	
		{ "SetShowSalesTextFlag",		systemSetShowSalesTextFlag,		METH_VARARGS },
		{ "IsShowSalesText",			systemIsShowSalesText,			METH_VARARGS },
#ifdef ENABLE_CHAT_COLOR_SYSTEM
		{ "GetChatColor",				systemGetChatColor,				METH_VARARGS },
		{ "SetChatColor",				systemSetChatColor,				METH_VARARGS },
#endif
#if defined(__BL_HIDE_EFFECT__)
		{ "SetBLEffectOption", systemSetBLEffectOption, METH_VARARGS },
		{ "GetBLEffectOption", systemGetBLEffectOption, METH_VARARGS },
#endif
		{ "GetShadowLevel",				systemGetShadowLevel,			METH_VARARGS },
		{ "SetShadowLevel",				systemSetShadowLevel,			METH_VARARGS },
#ifdef ENABLE_DOG_MODE
		{ "SetDogMode",				systemSetDogMode,				METH_VARARGS },
		{ "GetDogMode",				systemGetDogMode,				METH_VARARGS },
#endif
#ifdef ENABLE_SHOPNAMES_RANGE
		{ "SetShopNamesRange", systemSetShopNamesRange, METH_VARARGS },
		{ "GetShopNamesRange", systemGetShopNamesRange, METH_VARARGS },
#endif
#ifdef ENABLE_FOG_FIX
		{ "SetFogMode",				systemSetFogMode,				METH_VARARGS },
		{ "IsFogMode",				systemIsFogMode,				METH_VARARGS },
#endif
#ifdef WJ_SHOW_MOB_INFO
		{ "IsShowMobAIFlag",			systemIsShowMobAIFlag,			METH_VARARGS },
		{ "SetShowMobAIFlag",			systemSetShowMobAIFlag,			METH_VARARGS },

		{ "IsShowMobLevel",				systemIsShowMobLevel,			METH_VARARGS },
		{ "SetShowMobLevel",			systemSetShowMobLevel,			METH_VARARGS },
#endif
#ifdef ENABLE_HIDE_PET
		{ "SetHidePets", systemSetHidePets, METH_VARARGS },
		{ "IsHidePets", systemIsHidePets, METH_VARARGS },
#endif
#ifdef ENABLE_HIDE_MOUNT		
		{ "SetHideMounts", systemSetHideMounts, METH_VARARGS },
		{ "IsHideMounts", systemIsHideMounts, METH_VARARGS },
#endif
#ifdef ENABLE_HIDE_SHOPS
		{ "SetHideShops", systemSetHideShops, METH_VARARGS },
		{ "IsHideShops", systemIsHideShops, METH_VARARGS },
#endif

#ifdef ULTIMATE_PICKUP
		{ "SetPickupMode", systemSetPickupMode, METH_VARARGS },
		{ "GetPickupMode", systemGetPickupMode, METH_VARARGS },
		{ "SetPickupAutoMode", systemSetPickupAutoMode, METH_VARARGS },
		{ "GetPickupAutoMode", systemGetPickupAutoMode, METH_VARARGS },
		{ "SetPickupFilter", systemSetPickupFilter, METH_VARARGS },
		{ "GetPickupFilter", systemGetPickupFilter, METH_VARARGS },
		{ "IsSet", systemIsSet, METH_VARARGS },
		{ "SetBit", systemSetBit, METH_VARARGS },
		{ "RemoveBit", systemRemoveBit, METH_VARARGS },
#endif
#ifdef ENABLE_ENB_MODE
		{ "SetENBModeStatusFlag",		systemSetENBModeStatusFlag,		METH_VARARGS },
		{ "IsENBModeStatus",			systemIsENBModeStatus,			METH_VARARGS },
#endif
#ifdef ENABLE_REMEMBER_LAST_SPLIT
		{ "SetLastSplitData",            systemSetLastRememberData,        METH_VARARGS },
		{ "GetLastSplitData",            systemGetLastRememberData,        METH_VARARGS },
#endif
#ifdef ENABLE_FPS
		{ "SetFPS", systemSetFPS, METH_VARARGS },
		{ "GetFPS", systemGetFPS, METH_VARARGS },
#endif
#if defined(ENABLE_FOV_OPTION)
		{ "SetFOV", systemSetFOV, METH_VARARGS },
		{ "GetFOV", systemGetFOV, METH_VARARGS },
#endif

		{ NULL,							NULL,							NULL }
	};

	PyObject * poModule = Py_InitModule("systemSetting", s_methods);

	PyModule_AddIntConstant(poModule, "WINDOW_STATUS",		CPythonSystem::WINDOW_STATUS);
	PyModule_AddIntConstant(poModule, "WINDOW_INVENTORY",	CPythonSystem::WINDOW_INVENTORY);
	PyModule_AddIntConstant(poModule, "WINDOW_ABILITY",		CPythonSystem::WINDOW_ABILITY);
	PyModule_AddIntConstant(poModule, "WINDOW_SOCIETY",		CPythonSystem::WINDOW_SOCIETY);
	PyModule_AddIntConstant(poModule, "WINDOW_JOURNAL",		CPythonSystem::WINDOW_JOURNAL);
	PyModule_AddIntConstant(poModule, "WINDOW_COMMAND",		CPythonSystem::WINDOW_COMMAND);

	PyModule_AddIntConstant(poModule, "WINDOW_QUICK",		CPythonSystem::WINDOW_QUICK);
	PyModule_AddIntConstant(poModule, "WINDOW_GAUGE",		CPythonSystem::WINDOW_GAUGE);
	PyModule_AddIntConstant(poModule, "WINDOW_MINIMAP",		CPythonSystem::WINDOW_MINIMAP);
	PyModule_AddIntConstant(poModule, "WINDOW_CHAT",		CPythonSystem::WINDOW_CHAT);

#ifdef ULTIMATE_PICKUP
	PyModule_AddIntConstant(poModule, "PICKUP_FILTER_WEAPON", CPythonSystem::PICKUP_FILTER_WEAPON);
	PyModule_AddIntConstant(poModule, "PICKUP_FILTER_ARMOR", CPythonSystem::PICKUP_FILTER_ARMOR);
	PyModule_AddIntConstant(poModule, "PICKUP_FILTER_EAR", CPythonSystem::PICKUP_FILTER_EAR);
	PyModule_AddIntConstant(poModule, "PICKUP_FILTER_NECK", CPythonSystem::PICKUP_FILTER_NECK);
	PyModule_AddIntConstant(poModule, "PICKUP_FILTER_FOOTS", CPythonSystem::PICKUP_FILTER_FOOTS);
	PyModule_AddIntConstant(poModule, "PICKUP_FILTER_SHIELD", CPythonSystem::PICKUP_FILTER_SHIELD);
	PyModule_AddIntConstant(poModule, "PICKUP_FILTER_HEAD", CPythonSystem::PICKUP_FILTER_HEAD);
	PyModule_AddIntConstant(poModule, "PICKUP_FILTER_WRIST", CPythonSystem::PICKUP_FILTER_WRIST);
	PyModule_AddIntConstant(poModule, "PICKUP_FILTER_BOOK", CPythonSystem::PICKUP_FILTER_BOOK);
	PyModule_AddIntConstant(poModule, "PICKUP_FILTER_STONE", CPythonSystem::PICKUP_FILTER_STONE);
	PyModule_AddIntConstant(poModule, "PICKUP_FILTER_ETC", CPythonSystem::PICKUP_FILTER_ETC);
#endif
}
