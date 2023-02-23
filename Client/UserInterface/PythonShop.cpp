#include "stdafx.h"
#include "PythonShop.h"

#include "PythonNetworkStream.h"

//BOOL CPythonShop::GetSlotItemID(DWORD dwSlotPos, DWORD* pdwItemID)
//{
//	if (!CheckSlotIndex(dwSlotPos))
//		return FALSE;
//	const TShopItemData * itemData;
//	if (!GetItemData(dwSlotPos, &itemData))
//		return FALSE;
//	*pdwItemID=itemData->vnum;
//	return TRUE;
//}
void CPythonShop::SetTabCoinType(BYTE tabIdx, BYTE coinType)
{
	if (tabIdx >= m_bTabCount)
	{
		TraceError("Out of Index. tabIdx(%d) must be less than %d.", tabIdx, SHOP_TAB_COUNT_MAX);
		return;
	}
	m_aShoptabs[tabIdx].coinType = coinType;
}

BYTE CPythonShop::GetTabCoinType(BYTE tabIdx)
{
	if (tabIdx >= m_bTabCount)
	{
		TraceError("Out of Index. tabIdx(%d) must be less than %d.", tabIdx, SHOP_TAB_COUNT_MAX);
		return 0xff;
	}
	return m_aShoptabs[tabIdx].coinType;
}

void CPythonShop::SetTabName(BYTE tabIdx, const char* name)
{
	if (tabIdx >= m_bTabCount)
	{
		TraceError("Out of Index. tabIdx(%d) must be less than %d.", tabIdx, SHOP_TAB_COUNT_MAX);
		return;
	}
	m_aShoptabs[tabIdx].name = name;
}

const char* CPythonShop::GetTabName(BYTE tabIdx)
{
	if (tabIdx >= m_bTabCount)
	{
		TraceError("Out of Index. tabIdx(%d) must be less than %d.", tabIdx, SHOP_TAB_COUNT_MAX);
		return NULL;
	}

	return m_aShoptabs[tabIdx].name.c_str();
}

void CPythonShop::SetItemData(DWORD dwIndex, const TShopItemData & c_rShopItemData)
{
	BYTE tabIdx = dwIndex / SHOP_HOST_ITEM_MAX_NUM;
	DWORD dwSlotPos = dwIndex % SHOP_HOST_ITEM_MAX_NUM;

	SetItemData(tabIdx, dwSlotPos, c_rShopItemData);
}

BOOL CPythonShop::GetItemData(DWORD dwIndex, const TShopItemData ** c_ppItemData)
{
	BYTE tabIdx = dwIndex / SHOP_HOST_ITEM_MAX_NUM;
	DWORD dwSlotPos = dwIndex % SHOP_HOST_ITEM_MAX_NUM;

	return GetItemData(tabIdx, dwSlotPos, c_ppItemData);
}

void CPythonShop::SetItemData(BYTE tabIdx, DWORD dwSlotPos, const TShopItemData & c_rShopItemData)
{
	if (tabIdx >= SHOP_TAB_COUNT_MAX || dwSlotPos >= SHOP_HOST_ITEM_MAX_NUM)
	{
		TraceError("Out of Index. tabIdx(%d) must be less than %d. dwSlotPos(%d) must be less than %d", tabIdx, SHOP_TAB_COUNT_MAX, dwSlotPos, SHOP_HOST_ITEM_MAX_NUM);
		return;
	}

#ifdef ENABLE_DEBUG_NEW_MONEY
	TraceError("recevied item data -> vnum %d , count %d , price yang %d , price edeni %d , price sacchi %d ", c_rShopItemData.vnum , c_rShopItemData.count , c_rShopItemData.price , c_rShopItemData.priceEdeni , c_rShopItemData.priceSacchi);
#endif

	m_aShoptabs[tabIdx].items[dwSlotPos] = c_rShopItemData;
}

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
void CPythonShop::SetOfflineShopItemData(DWORD dwIndex, const TShopOfflineItemData & c_rShopItemData)
{
	BYTE tabIdx = dwIndex / OFFLINE_SHOP_HOST_ITEM_MAX_NUM;
	DWORD dwSlotPos = dwIndex % OFFLINE_SHOP_HOST_ITEM_MAX_NUM;

	SetOfflineShopItemData(tabIdx, dwSlotPos, c_rShopItemData);
}

void CPythonShop::SetOfflineShopItemData(BYTE tabIdx, DWORD dwSlotPos, const TShopOfflineItemData & c_rShopItemData)
{
	if (tabIdx >= SHOP_TAB_COUNT_MAX || dwSlotPos >= OFFLINE_SHOP_HOST_ITEM_MAX_NUM)
	{
		TraceError("Out of Index. tabIdx(%d) must be less than %d. dwSlotPos(%d) must be less than %d", tabIdx, SHOP_TAB_COUNT_MAX, dwSlotPos, OFFLINE_SHOP_HOST_ITEM_MAX_NUM);
		return;
	}

	m_aOfflineShoptabs[tabIdx].items[dwSlotPos] = c_rShopItemData;
}

BOOL CPythonShop::GetOfflineShopItemData(DWORD dwIndex, const TShopOfflineItemData ** c_ppItemData)
{
	BYTE tabIdx = dwIndex / OFFLINE_SHOP_HOST_ITEM_MAX_NUM;
	DWORD dwSlotPos = dwIndex % OFFLINE_SHOP_HOST_ITEM_MAX_NUM;

	return GetOfflineShopItemData(tabIdx, dwSlotPos, c_ppItemData);
}

