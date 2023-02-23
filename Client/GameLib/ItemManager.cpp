#include "StdAfx.h"
#include "../eterPack/EterPackManager.h"
#include "../eterLib/ResourceManager.h"

#include "ItemManager.h"

static DWORD s_adwItemProtoKey[4] =
{
	173217,
	72619434,
	408587239,
	27973291
};

BOOL CItemManager::SelectItemData(DWORD dwIndex)
{
	TItemMap::iterator f = m_ItemMap.find(dwIndex);

	if (m_ItemMap.end() == f)
	{
		int n = m_vec_ItemRange.size();
		for (int i = 0; i < n; i++)
		{
			CItemData * p = m_vec_ItemRange[i];
			const CItemData::TItemTable * pTable = p->GetTable();
			if ((pTable->dwVnum < dwIndex) &&
				dwIndex < (pTable->dwVnum + pTable->dwVnumRange))
			{
				m_pSelectedItemData = p;
				return TRUE;
			}
		}
		Tracef(" CItemManager::SelectItemData - FIND ERROR [%d]\n", dwIndex);
		return FALSE;
	}

	m_pSelectedItemData = f->second;

	return TRUE;
}

CItemData * CItemManager::GetSelectedItemDataPointer()
{
	return m_pSelectedItemData;
}

#ifdef __ENABLE_TRASH_BIN__
BOOL CItemManager::IsUnbreakableItem(DWORD dwIndex)
{
	std::vector<DWORD>& rvec = m_trashbin_unbreakable_vnum_vector;
	return (std::find(rvec.begin(), rvec.end(), dwIndex) != rvec.end());
}

int CItemManager::GetTrashBinRewardCount(DWORD dwIndex)
{
	std::map<DWORD, DWORD>::iterator it = m_trashbin_reward_map.find(dwIndex);

	if (it == m_trashbin_reward_map.end())
		return 0;

	else
		return it->second;
}

#endif

BOOL CItemManager::GetItemDataPointer(DWORD dwItemID, CItemData ** ppItemData)
{
	if (0 == dwItemID)
		return FALSE;

	TItemMap::iterator f = m_ItemMap.find(dwItemID);

	if (m_ItemMap.end() == f)
	{
		int n = m_vec_ItemRange.size();
		for (int i = 0; i < n; i++)
		{
			CItemData * p = m_vec_ItemRange[i];
			const CItemData::TItemTable * pTable = p->GetTable();
			if ((pTable->dwVnum < dwItemID) &&
				dwItemID < (pTable->dwVnum + pTable->dwVnumRange))
			{
				*ppItemData = p;
				return TRUE;
			}
		}
		Tracef(" CItemManager::GetItemDataPointer - FIND ERROR [%d]\n", dwItemID);
		return FALSE;
	}

	*ppItemData = f->second;

	return TRUE;
}

CItemData * CItemManager::MakeItemData(DWORD dwIndex)
{
	TItemMap::iterator f = m_ItemMap.find(dwIndex);

	if (m_ItemMap.end() == f)
	{
		CItemData * pItemData = CItemData::New();

		m_ItemMap.insert(TItemMap::value_type(dwIndex, pItemData));

		return pItemData;
	}

	return f->second;
}

////////////////////////////////////////////////////////////////////////////////////////
// Load Item Table

