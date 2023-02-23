#pragma once

#include "ItemData.h"
#include "../UserInterface/Locale_inc.h"

class CItemManager : public CSingleton<CItemManager>
{
	public:
		enum EItemDescCol
		{
			ITEMDESC_COL_VNUM,
			ITEMDESC_COL_NAME,
			ITEMDESC_COL_DESC,
			ITEMDESC_COL_SUMM,
			ITEMDESC_COL_NUM,
		};

#ifdef ENABLE_ACCE_COSTUME_SYSTEM
		enum EItemScaleColumn
		{
			ITEMSCALE_VNUM,
			ITEMSCALE_JOB,
			ITEMSCALE_SEX,
			ITEMSCALE_SCALE_X,
			ITEMSCALE_SCALE_Y,
			ITEMSCALE_SCALE_Z,
			ITEMSCALE_POSITION_X,
			ITEMSCALE_POSITION_Y,
			ITEMSCALE_POSITION_Z,
			ITEMSCALE_NUM,
		};
#endif
	public:
		typedef std::map<DWORD, CItemData*> TItemMap;
		typedef std::map<std::string, CItemData*> TItemNameMap;

#ifdef INGAME_WIKI
public:
	typedef std::vector<CItemData*> TItemVec;
	typedef std::vector<DWORD> TItemNumVec;

public:
	void WikiAddVnumToBlacklist(DWORD vnum)
	{
		auto it = m_ItemMap.find(vnum);
		if (it != m_ItemMap.end())
			it->second->SetBlacklisted(true);
	};

	TItemNumVec* WikiGetLastItems()
	{
		return &m_tempItemVec;
	}

	bool								CanLoadWikiItem(DWORD dwVnum);
	DWORD							GetWikiItemStartRefineVnum(DWORD dwVnum);
	std::string							GetWikiItemBaseRefineName(DWORD dwVnum);
	size_t								WikiLoadClassItems(BYTE classType, DWORD raceFilter);
	std::tuple<const char*, int>	SelectByNamePart(const char* namePart);

protected:
	TItemNumVec m_tempItemVec;

private:
	bool IsFilteredAntiflag(CItemData* itemData, DWORD raceFilter);
#endif

	public:
		CItemManager();
		virtual ~CItemManager();

		void			Destroy();
#ifdef __ENABLE_TRASH_BIN__
		BOOL			IsUnbreakableItem(DWORD dwIndex);
		int				GetTrashBinRewardCount(DWORD dwIndex);
#endif
		TItemMap		GetItems() const { return m_ItemMap; }
		BOOL			SelectItemData(DWORD dwIndex);
		CItemData *		GetSelectedItemDataPointer();

		BOOL			GetItemDataPointer(DWORD dwItemID, CItemData ** ppItemData);

		/////
		bool			LoadItemDesc(const char* c_szFileName);
		bool			LoadItemList(const char* c_szFileName);
		bool			LoadItemTable(const char* c_szFileName);
#ifdef ENABLE_SHINING_SYSTEM
		bool LoadShiningTable(const char* szShiningTable);
#endif
#ifdef __ENABLE_TRASH_BIN__
		bool			LoadTrashBinUnbreakableList(const char* c_szFileName);
		bool			LoadTrashBinRewardList(const char* c_szFileName);
#endif
		CItemData *		MakeItemData(DWORD dwIndex);
#ifdef ENABLE_ACCE_COSTUME_SYSTEM
		bool	LoadItemScale(const char* c_szFileName);
#endif
#ifdef INGAME_WIKI
		bool	CanIncrSelectedItemRefineLevel();
#endif

	protected:
		TItemMap m_ItemMap;
#ifdef __ENABLE_TRASH_BIN__
		std::vector<DWORD> m_trashbin_unbreakable_vnum_vector;
		std::map<DWORD,DWORD> m_trashbin_reward_map;
#endif
		std::vector<CItemData*>  m_vec_ItemRange;
		CItemData * m_pSelectedItemData;
		std::mutex m_mutex;
};