BOOL CPythonShop::GetOfflineShopItemData(BYTE tabIdx, DWORD dwSlotPos, const TShopOfflineItemData ** c_ppItemData)
{
	if (tabIdx >= SHOP_TAB_COUNT_MAX || dwSlotPos >= OFFLINE_SHOP_HOST_ITEM_MAX_NUM)
	{
		TraceError("Out of Index. tabIdx(%d) must be less than %d. dwSlotPos(%d) must be less than %d", tabIdx, SHOP_TAB_COUNT_MAX, dwSlotPos, OFFLINE_SHOP_HOST_ITEM_MAX_NUM);
		return FALSE;
	}

	*c_ppItemData = &m_aOfflineShoptabs[tabIdx].items[dwSlotPos];

	return TRUE;
}
#endif

BOOL CPythonShop::GetItemData(BYTE tabIdx, DWORD dwSlotPos, const TShopItemData ** c_ppItemData)
{
	if (tabIdx >= SHOP_TAB_COUNT_MAX || dwSlotPos >= SHOP_HOST_ITEM_MAX_NUM)
	{
		TraceError("Out of Index. tabIdx(%d) must be less than %d. dwSlotPos(%d) must be less than %d", tabIdx, SHOP_TAB_COUNT_MAX, dwSlotPos, SHOP_HOST_ITEM_MAX_NUM);
		return FALSE;
	}

	*c_ppItemData = &m_aShoptabs[tabIdx].items[dwSlotPos];

	return TRUE;
}
//
//BOOL CPythonShop::CheckSlotIndex(DWORD dwSlotPos)
//{
//	if (dwSlotPos >= SHOP_HOST_ITEM_MAX_NUM * SHOP_TAB_COUNT_MAX)
//		return FALSE;
//
//	return TRUE;
//}

void CPythonShop::ClearPrivateShopStock()
{
	m_PrivateShopItemStock.clear();
}

#ifdef ENABLE_CHEQUE_SYSTEM
void CPythonShop::AddPrivateShopItemStock(TItemPos ItemPos, BYTE dwDisplayPos, DWORD dwPrice, int byChequePrice)
#else
void CPythonShop::AddPrivateShopItemStock(TItemPos ItemPos, BYTE dwDisplayPos, DWORD dwPrice)
#endif
{
	DelPrivateShopItemStock(ItemPos);

	TShopItemTable SellingItem;
	SellingItem.vnum = 0;
	SellingItem.count = 0;
	SellingItem.pos = ItemPos;
	SellingItem.price = dwPrice;
#ifdef ENABLE_CHEQUE_SYSTEM
	SellingItem.cheque_price = byChequePrice;
#endif
	SellingItem.display_pos = dwDisplayPos;
	m_PrivateShopItemStock.insert(std::make_pair(ItemPos, SellingItem));
}

void CPythonShop::DelPrivateShopItemStock(TItemPos ItemPos)
{
	if (m_PrivateShopItemStock.end() == m_PrivateShopItemStock.find(ItemPos))
		return;

	m_PrivateShopItemStock.erase(ItemPos);
}

int CPythonShop::GetPrivateShopItemPrice(TItemPos ItemPos)
{
	TPrivateShopItemStock::iterator itor = m_PrivateShopItemStock.find(ItemPos);

	if (m_PrivateShopItemStock.end() == itor)
		return 0;

	TShopItemTable & rShopItemTable = itor->second;
	return rShopItemTable.price;
}

#ifdef ENABLE_CHEQUE_SYSTEM
int CPythonShop::GetPrivateShopItemCheque(TItemPos ItemPos)
{
	TPrivateShopItemStock::iterator itor = m_PrivateShopItemStock.find(ItemPos);

	if (m_PrivateShopItemStock.end() == itor)
		return 0;

	TShopItemTable & rShopItemTable = itor->second;
	return rShopItemTable.cheque_price;
}
#endif

struct ItemStockSortFunc
{
	bool operator() (TShopItemTable & rkLeft, TShopItemTable & rkRight)
	{
		return rkLeft.display_pos < rkRight.display_pos;
	}
};

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
void CPythonShop::ClearOfflineShopStock()
{
	m_OfflineShopItemStock.clear();
}


#ifdef ENABLE_CHEQUE_SYSTEM
void CPythonShop::AddOfflineShopItemStock(TItemPos ItemPos, BYTE dwDisplayPos, DWORD dwPrice, int bCheque)
#else
void CPythonShop::AddOfflineShopItemStock(TItemPos ItemPos, BYTE dwDisplayPos, DWORD dwPrice)
#endif
{
	DelOfflineShopItemStock(ItemPos);

	TShopItemTable SellingItem;
	SellingItem.vnum = 0;
	SellingItem.count = 0;
	SellingItem.pos = ItemPos;
	SellingItem.price = dwPrice;

#ifdef ENABLE_CHEQUE_SYSTEM
	SellingItem.cheque_price = bCheque;
#endif

	SellingItem.display_pos = dwDisplayPos;
	m_OfflineShopItemStock.insert(std::make_pair(ItemPos, SellingItem));
}

void CPythonShop::DelOfflineShopItemStock(TItemPos ItemPos)
{
	if (m_OfflineShopItemStock.end() == m_OfflineShopItemStock.find(ItemPos))
		return;

	m_OfflineShopItemStock.erase(ItemPos);
}

long long CPythonShop::GetOfflineShopItemPrice(TItemPos ItemPos)
{
	TOfflineShopItemStock::iterator itor = m_OfflineShopItemStock.find(ItemPos);

	if (m_OfflineShopItemStock.end() == itor)
		return 0;

	TShopItemTable & rShopItemTable = itor->second;
	return rShopItemTable.price;
}

#ifdef ENABLE_CHEQUE_SYSTEM
int CPythonShop::GetOfflineShopItemCheque(TItemPos ItemPos)
{
	TOfflineShopItemStock::iterator itor = m_OfflineShopItemStock.find(ItemPos);

	if (m_OfflineShopItemStock.end() == itor)
		return 0;

	TShopItemTable & rShopItemTable = itor->second;
	return rShopItemTable.cheque_price;
}
#endif