bool CItemManager::LoadItemList(const char * c_szFileName)
{
	CMappedFile File;
	LPCVOID pData;

	if (!CEterPackManager::Instance().Get(File, c_szFileName, &pData))
		return false;

	CMemoryTextFileLoader textFileLoader;
	textFileLoader.Bind(File.Size(), pData);

	CTokenVector TokenVector;
    for (DWORD i = 0; i < textFileLoader.GetLineCount(); ++i)
	{
		if (!textFileLoader.SplitLine(i, &TokenVector, "\t"))
			continue;

		if (!(TokenVector.size() == 3 || TokenVector.size() == 4))
		{
			TraceError(" CItemManager::LoadItemList(%s) - StrangeLine in %d\n", c_szFileName, i);
			continue;
		}

		const std::string & c_rstrID = TokenVector[0];
		const std::string & c_rstrType = TokenVector[1];
		const std::string & c_rstrIcon = TokenVector[2];

		DWORD dwItemVNum=atoi(c_rstrID.c_str());

		CItemData * pItemData = MakeItemData(dwItemVNum);

		extern BOOL USE_VIETNAM_CONVERT_WEAPON_VNUM;
		if (USE_VIETNAM_CONVERT_WEAPON_VNUM)
		{
			extern DWORD Vietnam_ConvertWeaponVnum(DWORD vnum);
			DWORD dwMildItemVnum = Vietnam_ConvertWeaponVnum(dwItemVNum);
			if (dwMildItemVnum == dwItemVNum)
			{
				if (4 == TokenVector.size())
				{
#ifdef ENABLE_AURA_SYSTEM
					if (!strcmp(c_rstrType.c_str(), "AURA"))
					{
						const std::string& c_rstrAuraEffectFileName = TokenVector[3];
						pItemData->SetAuraEffectID(c_rstrAuraEffectFileName.c_str());
						pItemData->SetDefaultItemData(c_rstrIcon.c_str());
					}
					else
					{
						const std::string& c_rstrModelFileName = TokenVector[3];
						pItemData->SetDefaultItemData(c_rstrIcon.c_str(), c_rstrModelFileName.c_str());
					}
#else
					const std::string& c_rstrModelFileName = TokenVector[3];
					pItemData->SetDefaultItemData(c_rstrIcon.c_str(), c_rstrModelFileName.c_str());
#endif
				}
				else
				{
					pItemData->SetDefaultItemData(c_rstrIcon.c_str());
				}
			}
			else
			{
				DWORD dwMildBaseVnum = dwMildItemVnum / 10 * 10;
				char szMildIconPath[MAX_PATH];
				sprintf(szMildIconPath, "icon/item/%.5d.tga", dwMildBaseVnum);
				if (4 == TokenVector.size())
				{
					char szMildModelPath[MAX_PATH];
					sprintf(szMildModelPath, "d:/ymir work/item/weapon/%.5d.gr2", dwMildBaseVnum);
					pItemData->SetDefaultItemData(szMildIconPath, szMildModelPath);
				}
				else
				{
					pItemData->SetDefaultItemData(szMildIconPath);
				}
			}
		}
		else
		{
			if (4 == TokenVector.size())
			{
#ifdef ENABLE_AURA_SYSTEM
				if (!strcmp(c_rstrType.c_str(), "AURA"))
				{
					const std::string& c_rstrAuraEffectFileName = TokenVector[3];
					pItemData->SetAuraEffectID(c_rstrAuraEffectFileName.c_str());
					pItemData->SetDefaultItemData(c_rstrIcon.c_str());
				}
				else
				{
					const std::string& c_rstrModelFileName = TokenVector[3];
					pItemData->SetDefaultItemData(c_rstrIcon.c_str(), c_rstrModelFileName.c_str());
				}
#else
				const std::string& c_rstrModelFileName = TokenVector[3];
				pItemData->SetDefaultItemData(c_rstrIcon.c_str(), c_rstrModelFileName.c_str());
#endif
			}
			else
			{
				pItemData->SetDefaultItemData(c_rstrIcon.c_str());
			}
		}
	}

	return true;
}

const std::string& __SnapString(const std::string& c_rstSrc, std::string& rstTemp)
{
	UINT uSrcLen=c_rstSrc.length();
	if (uSrcLen<2)
		return c_rstSrc;

	if (c_rstSrc[0]!='"')
		return c_rstSrc;

	UINT uLeftCut=1;

	UINT uRightCut=uSrcLen;
	if (c_rstSrc[uSrcLen-1]=='"')
		uRightCut=uSrcLen-1;

	rstTemp=c_rstSrc.substr(uLeftCut, uRightCut-uLeftCut);
	return rstTemp;
}

