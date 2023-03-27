#pragma once
#include "../GameLib/ItemData.h"

struct SAffects
{
	enum
	{
		AFFECT_MAX_NUM = 32,
	};

	SAffects() : dwAffects(0) {}
	SAffects(const DWORD & c_rAffects)
	{
		__SetAffects(c_rAffects);
	}
	int operator = (const DWORD & c_rAffects)
	{
		__SetAffects(c_rAffects);
	}

	BOOL IsAffect(BYTE byIndex)
	{
		return dwAffects & (1 << byIndex);
	}

	void __SetAffects(const DWORD & c_rAffects)
	{
		dwAffects = c_rAffects;
	}

	DWORD dwAffects;
};

extern std::string g_strGuildSymbolPathName;

const DWORD c_Name_Max_Length = 64;
#ifdef ___DECORUM___
const int ARENA_MAX_NUM = 3;
#endif
#ifdef ENABLE_6_7_BONUS_NEW_SYSTEM
const DWORD c_skillbook_slot_max = 10;
#endif
const DWORD c_FileName_Max_Length = 128;
const DWORD c_Short_Name_Max_Length = 32;

const DWORD c_Inventory_Page_Column = 5;
const DWORD c_Inventory_Page_Row = 9;
const DWORD c_Inventory_Page_Size = c_Inventory_Page_Column*c_Inventory_Page_Row; // x*y
#ifdef ENABLE_EXTEND_INVEN_SYSTEM
const DWORD c_Inventory_Page_Count = 4;
#else
const DWORD c_Inventory_Page_Count = 2;
#endif
const DWORD c_ItemSlot_Count = c_Inventory_Page_Size * c_Inventory_Page_Count;
const DWORD c_Equipment_Count = 12;
#if defined(__BL_SOUL_ROULETTE__)
const int ROULETTE_ITEM_MAX = 20;
#endif
const DWORD c_Equipment_Start = c_ItemSlot_Count;

const DWORD c_Equipment_Body = c_Equipment_Start + 0;
const DWORD c_Equipment_Head = c_Equipment_Start + 1;
const DWORD c_Equipment_Shoes = c_Equipment_Start + 2;
const DWORD c_Equipment_Wrist = c_Equipment_Start + 3;
const DWORD c_Equipment_Weapon = c_Equipment_Start + 4;
const DWORD c_Equipment_Neck = c_Equipment_Start + 5;
const DWORD c_Equipment_Ear = c_Equipment_Start + 6;
const DWORD c_Equipment_Unique1 = c_Equipment_Start + 7;
const DWORD c_Equipment_Unique2 = c_Equipment_Start + 8;
const DWORD c_Equipment_Arrow = c_Equipment_Start + 9;
const DWORD c_Equipment_Shield = c_Equipment_Start + 10;

// 새로 추가된 신규 반지 & 벨트
// 장착형 아이템에 할당할 수 있는 위치가 기존 장비, 채기랍 퀘스트 보상, 코스튬 시스템 등으로 인해서 공간이 잘려있다.
// 이게 다 채기랍 보상 버프를 장착아이템처럼 구현한 ㅅㄲ 때문에 난리났따... ㅆㅂ
//
// 정리하면, 기존 장비창들은 서버DB상 아이템 포지션이 90 ~ 102 이고,
// 2013년 초에 새로 추가되는 슬롯들은 111 ~ 부터 시작한다. 착용 장비에서 최대로 사용할 수 있는 값은 121 까지이고, 122부터는 용혼석에서 사용한다.
#ifdef ENABLE_NEW_EQUIPMENT_SYSTEM
#ifdef ENABLE_NEW_TALISMAN_GF
const DWORD c_New_Equipment_Start = c_Equipment_Start + 25;//22; //204
#else
const DWORD c_New_Equipment_Start = c_Equipment_Start + 21;
#endif