void CPythonShop::BuildOfflineShop(const char * c_szName, BYTE bTime)
{
	std::vector<TShopItemTable> ItemStock;
	ItemStock.reserve(m_OfflineShopItemStock.size());

	TOfflineShopItemStock::iterator itor = m_OfflineShopItemStock.begin();
	for (; itor != m_OfflineShopItemStock.end(); ++itor)
	{
		ItemStock.push_back(itor->second);
	}

	std::sort(ItemStock.begin(), ItemStock.end(), ItemStockSortFunc());

	CPythonNetworkStream::Instance().SendBuildOfflineShopPacket(c_szName, ItemStock, bTime);
}
#endif

void CPythonShop::BuildPrivateShop(const char * c_szName)
{
	std::vector<TShopItemTable> ItemStock;
	ItemStock.reserve(m_PrivateShopItemStock.size());

	TPrivateShopItemStock::iterator itor = m_PrivateShopItemStock.begin();
	for (; itor != m_PrivateShopItemStock.end(); ++itor)
	{
		ItemStock.push_back(itor->second);
	}

	std::sort(ItemStock.begin(), ItemStock.end(), ItemStockSortFunc());
	CPythonNetworkStream::Instance().SendBuildPrivateShopPacket(c_szName, ItemStock);

}

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
void CPythonShop::Open(BOOL isPrivateShop, BOOL isMainPrivateShop, BOOL isOfflineShop)
#else
void CPythonShop::Open(BOOL isPrivateShop, BOOL isMainPrivateShop)
#endif
{
	m_isShoping = TRUE;
	m_isPrivateShop = isPrivateShop;
	m_isMainPlayerPrivateShop = isMainPrivateShop;
#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	m_isOfflineShop = isOfflineShop;
#endif
}

void CPythonShop::Close()
{
	m_isShoping = FALSE;
	m_isPrivateShop = FALSE;
	m_isMainPlayerPrivateShop = FALSE;
}

BOOL CPythonShop::IsOpen()
{
	return m_isShoping;
}

BOOL CPythonShop::IsPrivateShop()
{
	return m_isPrivateShop;
}

BOOL CPythonShop::IsMainPlayerPrivateShop()
{
	return m_isMainPlayerPrivateShop;
}

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
BOOL CPythonShop::IsOfflineShop()
{
	return m_isOfflineShop;
}

void CPythonShop::SetShopDisplayedCount(DWORD dwDisplayedCount)
{
	m_dwDisplayedCount = dwDisplayedCount;
}

DWORD CPythonShop::GetShopDisplayedCount()
{
	return m_dwDisplayedCount;
}
#endif

#ifdef ENABLE_PRIVATESHOP_SEARCH_SYSTEM
void CPythonShop::SearchItemDataClear()
{
	m_vecSearchItem.clear();
}

void CPythonShop::SetSearchItemAppend(const TSearchItemInfo & c_rSearchItem)
{
	m_vecSearchItem.push_back(c_rSearchItem);
}

bool CPythonShop::GetSearchItem(DWORD dwIndex, const TSearchItemInfo ** c_ppSearchItem)
{
	*c_ppSearchItem = &m_vecSearchItem[dwIndex];
	return true;
}
#endif

void CPythonShop::Clear()
{
	m_isShoping = FALSE;
	m_isPrivateShop = FALSE;
	m_isMainPlayerPrivateShop = FALSE;
#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	m_isOfflineShop = FALSE;
#endif
	ClearPrivateShopStock();
#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	ClearOfflineShopStock();
#endif
#ifdef ENABLE_PRIVATESHOP_SEARCH_SYSTEM
	SearchItemDataClear();
#endif

	m_bTabCount = 1;

	for (int i = 0; i < SHOP_TAB_COUNT_MAX; i++)
		memset (m_aShoptabs[i].items, 0, sizeof(TShopItemData) * SHOP_HOST_ITEM_MAX_NUM);
}

CPythonShop::CPythonShop(void)
{
	Clear();
}

CPythonShop::~CPythonShop(void)
{
}

PyObject * shopOpen(PyObject * poSelf, PyObject * poArgs)
{
	int isPrivateShop = false;
	PyTuple_GetInteger(poArgs, 0, &isPrivateShop);

	int isMainPrivateShop = false;
	PyTuple_GetInteger(poArgs, 1, &isMainPrivateShop);
#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	int isOfflineShop = false;
	PyTuple_GetInteger(poArgs, 2, &isOfflineShop);
#endif

	CPythonShop& rkShop = CPythonShop::Instance();
#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	rkShop.Open(isPrivateShop, isMainPrivateShop, isOfflineShop);
#else
	rkShop.Open(isPrivateShop, isMainPrivateShop);
#endif
	return Py_BuildNone();
}

PyObject * shopClose(PyObject * poSelf, PyObject * poArgs)
{
	CPythonShop& rkShop=CPythonShop::Instance();
	rkShop.Close();
	return Py_BuildNone();
}

PyObject * shopIsOpen(PyObject * poSelf, PyObject * poArgs)
{
	CPythonShop& rkShop=CPythonShop::Instance();
	return Py_BuildValue("i", rkShop.IsOpen());
}

PyObject * shopIsPrviateShop(PyObject * poSelf, PyObject * poArgs)
{
	CPythonShop& rkShop=CPythonShop::Instance();
	return Py_BuildValue("i", rkShop.IsPrivateShop());
}

PyObject * shopIsMainPlayerPrivateShop(PyObject * poSelf, PyObject * poArgs)
{
	CPythonShop& rkShop=CPythonShop::Instance();
	return Py_BuildValue("i", rkShop.IsMainPlayerPrivateShop());
}