#ifdef __ENABLE_TRASH_BIN__
bool CItemManager::LoadTrashBinUnbreakableList(const char* c_szFileName)
{
	CMappedFile File;
	LPCVOID pData;

	if (!CEterPackManager::Instance().Get(File, c_szFileName, &pData))
		return false;

	CMemoryTextFileLoader textFileLoader;
	textFileLoader.Bind(File.Size(), pData);

	for (DWORD dwLineIndex = 0; dwLineIndex < textFileLoader.GetLineCount(); dwLineIndex++)
	{
		const string& rString = textFileLoader.GetLineString(dwLineIndex);

		if (rString.empty())
			continue;
		
		string substring = rString.substr(0,rString.find('#'));

		if (substring.empty() || substring.find_first_not_of("\t ") == string::npos)
			continue;

		//check noobProof
		for (int i = 0; i < substring.length(); i++)
		{
			char read = substring.at(i);
			if (read == ' ' || read == '\t' )
				continue;

			if (!isdigit(read))
			{
				TraceError("ERROR DURING READ FILE [%s] AT LINE [%d]", c_szFileName, dwLineIndex);
				TraceError("LINE ERROR [%s]", rString.c_str());
				return false;
			}
		}
		// end check noobProof

		m_trashbin_unbreakable_vnum_vector.push_back(atoi(substring.c_str()));
	}

	return true;
}


bool CItemManager::LoadTrashBinRewardList(const char* c_szFileName)
{
	CMappedFile File;
	LPCVOID pData;

	if (!CEterPackManager::Instance().Get(File, c_szFileName, &pData))
		return false;

	CMemoryTextFileLoader textFileLoader;
	textFileLoader.Bind(File.Size(), pData);

	for (DWORD dwLineIndex = 0; dwLineIndex < textFileLoader.GetLineCount(); dwLineIndex++)
	{
		const string& rString = textFileLoader.GetLineString(dwLineIndex);

		if (rString.empty())
			continue;

		string substring = rString.substr(0, rString.find('#'));

		if (substring.empty() || substring.find_first_not_of("\t ") == string::npos)
			continue;


		//check noobProof
		for (int i = 0; i < substring.length(); i++)
		{
			char read = substring.at(i);
			if (read == ' ' || read == '\t')
				continue;

			if (!isdigit(read))
			{
				TraceError("ERROR DURING READ FILE [%s] AT LINE [%d]", c_szFileName, dwLineIndex);
				TraceError("LINE ERROR [%s]", rString.c_str());
				return false;
			}
		}
		// end check noobProof


		DWORD key = 0, value = 0;

		size_t key_pos = 0 , key_end = 0, value_pos = 0 ,value_end = 0;


		//key search pos
		if ((key_pos = substring.find_first_not_of("\t")) == string::npos)
		{
			TraceError("ERROR DURING READ FILE [%s] AT LINE [%d]", c_szFileName, dwLineIndex);
			TraceError("LINE ERROR [%s]", rString.c_str());
			return false;
		}
		key_end = substring.find_first_not_of("0123456789", key_pos) +1;


		//value search pos
		if (key_end >= substring.length() - 1 || (value_pos = substring.find_first_not_of("\t ", key_end)) == string::npos)
		{
			TraceError("ERROR DURING READ FILE [%s] AT LINE [%d]", c_szFileName, dwLineIndex);
			TraceError("LINE ERROR [%s]", rString.c_str());
			return false;
		}
		value_end = substring.find_first_not_of("0123456789", value_pos) +1;


		//general check
		if (key_pos == string::npos ||  key_end == 0 || key_end == string::npos || 
			value_pos == 0 || value_pos == string::npos || value_end == 0 || value_end == string::npos)
		{
			TraceError("ERROR DURING READ FILE [%s] AT LINE [%d]", c_szFileName, dwLineIndex);
			TraceError("LINE ERROR [%s]", rString.c_str());
			return false;
		}

		key = atoi(substring.substr(key_pos , key_end - key_pos).c_str());
		value = atoi(substring.substr(value_pos , value_end - value_pos).c_str());
		

		m_trashbin_reward_map.insert(std::make_pair(key,value));
	}

	return true;
}