//troppe possibili combinazioni
//in caso di future rimozioni
//basta contarli e modificare di conseguenza
#ifdef ENABLE_NEW_TALISMAN_GF
#ifdef ENABLE_NEW_TALISMAN_SLOTS_NEW
#ifdef NEW_RING_SLOT
const DWORD c_New_Equipment_Count = 15;
#else
const DWORD c_New_Equipment_Count = 8;
#endif
#else
const DWORD c_New_Equipment_Count = 5; // Se non va prova 6
#endif
#else
const DWORD c_New_Equipment_Count = 4;
#endif
//const DWORD c_Equipment_Ring1 = c_New_Equipment_Start + 0;//204
//const DWORD c_Equipment_Ring2 = c_New_Equipment_Start + 1;//205
const DWORD c_Equipment_Belt = c_New_Equipment_Start + 0; //204
#ifdef ENABLE_NEW_TALISMAN_GF
const DWORD c_Equipment_Talisman = c_New_Equipment_Start + 1;//205
#endif
#ifdef ENABLE_NEW_TALISMAN_SLOTS_NEW
const DWORD c_Equipment_Talisman_2 = c_New_Equipment_Start + 2;//206
const DWORD c_Equipment_Talisman_3 = c_New_Equipment_Start + 3;//207
const DWORD c_Equipment_Talisman_4 = c_New_Equipment_Start + 4;//208
const DWORD c_Equipment_Talisman_5 = c_New_Equipment_Start + 5;//209
const DWORD c_Equipment_Talisman_6 = c_New_Equipment_Start + 6;//210
#endif

const DWORD	c_Costume_Aura = c_New_Equipment_Start + 7;
#ifdef ENABLE_EFFECT_COSTUME_SYSTEM
	const DWORD	c_Costume_Slot_Effect	= c_New_Equipment_Start + 8;
#endif
#ifdef NEW_RING_SLOT
	const DWORD c_Equipment_Ring_New = c_New_Equipment_Start + 9;
#endif
#endif

#ifdef ENABLE_SYSTEM_RUNE
	const DWORD c_Equipment_Rune = c_New_Equipment_Start + 10;
	const DWORD c_Equipment_Rune_Red = c_New_Equipment_Start + 11;
	const DWORD c_Equipment_Rune_Yellow = c_New_Equipment_Start + 12;
	const DWORD c_Equipment_Rune_Green = c_New_Equipment_Start + 13;
	const DWORD c_Equipment_Rune_Blue = c_New_Equipment_Start + 14;
	const DWORD c_Equipment_Rune_Black = c_New_Equipment_Start + 15;;
#endif

#ifdef FAST_EQUIP_WORLDARD 
	enum ChangeEquipConfig
	{
		CHANGE_EQUIP_PAGE_EXTRA = 10,
		CHANGE_EQUIP_SLOT_COUNT = 30 * CHANGE_EQUIP_PAGE_EXTRA,
	};

#endif

enum EDragonSoulDeckType
{
	DS_DECK_1,
	DS_DECK_2,
	DS_DECK_MAX_NUM = 2,
};

enum EDragonSoulGradeTypes
{
	DRAGON_SOUL_GRADE_NORMAL,
	DRAGON_SOUL_GRADE_BRILLIANT,
	DRAGON_SOUL_GRADE_RARE,
	DRAGON_SOUL_GRADE_ANCIENT,
	DRAGON_SOUL_GRADE_LEGENDARY,
#ifdef ENABLE_DS_GRADE_MYTH
	DRAGON_SOUL_GRADE_MYTH,
#endif
	DRAGON_SOUL_GRADE_MAX,

};

enum EDragonSoulStepTypes
{
	DRAGON_SOUL_STEP_LOWEST,
	DRAGON_SOUL_STEP_LOW,
	DRAGON_SOUL_STEP_MID,
	DRAGON_SOUL_STEP_HIGH,
	DRAGON_SOUL_STEP_HIGHEST,
	DRAGON_SOUL_STEP_MAX,
};