PyObject * shopGetItemID(PyObject * poSelf, PyObject * poArgs)
{
	int nPos;
	if (!PyTuple_GetInteger(poArgs, 0, &nPos))
		return Py_BuildException();

	const TShopItemData * c_pItemData;
	if (CPythonShop::Instance().GetItemData(nPos, &c_pItemData))
		return Py_BuildValue("i", c_pItemData->vnum);

	return Py_BuildValue("i", 0);
}

PyObject * shopGetItemCount(PyObject * poSelf, PyObject * poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildException();

	const TShopItemData * c_pItemData;
	if (CPythonShop::Instance().GetItemData(iIndex, &c_pItemData))
		return Py_BuildValue("i", c_pItemData->count);

	return Py_BuildValue("i", 0);
}

PyObject * shopGetItemPrice(PyObject * poSelf, PyObject * poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildException();

	const TShopItemData * c_pItemData;
	if (CPythonShop::Instance().GetItemData(iIndex, &c_pItemData))
#ifdef ENABLE_CHEQUE_SYSTEM
		return Py_BuildValue("i", c_pItemData->price.dwPrice);
#else
		return Py_BuildValue("i", c_pItemData->price);
#endif

	return Py_BuildValue("i", 0);
}

#ifdef ENABLE_CHEQUE_SYSTEM
PyObject * shopGetItemCheque(PyObject * poSelf, PyObject * poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildException();

	const TShopItemData * c_pItemData;
	if (CPythonShop::Instance().GetItemData(iIndex, &c_pItemData))
		return Py_BuildValue("i", c_pItemData->price.byChequePrice);

	return Py_BuildValue("i", 0);
}
#endif

PyObject * shopGetItemMetinSocket(PyObject * poSelf, PyObject * poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildException();
	int iMetinSocketIndex;
	if (!PyTuple_GetInteger(poArgs, 1, &iMetinSocketIndex))
		return Py_BuildException();

	const TShopItemData * c_pItemData;
	if (CPythonShop::Instance().GetItemData(iIndex, &c_pItemData))
		return Py_BuildValue("i", c_pItemData->alSockets[iMetinSocketIndex]);

	return Py_BuildValue("i", 0);
}

#ifdef ENABLE_MULTISHOP
PyObject* shopGetBuyWithItem(PyObject* poSelf, PyObject* poArgs)
{
	int nPos;
	if (!PyTuple_GetInteger(poArgs, 0, &nPos))
		return Py_BuildException();

	const TShopItemData* c_pItemData;
	if (CPythonShop::Instance().GetItemData(nPos, &c_pItemData))
		return Py_BuildValue("i", c_pItemData->wPriceVnum);

	return Py_BuildValue("i", 0);
}

PyObject* shopGetBuyWithItemCount(PyObject* poSelf, PyObject* poArgs)
{
	int nPos;
	if (!PyTuple_GetInteger(poArgs, 0, &nPos))
		return Py_BuildException();

	const TShopItemData* c_pItemData;
	if (CPythonShop::Instance().GetItemData(nPos, &c_pItemData))
		return Py_BuildValue("i", c_pItemData->wPrice);

	return Py_BuildValue("i", 0);
}
#endif

#ifdef ENABLE_FROZEN_SYSTEM
PyObject * shopGetItemAttribute(PyObject * poSelf, PyObject * poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildException();
	int iAttrSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 1, &iAttrSlotIndex))
		return Py_BuildException();

	if (iAttrSlotIndex >= 0 && iAttrSlotIndex < ITEM_ATTRIBUTE_SLOT_MAX_NUM)
	{
		const TShopItemData * c_pItemData;
		if (CPythonShop::Instance().GetItemData(iIndex, &c_pItemData))
			return Py_BuildValue("iii", c_pItemData->aAttr[iAttrSlotIndex].bType, c_pItemData->aAttr[iAttrSlotIndex].sValue, c_pItemData->aAttr[iAttrSlotIndex].isFrozen);
	}

	return Py_BuildValue("iii", 0, 0, 0);
}

#else

PyObject * shopGetItemAttribute(PyObject * poSelf, PyObject * poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildException();
	int iAttrSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 1, &iAttrSlotIndex))
		return Py_BuildException();

	if (iAttrSlotIndex >= 0 && iAttrSlotIndex < ITEM_ATTRIBUTE_SLOT_MAX_NUM)
	{
		const TShopItemData * c_pItemData;
		if (CPythonShop::Instance().GetItemData(iIndex, &c_pItemData))
			return Py_BuildValue("ii", c_pItemData->aAttr[iAttrSlotIndex].bType, c_pItemData->aAttr[iAttrSlotIndex].sValue);
	}

	return Py_BuildValue("ii", 0, 0);
}

#endif

PyObject * shopClearPrivateShopStock(PyObject * poSelf, PyObject * poArgs)
{
	CPythonShop::Instance().ClearPrivateShopStock();
	return Py_BuildNone();
}
PyObject * shopAddPrivateShopItemStock(PyObject * poSelf, PyObject * poArgs)
{
	BYTE bItemWindowType;
	if (!PyTuple_GetInteger(poArgs, 0, &bItemWindowType))
		return Py_BuildException();
	WORD wItemSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 1, &wItemSlotIndex))
		return Py_BuildException();
	int iDisplaySlotIndex;
	if (!PyTuple_GetInteger(poArgs, 2, &iDisplaySlotIndex))
		return Py_BuildException();

	int iPrice;
	if (!PyTuple_GetInteger(poArgs, 3, &iPrice))
		return Py_BuildException();
#ifdef ENABLE_CHEQUE_SYSTEM
	int bCheque;
	if (!PyTuple_GetInteger(poArgs, 4, &bCheque))
		return Py_BuildException();

	CPythonShop::Instance().AddPrivateShopItemStock(TItemPos(bItemWindowType, wItemSlotIndex), iDisplaySlotIndex, iPrice, bCheque);