#endif

bool CItemManager::LoadItemDesc(const char* c_szFileName)
{
	const VOID* pvData;
	CMappedFile kFile;
	if (!CEterPackManager::Instance().Get(kFile, c_szFileName, &pvData))
	{
		Tracenf("CItemManager::LoadItemDesc(c_szFileName=%s) - Load Error", c_szFileName);
		return false;
	}

	CMemoryTextFileLoader kTextFileLoader;
	kTextFileLoader.Bind(kFile.Size(), pvData);

	std::string stTemp;

	CTokenVector kTokenVector;
	for (DWORD i = 0; i < kTextFileLoader.GetLineCount(); ++i)
	{
		if (!kTextFileLoader.SplitLineByTab(i, &kTokenVector))
			continue;

		while (kTokenVector.size()<ITEMDESC_COL_NUM)
			kTokenVector.push_back("");

		//assert(kTokenVector.size()==ITEMDESC_COL_NUM);

		DWORD dwVnum=atoi(kTokenVector[ITEMDESC_COL_VNUM].c_str());
		const std::string& c_rstDesc=kTokenVector[ITEMDESC_COL_DESC];
		const std::string& c_rstSumm=kTokenVector[ITEMDESC_COL_SUMM];
		TItemMap::iterator f = m_ItemMap.find(dwVnum);
		if (m_ItemMap.end() == f)
			continue;

		CItemData* pkItemDataFind = f->second;

		pkItemDataFind->SetDescription(__SnapString(c_rstDesc, stTemp));
		pkItemDataFind->SetSummary(__SnapString(c_rstSumm, stTemp));
	}
	return true;
}

DWORD GetHashCode( const char* pString )
{
	   unsigned long i,len;
	   unsigned long ch;
	   unsigned long result;

	   len     = strlen( pString );
	   result = 5381;
	   for( i=0; i<len; i++ )
	   {
	   	   ch = (unsigned long)pString[i];
	   	   result = ((result<< 5) + result) + ch; // hash * 33 + ch
	   }

	   return result;
}