/*#ifdef ENABLE_NEW_TALISMAN_SLOTS_NEW

#ifdef ENABLE_COSTUME_SYSTEM
const DWORD c_Costume_Slot_Start = c_Equipment_Start + 19;
const DWORD	c_Costume_Slot_Body = c_Costume_Slot_Start + 0;
const DWORD	c_Costume_Slot_Hair = c_Costume_Slot_Start + 1;
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
const DWORD	c_Costume_Slot_Mount = c_Costume_Slot_Start + 2;
#endif
#ifdef ENABLE_ACCE_COSTUME_SYSTEM
const DWORD	c_Costume_Slot_Acce = c_Costume_Slot_Start + 3;
#endif
#ifdef ENABLE_WEAPON_COSTUME_SYSTEM
const DWORD	c_Costume_Slot_Weapon = c_Costume_Slot_Start + 4;
#endif
#ifdef ENABLE_AURA_SYSTEM
const DWORD c_Costume_Slot_Aura = c_Costume_Slot_Start + 5;
#endif


const DWORD c_Costume_Slot_Count = 2
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
	+1
#endif
#ifdef ENABLE_ACCE_COSTUME_SYSTEM
	+1
#endif
#ifdef ENABLE_WEAPON_COSTUME_SYSTEM
	+1
#endif
#ifdef ENABLE_AURA_SYSTEM
	+1
#endif
	;
const DWORD c_Costume_Slot_End = c_Costume_Slot_Start + c_Costume_Slot_Count;

#endif

#else*/

#ifdef ENABLE_COSTUME_SYSTEM
const DWORD c_Costume_Slot_Start = c_Equipment_Start + 19;//199
const DWORD	c_Costume_Slot_Body = c_Costume_Slot_Start + 0;//199
const DWORD	c_Costume_Slot_Hair = c_Costume_Slot_Start + 1;//200
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
const DWORD	c_Costume_Slot_Mount = c_Costume_Slot_Start + 2;//201
#endif
#ifdef ENABLE_ACCE_COSTUME_SYSTEM
const DWORD	c_Costume_Slot_Acce = c_Costume_Slot_Start + 3;//202
#endif

#ifdef ENABLE_WEAPON_COSTUME_SYSTEM
const DWORD	c_Costume_Slot_Weapon = c_Costume_Slot_Start + 4;//203
#endif
#ifdef ENABLE_AURA_SYSTEM
const DWORD c_Costume_Slot_Aura = c_Costume_Slot_Start + 5;//204
#endif

#if defined(ENABLE_WEAPON_COSTUME_SYSTEM) || defined(ENABLE_ACCE_COSTUME_SYSTEM)
const DWORD c_Costume_Slot_Count = 6;
#elif defined(ENABLE_MOUNT_COSTUME_SYSTEM)
const DWORD c_Costume_Slot_Count = 3;
#else
const DWORD c_Costume_Slot_Count = 2;
#endif
const DWORD c_Costume_Slot_End = c_Costume_Slot_Start + c_Costume_Slot_Count;//204


/*
#ifdef ENABLE_WEAPON_COSTUME_SYSTEM
const DWORD	c_Costume_Slot_Weapon = c_Costume_Slot_End + 1;//204
#endif
#ifdef ENABLE_AURA_SYSTEM
const DWORD c_Costume_Slot_Aura = c_Costume_Slot_End + 3;//206
#endif
*/
#endif

//#endif

#ifdef FAST_EQUIP_WORLDARD 
const DWORD c_change_equip_Slot_Start = c_New_Equipment_Start + c_New_Equipment_Count;
const DWORD c_change_equip_Slot_End = c_change_equip_Slot_Start + 30 * 10;
#endif		
// [주의] 숫자(32) 하드코딩 주의. 현재 서버에서 용혼석 슬롯은 32부터임.
// 서버 common/length.h 파일의 EWearPositions 열거형이 32까지 확장될 것을 염두하고(32 이상은 확장 하기 힘들게 되어있음.),
// 그 이후부터를 용혼석 장착 슬롯으로 사용.
#if defined(ENABLE_NEW_TALISMAN_SLOTS_NEW) || defined(ENABLE_NEW_TALISMAN_SLOTS)
const DWORD c_Wear_Max = 60;
#else
const DWORD c_Wear_Max = 32;
#endif
const DWORD c_DragonSoul_Equip_Start = c_ItemSlot_Count + c_Wear_Max;
const DWORD c_DragonSoul_Equip_Slot_Max = 6;
const DWORD c_DragonSoul_Equip_End = c_DragonSoul_Equip_Start + c_DragonSoul_Equip_Slot_Max * DS_DECK_MAX_NUM;