#else
	CPythonShop::Instance().AddPrivateShopItemStock(TItemPos(bItemWindowType, wItemSlotIndex), iDisplaySlotIndex, iPrice);
#endif
	return Py_BuildNone();
}

PyObject * shopDelPrivateShopItemStock(PyObject * poSelf, PyObject * poArgs)
{
	BYTE bItemWindowType;
	if (!PyTuple_GetInteger(poArgs, 0, &bItemWindowType))
		return Py_BuildException();
	WORD wItemSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 1, &wItemSlotIndex))
		return Py_BuildException();

	CPythonShop::Instance().DelPrivateShopItemStock(TItemPos(bItemWindowType, wItemSlotIndex));
	return Py_BuildNone();
}

PyObject * shopGetPrivateShopItemPrice(PyObject * poSelf, PyObject * poArgs)
{
	BYTE bItemWindowType;
	if (!PyTuple_GetInteger(poArgs, 0, &bItemWindowType))
		return Py_BuildException();
	WORD wItemSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 1, &wItemSlotIndex))
		return Py_BuildException();

	return Py_BuildValue("i", CPythonShop::Instance().GetPrivateShopItemPrice(TItemPos(bItemWindowType, wItemSlotIndex)));

	return Py_BuildValue("i", 0);
}

#ifdef ENABLE_CHEQUE_SYSTEM
PyObject * shopGetPrivateShopItemCheque(PyObject * poSelf, PyObject * poArgs)
{
	BYTE bItemWindowType;
	if (!PyTuple_GetInteger(poArgs, 0, &bItemWindowType))
		return Py_BuildException();
	WORD wItemSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 1, &wItemSlotIndex))
		return Py_BuildException();

	int8_t iValue = CPythonShop::Instance().GetPrivateShopItemCheque(TItemPos(bItemWindowType, wItemSlotIndex));
	return Py_BuildValue("i", iValue);
}
#endif

PyObject * shopBuildPrivateShop(PyObject * poSelf, PyObject * poArgs)
{
	char * szName;
	if (!PyTuple_GetString(poArgs, 0, &szName))
		return Py_BuildException();
	CPythonShop::Instance().BuildPrivateShop(szName);

	return Py_BuildNone();
}

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
PyObject * shopIsOfflineShop(PyObject * poSelf, PyObject * poArgs)
{
	CPythonShop & rkShop = CPythonShop::Instance();
	return Py_BuildValue("i", rkShop.IsOfflineShop());
}

PyObject * shopGetOfflineShopItemID(PyObject * poSelf, PyObject * poArgs)
{
	int nPos;
	if (!PyTuple_GetInteger(poArgs, 0, &nPos))
		return Py_BuildException();

	const TShopOfflineItemData * c_pItemData;
	if (CPythonShop::Instance().GetOfflineShopItemData(nPos, &c_pItemData))
		return Py_BuildValue("i", c_pItemData->vnum);

	return Py_BuildValue("i", 0);
}

PyObject * shopGetOfflineShopItemStatus(PyObject * poSelf, PyObject * poArgs)
{
	int nPos;
	if (!PyTuple_GetInteger(poArgs, 0, &nPos))
		return Py_BuildException();

	const TShopOfflineItemData * c_pItemData;
	if (CPythonShop::Instance().GetOfflineShopItemData(nPos, &c_pItemData))
		return Py_BuildValue("is", c_pItemData->bIsSold, c_pItemData->buyerName);

	return Py_BuildValue("is", 0, "");
}

PyObject * shopGetOfflineShopItemCount(PyObject * poSelf, PyObject * poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildException();

	const TShopOfflineItemData * c_pItemData;
	if (CPythonShop::Instance().GetOfflineShopItemData(iIndex, &c_pItemData))
		return Py_BuildValue("i", c_pItemData->count);

	return Py_BuildValue("i", 0);
}

// BEGIN_MAX_YANG
PyObject * shopGetOfflineShopItemPrice(PyObject * poSelf, PyObject * poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildException();

	const TShopOfflineItemData * c_pItemData;
	if (CPythonShop::Instance().GetOfflineShopItemData(iIndex, &c_pItemData))
		return Py_BuildValue("i", c_pItemData->price);

	return Py_BuildValue("i", 0);
}
// END_OF_MAX_YANG

PyObject * shopGetOfflineShopItemMetinSocket(PyObject * poSelf, PyObject * poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildException();
	int iMetinSocketIndex;
	if (!PyTuple_GetInteger(poArgs, 1, &iMetinSocketIndex))
		return Py_BuildException();

	const TShopOfflineItemData * c_pItemData;
	if (CPythonShop::Instance().GetOfflineShopItemData(iIndex, &c_pItemData))
		return Py_BuildValue("i", c_pItemData->alSockets[iMetinSocketIndex]);

	return Py_BuildValue("i", 0);
}

PyObject * shopGetOfflineShopItemAttribute(PyObject * poSelf, PyObject * poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildException();
	int iAttrSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 1, &iAttrSlotIndex))
		return Py_BuildException();

	if (iAttrSlotIndex >= 0 && iAttrSlotIndex < ITEM_ATTRIBUTE_SLOT_MAX_NUM)
	{
		const TShopOfflineItemData * c_pItemData;
		if (CPythonShop::Instance().GetOfflineShopItemData(iIndex, &c_pItemData))
#ifdef ENABLE_FROZEN_SYSTEM
			return Py_BuildValue("iii", c_pItemData->aAttr[iAttrSlotIndex].bType, c_pItemData->aAttr[iAttrSlotIndex].sValue, c_pItemData->aAttr[iAttrSlotIndex].isFrozen);
#else
			return Py_BuildValue("ii", c_pItemData->aAttr[iAttrSlotIndex].bType, c_pItemData->aAttr[iAttrSlotIndex].sValue);
#endif
	}

#ifdef ENABLE_FROZEN_SYSTEM
	return Py_BuildValue("iii", 0, 0, 0);