bool CItemManager::LoadItemTable(const char* c_szFileName)
{
	CMappedFile file;
	LPCVOID pvData;

	if (!CEterPackManager::Instance().Get(file, c_szFileName, &pvData))
		return false;

	DWORD dwFourCC, dwElements, dwDataSize;
	DWORD dwVersion=0;
	DWORD dwStride=0;

	file.Read(&dwFourCC, sizeof(DWORD));

	if (dwFourCC == MAKEFOURCC('M', 'I', 'P', 'X'))
	{
		file.Read(&dwVersion, sizeof(DWORD));
		file.Read(&dwStride, sizeof(DWORD));

		if (dwVersion != 1)
		{
			TraceError("CPythonItem::LoadItemTable: invalid item_proto[%s] VERSION[%d]", c_szFileName, dwVersion);
			return false;
		}

#ifdef ENABLE_PROTOSTRUCT_AUTODETECT
		if (!CItemData::TItemTableAll::IsValidStruct(dwStride))
#else
		if (dwStride != sizeof(CItemData::TItemTable))
#endif
		{
			TraceError("CPythonItem::LoadItemTable: invalid item_proto[%s] STRIDE[%d] != sizeof(SItemTable)",
				c_szFileName, dwStride, sizeof(CItemData::TItemTable));
			return false;
		}
	}
	else if (dwFourCC != MAKEFOURCC('M', 'I', 'P', 'T'))
	{
		TraceError("CPythonItem::LoadItemTable: invalid item proto type %s", c_szFileName);
		return false;
	}

	file.Read(&dwElements, sizeof(DWORD));
	file.Read(&dwDataSize, sizeof(DWORD));

	std::unique_ptr<BYTE[]> pbData(new BYTE[dwDataSize]());
	file.Read(pbData.get(), dwDataSize);

	/////

	CLZObject zObj;

	if (!CLZO::Instance().Decompress(zObj, pbData.get(), s_adwItemProtoKey))
		return false;

	/////

	char szName[64+1];
	std::map<DWORD,DWORD> itemNameMap;

	for (DWORD i = 0; i < dwElements; ++i)
	{
#ifdef ENABLE_PROTOSTRUCT_AUTODETECT
		CItemData::TItemTable t = {0};
		CItemData::TItemTableAll::Process(zObj.GetBuffer(), dwStride, i, t);
#else
		CItemData::TItemTable & t = *((CItemData::TItemTable *) zObj.GetBuffer() + i);
#endif
		CItemData::TItemTable * table = &t;

		CItemData * pItemData;
		DWORD dwVnum = table->dwVnum;

		TItemMap::iterator f = m_ItemMap.find(dwVnum);
		if (m_ItemMap.end() == f)
		{
			_snprintf(szName, sizeof(szName), "icon/item/%05d.tga", dwVnum);
#ifdef INGAME_WIKI
			pItemData = CItemData::New();
#endif

			if (CResourceManager::Instance().IsFileExist(szName) == false)
			{
				std::map<DWORD, DWORD>::iterator itVnum = itemNameMap.find(GetHashCode(table->szName));

				if (itVnum != itemNameMap.end())
					_snprintf(szName, sizeof(szName), "icon/item/%05d.tga", itVnum->second);
				else
					_snprintf(szName, sizeof(szName), "icon/item/%05d.tga", dwVnum-dwVnum % 10);

				if (CResourceManager::Instance().IsFileExist(szName) == false)
				{
					#ifdef _DEBUG
					TraceError("%16s(#%-5d) cannot find icon file. setting to default.", table->szName, dwVnum);
					#endif

#ifdef INGAME_WIKI
					pItemData->ValidateImage(false);
#endif
					const DWORD EmptyBowl = 27995;
					_snprintf(szName, sizeof(szName), "icon/item/%05d.tga", EmptyBowl);
				}
			}

			pItemData = CItemData::New();

			pItemData->SetDefaultItemData(szName);
			m_ItemMap.insert(TItemMap::value_type(dwVnum, pItemData));
#ifdef INGAME_WIKI
			pItemData->SetItemTableData(table);
			if (!CResourceManager::Instance().IsFileExist(pItemData->GetIconFileName().c_str()))
				pItemData->ValidateImage(false);
#endif
		}
		else
		{
			pItemData = f->second;
#ifdef INGAME_WIKI
			pItemData->SetItemTableData(table);
#endif
		}
		if (itemNameMap.find(GetHashCode(table->szName)) == itemNameMap.end())
			itemNameMap.insert(std::map<DWORD,DWORD>::value_type(GetHashCode(table->szName),table->dwVnum));
		pItemData->SetItemTableData(table);
		if (0 != table->dwVnumRange)
		{
			m_vec_ItemRange.push_back(pItemData);
		}
	}

	//delete [] pbData;
	return true;
}