// NOTE: 2013년 2월 5일 현재... 용혼석 데크는 2개가 존재하는데, 향후 확장 가능성이 있어서 3개 데크 여유분을 할당 해 둠. 그 뒤 공간은 벨트 인벤토리로 사용
const DWORD c_DragonSoul_Equip_Reserved_Count = c_DragonSoul_Equip_Slot_Max * 3;

#ifdef ENABLE_NEW_EQUIPMENT_SYSTEM
// 벨트 아이템이 제공하는 인벤토리
const DWORD c_Belt_Inventory_Slot_Start = c_DragonSoul_Equip_End + c_DragonSoul_Equip_Reserved_Count;
const DWORD c_Belt_Inventory_Width = 4;
const DWORD c_Belt_Inventory_Height = 4;
const DWORD c_Belt_Inventory_Slot_Count = c_Belt_Inventory_Width * c_Belt_Inventory_Height;
const DWORD c_Belt_Inventory_Slot_End = c_Belt_Inventory_Slot_Start + c_Belt_Inventory_Slot_Count;

const DWORD c_Inventory_Count = c_Belt_Inventory_Slot_End;
#else
const DWORD c_Inventory_Count = c_DragonSoul_Equip_End;
#endif

// 용혼석 전용 인벤토리
const DWORD c_DragonSoul_Inventory_Start = 0;
const DWORD c_DragonSoul_Inventory_Box_Size = 32;
#ifdef ENABLE_EXTENDED_DS_INVENTORY
const DWORD c_DragonSoul_Inventory_Page_Count = 3;
const DWORD c_DragonSoul_Inventory_Count = CItemData::DS_SLOT_NUM_TYPES * DRAGON_SOUL_GRADE_MAX * c_DragonSoul_Inventory_Page_Count * c_DragonSoul_Inventory_Box_Size;
#else
const DWORD c_DragonSoul_Inventory_Count = CItemData::DS_SLOT_NUM_TYPES * DRAGON_SOUL_GRADE_MAX * c_DragonSoul_Inventory_Box_Size;
#endif
const DWORD c_DragonSoul_Inventory_End = c_DragonSoul_Inventory_Start + c_DragonSoul_Inventory_Count;
#ifdef ENABLE_SPECIAL_STORAGE_SYSTEM
const DWORD c_Special_Storage_Inventory_Page_Size = 5 * 9;
const DWORD c_Special_Storage_Inventory_Page_Count = 4;
const DWORD c_Special_Storage_ItemSlot_Count = c_Special_Storage_Inventory_Page_Size * c_Special_Storage_Inventory_Page_Count;
#endif
enum ESlotType
{
	SLOT_TYPE_NONE,
	SLOT_TYPE_INVENTORY,
	SLOT_TYPE_SKILL,
	SLOT_TYPE_EMOTION,
	SLOT_TYPE_SHOP,
	SLOT_TYPE_EXCHANGE_OWNER,
	SLOT_TYPE_EXCHANGE_TARGET,
	SLOT_TYPE_QUICK_SLOT,
	SLOT_TYPE_SAFEBOX,
	SLOT_TYPE_PRIVATE_SHOP,
	SLOT_TYPE_MALL,
	SLOT_TYPE_DRAGON_SOUL_INVENTORY,
#ifdef ENABLE_SPECIAL_STORAGE_SYSTEM
	SLOT_TYPE_SKILLBOOK_INVENTORY,
	SLOT_TYPE_UPPITEM_INVENTORY,
	SLOT_TYPE_GHOSTSTONE_INVENTORY,
	SLOT_TYPE_GENERAL_INVENTORY,
#endif
#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	SLOT_TYPE_OFFLINE_SHOP,
#endif
#ifdef ENABLE_SWITCHBOT
	SLOT_TYPE_SWITCHBOT,
#endif
#ifdef FAST_EQUIP_WORLDARD
	SLOT_TYPE_CHANGE_EQUIP,
#endif
	SLOT_TYPE_MAX,
};