#else
	return Py_BuildValue("ii", 0, 0);
#endif
}

PyObject * shopClearOfflineShopStock(PyObject * poSelf, PyObject * poArgs)
{
	CPythonShop::Instance().ClearOfflineShopStock();
	return Py_BuildNone();
}
PyObject * shopAddOfflineShopItemStock(PyObject * poSelf, PyObject * poArgs)
{
	BYTE bItemWindowType;
	if (!PyTuple_GetInteger(poArgs, 0, &bItemWindowType))
		return Py_BuildException();
	WORD wItemSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 1, &wItemSlotIndex))
		return Py_BuildException();
	int iDisplaySlotIndex;
	if (!PyTuple_GetInteger(poArgs, 2, &iDisplaySlotIndex))
		return Py_BuildException();
	int llPrice;
	if (!PyTuple_GetInteger(poArgs, 3, &llPrice))
		return Py_BuildException();

#ifdef ENABLE_CHEQUE_SYSTEM
	int bCheque;
	if (!PyTuple_GetInteger(poArgs, 4, &bCheque))
		return Py_BuildException();

	CPythonShop::Instance().AddOfflineShopItemStock(TItemPos(bItemWindowType, wItemSlotIndex), iDisplaySlotIndex, llPrice, bCheque);
#else
	CPythonShop::Instance().AddOfflineShopItemStock(TItemPos(bItemWindowType, wItemSlotIndex), iDisplaySlotIndex, llPrice);
#endif
	return Py_BuildNone();
}
PyObject * shopDelOfflineShopItemStock(PyObject * poSelf, PyObject * poArgs)
{
	BYTE bItemWindowType;
	if (!PyTuple_GetInteger(poArgs, 0, &bItemWindowType))
		return Py_BuildException();
	WORD wItemSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 1, &wItemSlotIndex))
		return Py_BuildException();

	CPythonShop::Instance().DelOfflineShopItemStock(TItemPos(bItemWindowType, wItemSlotIndex));
	return Py_BuildNone();
}
PyObject * shopGetOfflineShopItemPrice2(PyObject * poSelf, PyObject * poArgs)
{
	BYTE bItemWindowType;
	if (!PyTuple_GetInteger(poArgs, 0, &bItemWindowType))
		return Py_BuildException();
	WORD wItemSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 1, &wItemSlotIndex))
		return Py_BuildException();

	// BEGIN_MAX_YANG
	int iValue = CPythonShop::Instance().GetOfflineShopItemPrice(TItemPos(bItemWindowType, wItemSlotIndex));
	return Py_BuildValue("i", iValue);
	// END_OF_MAX_YANG
}
PyObject * shopBuildOfflineShop(PyObject * poSelf, PyObject * poArgs)
{
	char * szName;
	if (!PyTuple_GetString(poArgs, 0, &szName))
		return Py_BuildException();

	BYTE bTime;
	if (!PyTuple_GetInteger(poArgs, 1, &bTime))
		return Py_BuildException();

	CPythonShop::Instance().BuildOfflineShop(szName, bTime);
	return Py_BuildNone();
}

PyObject * shopGetDisplayedCount(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonShop::instance().GetShopDisplayedCount());
}

#ifdef ENABLE_CHEQUE_SYSTEM
PyObject * shopGetOfflineShopItemCheque(PyObject * poSelf, PyObject * poArgs)
{
	int bCheque = 0;
	switch (PyTuple_Size(poArgs))
	{
	case 1:
		int iIndex;
		if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
			return Py_BuildException();

		const TShopOfflineItemData * c_pItemData;
		if (CPythonShop::Instance().GetOfflineShopItemData(iIndex, &c_pItemData))
			bCheque = c_pItemData->cheque;

		break;
	case 2:
		BYTE bItemWindowType;
		if (!PyTuple_GetInteger(poArgs, 0, &bItemWindowType))
			return Py_BuildException();
		WORD wItemSlotIndex;
		if (!PyTuple_GetInteger(poArgs, 1, &wItemSlotIndex))
			return Py_BuildException();

		bCheque = CPythonShop::Instance().GetOfflineShopItemCheque(TItemPos(bItemWindowType, wItemSlotIndex));
		break;
	default:
		return Py_BuildException();
	}

	return Py_BuildValue("i", bCheque);
}
#endif
#endif

#ifdef ENABLE_CHANGELOOK_SYSTEM
PyObject * shopGetOfflineShopItemTransmutation(PyObject * poSelf, PyObject * poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildException();

	const TShopOfflineItemData * c_pItemData;
	if (CPythonShop::Instance().GetOfflineShopItemData(iIndex, &c_pItemData))
		return Py_BuildValue("i", c_pItemData->transmutation);

	return Py_BuildValue("i", 0);
}
#endif

PyObject * shopGetTabCount(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonShop::instance().GetTabCount());
}

PyObject * shopGetTabName(PyObject * poSelf, PyObject * poArgs)
{
	BYTE bTabIdx;
	if (!PyTuple_GetInteger(poArgs, 0, &bTabIdx))
		return Py_BuildException();

	return Py_BuildValue("s", CPythonShop::instance().GetTabName(bTabIdx));
}

PyObject * shopGetTabCoinType(PyObject * poSelf, PyObject * poArgs)
{
	BYTE bTabIdx;
	if (!PyTuple_GetInteger(poArgs, 0, &bTabIdx))
		return Py_BuildException();

	return Py_BuildValue("i", CPythonShop::instance().GetTabCoinType(bTabIdx));
}

#ifdef ENABLE_CHANGELOOK_SYSTEM
PyObject * shopGetItemTransmutation(PyObject * poSelf, PyObject * poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildException();

	const TShopItemData * c_pItemData;
	if (CPythonShop::Instance().GetItemData(iIndex, &c_pItemData))
		return Py_BuildValue("i", c_pItemData->transmutation);

	return Py_BuildValue("i", 0);
}
#endif