#ifdef ENABLE_SHINING_SYSTEM
bool CItemManager::LoadShiningTable(const char* szShiningTable)
{
	CMappedFile File;
	LPCVOID pData;


	if (!CEterPackManager::Instance().Get(File, szShiningTable, &pData))
		return false;


	CMemoryTextFileLoader textFileLoader;
	textFileLoader.Bind(File.Size(), pData);


	CTokenVector TokenVector;
	for (DWORD i = 0; i < textFileLoader.GetLineCount(); ++i)
	{
		if (!textFileLoader.SplitLine(i, &TokenVector, "\t"))
			continue;


		if (TokenVector.size() >(1 + CItemData::ITEM_SHINING_MAX_COUNT))
		{
			TraceError("CItemManager::LoadShiningTable(%s) - LoadShiningTable in %d\n - RowSize: %d MaxRowSize: %d", szShiningTable, i, TokenVector.size(), CItemData::ITEM_SHINING_MAX_COUNT);
		}

		const std::string & c_rstrID = TokenVector[0];

		int pos = c_rstrID.find("~");

		if (std::string::npos == pos)
		{
			DWORD dwItemVNum = atoi(c_rstrID.c_str());

			CItemData * pItemData = MakeItemData(dwItemVNum);
			if (pItemData)
			{
				for (BYTE i = 0; i < CItemData::ITEM_SHINING_MAX_COUNT; i++)
				{
					if (i < (TokenVector.size() - 1))
					{
						const std::string & c_rstrEffectPath = TokenVector[i + 1];
						pItemData->SetItemShiningTableData(i, c_rstrEffectPath.c_str());
					}
					else
					{
						pItemData->SetItemShiningTableData(i, "");
					}
				}
			}
		}
		else
		{
			std::string s_start_vnum(c_rstrID.substr(0, pos));
			std::string s_end_vnum(c_rstrID.substr(pos + 1));

			int start_vnum = atoi(s_start_vnum.c_str());
			int end_vnum = atoi(s_end_vnum.c_str());
			DWORD vnum = start_vnum;

			while (vnum <= end_vnum)
			{
				CItemData * pItemData = MakeItemData(vnum);
				if (pItemData)
				{
					for (BYTE i = 0; i < CItemData::ITEM_SHINING_MAX_COUNT; i++)
					{
						if (i < (TokenVector.size() - 1))
						{
							const std::string & c_rstrEffectPath = TokenVector[i + 1];
							pItemData->SetItemShiningTableData(i, c_rstrEffectPath.c_str());
						}
						else
						{
							pItemData->SetItemShiningTableData(i, "");
						}
					}
				}

				++vnum;
			}
		}
	}

	return true;
}
#endif

#ifdef ENABLE_ACCE_COSTUME_SYSTEM
bool CItemManager::LoadItemScale(const char * c_szFileName)
{
	const VOID* pvData;
	CMappedFile kFile;
	if (!CEterPackManager::Instance().Get(kFile, c_szFileName, &pvData))
		return false;
	CMemoryTextFileLoader kTextFileLoader;
	kTextFileLoader.Bind(kFile.Size(), pvData);
	CTokenVector kTokenVector;
	for (DWORD i = 0; i < kTextFileLoader.GetLineCount(); ++i)
	{
		if (!kTextFileLoader.SplitLineByTab(i, &kTokenVector))
			continue;
		if (kTokenVector.size() < ITEMSCALE_NUM)
		{
			TraceError("LoadItemScale: invalid line %d (%s).", i, c_szFileName);
			continue;
		}
		const std::string & strJob = kTokenVector[ITEMSCALE_JOB];
		const std::string & strSex = kTokenVector[ITEMSCALE_SEX];
		const std::string & strScaleX = kTokenVector[ITEMSCALE_SCALE_X];
		const std::string & strScaleY = kTokenVector[ITEMSCALE_SCALE_Y];
		const std::string & strScaleZ = kTokenVector[ITEMSCALE_SCALE_Z];
		const std::string & strPositionX = kTokenVector[ITEMSCALE_POSITION_X];
		const std::string & strPositionY = kTokenVector[ITEMSCALE_POSITION_Y];
		const std::string & strPositionZ = kTokenVector[ITEMSCALE_POSITION_Z];
		for (int j = 0; j < 5; ++j)
		{
			CItemData * pItemData = MakeItemData(atoi(kTokenVector[ITEMSCALE_VNUM].c_str()) + j);
			pItemData->SetItemScale(strJob, strSex, strScaleX, strScaleY, strScaleZ, strPositionX, strPositionY, strPositionZ);
		}
	}
	return true;
}
#endif
void CItemManager::Destroy()
{
	TItemMap::iterator i;
	for (i=m_ItemMap.begin(); i!=m_ItemMap.end(); ++i)
		CItemData::Delete(i->second);

	m_ItemMap.clear();
#ifdef INGAME_WIKI
	m_tempItemVec.clear();
#endif
}

#ifdef INGAME_WIKI
bool CItemManager::CanLoadWikiItem(DWORD dwVnum)
{
	DWORD StartRefineVnum = GetWikiItemStartRefineVnum(dwVnum);

	if (StartRefineVnum != dwVnum)
		return false;

	if (StartRefineVnum % 10 != 0)
		return false;

	CItemData* tbl = nullptr;
	if (!GetItemDataPointer(StartRefineVnum, &tbl))
		return false;

	return true;
}