enum EWindows
{
	RESERVED_WINDOW,
	INVENTORY,				// 기본 인벤토리. (45칸 짜리가 2페이지 존재 = 90칸)
	EQUIPMENT,
	SAFEBOX,
	MALL,
	DRAGON_SOUL_INVENTORY,
#ifdef ENABLE_SPECIAL_STORAGE_SYSTEM
	SKILLBOOK_INVENTORY,
	UPPITEM_INVENTORY,
	GHOSTSTONE_INVENTORY,
	GENERAL_INVENTORY,
#endif
	BELT_INVENTORY,			// NOTE: W2.1 버전에 새로 추가되는 벨트 슬롯 아이템이 제공하는 벨트 인벤토리
#ifdef ENABLE_SWITCHBOT
	SWITCHBOT,
#endif
// #ifdef ENABLE_6_7_BONUS_NEW_SYSTEM
	// BONUS_NEW_67,
// #endif
#ifdef FAST_EQUIP_WORLDARD
	CHANGE_EQUIP,
#endif
	GROUND, //ground always needs to be the last one
	WINDOW_TYPE_MAX,
};

enum EDSInventoryMaxNum
{
	DS_INVENTORY_MAX_NUM = c_DragonSoul_Inventory_Count,
	DS_REFINE_WINDOW_MAX_NUM = 15,
};

#pragma pack (push, 1)
#define WORD_MAX 0xffff

#ifdef ENABLE_SWITCHBOT
enum ESwitchbotValues
{
	SWITCHBOT_SLOT_COUNT = 3,
	SWITCHBOT_ALTERNATIVE_COUNT = 2,
	MAX_NORM_ATTR_NUM = 5,
};

enum EAttributeSet
{
	ATTRIBUTE_SET_WEAPON,
	ATTRIBUTE_SET_BODY,
	ATTRIBUTE_SET_WRIST,
	ATTRIBUTE_SET_FOOTS,
	ATTRIBUTE_SET_NECK,
	ATTRIBUTE_SET_HEAD,
	ATTRIBUTE_SET_SHIELD,
	ATTRIBUTE_SET_EAR,
	ATTRIBUTE_SET_MAX_NUM,
};
#endif

#ifdef WJ_ENABLE_TRADABLE_ICON
enum ETopWindowTypes
{
	ON_TOP_WND_NONE,
	ON_TOP_WND_SHOP,
	ON_TOP_WND_EXCHANGE,
	ON_TOP_WND_SAFEBOX,
	ON_TOP_WND_PRIVATE_SHOP,
	ON_TOP_WND_ITEM_COMB,
	ON_TOP_WND_PET_FEED,
#if defined(__BL_MAILBOX__)
	ON_TOP_WND_MAILBOX,
#endif
	ON_TOP_WND_MAX,
};
#endif

