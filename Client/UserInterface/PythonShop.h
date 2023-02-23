#pragma once

#include "Packet.h"

/*
 *	상점 처리
 *
 *	2003-01-16 anoa	일차 완료
 *	2003-12-26 levites 수정
 *
 *	2012-10-29 rtsummit 새로운 화폐 출현 및 tab 기능 추가로 인한 shop 확장.
 *
 */
typedef enum
{
	SHOP_COIN_TYPE_GOLD, // DEFAULT VALUE
	SHOP_COIN_TYPE_SECONDARY_COIN,
} EShopCoinType;


class CPythonShop : public CSingleton<CPythonShop>
{
	public:
		CPythonShop(void);
		virtual ~CPythonShop(void);

		void Clear();

		void SetItemData(DWORD dwIndex, const TShopItemData & c_rShopItemData);
		BOOL GetItemData(DWORD dwIndex, const TShopItemData ** c_ppItemData);

		void SetItemData(BYTE tabIdx, DWORD dwSlotPos, const TShopItemData & c_rShopItemData);
		BOOL GetItemData(BYTE tabIdx, DWORD dwSlotPos, const TShopItemData ** c_ppItemData);
#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
		void SetOfflineShopItemData(DWORD dwIndex, const TShopOfflineItemData & c_rShopItemData);
		BOOL GetOfflineShopItemData(DWORD dwIndex, const TShopOfflineItemData ** c_ppItemData);

		void SetOfflineShopItemData(BYTE tabIdx, DWORD dwSlotPos, const TShopOfflineItemData & c_rShopItemData);
		BOOL GetOfflineShopItemData(BYTE tabIdx, DWORD dwSlotPos, const TShopOfflineItemData ** c_ppItemData);
#endif
		void SetTabCount(BYTE bTabCount) { m_bTabCount = bTabCount; }
		BYTE GetTabCount() { return m_bTabCount; }

		void SetTabCoinType(BYTE tabIdx, BYTE coinType);
		BYTE GetTabCoinType(BYTE tabIdx);

		void SetTabName(BYTE tabIdx, const char* name);
		const char* GetTabName(BYTE tabIdx);


		//BOOL GetSlotItemID(DWORD dwSlotPos, DWORD* pdwItemID);
#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
		void	SetShopDisplayedCount(DWORD dwDisplayedCount);
		DWORD	GetShopDisplayedCount();
#endif
#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
		void Open(BOOL isPrivateShop, BOOL isMainPrivateShop, BOOL isOfflineShop);
#else
		void Open(BOOL isPrivateShop, BOOL isMainPrivateShop);
#endif
		void Close();
		BOOL IsOpen();
		BOOL IsPrivateShop();
		BOOL IsMainPlayerPrivateShop();
#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
		BOOL IsOfflineShop();
#endif
		void ClearPrivateShopStock();
#ifdef ENABLE_CHEQUE_SYSTEM
		void AddPrivateShopItemStock(TItemPos ItemPos, BYTE byDisplayPos, DWORD dwPrice, int byChequePrice);
#else
		void AddPrivateShopItemStock(TItemPos ItemPos, BYTE byDisplayPos, DWORD dwPrice);
#endif
		void DelPrivateShopItemStock(TItemPos ItemPos);
		int GetPrivateShopItemPrice(TItemPos ItemPos);
#ifdef ENABLE_CHEQUE_SYSTEM
		int GetPrivateShopItemCheque(TItemPos ItemPos);
#endif
		void BuildPrivateShop(const char * c_szName);
#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
		void ClearOfflineShopStock();
#ifdef ENABLE_CHEQUE_SYSTEM
		void AddOfflineShopItemStock(TItemPos ItemPos, BYTE byDisplayPos, DWORD dwPrice, int bCheque);
#else
		void AddOfflineShopItemStock(TItemPos ItemPos, BYTE byDisplayPos, DWORD dwPrice);
#endif
		void DelOfflineShopItemStock(TItemPos ItemPos);
		long long GetOfflineShopItemPrice(TItemPos ItemPos);

#ifdef ENABLE_CHEQUE_SYSTEM
		int GetOfflineShopItemCheque(TItemPos ItemPos);
#endif

		void BuildOfflineShop(const char * c_szName, BYTE bTime);
#endif

	protected:
		BOOL	CheckSlotIndex(DWORD dwIndex);

	protected:
		BOOL				m_isShoping;
		BOOL				m_isPrivateShop;
		BOOL				m_isMainPlayerPrivateShop;
#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
		BOOL				m_isOfflineShop;
#endif

		struct ShopTab
		{
			ShopTab()
			{
				coinType = SHOP_COIN_TYPE_GOLD;
			}
			BYTE				coinType;
			std::string			name;
			TShopItemData		items[SHOP_HOST_ITEM_MAX_NUM];
		};
#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
		struct OfflineShopTab
		{
			OfflineShopTab()
			{
				coinType = SHOP_COIN_TYPE_GOLD;
			}
			BYTE				coinType;
			std::string			name;
			TShopOfflineItemData		items[OFFLINE_SHOP_HOST_ITEM_MAX_NUM];
		};

		DWORD				m_dwDisplayedCount;
#endif
		BYTE m_bTabCount;
		ShopTab m_aShoptabs[SHOP_TAB_COUNT_MAX];
#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
		OfflineShopTab m_aOfflineShoptabs[SHOP_TAB_COUNT_MAX];
#endif
		typedef std::map<TItemPos, TShopItemTable> TPrivateShopItemStock;
		TPrivateShopItemStock	m_PrivateShopItemStock;
#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
		typedef std::map<TItemPos, TShopItemTable> TOfflineShopItemStock;
		TOfflineShopItemStock	m_OfflineShopItemStock;
#endif

#ifdef ENABLE_PRIVATESHOP_SEARCH_SYSTEM
	public:
		typedef struct SSearchItemInfo
		{
			DWORD	dwVID;

			char	szOwnerName[CHARACTER_NAME_MAX_LEN + 1];
			BYTE	bPos;

			DWORD	dwGold;

#ifdef ENABLE_CHEQUE_SYSTEM
			int	bCheque;
#endif

			DWORD	dwVnum;
			BYTE	bCount;
			DWORD	dwFlags;
			DWORD	dwAntiFlags;
			long	alSockets[ITEM_SOCKET_SLOT_MAX_NUM];
			TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_SLOT_MAX_NUM];

#ifdef ENABLE_CHANGELOOK_SYSTEM
			DWORD	dwTransmutation;
#endif
		} TSearchItemInfo;

		void	SearchItemDataClear();

		void	SetSearchItemAppend(const TSearchItemInfo & c_rSearchItem);
		bool	GetSearchItem(DWORD dwIndex, const TSearchItemInfo ** c_ppSearchItem);

		DWORD	GetSearchItemResultCount() const { return m_vecSearchItem.size(); }

	protected:
		typedef std::vector<TSearchItemInfo> TShopSearchItem;
		TShopSearchItem	m_vecSearchItem;
#endif
};