#ifdef ENABLE_PRIVATESHOP_SEARCH_SYSTEM
PyObject* shopSearchItemDataClear(PyObject* poSelf, PyObject* poArgs)
{
	CPythonShop::Instance().SearchItemDataClear();
	return Py_BuildNone();
}

PyObject* shopGetSearchItemResultCount(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonShop::Instance().GetSearchItemResultCount());
}

PyObject* shopGetSearchItemShopVID(PyObject* poSelf, PyObject* poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildException();

	const CPythonShop::TSearchItemInfo* c_pItemData;
	if (CPythonShop::Instance().GetSearchItem(iIndex, &c_pItemData))
		return Py_BuildValue("i", c_pItemData->dwVID);

	return Py_BuildValue("i", 0);
}

PyObject* shopGetSearchItemOwnerName(PyObject* poSelf, PyObject* poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildException();

	const CPythonShop::TSearchItemInfo* c_pItemData;
	if (CPythonShop::Instance().GetSearchItem(iIndex, &c_pItemData))
		return Py_BuildValue("s", c_pItemData->szOwnerName);

	return Py_BuildValue("s", "");
}

PyObject* shopGetSearchItemPos(PyObject* poSelf, PyObject* poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildException();

	const CPythonShop::TSearchItemInfo* c_pItemData;
	if (CPythonShop::Instance().GetSearchItem(iIndex, &c_pItemData))
		return Py_BuildValue("i", c_pItemData->bPos);

	return Py_BuildValue("i", 0);
}

PyObject* shopGetSearchItemGold(PyObject* poSelf, PyObject* poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildException();

	const CPythonShop::TSearchItemInfo* c_pItemData;
	if (CPythonShop::Instance().GetSearchItem(iIndex, &c_pItemData))
		return Py_BuildValue("i", c_pItemData->dwGold);

	return Py_BuildValue("i", 0);
}

PyObject* shopGetSearchItemVnum(PyObject* poSelf, PyObject* poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildException();

	const CPythonShop::TSearchItemInfo* c_pItemData;
	if (CPythonShop::Instance().GetSearchItem(iIndex, &c_pItemData))
		return Py_BuildValue("i", c_pItemData->dwVnum);

	return Py_BuildValue("i", 0);
}

PyObject* shopGetSearchItemCount(PyObject* poSelf, PyObject* poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildException();

	const CPythonShop::TSearchItemInfo* c_pItemData;
	if (CPythonShop::Instance().GetSearchItem(iIndex, &c_pItemData))
		return Py_BuildValue("i", c_pItemData->bCount);

	return Py_BuildValue("i", 0);
}

PyObject* shopGetSearchItemMetinSocket(PyObject* poSelf, PyObject* poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildException();
	int iMetinSocketIndex;
	if (!PyTuple_GetInteger(poArgs, 1, &iMetinSocketIndex))
		return Py_BuildException();

	const CPythonShop::TSearchItemInfo* c_pItemData;
	if (CPythonShop::Instance().GetSearchItem(iIndex, &c_pItemData))
		return Py_BuildValue("i", c_pItemData->alSockets[iMetinSocketIndex]);

	return Py_BuildValue("i", 0);
}

PyObject* shopGetSearchItemAttribute(PyObject* poSelf, PyObject* poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildException();
	int iAttrSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 1, &iAttrSlotIndex))
		return Py_BuildException();

	if (iAttrSlotIndex >= 0 && iAttrSlotIndex < ITEM_ATTRIBUTE_SLOT_MAX_NUM)
	{
		const CPythonShop::TSearchItemInfo* c_pItemData;
		if (CPythonShop::Instance().GetSearchItem(iIndex, &c_pItemData))
#ifndef ENABLE_FROZEN_SYSTEM
			return Py_BuildValue("ii", c_pItemData->aAttr[iAttrSlotIndex].bType, c_pItemData->aAttr[iAttrSlotIndex].sValue);
#else
			return Py_BuildValue("iii", c_pItemData->aAttr[iAttrSlotIndex].bType, c_pItemData->aAttr[iAttrSlotIndex].sValue, c_pItemData->aAttr[iAttrSlotIndex].isFrozen);
#endif
	}

	return Py_BuildValue("ii", 0, 0);
}

#ifdef ENABLE_CHEQUE_SYSTEM
PyObject* shopGetSearchItemCheque(PyObject* poSelf, PyObject* poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildException();

	const CPythonShop::TSearchItemInfo* c_pItemData;
	if (CPythonShop::Instance().GetSearchItem(iIndex, &c_pItemData))
		return Py_BuildValue("i", c_pItemData->bCheque);

	return Py_BuildValue("i", 0);
}
#endif
#endif

#ifdef ENABLE_CHANGELOOK_SYSTEM
PyObject* shopGetSearchItemTransmutation(PyObject* poSelf, PyObject* poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildException();

	const CPythonShop::TSearchItemInfo* c_pItemData;
	if (CPythonShop::Instance().GetSearchItem(iIndex, &c_pItemData))
		return Py_BuildValue("i", c_pItemData->dwTransmutation);

	return Py_BuildValue("i", 0);
}
#endif