typedef struct SItemPos
{
	BYTE window_type;
	WORD cell;
	SItemPos()
	{
		window_type = INVENTORY;
		cell = WORD_MAX;
	}
	SItemPos(BYTE _window_type, WORD _cell)
	{
		window_type = _window_type;
		cell = _cell;
	}

	// 기존에 cell의 형을 보면 BYTE가 대부분이지만, oi
	// 어떤 부분은 int, 어떤 부분은 WORD로 되어있어,
	// 가장 큰 자료형인 int로 받는다.
	//  int operator=(const int _cell)
	//  {
	//window_type = INVENTORY;
	//      cell = _cell;
	//      return cell;
	//  }
	bool IsValidCell()
	{
		switch (window_type)
		{
		case INVENTORY:
			return cell < c_Inventory_Count;
			break;
		case EQUIPMENT:
			return cell < c_DragonSoul_Equip_End;
			break;
		case DRAGON_SOUL_INVENTORY:
			return cell < (DS_INVENTORY_MAX_NUM);
			break;
#ifdef ENABLE_SWITCHBOT
		case SWITCHBOT:
			return cell < SWITCHBOT_SLOT_COUNT;
			break;
#endif
#ifdef FAST_EQUIP_WORLDARD
		case CHANGE_EQUIP:
			return cell < CHANGE_EQUIP_SLOT_COUNT;
			break;
#endif
#ifdef ENABLE_SPECIAL_STORAGE_SYSTEM
		case SKILLBOOK_INVENTORY:
			return cell < c_Special_Storage_ItemSlot_Count;
			break;

		case UPPITEM_INVENTORY:
			return cell < c_Special_Storage_ItemSlot_Count;
			break;

		case GHOSTSTONE_INVENTORY:
			return cell < c_Special_Storage_ItemSlot_Count;
			break;

		case GENERAL_INVENTORY:
			return cell < c_Special_Storage_ItemSlot_Count;
			break;
#endif
		default:
			return false;
		}
	}
	bool IsEquipCell()
	{
		switch (window_type)
		{
		case INVENTORY:
		case EQUIPMENT:
			return (c_Equipment_Start + c_Wear_Max > cell) && (c_Equipment_Start <= cell);
			break;

		case BELT_INVENTORY:
		case DRAGON_SOUL_INVENTORY:
#ifdef ENABLE_SPECIAL_STORAGE_SYSTEM
		case SKILLBOOK_INVENTORY:
		case UPPITEM_INVENTORY:
		case GHOSTSTONE_INVENTORY:
		case GENERAL_INVENTORY:
#endif
			return false;
			break;

		default:
			return false;
		}
	}

#ifdef ENABLE_NEW_EQUIPMENT_SYSTEM
	bool IsBeltInventoryCell()
	{
		bool bResult = c_Belt_Inventory_Slot_Start <= cell && c_Belt_Inventory_Slot_End > cell;
		return bResult;
	}
#endif

	bool operator==(const struct SItemPos& rhs) const
	{
		return (window_type == rhs.window_type) && (cell == rhs.cell);
	}

	bool operator<(const struct SItemPos& rhs) const
	{
		return (window_type < rhs.window_type) || ((window_type == rhs.window_type) && (cell < rhs.cell));
	}
} TItemPos;
#pragma pack(pop)

const DWORD c_QuickBar_Line_Count = 3;
const DWORD c_QuickBar_Slot_Count = 12;

const float c_Idle_WaitTime = 5.0f;

const int c_Monster_Race_Start_Number = 6;
const int c_Monster_Model_Start_Number = 20001;

const float c_fAttack_Delay_Time = 0.2f;
const float c_fHit_Delay_Time = 0.1f;
const float c_fCrash_Wave_Time = 0.2f;
const float c_fCrash_Wave_Distance = 3.0f;

const float c_fHeight_Step_Distance = 50.0f;

enum
{
	DISTANCE_TYPE_FOUR_WAY,
	DISTANCE_TYPE_EIGHT_WAY,
	DISTANCE_TYPE_ONE_WAY,
	DISTANCE_TYPE_MAX_NUM,
};

const float c_fMagic_Script_Version = 1.0f;
const float c_fSkill_Script_Version = 1.0f;
const float c_fMagicSoundInformation_Version = 1.0f;
const float c_fBattleCommand_Script_Version = 1.0f;
const float c_fEmotionCommand_Script_Version = 1.0f;
const float c_fActive_Script_Version = 1.0f;
const float c_fPassive_Script_Version = 1.0f;

// Used by PushMove
const float c_fWalkDistance = 175.0f;
const float c_fRunDistance = 310.0f;

#define FILE_MAX_LEN 128

enum
{
#ifdef ENABLE_FIX_PET_TRANSPORT_BOX
	ITEM_SOCKET_SLOT_MAX_NUM = 4,
#else
	ITEM_SOCKET_SLOT_MAX_NUM = 3,
#endif
	// refactored attribute slot begin
	ITEM_ATTRIBUTE_SLOT_NORM_NUM = 5,
	ITEM_ATTRIBUTE_SLOT_RARE_NUM = 2,