DWORD CItemManager::GetWikiItemStartRefineVnum(DWORD dwVnum)
{
	auto baseItemName = GetWikiItemBaseRefineName(dwVnum);
	if (!baseItemName.size())
		return 0;

	DWORD manage_vnum = dwVnum;
	while (!(strcmp(baseItemName.c_str(), GetWikiItemBaseRefineName(manage_vnum).c_str())))
		--manage_vnum;

	return (manage_vnum + 1);
}

std::string CItemManager::GetWikiItemBaseRefineName(DWORD dwVnum)
{
	CItemData* tbl = nullptr;
	if (!GetItemDataPointer(dwVnum, &tbl))
		return "";

	auto* p = const_cast<char*>(strrchr(tbl->GetName(), '+'));
	if (!p)
		return "";

	std::string sFirstItemName(tbl->GetName(),
		(tbl->GetName() + (p - tbl->GetName())));

	return sFirstItemName;
}

bool CItemManager::IsFilteredAntiflag(CItemData* itemData, DWORD raceFilter)
{
	if (raceFilter != 0)
	{
		if (!itemData->IsAntiFlag(CItemData::ITEM_ANTIFLAG_SHAMAN) && raceFilter & CItemData::ITEM_ANTIFLAG_SHAMAN)
			return false;

		if (!itemData->IsAntiFlag(CItemData::ITEM_ANTIFLAG_SURA) && raceFilter & CItemData::ITEM_ANTIFLAG_SURA)
			return false;

		if (!itemData->IsAntiFlag(CItemData::ITEM_ANTIFLAG_ASSASSIN) && raceFilter & CItemData::ITEM_ANTIFLAG_ASSASSIN)
			return false;

		if (!itemData->IsAntiFlag(CItemData::ITEM_ANTIFLAG_WARRIOR) && raceFilter & CItemData::ITEM_ANTIFLAG_WARRIOR)
			return false;

#ifdef INGAME_WIKI_WOLFMAN
		if (!itemData->IsAntiFlag(CItemData::ITEM_ANTIFLAG_WOLFMAN) && raceFilter & CItemData::ITEM_ANTIFLAG_WOLFMAN)
			return false;
#endif
	}

	return true;
}