void initshop()
{
	static PyMethodDef s_methods[] =
	{
		// Shop
		{ "Open",						shopOpen,						METH_VARARGS },
		{ "Close",						shopClose,						METH_VARARGS },
		{ "IsOpen",						shopIsOpen,						METH_VARARGS },
		{ "IsPrivateShop",				shopIsPrviateShop,				METH_VARARGS },
		{ "IsMainPlayerPrivateShop",	shopIsMainPlayerPrivateShop,	METH_VARARGS },
		{ "GetItemID",					shopGetItemID,					METH_VARARGS },
		{ "GetItemCount",				shopGetItemCount,				METH_VARARGS },
		{ "GetItemPrice",				shopGetItemPrice,				METH_VARARGS },
#ifdef ENABLE_MULTISHOP
		{ "GetBuyWithItem",				shopGetBuyWithItem,				METH_VARARGS },
		{ "GetBuyWithItemCount",		shopGetBuyWithItemCount,		METH_VARARGS },
#endif
#ifdef ENABLE_CHEQUE_SYSTEM
		{ "GetItemCheque",				shopGetItemCheque,				METH_VARARGS },
#endif
		{ "GetItemMetinSocket",			shopGetItemMetinSocket,			METH_VARARGS },
		{ "GetItemAttribute",			shopGetItemAttribute,			METH_VARARGS },
		{ "GetTabCount",				shopGetTabCount,				METH_VARARGS },
		{ "GetTabName",					shopGetTabName,					METH_VARARGS },
		{ "GetTabCoinType",				shopGetTabCoinType,				METH_VARARGS },

		// Private Shop
		{ "ClearPrivateShopStock",		shopClearPrivateShopStock,		METH_VARARGS },
		{ "AddPrivateShopItemStock",	shopAddPrivateShopItemStock,	METH_VARARGS },
		{ "DelPrivateShopItemStock",	shopDelPrivateShopItemStock,	METH_VARARGS },
		{ "GetPrivateShopItemPrice",	shopGetPrivateShopItemPrice,	METH_VARARGS },
#ifdef ENABLE_CHEQUE_SYSTEM
		{ "GetPrivateShopItemCheque", shopGetPrivateShopItemCheque, METH_VARARGS },
#endif
		{ "BuildPrivateShop",			shopBuildPrivateShop,			METH_VARARGS },
#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
		{ "IsOfflineShop", shopIsOfflineShop, METH_VARARGS },
		{ "GetOfflineShopItemID", shopGetOfflineShopItemID, METH_VARARGS },
		{ "GetOfflineShopItemCount", shopGetOfflineShopItemCount, METH_VARARGS },
		{ "GetOfflineShopItemPrice", shopGetOfflineShopItemPrice, METH_VARARGS },
		{ "GetOfflineShopItemStatus", shopGetOfflineShopItemStatus, METH_VARARGS },
		{ "GetOfflineShopItemMetinSocket", shopGetOfflineShopItemMetinSocket, METH_VARARGS },
		{ "GetOfflineShopItemAttribute", shopGetOfflineShopItemAttribute, METH_VARARGS },

		{ "ClearOfflineShopStock", shopClearOfflineShopStock, METH_VARARGS },
		{ "AddOfflineShopItemStock", shopAddOfflineShopItemStock, METH_VARARGS },
		{ "DelOfflineShopItemStock", shopDelOfflineShopItemStock, METH_VARARGS },
		{ "GetOfflineShopItemPrice2", shopGetOfflineShopItemPrice2, METH_VARARGS },
		{ "BuildOfflineShop", shopBuildOfflineShop, METH_VARARGS },
		{ "GetDisplayedCount", shopGetDisplayedCount, METH_VARARGS },

#ifdef ENABLE_CHEQUE_SYSTEM
		{ "GetOfflineShopItemCheque", shopGetOfflineShopItemCheque, METH_VARARGS },
#endif
#endif
#ifdef ENABLE_CHANGELOOK_SYSTEM
		{ "GetOfflineShopItemTransmutation", shopGetOfflineShopItemTransmutation, METH_VARARGS },
#endif
#ifdef ENABLE_CHANGELOOK_SYSTEM
		{ "GetItemTransmutation", shopGetItemTransmutation, METH_VARARGS },
#endif
#ifdef ENABLE_PRIVATESHOP_SEARCH_SYSTEM
		{ "SearchItemDataClear", shopSearchItemDataClear, METH_VARARGS },
		{ "GetSearchItemResultCount", shopGetSearchItemResultCount, METH_VARARGS },

		{ "GetSearchItemShopVID", shopGetSearchItemShopVID, METH_VARARGS },
		{ "GetSearchItemOwnerName", shopGetSearchItemOwnerName, METH_VARARGS },
		{ "GetSearchItemPos", shopGetSearchItemPos, METH_VARARGS },
		{ "GetSearchItemGold", shopGetSearchItemGold, METH_VARARGS },
		{ "GetSearchItemVnum", shopGetSearchItemVnum, METH_VARARGS },
		{ "GetSearchItemCount", shopGetSearchItemCount, METH_VARARGS },
		{ "GetSearchItemMetinSocket", shopGetSearchItemMetinSocket, METH_VARARGS },
		{ "GetSearchItemAttribute", shopGetSearchItemAttribute, METH_VARARGS },

#ifdef ENABLE_CHEQUE_SYSTEM
		{ "GetSearchItemCheque", shopGetSearchItemCheque, METH_VARARGS },
#endif
#endif
#ifdef ENABLE_CHANGELOOK_SYSTEM
		{ "GetSearchItemTransmutation", shopGetSearchItemTransmutation, METH_VARARGS },
#endif
		{ NULL,							NULL,							NULL },
	};
	PyObject * poModule = Py_InitModule("shop", s_methods);

	PyModule_AddIntConstant(poModule, "SHOP_SLOT_COUNT", SHOP_HOST_ITEM_MAX_NUM);
	PyModule_AddIntConstant(poModule, "SHOP_COIN_TYPE_GOLD", SHOP_COIN_TYPE_GOLD);
	PyModule_AddIntConstant(poModule, "SHOP_COIN_TYPE_SECONDARY_COIN", SHOP_COIN_TYPE_SECONDARY_COIN);
#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	PyModule_AddIntConstant(poModule, "OFFLINE_SHOP_SLOT_COUNT", OFFLINE_SHOP_HOST_ITEM_MAX_NUM);
#endif
}