	ITEM_ATTRIBUTE_SLOT_NORM_START = 0,
	ITEM_ATTRIBUTE_SLOT_NORM_END = ITEM_ATTRIBUTE_SLOT_NORM_START + ITEM_ATTRIBUTE_SLOT_NORM_NUM,

	ITEM_ATTRIBUTE_SLOT_RARE_START = ITEM_ATTRIBUTE_SLOT_NORM_END,
	ITEM_ATTRIBUTE_SLOT_RARE_END = ITEM_ATTRIBUTE_SLOT_RARE_START + ITEM_ATTRIBUTE_SLOT_RARE_NUM,

	ITEM_ATTRIBUTE_SLOT_MAX_NUM = ITEM_ATTRIBUTE_SLOT_RARE_END, // 7
	// refactored attribute slot end
};

#pragma pack(push)
#pragma pack(1)

typedef struct SQuickSlot
{
	BYTE Type;
	BYTE Position;
} TQuickSlot;

typedef struct TPlayerItemAttribute
{
	BYTE        bType;
	short       sValue;
#ifdef ENABLE_FROZEN_SYSTEM
	bool        isFrozen;
#endif
} TPlayerItemAttribute;

typedef struct packet_item
{
	DWORD       vnum;
	short        count;
#ifdef ENABLE_CHANGELOOK_SYSTEM
	DWORD	transmutation;
#endif
	DWORD		flags;
	DWORD		anti_flags;
	long		alSockets[ITEM_SOCKET_SLOT_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_SLOT_MAX_NUM];
} TItemData;

#ifdef ENABLE_CHEQUE_SYSTEM
typedef struct SItemPriceType
{
	SItemPriceType(){ dwPrice = byChequePrice = 0; }
	SItemPriceType(DWORD gold, int cheque)
	{
		dwPrice = gold;
		byChequePrice = cheque;
	}
	DWORD	dwPrice;
	int	byChequePrice;
} TItemPriceType;
#endif

typedef struct packet_shop_item
{
	DWORD       vnum;
#ifdef ENABLE_CHEQUE_SYSTEM
	TItemPriceType	price;
#else
	DWORD		price;
#endif

	short        count;
#ifdef ENABLE_MULTISHOP
	DWORD		wPriceVnum;
	DWORD		wPrice;
#endif
	BYTE		display_pos;
	long		alSockets[ITEM_SOCKET_SLOT_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_SLOT_MAX_NUM];
#ifdef ENABLE_CHANGELOOK_SYSTEM
	DWORD	transmutation;
#endif
} TShopItemData;

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
typedef struct packet_shop_offline_item
{
	DWORD		vnum;
	// BEGIN_MAX_YANG
	DWORD		price;
	// END_OF_MAX_YANG

#ifdef ENABLE_CHEQUE_SYSTEM
	int		cheque;
#endif

	BYTE		count;
	BYTE		display_pos;
	bool		bIsSold;
	char		buyerName[24];
	long		alSockets[ITEM_SOCKET_SLOT_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_SLOT_MAX_NUM];

#ifdef ENABLE_CHANGELOOK_SYSTEM
	DWORD		transmutation;
#endif

	packet_shop_offline_item() {
		buyerName[24] = '\0';
		bIsSold = false;
	}
} TShopOfflineItemData;
#endif

#pragma pack(pop)

inline float GetSqrtDistance(int ix1, int iy1, int ix2, int iy2) // By sqrt
{
	float dx, dy;

	dx = float(ix1 - ix2);
	dy = float(iy1 - iy2);

	return sqrtf(dx*dx + dy*dy);
}

// DEFAULT_FONT
void DefaultFont_Startup();
void DefaultFont_Cleanup();
void DefaultFont_SetName(const char * c_szFontName);
CResource* DefaultFont_GetResource();
CResource* DefaultItalicFont_GetResource();
// END_OF_DEFAULT_FONT

void SetGuildSymbolPath(const char * c_szPathName);
const char * GetGuildSymbolFileName(DWORD dwGuildID);
BYTE SlotTypeToInvenType(BYTE bSlotType);