size_t CItemManager::WikiLoadClassItems(BYTE classType, DWORD raceFilter)
{
	m_tempItemVec.clear();

	for (TItemMap::iterator it = m_ItemMap.begin(); it != m_ItemMap.end(); ++it)
	{
		if (!it->second->IsValidImage() || it->first < 10 || it->second->IsBlacklisted())
		{
#ifdef __DBG__
			Tracef("skip %lu %d %d\n", it->first, it->second->IsBlacklisted(), it->second->IsValidImage());
#endif
			continue;
		}

		bool _can_load = CanLoadWikiItem(it->first);

		switch (classType)
		{
		case 0: // weapon
			if (_can_load && it->second->GetType() == CItemData::ITEM_TYPE_WEAPON && !IsFilteredAntiflag(it->second, raceFilter))
				m_tempItemVec.push_back(it->first);
			break;
		case 1: // body
			if (_can_load && it->second->GetType() == CItemData::ITEM_TYPE_ARMOR && it->second->GetSubType() == CItemData::ARMOR_BODY && !IsFilteredAntiflag(it->second, raceFilter))
				m_tempItemVec.push_back(it->first);
			break;
		case 2:
			if (_can_load && it->second->GetType() == CItemData::ITEM_TYPE_ARMOR && it->second->GetSubType() == CItemData::ARMOR_EAR && !IsFilteredAntiflag(it->second, raceFilter))
				m_tempItemVec.push_back(it->first);
			break;
		case 3:
			if (_can_load && it->second->GetType() == CItemData::ITEM_TYPE_ARMOR && it->second->GetSubType() == CItemData::ARMOR_FOOTS && !IsFilteredAntiflag(it->second, raceFilter))
				m_tempItemVec.push_back(it->first);
			break;
		case 4:
			if (_can_load && it->second->GetType() == CItemData::ITEM_TYPE_ARMOR && it->second->GetSubType() == CItemData::ARMOR_HEAD && !IsFilteredAntiflag(it->second, raceFilter))
				m_tempItemVec.push_back(it->first);
			break;
		case 5:
			if (_can_load && it->second->GetType() == CItemData::ITEM_TYPE_ARMOR && it->second->GetSubType() == CItemData::ARMOR_NECK && !IsFilteredAntiflag(it->second, raceFilter))
				m_tempItemVec.push_back(it->first);
			break;
		case 6:
			if (_can_load && it->second->GetType() == CItemData::ITEM_TYPE_ARMOR && it->second->GetSubType() == CItemData::ARMOR_SHIELD && !IsFilteredAntiflag(it->second, raceFilter))
				m_tempItemVec.push_back(it->first);
			break;
		case 7:
			if (_can_load && it->second->GetType() == CItemData::ITEM_TYPE_ARMOR && it->second->GetSubType() == CItemData::ARMOR_WRIST && !IsFilteredAntiflag(it->second, raceFilter))
				m_tempItemVec.push_back(it->first);
			break;
		case 8: // chests
			if (it->second->GetType() == CItemData::ITEM_TYPE_GIFTBOX)
				m_tempItemVec.push_back(it->first);
			break;
		case 9: // belts
			if (_can_load && it->second->GetType() == CItemData::ITEM_TYPE_BELT)
				m_tempItemVec.push_back(it->first);
			break;
		case 10: // talisman
			if (_can_load && it->second->GetType() == CItemData::ITEM_TYPE_ARMOR && it->second->GetSubType() == CItemData::ARMOR_TALISMAN && !IsFilteredAntiflag(it->second, raceFilter))
				m_tempItemVec.push_back(it->first);
			break;
		}
	}

	return m_tempItemVec.size();
}

std::tuple<const char*, int> CItemManager::SelectByNamePart(const char* namePart)
{
	char searchName[CItemData::ITEM_NAME_MAX_LEN + 1];
	memcpy(searchName, namePart, sizeof(searchName));
	for (size_t j = 0; j < sizeof(searchName); j++)
		searchName[j] = static_cast<char>(tolower(searchName[j]));
	std::string tempSearchName = searchName;

	for (TItemMap::iterator i = m_ItemMap.begin(); i != m_ItemMap.end(); i++)
	{
		const CItemData::TItemTable* tbl = i->second->GetTable();

		if (!i->second->IsBlacklisted())
		{
			DWORD StartRefineVnum = GetWikiItemStartRefineVnum(i->first);
			if (StartRefineVnum != 0)
			{
				CItemData* _sRb = nullptr;
				if (!GetItemDataPointer(StartRefineVnum, &_sRb))
					continue;

				if (_sRb->IsBlacklisted())
					continue;
			}
		}
		else
			continue;

		CItemData* itemData = nullptr;
		if (!GetItemDataPointer(i->first, &itemData))
			continue;

		std::string tempName = itemData->GetName();
		if (!tempName.size())
			continue;

		std::transform(tempName.begin(), tempName.end(), tempName.begin(), ::tolower);

		const size_t tempSearchNameLenght = tempSearchName.length();
		if (tempName.length() < tempSearchNameLenght)
			continue;

		if (!tempName.substr(0, tempSearchNameLenght).compare(tempSearchName))
			return std::make_tuple(itemData->GetName(), i->first);
	}

	return std::make_tuple("", -1);
}

bool CItemManager::CanIncrSelectedItemRefineLevel()
{
	if (!m_pSelectedItemData) {
		return false;
	}
	return m_pSelectedItemData->GetRefinedVnum() == 0 ? false : true;
}
#endif


CItemManager::CItemManager() : m_pSelectedItemData(NULL)
{
}
CItemManager::~CItemManager()
{
	Destroy();
}
