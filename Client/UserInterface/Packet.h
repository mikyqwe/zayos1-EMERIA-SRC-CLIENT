#pragma once

#ifdef USE_AHNLAB_HACKSHIELD
#include "Hackshield.h"
#include METIN2HS_INCLUDE_ANTICPXSVR
#else
#pragma pack(push) //���� alignment ����
#pragma pack(8)

#define ANTICPX_TRANS_BUFFER_MAX			400

typedef struct _AHNHS_TRANS_BUFFER
{
	unsigned char byBuffer[ANTICPX_TRANS_BUFFER_MAX/* �ۼ��� ��Ŷ�� �ִ� ũ�� */];
	unsigned short nLength;
} AHNHS_TRANS_BUFFER, *PAHNHS_TRANS_BUFFER;

#pragma pack(pop) // ���� alignment ����.
#endif /* !USE_AHNLAB_HACKSHIELD */

#include "Locale.h"
#include "Locale_inc.h"
#include "../gamelib/RaceData.h"
#include "../gamelib/ItemData.h"

typedef BYTE TPacketHeader;

enum
{
	/////////////////////////////////////////////////
	// To Server
	// HEADER_BLANK is the not use(for future use)
	HEADER_CG_LOGIN								= 1,
	HEADER_CG_ATTACK							= 2,
	HEADER_CG_CHAT								= 3,
	HEADER_CG_PLAYER_CREATE						= 4,		// ���ο� �÷��̾ ����
	HEADER_CG_PLAYER_DESTROY					= 5,		// �÷��̾ ����.
	HEADER_CG_PLAYER_SELECT						= 6,
	HEADER_CG_CHARACTER_MOVE					= 7,
	HEADER_CG_SYNC_POSITION  					= 8,
	HEADER_CG_DIRECT_ENTER						= 9,
	HEADER_CG_ENTERGAME							= 10,
	HEADER_CG_ITEM_USE							= 11,
	HEADER_CG_ITEM_DROP							= 12,
	HEADER_CG_ITEM_MOVE							= 13,
	HEADER_CG_ITEM_PICKUP						= 15,
	HEADER_CG_QUICKSLOT_ADD                     = 16,
	HEADER_CG_QUICKSLOT_DEL                     = 17,
	HEADER_CG_QUICKSLOT_SWAP                    = 18,
	HEADER_CG_WHISPER							= 19,
#ifdef NEW_ADD_INVENTORY
	ENVANTER_BLACK		            			= 23,
#endif
	HEADER_CG_ITEM_DROP2                        = 20,
#ifdef ENABLE_SELL_ITEM
	HEADER_CG_ITEM_SELL = 22,
#endif
	//HEADER_BLANK21								= 21,
	//HEADER_BLANK22								= 22,
	//HEADER_BLANK22								= 23,
	//HEADER_BLANK24								= 24,
	//HEADER_BLANK25								= 25,
	HEADER_CG_ON_CLICK							= 26,
	HEADER_CG_EXCHANGE							= 27,
    HEADER_CG_CHARACTER_POSITION                = 28,
    HEADER_CG_SCRIPT_ANSWER						= 29,
	HEADER_CG_QUEST_INPUT_STRING				= 30,
    HEADER_CG_QUEST_CONFIRM                     = 31,
	//HEADER_BLANK32								= 32,
	//HEADER_BLANK33								= 33,
	//HEADER_BLANK34								= 34,
	//HEADER_BLANK35								= 35,
	//HEADER_BLANK36								= 36,
	//HEADER_BLANK37								= 37,
	//HEADER_BLANK38								= 38,
	//HEADER_BLANK39								= 39,
	//HEADER_BLANK40								= 40,
	HEADER_CG_PVP								= 41,
	//HEADER_BLANK42								= 42,
	//HEADER_BLANK43								= 43,
	//HEADER_BLANK44								= 44,
	//HEADER_BLANK45								= 45,
	//HEADER_BLANK46								= 46,
	//HEADER_BLANK47								= 47,
	//HEADER_BLANK48								= 48,
	//HEADER_BLANK49								= 49,
    HEADER_CG_SHOP								= 50,
	HEADER_CG_FLY_TARGETING						= 51,
	HEADER_CG_USE_SKILL							= 52,
    HEADER_CG_ADD_FLY_TARGETING                 = 53,
	HEADER_CG_SHOOT								= 54,
	HEADER_CG_MYSHOP                            = 55,
#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	HEADER_CG_OFFLINE_SHOP						= 56,
	HEADER_CG_MY_OFFLINE_SHOP					= 57,
#endif
	//HEADER_BLANK56								= 56,
	//HEADER_BLANK57								= 57,
	//HEADER_BLANK58								= 58,
	//HEADER_BLANK59								= 59,
	HEADER_CG_ITEM_USE_TO_ITEM					= 60,
    HEADER_CG_TARGET                            = 61,
	//HEADER_BLANK62								= 62,
	//HEADER_BLANK63								= 63,
	//HEADER_BLANK64								= 64,
	HEADER_CG_WARP								= 65,
    HEADER_CG_SCRIPT_BUTTON						= 66,
    HEADER_CG_MESSENGER                         = 67,
	//HEADER_BLANK68								= 68,
    HEADER_CG_MALL_CHECKOUT                     = 69,
    HEADER_CG_SAFEBOX_CHECKIN                   = 70,   // �������� â���� �ִ´�.
    HEADER_CG_SAFEBOX_CHECKOUT                  = 71,   // �������� â���� ���� ���´�.
    HEADER_CG_PARTY_INVITE                      = 72,
    HEADER_CG_PARTY_INVITE_ANSWER               = 73,
    HEADER_CG_PARTY_REMOVE                      = 74,
    HEADER_CG_PARTY_SET_STATE                   = 75,
    HEADER_CG_PARTY_USE_SKILL                   = 76,
    HEADER_CG_SAFEBOX_ITEM_MOVE                 = 77,
	HEADER_CG_PARTY_PARAMETER                   = 78,
	//HEADER_BLANK68								= 79,
	HEADER_CG_GUILD								= 80,
	HEADER_CG_ANSWER_MAKE_GUILD					= 81,
    HEADER_CG_FISHING                           = 82,
    HEADER_CG_GIVE_ITEM                         = 83,
	//HEADER_BLANK84								= 84,
	//HEADER_BLANK85								= 85,
	//HEADER_BLANK86								= 86,
	//HEADER_BLANK87								= 87,
	//HEADER_BLANK88								= 88,
	//HEADER_BLANK89								= 89,
    HEADER_CG_EMPIRE                            = 90,
	//HEADER_BLANK91								= 91,
	//HEADER_BLANK92								= 92,
	//HEADER_BLANK93								= 93,
	//HEADER_BLANK94								= 94,
	//HEADER_BLANK95								= 95,
    HEADER_CG_REFINE                            = 96,
	//HEADER_BLANK97								= 97,
	//HEADER_BLANK98								= 98,
	//HEADER_BLANK99								= 99,

	HEADER_CG_MARK_LOGIN						= 100,
	HEADER_CG_MARK_CRCLIST						= 101,
	HEADER_CG_MARK_UPLOAD						= 102,
	HEADER_CG_MARK_IDXLIST						= 104,

	HEADER_CG_CRC_REPORT						= 103,

	HEADER_CG_HACK								= 105,
    HEADER_CG_CHANGE_NAME                       = 106,
    HEADER_CG_SMS                               = 107,
    HEADER_CG_CHINA_MATRIX_CARD                 = 108,
    HEADER_CG_LOGIN2                            = 109,
	HEADER_CG_DUNGEON							= 110,
	HEADER_CG_LOGIN3							= 111,
	HEADER_CG_GUILD_SYMBOL_UPLOAD				= 112,
	HEADER_CG_GUILD_SYMBOL_CRC					= 113,
	HEADER_CG_SCRIPT_SELECT_ITEM				= 114,
	HEADER_CG_LOGIN4							= 115,

	HEADER_CG_LOGIN5_OPENID						= 116,	//OpenID : �����? ���� ����Ű�� ������ ����.
#ifdef ENABLE_PRIVATESHOP_SEARCH_SYSTEM
	HEADER_CG_PSHOP_SEARCH = 117,
	HEADER_CG_PSHOP_SEARCH_NAME_ONLY = 118,
	HEADER_CG_PSHOP_SEARCH_BUY = 119,
#endif
#ifdef ENABLE_MULTI_LANGUAGE_SYSTEM
	HEADER_CG_REQUEST_FLAG						= 135,
#endif
#ifdef ENABLE_SWITCHBOT
	HEADER_CG_SWITCHBOT = 171,
#endif
#ifdef WON_EXCHANGE
	HEADER_CG_WON_EXCHANGE				= 172,
#endif
#ifdef ENABLE_SORT_INVEN
	SORT_INVEN = 140,
#endif
#if defined(__BL_SOUL_ROULETTE__)
	HEADER_CG_SOUL_ROULETTE = 145,
#endif
#if defined(__BL_SOUL_ROULETTE__)
	HEADER_GC_SOUL_ROULETTE = 146,
#endif
	HEADER_CG_RUNUP_MATRIX_ANSWER               = 201,
	HEADER_CG_NEWCIBN_PASSPOD_ANSWER			= 202,

	HEADER_CG_HS_ACK							= 203,
	HEADER_CG_XTRAP_ACK							= 204,

	HEADER_CG_DRAGON_SOUL_REFINE			= 205,
	HEADER_CG_STATE_CHECKER					= 206,
#ifdef ENABLE_ANTY_WAIT_HACK
	HEADER_CG_ANTY_WAIT_HACK 				= 207,
#endif
#ifdef ENABLE_CUBE_RENEWAL_WORLDARD
	HEADER_CG_CUBE_RENEWAL = 215,
#endif
#ifdef ENABLE_6_7_BONUS_NEW_SYSTEM
	HEADER_CG_67_BONUS_NEW = 216,
#endif
#ifdef __ULTIMATE_TOOLTIP__
	HEADER_CG_CHEST_DROP_INFO				= 217,
#endif

#ifdef ENABLE_GUILD_REQUEST
		HEADER_CG_GUILD_REQUEST = 218,
#endif

#ifdef ENABLE_NEW_FISHING_SYSTEM
		HEADER_CG_FISHING_NEW = 219,
#endif

#ifdef ENABLE_HUNTING_SYSTEM
	HEADER_CG_SEND_HUNTING_ACTION = 223,
#endif

#ifdef SYSTEM_PDA
		HEADER_CG_SOULSTONE_USE = 224,
#endif

#if defined(BL_REMOTE_SHOP)
		HEADER_CG_REMOTE_SHOP = 225,
#endif

#ifdef ENABLE_ANTI_MULTIPLE_FARM
		HEADER_CG_ANTI_FARM = 226,
#endif

#if defined(ENABLE_CSHIELD) && !defined(_DEBUG)
		HEADER_CG_CSHIELD = 240,
#endif

#if defined(__BL_MAILBOX__)
		HEADER_CG_MAILBOX_WRITE = 227,
		HEADER_CG_MAILBOX_WRITE_CONFIRM = 228,
		HEADER_CG_MAILBOX_PROCESS = 229,
#endif

	HEADER_CG_KEY_AGREEMENT						= 0xfb, // _IMPROVED_PACKET_ENCRYPTION_
	HEADER_CG_TIME_SYNC							= 0xfc,
	HEADER_CG_CLIENT_VERSION					= 0xfd,
	HEADER_CG_CLIENT_VERSION2					= 0xf1,
	HEADER_CG_PONG								= 0xfe,
    HEADER_CG_HANDSHAKE                         = 0xff,
	/////////////////////////////////////////////////
	// From Server

	HEADER_GC_CHARACTER_ADD						= 1,
	HEADER_GC_CHARACTER_DEL						= 2,
	HEADER_GC_CHARACTER_MOVE					= 3,
	HEADER_GC_CHAT								= 4,
	HEADER_GC_SYNC_POSITION 					= 5,
	HEADER_GC_LOGIN_SUCCESS3					= 6,
	HEADER_GC_LOGIN_FAILURE						= 7,
	HEADER_GC_PLAYER_CREATE_SUCCESS				= 8,
	HEADER_GC_PLAYER_CREATE_FAILURE				= 9,
	HEADER_GC_PLAYER_DELETE_SUCCESS				= 10,
	HEADER_GC_PLAYER_DELETE_WRONG_SOCIAL_ID		= 11,
	// 12
	HEADER_GC_STUN								= 13,
	HEADER_GC_DEAD								= 14,

	HEADER_GC_MAIN_CHARACTER					= 15,
	HEADER_GC_PLAYER_POINTS						= 16,
	HEADER_GC_PLAYER_POINT_CHANGE				= 17,
	HEADER_GC_CHANGE_SPEED						= 18,
	HEADER_GC_CHARACTER_UPDATE                  = 19,
#if defined(GAIDEN)
	HEADER_GC_ITEM_DEL							= 20, // ������ â�� �߰�
	HEADER_GC_ITEM_SET							= 21, // ������ â�� �߰�
#else
	HEADER_GC_ITEM_SET							= 20, // ������ â�� �߰�
	HEADER_GC_ITEM_SET2							= 21, // ������ â�� �߰�
#endif
	HEADER_GC_ITEM_USE							= 22, // ������ ���? (���� ����鿡��? �����ֱ� ����)
	HEADER_GC_ITEM_DROP							= 23, // ������ ������
	HEADER_GC_ITEM_UPDATE						= 25, // ������ ��ġ ������Ʈ
	HEADER_GC_ITEM_GROUND_ADD					= 26, // �ٴڿ� ������ �߰�
	HEADER_GC_ITEM_GROUND_DEL					= 27, // �ٴڿ��� ������ ����
    HEADER_GC_QUICKSLOT_ADD                     = 28,
    HEADER_GC_QUICKSLOT_DEL                     = 29,
    HEADER_GC_QUICKSLOT_SWAP                    = 30,
	HEADER_GC_ITEM_OWNERSHIP					= 31,
	HEADER_GC_LOGIN_SUCCESS4					= 32,
	HEADER_GC_ITEM_UNBIND_TIME					= 33,
	HEADER_GC_WHISPER							= 34,
	HEADER_GC_ALERT								= 35,

	HEADER_GC_MOTION							= 36,

	HEADER_GC_SHOP							    = 38,
	HEADER_GC_SHOP_SIGN							= 39,

	// 39 ~ 41 Balnk
	HEADER_GC_DUEL_START						= 40,
	HEADER_GC_PVP								= 41,
	HEADER_GC_EXCHANGE							= 42,
    HEADER_GC_CHARACTER_POSITION                = 43,

	HEADER_GC_PING								= 44,

	HEADER_GC_SCRIPT							= 45,
    HEADER_GC_QUEST_CONFIRM                     = 46,
#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	HEADER_GC_OFFLINE_SHOP						= 47,
	HEADER_GC_OFFLINE_SHOP_SIGN					= 48,
#endif	
#ifdef GUILD_WAR_COUNTER
	HEDAER_GC_GUILD_WAR							= 57,
#endif

#ifdef TARGET_INFORMATION_SYSTEM
		HEADER_GC_TARGET_INFO					= 58,
		HEADER_CG_TARGET_INFO_LOAD				= 59,
#endif

	HEADER_GC_MOUNT								= 61,
	HEADER_GC_OWNERSHIP                         = 62,
    HEADER_GC_TARGET                            = 63,

	HEADER_GC_WARP								= 65,
	HEADER_GC_ADD_FLY_TARGETING                 = 69,

	HEADER_GC_CREATE_FLY						= 70,
	HEADER_GC_FLY_TARGETING						= 71,
	HEADER_GC_SKILL_LEVEL						= 72,
	HEADER_GC_SKILL_COOLTIME_END				= 73,
    HEADER_GC_MESSENGER                         = 74,
	HEADER_GC_GUILD								= 75,
	HEADER_GC_SKILL_LEVEL_NEW					= 76,

    HEADER_GC_PARTY_INVITE                      = 77,
    HEADER_GC_PARTY_ADD                         = 78,
    HEADER_GC_PARTY_UPDATE                      = 79,
    HEADER_GC_PARTY_REMOVE                      = 80,

    HEADER_GC_QUEST_INFO                        = 81,
    HEADER_GC_REQUEST_MAKE_GUILD                = 82,
	HEADER_GC_PARTY_PARAMETER                   = 83,

    HEADER_GC_SAFEBOX_MONEY_CHANGE              = 84,
    HEADER_GC_SAFEBOX_SET                       = 85,
    HEADER_GC_SAFEBOX_DEL                       = 86,
    HEADER_GC_SAFEBOX_WRONG_PASSWORD            = 87,
    HEADER_GC_SAFEBOX_SIZE                      = 88,

    HEADER_GC_FISHING                           = 89,

    HEADER_GC_EMPIRE                            = 90,

    HEADER_GC_PARTY_LINK                        = 91,
    HEADER_GC_PARTY_UNLINK                      = 92,

    HEADER_GC_REFINE_INFORMATION                = 95,

	HEADER_GC_OBSERVER_ADD						= 96,
	HEADER_GC_OBSERVER_REMOVE					= 97,
	HEADER_GC_OBSERVER_MOVE						= 98,
	HEADER_GC_VIEW_EQUIP                        = 99,

	HEADER_GC_MARK_BLOCK						= 100,
	HEADER_GC_MARK_DIFF_DATA                    = 101,
	HEADER_GC_MARK_IDXLIST						= 102,

	//HEADER_GC_SLOW_TIMER						= 105,
    HEADER_GC_TIME                              = 106,
    HEADER_GC_CHANGE_NAME                       = 107,

	HEADER_GC_DUNGEON							= 110,
	HEADER_GC_WALK_MODE							= 111,
	HEADER_GC_CHANGE_SKILL_GROUP				= 112,

#if defined(GAIDEN)
	HEADER_GC_MAIN_CHARACTER					= 113,
	HEADER_GC_MAIN_CHARACTER3_BGM				= 137,
	HEADER_GC_MAIN_CHARACTER4_BGM_VOL			= 138,
#else
	// SUPPORT_BGM
	HEADER_GC_MAIN_CHARACTER2_EMPIRE			= 113,
	// END_OF_SUPPORT_BGM
#endif

    HEADER_GC_SEPCIAL_EFFECT                    = 114,
	HEADER_GC_NPC_POSITION						= 115,

    HEADER_GC_CHINA_MATRIX_CARD                 = 116,
    HEADER_GC_CHARACTER_UPDATE2                 = 117,
    HEADER_GC_LOGIN_KEY                         = 118,
    HEADER_GC_REFINE_INFORMATION_NEW            = 119,
    HEADER_GC_CHARACTER_ADD2                    = 120,
    HEADER_GC_CHANNEL                           = 121,

    HEADER_GC_MALL_OPEN                         = 122,
	HEADER_GC_TARGET_UPDATE                     = 123,
	HEADER_GC_TARGET_DELETE                     = 124,
	HEADER_GC_TARGET_CREATE_NEW                 = 125,

	HEADER_GC_AFFECT_ADD                        = 126,
	HEADER_GC_AFFECT_REMOVE                     = 127,

    HEADER_GC_MALL_SET                          = 128,
    HEADER_GC_MALL_DEL                          = 129,
	HEADER_GC_LAND_LIST                         = 130,
	HEADER_GC_LOVER_INFO						= 131,
	HEADER_GC_LOVE_POINT_UPDATE					= 132,
	HEADER_GC_GUILD_SYMBOL_DATA					= 133,
    HEADER_GC_DIG_MOTION                        = 134,

	HEADER_GC_DAMAGE_INFO						= 135,
	HEADER_GC_CHAR_ADDITIONAL_INFO				= 136,

	// SUPPORT_BGM
	HEADER_GC_MAIN_CHARACTER3_BGM				= 137,
	HEADER_GC_MAIN_CHARACTER4_BGM_VOL			= 138,
	// END_OF_SUPPORT_BGM
#ifdef ENABLE_ENTITY_PRELOADING
	HEADER_GC_PRELOAD_ENTITIES				= 139,
#endif
#ifdef NEW_PET_SYSTEM
	HEADER_CG_PetSetName						= 147,
#endif

    HEADER_GC_AUTH_SUCCESS                      = 150,
    HEADER_GC_PANAMA_PACK						= 151,

	//HYBRID CRYPT
	HEADER_GC_HYBRIDCRYPT_KEYS					= 152,
	HEADER_GC_HYBRIDCRYPT_SDB					= 153, // SDB means Supplmentary Data Blocks
	//HYBRID CRYPT
#ifdef __ULTIMATE_TOOLTIP__
	HEADER_GC_CHEST_DROP_INFO = 219,
#endif
	HEADER_GC_AUTH_SUCCESS_OPENID				= 154,
#ifdef ENABLE_PRIVATESHOP_SEARCH_SYSTEM
	HEADER_GC_PSHOP_SEARCH_ITEM_INFO			= 155,
#endif
#ifdef ENABLE_ANTY_WAIT_HACK
	HEADER_GC_ANTY_WAIT_HACK					= 156,
#endif

#ifdef ___DECORUM___
	HEADER_GC_DECORUM_STAT			= 164,
	HEADER_GC_DECORUM_BASE			= 165,
	HEADER_GC_DECORUM_BATTLE		= 166,
	HEADER_GC_DECORUM_KILLS			= 167,
#endif
#ifdef ENABLE_SWITCHBOT
	HEADER_GC_SWITCHBOT				= 171,
#endif

	HEADER_GC_RUNUP_MATRIX_QUIZ                 = 201,
	HEADER_GC_NEWCIBN_PASSPOD_REQUEST			= 202,
	HEADER_GC_NEWCIBN_PASSPOD_FAILURE			= 203,
#if defined(GAIDEN)
	HEADER_GC_ONTIME							= 204,
	HEADER_GC_RESET_ONTIME						= 205,

	// AUTOBAN
	HEADER_GC_AUTOBAN_QUIZ						= 206,
	// END_OF_AUTOBAN

	HEADER_GC_HS_REQUEST						= 207,	// Origially it's 204 on devel branch
#else
	HEADER_GC_HS_REQUEST						= 204,
	HEADER_GC_XTRAP_CS1_REQUEST					= 205,
#endif

#ifdef __AUCTION__
	HEADER_GC_AUCTOIN_ITEM_LIST					= 206,
#endif

	HEADER_GC_SPECIFIC_EFFECT					= 208,
	HEADER_GC_DRAGON_SOUL_REFINE				= 209,
	HEADER_GC_RESPOND_CHANNELSTATUS				= 210,
#if defined(__BL_DROP_DESTROY_TIME__)
		HEADER_GC_ITEM_DESTROY_TIME = 212,
#endif

	// @fixme007
	HEADER_GC_UNK_213							= 213,

#ifdef ENABLE_NEW_DETAILS_GUI
	HEADER_GC_KILL_LOG = 214,
#endif

#ifdef ENABLE_CUBE_RENEWAL_WORLDARD
	HEADER_GC_CUBE_RENEWAL = 217,
#endif
#ifdef ENABLE_6_7_BONUS_NEW_SYSTEM
		HEADER_GC_67_BONUS_NEW = 220,
#endif
	HEADER_GC_SEARCH_SYSTEM = 221,

#if defined(ENABLE_CSHIELD) && !defined(_DEBUG)
		HEADER_GC_CSHIELD = 240,
#endif

#ifdef ENABLE_NEW_FISHING_SYSTEM
		HEADER_GC_FISHING_NEW = 222,
#endif

#ifdef ENABLE_ANTI_MULTIPLE_FARM
		HEADER_GC_ANTI_FARM = 225,
#endif

#ifdef ENABLE_GUILD_REQUEST
		HEADER_GC_GUILD_REQUEST = 226,
#endif

#ifdef ENABLE_ATLAS_BOSS
		HEADER_GC_BOSS_POSITION = 227,
#endif

#if defined(__BL_PARTY_POSITION__)
		HEADER_GC_PARTY_POSITION_INFO = 235,
#endif

#if defined(__BL_MAILBOX__)
		HEADER_GC_MAILBOX_PROCESS = 235,
		HEADER_GC_MAILBOX = 236,
		HEADER_GC_MAILBOX_ADD_DATA = 237,
		HEADER_GC_MAILBOX_ALL = 238,
		HEADER_GC_MAILBOX_UNREAD = 239,
#endif
#ifdef ENABLE_MAINTENANCE_SYSTEM
		HEADER_GC_MAINTENANCE = 241,
#endif	

#if defined(__BL_KILL_BAR__)
		HEADER_GC_KILLBAR = 242,
#endif

#ifdef ENABLE_HUNTING_SYSTEM
	HEADER_GC_HUNTING_OPEN_MAIN = 244,
	HEADER_GC_HUNTING_OPEN_SELECT = 245,
	HEADER_GC_HUNTING_OPEN_REWARD = 246,
	HEADER_GC_HUNTING_UPDATE = 247,
	HEADER_GC_HUNTING_RECIVE_RAND_ITEMS = 248,
#endif

#ifdef ENABLE_EVENT_MANAGER
	HEADER_GC_EVENT_MANAGER = 249,
#endif

		HEADER_GC_KEY_AGREEMENT_COMPLETED = 0xfa, // _IMPROVED_PACKET_ENCRYPTION_ 250
		HEADER_GC_KEY_AGREEMENT = 0xfb, // _IMPROVED_PACKET_ENCRYPTION_ 251
		HEADER_GC_HANDSHAKE_OK = 0xfc, // 252
		HEADER_GC_PHASE = 0xfd,	// 253
		HEADER_GC_BINDUDP = 0xfe, // 254
		HEADER_GC_HANDSHAKE = 0xff, // 255

	/////////////////////////////////////////////////
	// Client To Client for UDP
	/*
	HEADER_CC_STATE_WAITING						= 1,
	HEADER_CC_STATE_WALKING						= 2,
	HEADER_CC_STATE_GOING						= 3,
	HEADER_CC_EVENT_NORMAL_ATTACKING			= 4,
	HEADER_CC_EVENT_COMBO_ATTACKING				= 5,
	HEADER_CC_EVENT_HIT							= 6,
	*/
};

#ifdef ENABLE_ANTI_MULTIPLE_FARM
enum EMacAdress
{
	MA_LENGTH = 255,
	MULTIPLE_FARM_MAX_ACCOUNT = 2,
};
#endif

enum
{
	ID_MAX_NUM = 30,
	PASS_MAX_NUM = 16,
	CHAT_MAX_NUM = 128,
	PATH_NODE_MAX_NUM = 64,
	SHOP_SIGN_MAX_LEN = 32,

	PLAYER_PER_ACCOUNT3 = 3,
#ifndef ENABLE_PLAYER_PER_ACCOUNT5
	PLAYER_PER_ACCOUNT4 = 4,
#else
	PLAYER_PER_ACCOUNT4 = 5,
	PLAYER_PER_ACCOUNT5 = 5,
#endif

	PLAYER_ITEM_SLOT_MAX_NUM = 20,		// �÷��̾��� ���Դ� ����? ����.

	QUICKSLOT_MAX_LINE = 4,
	QUICKSLOT_MAX_COUNT_PER_LINE = 8, // Ŭ���̾�Ʈ ���� ������
	QUICKSLOT_MAX_COUNT = QUICKSLOT_MAX_LINE * QUICKSLOT_MAX_COUNT_PER_LINE,

	QUICKSLOT_MAX_NUM = 36, // ������ ������ �ִ� ��
#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	OFFLINE_SHOP_HOST_ITEM_MAX_NUM = 64,
#endif
	SHOP_HOST_ITEM_MAX_NUM = 40,

	METIN_SOCKET_COUNT = 6,

	PARTY_AFFECT_SLOT_MAX_NUM = 7,

	GUILD_GRADE_NAME_MAX_LEN = 8,
	GUILD_NAME_MAX_LEN = 12,
	GUILD_GRADE_COUNT = 15,
	GULID_COMMENT_MAX_LEN = 50,

	MARK_CRC_NUM = 8*8,
	MARK_DATA_SIZE = 16*12,
	SYMBOL_DATA_SIZE = 128*256,
	QUEST_INPUT_STRING_MAX_NUM = 64,

	PRIVATE_CODE_LENGTH = 8,

	REFINE_MATERIAL_MAX_NUM = 5,

	CHINA_MATRIX_ANSWER_MAX_LEN	= 8,

	RUNUP_MATRIX_QUIZ_MAX_LEN	= 8,
	RUNUP_MATRIX_ANSWER_MAX_LEN = 4,
	NEWCIBN_PASSPOD_ANSWER_MAX_LEN = 8,
	NEWCIBN_PASSPOD_FAILURE_MAX_LEN = 128,

	WEAR_MAX_NUM = CItemData::WEAR_MAX_NUM,

	OPENID_AUTHKEY_LEN = 32,

	SHOP_TAB_NAME_MAX = 32,
	SHOP_TAB_COUNT_MAX = 3,
};

#pragma pack(push)
#pragma pack(1)

#ifdef ENABLE_GUILD_REQUEST
typedef struct SGuildRequest
{
	WORD	index;
	DWORD	g_id;
	char	name[GUILD_NAME_MAX_LEN + 1];
	BYTE	level;
	DWORD	ladder_point;
	WORD	member[2];
	bool	isRequest;
} TGuildRequest;
typedef struct SPacketGCGuildRequest
{
	BYTE	header;
	DWORD	size;
	BYTE	sub_index;
} TPacketGCGuildRequest;
typedef struct SPacketCGGuildRequest
{
	BYTE	bHeader;
	BYTE	sub_index;
} TPacketCGGuildRequest;
enum
{
	SUB_REQUEST_PAGEINDEX,
	SUB_REQUEST_NAME,
	SUB_REQUEST_REQUEST,
};
typedef struct guild_request_
{
	WORD	index;
	DWORD	pid;
	char	name[CHARACTER_NAME_MAX_LEN + 1];
	int		level;
	int		race;
	int		skillIndex;
} TGuild_request;
#endif

#ifdef ENABLE_NEW_DETAILS_GUI
enum
{
	KILL_SHINSO,
	KILL_JINNO,
	KILL_CHUNJO,
	KILL_ALLPLAYER,
	KILL_DUELWON,
	KILL_DUELLOST,
	KILL_MONSTERS,
	KILL_METINSTONE,
	KILL_BOSSES,
	KILL_MAX_NUM,
};
typedef struct SPacketGCKillLOG
{
	BYTE	header;
	long	kill_log[KILL_MAX_NUM];
} TPacketGCKillLOG;
#endif

#ifdef __ULTIMATE_TOOLTIP__
typedef struct SPacketCGChestDropInfo
{
	BYTE	header;
	WORD	wInventoryCell;
	DWORD	item_vnum;
} TPacketCGChestDropInfo;

typedef struct SChestDropInfoTable
{
	BYTE	bPageIndex;
	BYTE	bSlotIndex;
	DWORD	dwItemVnum;
	int		bItemCount;
} TChestDropInfoTable;

typedef struct SPacketGCChestDropInfo
{
	BYTE	bHeader;
	WORD	wSize;
	DWORD	dwChestVnum;
	BYTE	sub;
} TPacketGCChestDropInfo;
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Mark
typedef struct command_mark_login
{
    BYTE    header;
    DWORD   handle;
    DWORD   random_key;
} TPacketCGMarkLogin;

typedef struct command_mark_upload
{
    BYTE    header;
    DWORD   gid;
    BYTE    image[16*12*4];
} TPacketCGMarkUpload;

typedef struct command_mark_idxlist
{
    BYTE    header;
} TPacketCGMarkIDXList;

typedef struct command_mark_crclist
{
    BYTE    header;
    BYTE    imgIdx;
    DWORD   crclist[80];
} TPacketCGMarkCRCList;

typedef struct packet_mark_idxlist
{
    BYTE    header;
	DWORD	bufSize;
    WORD    count;
    //�ڿ� size * (WORD + WORD)��ŭ ������ ����
} TPacketGCMarkIDXList;

typedef struct packet_mark_block
{
    BYTE    header;
    DWORD   bufSize;
	BYTE	imgIdx;
    DWORD   count;
    // �ڿ� 64 x 48 x �ȼ�ũ��(4����Ʈ) = 12288��ŭ ������ ����
} TPacketGCMarkBlock;

typedef struct command_symbol_upload
{
	BYTE	header;
	WORD	size;
	DWORD	handle;
} TPacketCGSymbolUpload;

typedef struct command_symbol_crc
{
	BYTE	header;
	DWORD	dwGuildID;
	DWORD	dwCRC;
	DWORD	dwSize;
} TPacketCGSymbolCRC;

typedef struct packet_symbol_data
{
    BYTE header;
    WORD size;
    DWORD guild_id;
} TPacketGCGuildSymbolData;

//
//
//
typedef struct packet_observer_add
{
	BYTE	header;
	DWORD	vid;
	WORD	x;
	WORD	y;
} TPacketGCObserverAdd;

typedef struct packet_observer_move
{
	BYTE	header;
	DWORD	vid;
	WORD	x;
	WORD	y;
} TPacketGCObserverMove;


typedef struct packet_observer_remove
{
	BYTE	header;
	DWORD	vid;
} TPacketGCObserverRemove;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// To Server

typedef struct command_checkin
{
	BYTE header;
	char name[ID_MAX_NUM+1];
	char pwd[PASS_MAX_NUM+1];
} TPacketCGCheckin;

typedef struct command_login
{
    BYTE header;
    char name[ID_MAX_NUM + 1];
    char pwd[PASS_MAX_NUM + 1];
} TPacketCGLogin;

// start - ���� ���� ������ ���� ��Ŷ��
typedef struct command_login2
{
	BYTE	header;
	char	name[ID_MAX_NUM + 1];
	DWORD	login_key;
    DWORD	adwClientKey[4];
#ifdef ENABLE_ANTI_MULTIPLE_FARM
	char	cMAIf[MA_LENGTH + 1];
#endif
} TPacketCGLogin2;

typedef struct command_login3
{
    BYTE	header;
    char	name[ID_MAX_NUM + 1];
    char	pwd[PASS_MAX_NUM + 1];
    DWORD	adwClientKey[4];
	#ifdef ENABLE_MULTI_LANGUAGE_SYSTEM
	char	language[LANGUAGE_MAX_LEN + 1];
	#endif
} TPacketCGLogin3;

typedef struct command_login5
{
    BYTE	header;
    char	authKey[OPENID_AUTHKEY_LEN + 1];
    DWORD	adwClientKey[4];
} TPacketCGLogin5;

#ifdef NEW_PET_SYSTEM
typedef struct packet_RequestPetName
{
	BYTE byHeader;
	char petname[13];
} TPacketCGRequestPetName;
#endif

// end - ���� ���� ������ ���� ��Ŷ��

typedef struct command_direct_enter
{
    BYTE        bHeader;
    char        login[ID_MAX_NUM + 1];
    char        passwd[PASS_MAX_NUM + 1];
    BYTE        index;
} TPacketCGDirectEnter;

typedef struct command_player_select
{
	BYTE	header;
	BYTE	player_index;
} TPacketCGSelectCharacter;

typedef struct command_attack
{
	BYTE	header;
	BYTE	bType;			// ���� ����
	DWORD	dwVictimVID;	// �� VID
	BYTE	bCRCMagicCubeProcPiece;
	BYTE	bCRCMagicCubeFilePiece;
} TPacketCGAttack;

typedef struct command_chat
{
	BYTE	header;
	WORD	length;
	BYTE	type;
} TPacketCGChat;

typedef struct command_whisper
{
    BYTE        bHeader;
    WORD        wSize;
    char        szNameTo[CHARACTER_NAME_MAX_LEN + 1];
} TPacketCGWhisper;

typedef struct command_sms
{
    BYTE        bHeader;
    WORD        wSize;
    char        szNameTo[CHARACTER_NAME_MAX_LEN + 1];
} TPacketCGSMS;

enum EBattleMode
{
	BATTLEMODE_ATTACK = 0,
	BATTLEMODE_DEFENSE = 1,
};

typedef struct command_EnterFrontGame
{
	BYTE header;
} TPacketCGEnterFrontGame;

typedef struct command_item_use
{
	BYTE header;
	TItemPos pos;
} TPacketCGItemUse;

#if defined(BL_REMOTE_SHOP)
typedef struct command_remote_shop
{
	BYTE	header;
	BYTE	shop_index;
} TPacketCGRemoteShop;
#endif

typedef struct command_item_use_to_item
{
	BYTE header;
	TItemPos source_pos;
	TItemPos target_pos;
} TPacketCGItemUseToItem;

typedef struct command_item_drop
{
	BYTE  header;
	TItemPos pos;
	DWORD elk;
} TPacketCGItemDrop;

typedef struct command_item_drop2
{
    BYTE        header;
    TItemPos pos;
    DWORD       gold;
    short        count;
} TPacketCGItemDrop2;

typedef struct command_item_move
{
	BYTE header;
	TItemPos pos;
	TItemPos change_pos;
	short num;
} TPacketCGItemMove;

#ifdef ENABLE_SELL_ITEM
typedef struct command_item_sell
{
	BYTE        header;
	TItemPos    pos;
	DWORD		gold;
} TPacketCGItemSell;
#endif

#ifdef ENABLE_SORT_INVEN
typedef struct sort_inven
{
	BYTE	header;
	BYTE 	option;
} TPacketCGSortInven;
#endif

#ifdef NEW_ADD_INVENTORY
typedef struct envanter_paketi
{
	BYTE		header;
} TPacketCGEnvanter;
#endif

typedef struct command_item_pickup
{
	BYTE header;
	DWORD vid;
} TPacketCGItemPickUp;

#if defined(__BL_SOUL_ROULETTE__)
typedef struct command_cgroulette
{
	BYTE	header;
	BYTE 	option;
} TPacketCGSoulRoulette;
typedef struct command_gcroulette
{
	BYTE	header;
	BYTE 	option;
	int		yang;
	struct {
		DWORD vnum;
		BYTE count;
	} ItemInfo[ROULETTE_ITEM_MAX];
} TPacketGCSoulRoulette;
#endif

typedef struct command_quickslot_add
{
    BYTE        header;
    BYTE        pos;
	TQuickSlot	slot;
}TPacketCGQuickSlotAdd;

typedef struct command_quickslot_del
{
    BYTE        header;
    BYTE        pos;
}TPacketCGQuickSlotDel;

typedef struct command_quickslot_swap
{
    BYTE        header;
    BYTE        pos;
    BYTE        change_pos;
}TPacketCGQuickSlotSwap;

typedef struct command_on_click
{
	BYTE		header;
	DWORD		vid;
} TPacketCGOnClick;


enum
{
    SHOP_SUBHEADER_CG_END,
	SHOP_SUBHEADER_CG_BUY,
	SHOP_SUBHEADER_CG_SELL,
	SHOP_SUBHEADER_CG_SELL2,
#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	SHOP_SUBHEADER_CG_PUT_ITEM,
	SHOP_SUBHEADER_CG_DESTROY_OFFLINE_SHOP,
	SHOP_SUBHEADER_CG_DEL_ITEM,
	SHOP_SUBHEADER_CG_EDIT_ITEM
#endif
};

#ifdef ENABLE_EVENT_MANAGER
typedef struct SPacketGCEventManager
{
	BYTE	header;
	DWORD	size;
} TPacketGCEventManager;
typedef struct event_struct_
{
	BYTE	eventIndex;
	char	startText[40];
	int		startRealTime;
	char	endText[40];
	int		endRealTime;
	bool	isAlreadyStart;
	BYTE	empireFlag;
	BYTE	channelFlag;
	DWORD	value[4];

	/*
	BYTE	eventIndex;
	char	startText[40];
	tm		startTime;
	time_t	startRealTime;
	char	endText[40];
	tm		endTime;
	time_t	endRealTime;
	BYTE	empireFlag;
	BYTE	channelFlag;
	DWORD	value[4];
	*/
}TEventManagerData;
enum
{
	EVENT_NONE = 0,
	BONUS_EVENT = 1,
	DOUBLE_BOSS_LOOT_EVENT = 2,
	DOUBLE_METIN_LOOT_EVENT = 3,
	DOUBLE_MISSION_BOOK_EVENT = 4,
	DUNGEON_COOLDOWN_EVENT = 5,
	DUNGEON_TICKET_LOOT_EVENT = 6,
	EMPIRE_WAR_EVENT = 7,
	MOONLIGHT_EVENT = 8,
	TOURNAMENT_EVENT = 9,
	WHELL_OF_FORTUNE_EVENT = 10,
	HALLOWEEN_EVENT = 11,
	NPC_SEARCH_EVENT = 12,
};
#endif

#ifdef ENABLE_RENEWAL_PVP
enum
{
	PVP_CRITICAL_DAMAGE_SKILLS,
	PVP_POISONING,
	PVP_HALF_HUMAN,
	PVP_BUFFI_SKILLS,
	PVP_MISS_HITS,
	PVP_DISPEL_EFFECTS,
	PVP_HP_ELIXIR,
	PVP_WHITE_DEW,
	PVP_YELLOW_DEW,
	PVP_ORANGE_DEW,
	PVP_RED_DEW,
	PVP_BLUE_DEW,
	PVP_GREEN_DEW,
	PVP_ZIN_WATER,
	PVP_SAMBO_WATER,
	PVP_ATTACKSPEED_FISH,
	PVP_DRAGON_GOD_ATTACK,
	PVP_DRAGON_GOD_DEFENCE,
	PVP_DRAGON_GOD_LIFE,
	PVP_PIERCING_STRIKE,
	PVP_CRITICAL_STRIKE,
	PVP_PET,
	PVP_NEW_PET,
	PVP_ENERGY,
	PVP_BET,
	PVP_MAX,
};
#endif


typedef struct command_shop
{
	BYTE        header;
	short		subheader;
} TPacketCGShop;

enum
{
	EXCHANGE_SUBHEADER_CG_START,			// arg1 == vid of target character
	EXCHANGE_SUBHEADER_CG_ITEM_ADD,		// arg1 == position of item
	EXCHANGE_SUBHEADER_CG_ITEM_DEL,		// arg1 == position of item
	EXCHANGE_SUBHEADER_CG_ELK_ADD,			// arg1 == amount of elk
#ifdef ENABLE_CHEQUE_SYSTEM
	EXCHANGE_SUBHEADER_CG_CHEQUE_ADD,	/* arg1 == amount of cheque */
#endif
	EXCHANGE_SUBHEADER_CG_ACCEPT,			// arg1 == not used
	EXCHANGE_SUBHEADER_CG_CANCEL,			// arg1 == not used
};

typedef struct command_exchange
{
	BYTE		header;
	BYTE		subheader;
	DWORD		arg1;
	BYTE		arg2;
	TItemPos	Pos;
} TPacketCGExchange;

typedef struct command_position
{
    BYTE        header;
    BYTE        position;
} TPacketCGPosition;

typedef struct command_script_answer
{
    BYTE        header;
	BYTE		answer;
} TPacketCGScriptAnswer;

typedef struct command_script_button
{
    BYTE        header;
	unsigned int			idx;
} TPacketCGScriptButton;

typedef struct command_target
{
    BYTE        header;
    DWORD       dwVID;
} TPacketCGTarget;

typedef struct command_move
{
	BYTE		bHeader;
	BYTE		bFunc;
	BYTE		bArg;
	BYTE		bRot;
	LONG		lX;
	LONG		lY;
	DWORD		dwTime;
} TPacketCGMove;

typedef struct command_sync_position_element
{
    DWORD       dwVID;
    long        lX;
    long        lY;
} TPacketCGSyncPositionElement;

typedef struct command_sync_position
{
    BYTE        bHeader;
	WORD		wSize;
} TPacketCGSyncPosition;

typedef struct command_fly_targeting
{
	BYTE		bHeader;
	DWORD		dwTargetVID;
	long		lX;
	long		lY;
} TPacketCGFlyTargeting;

typedef struct packet_fly_targeting
{
    BYTE        bHeader;
	DWORD		dwShooterVID;
	DWORD		dwTargetVID;
	long		lX;
	long		lY;
} TPacketGCFlyTargeting;

typedef struct packet_shoot
{
    BYTE		bHeader;
    BYTE		bType;
} TPacketCGShoot;

typedef struct command_warp
{
	BYTE			bHeader;
} TPacketCGWarp;

enum
{
#ifdef ENABLE_MESSENGER_BLOCK
	MESSENGER_SUBHEADER_GC_BLOCK_LIST,
	MESSENGER_SUBHEADER_GC_BLOCK_LOGIN,
	MESSENGER_SUBHEADER_GC_BLOCK_LOGOUT,
	MESSENGER_SUBHEADER_GC_BLOCK_INVITE,//not used
#endif	
#ifdef ENABLE_MESSENGER_TEAM
	MESSENGER_SUBHEADER_GC_TEAM_LIST,
	MESSENGER_SUBHEADER_GC_TEAM_LOGIN,
	MESSENGER_SUBHEADER_GC_TEAM_LOGOUT,
#endif
	MESSENGER_SUBHEADER_GC_LIST,
	MESSENGER_SUBHEADER_GC_LOGIN,
	MESSENGER_SUBHEADER_GC_LOGOUT,
	MESSENGER_SUBHEADER_GC_INVITE,
	MESSENGER_SUBHEADER_GC_MOBILE,
};

typedef struct packet_messenger
{
    BYTE header;
    WORD size;
    BYTE subheader;
} TPacketGCMessenger;

typedef struct packet_messenger_list_offline
{
    BYTE connected; // always 0
	BYTE length;
} TPacketGCMessengerListOffline;

#ifdef ENABLE_MESSENGER_TEAM
typedef struct packet_messenger_team_list_offline
{
	BYTE connected; // always 0
	BYTE length;
} TPacketGCMessengerTeamListOffline;

typedef struct packet_messenger_team_list_online
{
	BYTE connected; // always 1
	BYTE length;
} TPacketGCMessengerTeamListOnline;
#endif

#ifdef ENABLE_MESSENGER_BLOCK
typedef struct packet_messenger_block_list_offline
{
	BYTE connected; // always 0
	BYTE length;
} TPacketGCMessengerBlockListOffline;

typedef struct packet_messenger_block_list_online
{
	BYTE connected; // always 1
	BYTE length;
} TPacketGCMessengerBlockListOnline;
#endif

enum
{
	MESSENGER_CONNECTED_STATE_OFFLINE,
	MESSENGER_CONNECTED_STATE_ONLINE,
	MESSENGER_CONNECTED_STATE_MOBILE,
};

typedef struct packet_messenger_list_online
{
    BYTE connected;
	BYTE length;
	//BYTE length_char_name;
} TPacketGCMessengerListOnline;

typedef struct packet_messenger_login
{
	//BYTE length_login;
	//BYTE length_char_name;
	BYTE length;
} TPacketGCMessengerLogin;

typedef struct packet_messenger_logout
{
	BYTE length;
} TPacketGCMessengerLogout;

enum
{
#ifdef ENABLE_MESSENGER_BLOCK
	MESSENGER_SUBHEADER_CG_ADD_BLOCK_BY_VID,
    MESSENGER_SUBHEADER_CG_ADD_BLOCK_BY_NAME,
    MESSENGER_SUBHEADER_CG_REMOVE_BLOCK,
#endif
	MESSENGER_SUBHEADER_CG_ADD_BY_VID,
    MESSENGER_SUBHEADER_CG_ADD_BY_NAME,
    MESSENGER_SUBHEADER_CG_REMOVE,
};

typedef struct command_messenger
{
    BYTE header;
    BYTE subheader;
} TPacketCGMessenger;

typedef struct command_messenger_remove
{
	BYTE length;
} TPacketCGMessengerRemove;

enum
{
	SAFEBOX_MONEY_STATE_SAVE,
	SAFEBOX_MONEY_STATE_WITHDRAW,
};

typedef struct command_safebox_money
{
    BYTE        bHeader;
    BYTE        bState;
    DWORD       dwMoney;
} TPacketCGSafeboxMoney;

typedef struct command_safebox_checkout
{
    BYTE        bHeader;
    BYTE        bSafePos;
    TItemPos	ItemPos;
} TPacketCGSafeboxCheckout;

typedef struct command_safebox_checkin
{
    BYTE        bHeader;
    BYTE        bSafePos;
    TItemPos	ItemPos;
} TPacketCGSafeboxCheckin;

typedef struct command_mall_checkout
{
    BYTE        bHeader;
    BYTE        bMallPos;
    TItemPos	ItemPos;
} TPacketCGMallCheckout;

#ifdef ENABLE_MAINTENANCE_SYSTEM
typedef struct SPacketGCMaintenance {
	BYTE	header;
	DWORD	dwTimeLeft;
	DWORD	dwDuration;
} TPacketGCMaintenance;
#endif

///////////////////////////////////////////////////////////////////////////////////
// Party

typedef struct command_use_skill
{
    BYTE                bHeader;
    DWORD               dwVnum;
	DWORD				dwTargetVID;
} TPacketCGUseSkill;

typedef struct command_party_invite
{
    BYTE header;
    DWORD vid;
} TPacketCGPartyInvite;

typedef struct command_party_invite_answer
{
    BYTE header;
    DWORD leader_pid;
    BYTE accept;
} TPacketCGPartyInviteAnswer;

typedef struct command_party_remove
{
    BYTE header;
    DWORD pid;
} TPacketCGPartyRemove;

typedef struct command_party_set_state
{
    BYTE byHeader;
    DWORD dwVID;
	BYTE byState;
    BYTE byFlag;
} TPacketCGPartySetState;

typedef struct packet_party_link
{
    BYTE header;
    DWORD pid;
    DWORD vid;
} TPacketGCPartyLink;

typedef struct packet_party_unlink
{
    BYTE header;
    DWORD pid;
	DWORD vid;
} TPacketGCPartyUnlink;

typedef struct command_party_use_skill
{
    BYTE byHeader;
	BYTE bySkillIndex;
    DWORD dwTargetVID;
} TPacketCGPartyUseSkill;

enum
{
	GUILD_SUBHEADER_CG_ADD_MEMBER,
	GUILD_SUBHEADER_CG_REMOVE_MEMBER,
	GUILD_SUBHEADER_CG_CHANGE_GRADE_NAME,
	GUILD_SUBHEADER_CG_CHANGE_GRADE_AUTHORITY,
	GUILD_SUBHEADER_CG_OFFER,
	GUILD_SUBHEADER_CG_POST_COMMENT,
	GUILD_SUBHEADER_CG_DELETE_COMMENT,
	GUILD_SUBHEADER_CG_REFRESH_COMMENT,
	GUILD_SUBHEADER_CG_CHANGE_MEMBER_GRADE,
	GUILD_SUBHEADER_CG_USE_SKILL,
	GUILD_SUBHEADER_CG_CHANGE_MEMBER_GENERAL,
	GUILD_SUBHEADER_CG_GUILD_INVITE_ANSWER,
	GUILD_SUBHEADER_CG_CHARGE_GSP,
	GUILD_SUBHEADER_CG_DEPOSIT_MONEY,
	GUILD_SUBHEADER_CG_WITHDRAW_MONEY,
};

typedef struct command_guild
{
    BYTE byHeader;
	BYTE bySubHeader;
} TPacketCGGuild;

#ifdef ENABLE_MULTI_LANGUAGE_SYSTEM
typedef struct SPacketCGRequestFlag
{
	BYTE	header;
	char	name[CHARACTER_NAME_MAX_LEN + 1];
} TPacketCGRequestFlag;
#endif

typedef struct command_guild_answer_make_guild
{
	BYTE header;
	char guild_name[GUILD_NAME_MAX_LEN+1];
} TPacketCGAnswerMakeGuild;

typedef struct command_give_item
{
	BYTE byHeader;
	DWORD dwTargetVID;
	TItemPos ItemPos;
	short byItemCount;
} TPacketCGGiveItem;

typedef struct SPacketCGHack
{
    BYTE        bHeader;
    char        szBuf[255 + 1];
} TPacketCGHack;

typedef struct command_dungeon
{
	BYTE		bHeader;
	WORD		size;
} TPacketCGDungeon;

// Private Shop
typedef struct SShopItemTable
{
	DWORD		vnum;
	BYTE		count;
#ifdef ENABLE_MULTISHOP
	DWORD		wPriceVnum;
	DWORD		wPrice;
#endif
	TItemPos	pos;
	DWORD		price;
#ifdef ENABLE_CHEQUE_SYSTEM
	int		cheque_price;
#endif
	BYTE		display_pos;
} TShopItemTable;

typedef struct SPacketCGMyShop
{
    BYTE        bHeader;
    char        szSign[SHOP_SIGN_MAX_LEN + 1];
    short        bCount;	// count of TShopItemTable, max 39
} TPacketCGMyShop;

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
typedef struct SOfflineShopItemTable
{
	TItemPos	item_pos;
	// BEGIN_MAX_YANG
	int			price;
	// END_OF_MAX_YANG

#ifdef ENABLE_CHEQUE_SYSTEM
	int		cheque;
#endif
} TOfflineShopItemTable;

typedef struct TPacketCGMyOfflineShop
{
	BYTE	bHeader;
	char	szSign[SHOP_SIGN_MAX_LEN + 1];
	BYTE	bCount;
	BYTE	bTime;
} TPacketCGMyOfflineShop;
#endif

typedef struct SPacketCGRefine
{
	BYTE		header;
	BYTE		pos;
	BYTE		type;
} TPacketCGRefine;

typedef struct SPacketCGChangeName
{
    BYTE header;
    BYTE index;
    char name[CHARACTER_NAME_MAX_LEN+1];
} TPacketCGChangeName;

typedef struct command_client_version
{
	BYTE header;
	char filename[32+1];
	char timestamp[32+1];
} TPacketCGClientVersion;

typedef struct command_client_version2
{
	BYTE header;
	char filename[32+1];
	char timestamp[32+1];
} TPacketCGClientVersion2;

typedef struct command_crc_report
{
	BYTE header;
	BYTE byPackMode;
	DWORD dwBinaryCRC32;
	DWORD dwProcessCRC32;
	DWORD dwRootPackCRC32;
} TPacketCGCRCReport;

typedef struct command_china_matrix_card
{
	BYTE	bHeader;
	char	szAnswer[CHINA_MATRIX_ANSWER_MAX_LEN + 1];
} TPacketCGChinaMatrixCard;

typedef struct command_runup_matrix_answer
{
	BYTE	bHeader;
	char	szAnswer[RUNUP_MATRIX_ANSWER_MAX_LEN + 1];
} TPacketCGRunupMatrixAnswer;

typedef struct command_newcibn_passpod_answer
{
	BYTE	bHeader;
	char	szAnswer[NEWCIBN_PASSPOD_ANSWER_MAX_LEN + 1];
} TPacketCGNEWCIBNPasspodAnswer;

enum EPartyExpDistributionType
{
    PARTY_EXP_DISTRIBUTION_NON_PARITY,
    PARTY_EXP_DISTRIBUTION_PARITY,
};

typedef struct command_party_parameter
{
    BYTE        bHeader;
    BYTE        bDistributeMode;
} TPacketCGPartyParameter;

typedef struct command_quest_input_string
{
    BYTE        bHeader;
    char		szString[QUEST_INPUT_STRING_MAX_NUM+1];
} TPacketCGQuestInputString;

typedef struct command_quest_confirm
{
    BYTE header;
    BYTE answer;
    DWORD requestPID;
} TPacketCGQuestConfirm;

typedef struct command_script_select_item
{
    BYTE header;
    DWORD selection;
} TPacketCGScriptSelectItem;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// From Server
enum EPhase
{
    PHASE_CLOSE,				// �����? ���� (�Ǵ� ���� �� ����)
    PHASE_HANDSHAKE,			// �Ǽ�..;;
    PHASE_LOGIN,				// �α��� ��
    PHASE_SELECT,				// ĳ���� ���� ȭ��
    PHASE_LOADING,				// ���� �� �ε� ȭ��
    PHASE_GAME,					// ���� ȭ��
    PHASE_DEAD,					// �׾��� ��.. (���� �ȿ� �ִ� ���� ����..)

	PHASE_DBCLIENT_CONNECTING,	// ������
    PHASE_DBCLIENT,				// ������
    PHASE_P2P,					// ������
    PHASE_AUTH,					// �α��� ���� ��
};

typedef struct packet_phase
{
    BYTE        header;
    BYTE        phase;
} TPacketGCPhase;

typedef struct packet_blank		// ������Ŷ.
{
	BYTE		header;
} TPacketGCBlank;

typedef struct packet_blank_dynamic
{
	BYTE		header;
	WORD		size;
} TPacketGCBlankDynamic;

typedef struct packet_header_handshake
{
	BYTE		header;
	DWORD		dwHandshake;
	DWORD		dwTime;
	LONG		lDelta;
} TPacketGCHandshake;

typedef struct packet_header_bindudp
{
	BYTE		header;
	DWORD		addr;
	WORD		port;
} TPacketGCBindUDP;

typedef struct packet_header_dynamic_size
{
	BYTE		header;
	WORD		size;
} TDynamicSizePacketHeader;

typedef struct SSimplePlayerInformation
{
    DWORD               dwID;
    char                szName[CHARACTER_NAME_MAX_LEN + 1];
    BYTE                byJob;
    BYTE                byLevel;
    DWORD               dwPlayMinutes;
    BYTE                byST, byHT, byDX, byIQ;
//	WORD				wParts[CRaceData::PART_MAX_NUM];
    WORD                wMainPart;
    BYTE                bChangeName;
	WORD				wHairPart;
#ifdef ENABLE_ACCE_COSTUME_SYSTEM
	WORD				wAccePart;
#endif
#ifdef ENABLE_AURA_SYSTEM
	WORD	wAuraPart;
#endif
    BYTE                bDummy[4];
	long				x, y;
	LONG				lAddr;
	WORD				wPort;
	BYTE				bySkillGroup;
} TSimplePlayerInformation;

typedef struct packet_login_success3
{
	BYTE						header;
	TSimplePlayerInformation	akSimplePlayerInformation[PLAYER_PER_ACCOUNT3];
    DWORD						guild_id[PLAYER_PER_ACCOUNT3];
    char						guild_name[PLAYER_PER_ACCOUNT3][GUILD_NAME_MAX_LEN+1];
	DWORD handle;
	DWORD random_key;
} TPacketGCLoginSuccess3;

typedef struct packet_login_success4
{
	BYTE						header;
	TSimplePlayerInformation	akSimplePlayerInformation[PLAYER_PER_ACCOUNT4];
    DWORD						guild_id[PLAYER_PER_ACCOUNT4];
    char						guild_name[PLAYER_PER_ACCOUNT4][GUILD_NAME_MAX_LEN+1];
	DWORD handle;
	DWORD random_key;
} TPacketGCLoginSuccess4;
#ifdef ENABLE_PLAYER_PER_ACCOUNT5
typedef struct packet_login_success5
{
	BYTE						header;
	TSimplePlayerInformation	akSimplePlayerInformation[PLAYER_PER_ACCOUNT5];
    DWORD						guild_id[PLAYER_PER_ACCOUNT5];
    char						guild_name[PLAYER_PER_ACCOUNT5][GUILD_NAME_MAX_LEN+1];
	DWORD handle;
	DWORD random_key;
} TPacketGCLoginSuccess5;
#endif

enum { LOGIN_STATUS_MAX_LEN = 8 };
typedef struct packet_login_failure
{
	BYTE	header;
	char	szStatus[LOGIN_STATUS_MAX_LEN + 1];
} TPacketGCLoginFailure;

typedef struct command_player_create
{
	BYTE        header;
	BYTE        index;
	char        name[CHARACTER_NAME_MAX_LEN + 1];
	WORD        job;
	BYTE		shape;
	BYTE		CON;
	BYTE		INT;
	BYTE		STR;
	BYTE		DEX;
} TPacketCGCreateCharacter;

typedef struct command_player_create_success
{
    BYTE						header;
    BYTE						bAccountCharacterSlot;
    TSimplePlayerInformation	kSimplePlayerInfomation;
} TPacketGCPlayerCreateSuccess;

typedef struct command_create_failure
{
	BYTE	header;
	BYTE	bType;
} TPacketGCCreateFailure;

typedef struct command_player_delete
{
	BYTE        header;
	BYTE        index;
	char		szPrivateCode[PRIVATE_CODE_LENGTH];
} TPacketCGDestroyCharacter;

typedef struct packet_player_delete_success
{
	BYTE        header;
	BYTE        account_index;
} TPacketGCDestroyCharacterSuccess;

#if defined(__BL_KILL_BAR__)
typedef struct command_kill_bar
{
	BYTE	bHeader;
	BYTE	bKillerRace;
	BYTE	bKillerWeaponType;
	BYTE	bVictimRace;
	char	szKiller[CHARACTER_NAME_MAX_LEN + 1];
	char	szVictim[CHARACTER_NAME_MAX_LEN + 1];
} TPacketGCKillBar;
#endif

enum
{
	ADD_CHARACTER_STATE_DEAD   = (1 << 0),
	ADD_CHARACTER_STATE_SPAWN  = (1 << 1),
	ADD_CHARACTER_STATE_GUNGON = (1 << 2),
	ADD_CHARACTER_STATE_KILLER = (1 << 3),
	ADD_CHARACTER_STATE_PARTY  = (1 << 4),
};

enum EPKModes
{
	PK_MODE_PEACE,
	PK_MODE_REVENGE,
	PK_MODE_FREE,
	PK_MODE_PROTECT,
	PK_MODE_GUILD,
	PK_MODE_MAX_NUM,
};

// 2004.11.20.myevan.CRaceData::PART_MAX_NUM �����ϰ� ���� - �������� ����ϴ°Ͱ�? ��ġ���� ����
enum ECharacterEquipmentPart
{
	CHR_EQUIPPART_ARMOR,
	CHR_EQUIPPART_WEAPON,
	CHR_EQUIPPART_HEAD,
	CHR_EQUIPPART_HAIR,
#ifdef ENABLE_ACCE_COSTUME_SYSTEM
	CHR_EQUIPPART_ACCE,
#endif
#ifdef ENABLE_NEW_ARROW_SYSTEM
	CHR_PART_ARROW_TYPE,
#endif
#ifdef ENABLE_AURA_SYSTEM
	CHR_EQUIPPART_AURA,
#endif
#ifdef ENABLE_EFFECT_COSTUME_SYSTEM
	CHR_EQUIPPART_EFFECT,
#endif
	CHR_EQUIPPART_NUM,
};

typedef struct packet_char_additional_info
{
	BYTE    header;
	DWORD   dwVID;
	char    name[CHARACTER_NAME_MAX_LEN + 1];
	WORD    awPart[CHR_EQUIPPART_NUM];
	BYTE	bEmpire;
	DWORD   dwGuildID;
	DWORD   dwLevel;
	short   sAlignment; //����ġ
	BYTE    bPKMode;
	DWORD   dwMountVnum;
#ifdef ___DECORUM___
	DWORD	dwDecorumPoint;
#endif
#ifdef ENABLE_MULTI_LANGUAGE_SYSTEM
	char    language[LANGUAGE_MAX_LEN + 1];	
#endif
#ifdef GUILD_RANK_EFFECT
	DWORD	guildRank;
#endif
} TPacketGCCharacterAdditionalInfo;

typedef struct packet_add_char
{
    BYTE        header;

    DWORD       dwVID;

    //char        name[CHARACTER_NAME_MAX_LEN + 1];

    float       angle;
    long        x;
    long        y;
    long        z;

	BYTE		bType;
    WORD        wRaceNum;
    //WORD        awPart[CHR_EQUIPPART_NUM];
    BYTE        bMovingSpeed;
    BYTE        bAttackSpeed;

    BYTE        bStateFlag;
    DWORD       dwAffectFlag[2];        // ??
    //BYTE      bEmpire;
    //DWORD     dwGuild;
    //short     sAlignment;
	//BYTE		bPKMode;
	//DWORD		dwMountVnum;
} TPacketGCCharacterAdd;

typedef struct packet_add_char2
{
    BYTE        header;

    DWORD       dwVID;

    char        name[CHARACTER_NAME_MAX_LEN + 1];

    float       angle;
    long        x;
    long        y;
    long        z;

	BYTE		bType;
    WORD        wRaceNum;
    WORD        awPart[CHR_EQUIPPART_NUM];
    BYTE        bMovingSpeed;
    BYTE        bAttackSpeed;

    BYTE        bStateFlag;
    DWORD       dwAffectFlag[2];        // ??
    BYTE        bEmpire;

    DWORD       dwGuild;
    short       sAlignment;
	BYTE		bPKMode;
	DWORD		dwMountVnum;
} TPacketGCCharacterAdd2;

typedef struct packet_update_char
{
    BYTE        header;
    DWORD       dwVID;

    WORD        awPart[CHR_EQUIPPART_NUM];
    BYTE        bMovingSpeed;
	BYTE		bAttackSpeed;

    BYTE        bStateFlag;
    DWORD       dwAffectFlag[2];

	DWORD		dwGuildID;
    short       sAlignment;
#ifdef NEW_PET_SYSTEM
	DWORD		dwLevel;
#endif
	BYTE		bPKMode;
	DWORD		dwMountVnum;
#ifdef GUILD_RANK_EFFECT
	DWORD	guildRank;
#endif
} TPacketGCCharacterUpdate;

typedef struct packet_update_char2
{
    BYTE        header;
    DWORD       dwVID;

    WORD        awPart[CHR_EQUIPPART_NUM];
    BYTE        bMovingSpeed;
	BYTE		bAttackSpeed;

    BYTE        bStateFlag;
    DWORD       dwAffectFlag[2];

	DWORD		dwGuildID;
    short       sAlignment;
	BYTE		bPKMode;
	DWORD		dwMountVnum;
#ifdef ___DECORUM___
	DWORD		dwDecorumPoint;
#endif	
} TPacketGCCharacterUpdate2;

typedef struct packet_del_char
{
	BYTE	header;
    DWORD	dwVID;
} TPacketGCCharacterDelete;

typedef struct packet_GlobalTime
{
	BYTE	header;
	float	GlobalTime;
} TPacketGCGlobalTime;

enum EChatType
{
	CHAT_TYPE_TALKING,  /* �׳� ä�� */
	CHAT_TYPE_INFO,     /* ���� (�������� ������, ����ġ�� �����?. ��) */
	CHAT_TYPE_NOTICE,   /* �������� */
	CHAT_TYPE_PARTY,    /* ��Ƽ�� */
	CHAT_TYPE_GUILD,    /* ���? */
	CHAT_TYPE_COMMAND,	/* ���� */
	CHAT_TYPE_SHOUT,	/* ��ġ�� */
	CHAT_TYPE_WHISPER,	// �����ʹ� �������� �ʴ� Only Client Enum
	CHAT_TYPE_BIG_NOTICE,
	CHAT_TYPE_MONARCH_NOTICE,
	// CHAT_TYPE_UNK_10,
#ifdef ENABLE_DICE_SYSTEM
	CHAT_TYPE_DICE_INFO, //11
#endif
#if defined(ENABLE_CHATTING_WINDOW_RENEWAL)
	CHAT_TYPE_EXP_INFO,
	CHAT_TYPE_ITEM_INFO,
	CHAT_TYPE_MONEY_INFO,
#endif
	CHAT_TYPE_MISSION,
	CHAT_TYPE_MAX_NUM,
};

typedef struct packet_chatting
{
	BYTE	header;
	WORD	size;
	BYTE	type;
	DWORD	dwVID;
	BYTE	bEmpire;
} TPacketGCChat;

typedef struct packet_whisper   // ���� ��Ŷ
{
    BYTE        bHeader;
    WORD        wSize;
    BYTE        bType;
    char        szNameFrom[CHARACTER_NAME_MAX_LEN + 1];
#ifdef ENABLE_MULTI_LANGUAGE_SYSTEM
    char		bLanguage[2 + 1];
    char		bEmpire[2 + 1];
#endif
} TPacketGCWhisper;

typedef struct packet_stun
{
	BYTE		header;
	DWORD		vid;
} TPacketGCStun;

#ifdef RENEWAL_DEAD_PACKET
enum EReviveTypes
{
	REVIVE_TYPE_HERE,
	REVIVE_TYPE_TOWN,
	REVIVE_TYPE_AUTO_TOWN,
	REVIVE_TYPE_MAX
};
#endif
typedef struct packet_dead
{
#ifdef RENEWAL_DEAD_PACKET
	packet_dead() { memset(&t_d, 0, sizeof(t_d)); }
#endif
	BYTE	header;
	DWORD	vid;
#ifdef RENEWAL_DEAD_PACKET
	BYTE	t_d[REVIVE_TYPE_MAX];
#endif
} TPacketGCDead;

#if defined(__BL_PARTY_POSITION__)
struct TPartyPosition
{
	DWORD	dwPID;
	long	lX;
	long	lY;
	float	fRot;
};

typedef struct SPacketGCPartyPosition
{
	BYTE bHeader;
	WORD wSize;
} TPacketGCPartyPosition;
#endif

#if defined(__BL_RANKING__)
enum ERANKINFO
{
	HEADER_GC_RANKING = 160,
};

typedef struct packet_ranking_send
{
	BYTE bHeader;
	WORD wSize;
} TPacketGCRankingSend;

typedef struct packet_ranking_info
{
	char	szName[CHARACTER_NAME_MAX_LEN + 1];
	int		iLevel;
	BYTE	bJob;
	BYTE	bEmpire;
	char	szGuildName[GUILD_NAME_MAX_LEN + 1];
} TPacketGCRankingInfo;
#endif

#if defined(GUILD_RANK_SYSTEM)
enum GUILD_RANK
{
	SUBHEADER_GUILD_RANKING_SEND,
	SUBHEADER_GUILD_RANKING_OPEN,
	HEADER_GC_GUILD_RANKING = 161,
};
typedef struct packet_guild_ranking_send
{
	BYTE	header;
	BYTE	subheader;
	int	id;
	char	szGuildName[GUILD_NAME_MAX_LEN + 1];
	char	szOwnerName[CHARACTER_NAME_MAX_LEN + 1];
	int	level;
	int	point;
	int	win;
	int draw;
	int loss;
} TPacketGCGuildRankingSend;
#endif

typedef struct packet_main_character
{
    BYTE        header;
    DWORD       dwVID;
	WORD		wRaceNum;
    char        szName[CHARACTER_NAME_MAX_LEN + 1];
    long        lX, lY, lZ;
	BYTE		bySkillGroup;
} TPacketGCMainCharacter;

// SUPPORT_BGM
typedef struct packet_main_character2_empire
{
    BYTE        header;
    DWORD       dwVID;
	WORD		wRaceNum;
    char        szName[CHARACTER_NAME_MAX_LEN + 1];
    long        lX, lY, lZ;
	BYTE		byEmpire;
	BYTE		bySkillGroup;
} TPacketGCMainCharacter2_EMPIRE;

typedef struct packet_main_character3_bgm
{
	enum
	{
		MUSIC_NAME_MAX_LEN = 24,
	};
    BYTE        header;
    DWORD       dwVID;
	WORD		wRaceNum;
    char        szUserName[CHARACTER_NAME_MAX_LEN + 1];
	char        szBGMName[MUSIC_NAME_MAX_LEN + 1];
    long        lX, lY, lZ;
	BYTE		byEmpire;
	BYTE		bySkillGroup;
} TPacketGCMainCharacter3_BGM;

typedef struct packet_main_character4_bgm_vol
{
	enum
	{
		MUSIC_NAME_MAX_LEN = 24,
	};
    BYTE        header;
    DWORD       dwVID;
	WORD		wRaceNum;
    char        szUserName[CHARACTER_NAME_MAX_LEN + 1];
	char        szBGMName[MUSIC_NAME_MAX_LEN + 1];
	float		fBGMVol;
    long        lX, lY, lZ;
	BYTE		byEmpire;
	BYTE		bySkillGroup;
} TPacketGCMainCharacter4_BGM_VOL;
// END_OF_SUPPORT_BGM

enum EPointTypes
{
    POINT_NONE,                 // 0
    POINT_LEVEL,                // 1
    POINT_VOICE,                // 2
    POINT_EXP,                  // 3
    POINT_NEXT_EXP,             // 4
    POINT_HP,                   // 5
    POINT_MAX_HP,               // 6
    POINT_SP,                   // 7
    POINT_MAX_SP,               // 8
    POINT_STAMINA,              // 9  ���׹̳�
    POINT_MAX_STAMINA,          // 10 �ִ� ���׹̳�

    POINT_GOLD,                 // 11
    POINT_ST,                   // 12 �ٷ�
    POINT_HT,                   // 13 ü��
    POINT_DX,                   // 14 ��ø��
    POINT_IQ,                   // 15 ���ŷ�
    POINT_ATT_POWER,            // 16 ���ݷ�
    POINT_ATT_SPEED,            // 17 ���ݼӵ�
    POINT_EVADE_RATE,           // 18 ȸ����
    POINT_MOV_SPEED,            // 19 �̵��ӵ�
    POINT_DEF_GRADE,            // 20 �����?
	POINT_CASTING_SPEED,        // 21 �ֹ��ӵ� (��ٿ�Ÿ��?*100) / (100 + �̰�) = ���� ��ٿ�? Ÿ��
	POINT_MAGIC_ATT_GRADE,      // 22 �������ݷ�
    POINT_MAGIC_DEF_GRADE,      // 23 ��������
    POINT_EMPIRE_POINT,         // 24 ��������
    POINT_LEVEL_STEP,           // 25 �� ���������� �ܰ�.. (1 2 3 �� �� ����, 4 �Ǹ� ���� ��)
    POINT_STAT,                 // 26 �ɷ�ġ �ø� �� �ִ� ����
	POINT_SUB_SKILL,            // 27 ���� ��ų ����Ʈ
	POINT_SKILL,                // 28 ��Ƽ�� ��ų ����Ʈ
//    POINT_SKILL_PASV,           // 27 �нú� ���? �ø� �� �ִ� ����
//    POINT_SKILL_ACTIVE,         // 28 ��Ƽ�� ��ų ����Ʈ
	POINT_MIN_ATK,				// 29 �ּ� �ı���
	POINT_MAX_ATK,				// 30 �ִ� �ı���
    POINT_PLAYTIME,             // 31 �÷��̽ð�
    POINT_HP_REGEN,             // 32 HP ȸ����
    POINT_SP_REGEN,             // 33 SP ȸ����

    POINT_BOW_DISTANCE,         // 34 Ȱ �����Ÿ� ����ġ (meter)

    POINT_HP_RECOVERY,          // 35 ü�� ȸ�� ������
    POINT_SP_RECOVERY,          // 36 ���ŷ� ȸ�� ������

    POINT_POISON_PCT,           // 37 �� Ȯ��
    POINT_STUN_PCT,             // 38 ���� Ȯ��
    POINT_SLOW_PCT,             // 39 ���ο� Ȯ��
    POINT_CRITICAL_PCT,         // 40 ũ��Ƽ�� Ȯ��
    POINT_PENETRATE_PCT,        // 41 ����Ÿ�� Ȯ��
    POINT_CURSE_PCT,            // 42 ���� Ȯ��

    POINT_ATTBONUS_HUMAN,       // 43 �ΰ����� ����
    POINT_ATTBONUS_ANIMAL,      // 44 �������� ������ % ����
    POINT_ATTBONUS_ORC,         // 45 ���Ϳ��� ������ % ����
    POINT_ATTBONUS_MILGYO,      // 46 �б����� ������ % ����
    POINT_ATTBONUS_UNDEAD,      // 47 ��ü���� ������ % ����
    POINT_ATTBONUS_DEVIL,       // 48 ����(�Ǹ�)���� ������ % ����
    POINT_ATTBONUS_INSECT,      // 49 ������
    POINT_ATTBONUS_FIRE,        // 50 ȭ����
    POINT_ATTBONUS_ICE,         // 51 ������
    POINT_ATTBONUS_DESERT,      // 52 �縷��
    POINT_ATTBONUS_UNUSED0,     // 53 UNUSED0
    POINT_ATTBONUS_UNUSED1,     // 54 UNUSED1
    POINT_ATTBONUS_UNUSED2,     // 55 UNUSED2
    POINT_ATTBONUS_UNUSED3,     // 56 UNUSED3
    POINT_ATTBONUS_UNUSED4,     // 57 UNUSED4
    POINT_ATTBONUS_UNUSED5,     // 58 UNUSED5
    POINT_ATTBONUS_UNUSED6,     // 59 UNUSED6
    POINT_ATTBONUS_UNUSED7,     // 60 UNUSED7
    POINT_ATTBONUS_UNUSED8,     // 61 UNUSED8
    POINT_ATTBONUS_UNUSED9,     // 62 UNUSED9

    POINT_STEAL_HP,             // 63 ������ ����
    POINT_STEAL_SP,             // 64 ���ŷ� ����

    POINT_MANA_BURN_PCT,        // 65 ���� ��

    /// ���ؽ� ���ʽ� ///

    POINT_DAMAGE_SP_RECOVER,    // 66 ���ݴ��� �� ���ŷ� ȸ�� Ȯ��

    POINT_BLOCK,                // 67 ������
    POINT_DODGE,                // 68 ȸ����

    POINT_RESIST_SWORD,         // 69
    POINT_RESIST_TWOHAND,       // 70
    POINT_RESIST_DAGGER,        // 71
    POINT_RESIST_BELL,          // 72
    POINT_RESIST_FAN,           // 73
    POINT_RESIST_BOW,           // 74  ȭ��   ����   : �����? ����
    POINT_RESIST_FIRE,          // 75  ȭ��   ����   : ȭ�����ݿ� ���� �����? ����
    POINT_RESIST_ELEC,          // 76  ����   ����   : ������ݿ�? ���� �����? ����
    POINT_RESIST_MAGIC,         // 77  ����   ����   : �������� ���� �����? ����
    POINT_RESIST_WIND,          // 78  �ٶ�   ����   : �ٶ����ݿ� ���� �����? ����

    POINT_REFLECT_MELEE,        // 79 ���� �ݻ�

    /// Ư�� ���ؽ� ///
    POINT_REFLECT_CURSE,        // 80 ���� �ݻ�
    POINT_POISON_REDUCE,        // 81 �������� ����

    /// �� �Ҹ��? ///
    POINT_KILL_SP_RECOVER,      // 82 �� �Ҹ��? MP ȸ��
    POINT_EXP_DOUBLE_BONUS,     // 83
    POINT_GOLD_DOUBLE_BONUS,    // 84
    POINT_ITEM_DROP_BONUS,      // 85

    /// ȸ�� ���� ///
    POINT_POTION_BONUS,         // 86
    POINT_KILL_HP_RECOVER,      // 87

    POINT_IMMUNE_STUN,          // 88
    POINT_IMMUNE_SLOW,          // 89
    POINT_IMMUNE_FALL,          // 90
    //////////////////

    POINT_PARTY_ATT_GRADE,      // 91
    POINT_PARTY_DEF_GRADE,      // 92

    POINT_ATT_BONUS,            // 93
    POINT_DEF_BONUS,            // 94

    POINT_ATT_GRADE_BONUS,			// 95
    POINT_DEF_GRADE_BONUS,			// 96
    POINT_MAGIC_ATT_GRADE_BONUS,	// 97
    POINT_MAGIC_DEF_GRADE_BONUS,	// 98

    POINT_RESIST_NORMAL_DAMAGE,		// 99

	POINT_STAT_RESET_COUNT = 112,
    POINT_HORSE_SKILL = 113,

	POINT_MALL_ATTBONUS,		// 114 ���ݷ� +x%
	POINT_MALL_DEFBONUS,		// 115 ���� +x%
	POINT_MALL_EXPBONUS,		// 116 ����ġ +x%
	POINT_MALL_ITEMBONUS,		// 117 ������ �����? x/10��
	POINT_MALL_GOLDBONUS,		// 118 �� �����? x/10��
    POINT_MAX_HP_PCT,			// 119 �ִ������? +x%
    POINT_MAX_SP_PCT,			// 120 �ִ����ŷ� +x%

	POINT_SKILL_DAMAGE_BONUS,       // 121 ��ų ������ *(100+x)%
	POINT_NORMAL_HIT_DAMAGE_BONUS,  // 122 ��Ÿ ������ *(100+x)%

    POINT_SKILL_DEFEND_BONUS,       // 123 ��ų ���? ������
    POINT_NORMAL_HIT_DEFEND_BONUS,  // 124 ��Ÿ ���? ������
    POINT_PC_BANG_EXP_BONUS,        // 125
	POINT_PC_BANG_DROP_BONUS,       // 126 PC�� ���� ��ӷ�? ���ʽ�

	POINT_ENERGY = 128,				// 128 ���?

	// ���? ui ��.
	// �̷��� �ϰ� ���� �ʾ�����,
	// uiTaskBar������ affect�� ������ �� ����,
	// ������ Ŭ�󸮾�Ʈ������ blend_affect�� �������� �ʾ�,
	// �ӽ÷� �̷��� �д�.
	POINT_ENERGY_END_TIME = 129,	// 129 ���? ���� �ð�
#ifdef ENABLE_SPECIAL_STATS_SYSTEM
	POINT_SPECIAL_STAT1 = 144,  //Fortuna
	POINT_SPECIAL_STAT2,  //Agilita'
	POINT_SPECIAL_STAT3,  //Carisma
	POINT_SPECIAL_STAT4,  //Tenacia
	POINT_SPECIAL_STAT5,  //Avidit?	POINT_SPECIAL_STAT6,  //Talento
	POINT_SPECIAL_STAT_EFFECT,
#endif
#ifdef ENABLE_MAGIC_REDUCTION_SYSTEM
	POINT_RESIST_MAGIC_REDUCTION,
#endif
#ifdef ENABLE_NEW_TALISMAN_GF
	POINT_ATTBONUS_ELEC = 155,
	POINT_ATTBONUS_WIND = 156,
	POINT_ATTBONUS_EARTH = 157,
	POINT_ATTBONUS_DARK = 158,
	POINT_RESIST_HUMAN,

	POINT_RESIST_SWORD_REDUCTION,
	POINT_RESIST_TWOHAND_REDUCTION,
	POINT_RESIST_DAGGER_REDUCTION,
	POINT_RESIST_BELL_REDUCTION,
	POINT_RESIST_FAN_REDUCTION,
	POINT_RESIST_BOW_REDUCTION,
	POINT_ATTBONUS_ZODIAC,
#endif
#ifdef NEW_ADD_INVENTORY
	POINT_BLACK = 190,
#endif
	// Ŭ���̾�Ʈ ����Ʈ
	POINT_MIN_WEP = 200,
	POINT_MAX_WEP,
	POINT_MIN_MAGIC_WEP,
	POINT_MAX_MAGIC_WEP,
	POINT_HIT_RATE,
#ifdef ENABLE_CHEQUE_SYSTEM
	POINT_CHEQUE = 207,
#endif
#ifdef ENABLE_BONUS_BOSS
	POINT_ATTBONUS_BOSS = 138,
#endif
#ifdef ENABLE_BONUS_METIN
		POINT_ATTBONUS_METIN = 139,
#endif
    //POINT_MAX_NUM = 255,=>stdafx.h ��/
};

typedef struct packet_points
{
    BYTE        header;
    long        points[POINT_MAX_NUM];
} TPacketGCPoints;

typedef struct packet_point_change
{
    int         header;

	DWORD		dwVID;
	BYTE		Type;

	long        amount; // �ٲ� ��
    long        value;  // ���� ��
} TPacketGCPointChange;

typedef struct packet_motion
{
	BYTE		header;
	DWORD		vid;
	DWORD		victim_vid;
	WORD		motion;
} TPacketGCMotion;

#if defined(GAIDEN)
struct TPacketGCItemDelDeprecate
{
    TPacketGCItemDelDeprecate() :
        header(HEADER_GC_ITEM_DEL),
        pos(0),
        vnum(0),
        count(0)
    {
        memset(&alSockets, 0, sizeof(alSockets));
        memset(&aAttr, 0, sizeof(aAttr));
    }

    BYTE    header;
    BYTE    pos;
    DWORD   vnum;
    BYTE    count;
    long    alSockets[ITEM_SOCKET_SLOT_MAX_NUM];
    TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_SLOT_MAX_NUM];
};

typedef struct packet_set_item
{
	BYTE		header;
	BYTE		pos;
	DWORD		vnum;
	BYTE		count;
	DWORD		flags;	// �÷��� �߰�
	long		alSockets[ITEM_SOCKET_SLOT_MAX_NUM];
    TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_SLOT_MAX_NUM];
} TPacketGCItemSet;

typedef struct packet_item_del
{
    BYTE        header;
    BYTE        pos;
} TPacketGCItemDel;
#else
typedef struct packet_set_item
{
	BYTE		header;
	TItemPos	Cell;
	DWORD		vnum;
	short		count;
#ifdef ENABLE_CHANGELOOK_SYSTEM
	DWORD	transmutation;
#endif
	long		alSockets[ITEM_SOCKET_SLOT_MAX_NUM];
    TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_SLOT_MAX_NUM];
} TPacketGCItemSet;

typedef struct packet_set_item2
{
	BYTE		header;
	TItemPos	Cell;
	DWORD		vnum;
	short		count;
#ifdef ENABLE_CHANGELOOK_SYSTEM
	DWORD		transmutation;
#endif
	DWORD		flags;	// �÷��� �߰�
	DWORD		anti_flags;	// �÷��� �߰�
	bool		highlight;
	long		alSockets[ITEM_SOCKET_SLOT_MAX_NUM];
    TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_SLOT_MAX_NUM];
} TPacketGCItemSet2;
#endif

#if defined(__BL_MAILBOX__)
typedef struct packet_mailbox_process
{
	packet_mailbox_process()
		: bArg1(0), bArg2(0) {}
	BYTE							bHeader;
	BYTE							bSubHeader;
	BYTE							bArg1;
	BYTE							bArg2;
} TPacketMailboxProcess;

typedef struct SMailBoxRespondUnreadData
{
	BYTE							bHeader;
	BYTE							bItemMessageCount;
	BYTE							bCommonMessageCount;
	bool							bGMVisible;
} TMailBoxRespondUnreadData;

typedef struct packet_mailbox_process_all
{
	BYTE							Index;
} TPacketGCMailboxProcessAll;

typedef struct packet_mailbox_add_data
{
	BYTE							bHeader;
	BYTE							Index;
	char							szFrom[CHARACTER_NAME_MAX_LEN + 1];
	char							szMessage[100 + 1];
	int								iYang;
	int								iWon;
	DWORD							ItemVnum;
	DWORD							ItemCount;
	long							alSockets[ITEM_SOCKET_SLOT_MAX_NUM];
	TPlayerItemAttribute			aAttr[ITEM_ATTRIBUTE_SLOT_MAX_NUM];
#if defined(__BL_TRANSMUTATION__)
	DWORD							dwTransmutationVnum;
#endif
} TPacketGCMailBoxAddData;

typedef struct packet_mailbox_message
{
	time_t							SendTime;
	time_t							DeleteTime;
	char							szTitle[25 + 1];
	bool							bIsGMPost;
	bool							bIsItemExist;
	bool							bIsConfirm;
} TPacketGCMailBoxMessage;

typedef struct packet_mailbox
{
	BYTE							bHeader;
	WORD							wSize;
} TPacketGCMailBox;

typedef struct packet_mailbox_write
{
	BYTE							bHeader;
	char							szName[CHARACTER_NAME_MAX_LEN + 1];
	char							szTitle[25 + 1];
	char							szMessage[100 + 1];
	TItemPos						pos;
	int								iYang;
	int								iWon;
} TPacketCGMailboxWrite;

typedef struct packet_mailbox_write_confirm
{
	BYTE							bHeader;
	char							szName[CHARACTER_NAME_MAX_LEN + 1];
} TPacketCGMailboxWriteConfirm;
#endif

typedef struct packet_item_del
{
    BYTE        header;
    BYTE        pos;
} TPacketGCItemDel;

typedef struct packet_use_item
{
	BYTE		header;
	TItemPos	Cell;
	DWORD		ch_vid;
	DWORD		victim_vid;

	DWORD		vnum;
} TPacketGCItemUse;

typedef struct packet_update_item
{
	BYTE		header;
	TItemPos	Cell;
	short		count;
#ifdef ENABLE_CHANGELOOK_SYSTEM
	DWORD	transmutation;
#endif
	long		alSockets[ITEM_SOCKET_SLOT_MAX_NUM];
    TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_SLOT_MAX_NUM];
} TPacketGCItemUpdate;

typedef struct packet_ground_add_item
{
    BYTE        bHeader;
    long        lX;
	long		lY;
	long		lZ;

    DWORD       dwVID;
    DWORD       dwVnum;
} TPacketGCItemGroundAdd;

typedef struct packet_ground_del_item
{
	BYTE		header;
	DWORD		vid;
} TPacketGCItemGroundDel;

typedef struct packet_item_ownership
{
    BYTE        bHeader;
    DWORD       dwVID;
    char        szName[CHARACTER_NAME_MAX_LEN + 1];
} TPacketGCItemOwnership;

#if defined(__BL_DROP_DESTROY_TIME__)
typedef struct packet_item_destroy_time
{
	BYTE	bHeader;
	DWORD	dwVID;
	time_t	tDestroyTime;
} TPacketGCItemDestroyTime;
#endif

typedef struct packet_quickslot_add
{
    BYTE        header;
    BYTE        pos;
	TQuickSlot	slot;
} TPacketGCQuickSlotAdd;

typedef struct packet_quickslot_del
{
    BYTE        header;
    BYTE        pos;
} TPacketGCQuickSlotDel;

typedef struct packet_quickslot_swap
{
    BYTE        header;
    BYTE        pos;
    BYTE        change_pos;
} TPacketGCQuickSlotSwap;

typedef struct packet_shop_start
{
	struct packet_shop_item		items[SHOP_HOST_ITEM_MAX_NUM];
} TPacketGCShopStart;

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
typedef struct packet_offline_shop_start
{
	struct packet_shop_offline_item		items[OFFLINE_SHOP_HOST_ITEM_MAX_NUM];
	DWORD	m_dwDisplayedCount;
	DWORD	dwRemainTime;
	int		map, x, y;
} TPacketGCOfflineShopStart;

typedef struct packet_shop_update_offline_item
{
	BYTE								pos;
	struct packet_shop_offline_item		item;
} TPacketGCShopUpdateOfflineShopItem;

typedef struct packet_offline_shop_money
{
	long long	llMoney;

#ifdef ENABLE_CHEQUE_SYSTEM
	int		bCheque;
#endif
} TPacketGCOfflineShopMoney;
#endif

typedef struct packet_shop_start_ex // ������ TSubPacketShopTab* shop_tabs �� �����?.
{
	typedef struct sub_packet_shop_tab
	{
		char name[SHOP_TAB_NAME_MAX];
		BYTE coin_type;
		packet_shop_item items[SHOP_HOST_ITEM_MAX_NUM];
	} TSubPacketShopTab;
	DWORD owner_vid;
	BYTE shop_tab_count;
} TPacketGCShopStartEx;


typedef struct packet_shop_update_item
{
	BYTE						pos;
	struct packet_shop_item		item;
} TPacketGCShopUpdateItem;

typedef struct packet_shop_update_price
{
	int iElkAmount;
} TPacketGCShopUpdatePrice;

enum EPacketShopSubHeaders
{
	SHOP_SUBHEADER_GC_START,
    SHOP_SUBHEADER_GC_END,
	SHOP_SUBHEADER_GC_UPDATE_ITEM,
	SHOP_SUBHEADER_GC_UPDATE_PRICE,
    SHOP_SUBHEADER_GC_OK,
    SHOP_SUBHEADER_GC_NOT_ENOUGH_MONEY,
    SHOP_SUBHEADER_GC_SOLDOUT,
    SHOP_SUBHEADER_GC_INVENTORY_FULL,
    SHOP_SUBHEADER_GC_INVALID_POS,
	SHOP_SUBHEADER_GC_SOLD_OUT,
	SHOP_SUBHEADER_GC_START_EX,
	SHOP_SUBHEADER_GC_NOT_ENOUGH_MONEY_EX,
#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	SHOP_SUBHEADER_CG_REFRESH_OFFLINE_SHOP_MONEY,
	SHOP_SUBHEADER_GC_OFFLINE_SHOP_INFO,
#endif
#ifdef ENABLE_MULTISHOP
	SHOP_SUBHEADER_GC_NOT_ENOUGH_ITEM,
#endif	
};

typedef struct packet_shop
{
	BYTE        header;
	WORD		size;
	BYTE        subheader;
} TPacketGCShop;

typedef struct packet_exchange
{
	BYTE        header;
	BYTE        subheader;
	BYTE        is_me;
	DWORD       arg1;
	TItemPos       arg2;
	DWORD       arg3;
#ifdef WJ_ENABLE_TRADABLE_ICON
	TItemPos       arg4;
#endif
	long		alValues[ITEM_SOCKET_SLOT_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_SLOT_MAX_NUM];
#ifdef ENABLE_CHANGELOOK_SYSTEM
	DWORD	dwTransmutation;
#endif
} TPacketGCExchange;

enum
{
    EXCHANGE_SUBHEADER_GC_START,			// arg1 == vid
    EXCHANGE_SUBHEADER_GC_ITEM_ADD,		// arg1 == vnum  arg2 == pos  arg3 == count
	EXCHANGE_SUBHEADER_GC_ITEM_DEL,		// arg1 == pos
    EXCHANGE_SUBHEADER_GC_ELK_ADD,			// arg1 == elk
#ifdef ENABLE_CHEQUE_SYSTEM
	EXCHANGE_SUBHEADER_GC_CHEQUE_ADD,	/* arg1 == cheque */
#endif
    EXCHANGE_SUBHEADER_GC_ACCEPT,			// arg1 == accept
    EXCHANGE_SUBHEADER_GC_END,				// arg1 == not used
    EXCHANGE_SUBHEADER_GC_ALREADY,			// arg1 == not used
    EXCHANGE_SUBHEADER_GC_LESS_ELK,		// arg1 == not used
#ifdef ENABLE_CHEQUE_SYSTEM
	EXCHANGE_SUBHEADER_GC_LESS_CHEQUE,	/* arg1 == not used */
#endif
};

typedef struct packet_position
{
    BYTE        header;
	DWORD		vid;
    BYTE        position;
} TPacketGCPosition;

typedef struct packet_ping
{
	BYTE		header;
} TPacketGCPing;

typedef struct packet_pong
{
	BYTE		bHeader;
} TPacketCGPong;

typedef struct packet_script
{
    BYTE		header;
    WORD        size;
	BYTE		skin;
    WORD        src_size;
} TPacketGCScript;

typedef struct packet_target
{
    BYTE        header;
    DWORD       dwVID;
#if defined(ENABLE_SHIP_DEFENSE)
	bool bAlliance;
	int64_t iAllianceMinHP, iAllianceMaxHP;
#endif
    BYTE        bHPPercent;
#ifdef ELEMENT_TARGET
	BYTE 		bElement;
#endif
} TPacketGCTarget;

#ifdef TARGET_INFORMATION_SYSTEM
typedef struct packet_target_info
{
	BYTE	header;
	DWORD	dwVID;
	DWORD	race;
	DWORD	dwVnum;
	BYTE	count;
#ifdef ENABLE_SEND_TARGET_INFO_EXTENDED
	DWORD	rarity;
#endif
} TPacketGCTargetInfo;
typedef struct packet_target_info_load
{
	BYTE header;
	DWORD dwVID;
} TPacketCGTargetInfoLoad;
#endif

typedef struct packet_damage_info
{
	BYTE header;
	DWORD dwVictimVID;
	DWORD dwAttackerVID;
	BYTE flag;
	int  damage;
} TPacketGCDamageInfo;

typedef struct packet_mount
{
    BYTE        header;
    DWORD       vid;
    DWORD       mount_vid;
    BYTE        pos;
	DWORD		_x, _y;
} TPacketGCMount;

typedef struct packet_change_speed
{
	BYTE		header;
	DWORD		vid;
	WORD		moving_speed;
} TPacketGCChangeSpeed;

typedef struct packet_move
{
	BYTE		bHeader;
	BYTE		bFunc;
	BYTE		bArg;
	BYTE		bRot;
	DWORD		dwVID;
	LONG		lX;
	LONG		lY;
	DWORD		dwTime;
	DWORD		dwDuration;
} TPacketGCMove;

enum
{
	QUEST_SEND_IS_BEGIN         = 1 << 0,
    QUEST_SEND_TITLE            = 1 << 1,  // 28�� ����
    QUEST_SEND_CLOCK_NAME       = 1 << 2,  // 16�� ����
    QUEST_SEND_CLOCK_VALUE      = 1 << 3,
    QUEST_SEND_COUNTER_NAME     = 1 << 4,  // 16�� ����
    QUEST_SEND_COUNTER_VALUE    = 1 << 5,
	QUEST_SEND_ICON_FILE		= 1 << 6,  // 24�� ����
};

typedef struct packet_quest_info
{
	BYTE	header;
	WORD	size;
	WORD	index;
#ifdef ENABLE_QUEST_RENEWAL
	WORD c_index;
#endif
	BYTE	flag;
	char	szTitle[30 + 1];
	BYTE	isBegin;
	char	szClockName[16 + 1];
	int		iClockValue;
	char	szCounterName[16 + 1];
	int		iCounterValue;
	char	szIconFileName[24 + 1];
} TPacketGCQuestInfo;

typedef struct packet_quest_confirm
{
    BYTE header;
    char msg[64+1];
    long timeout;
    DWORD requestPID;
} TPacketGCQuestConfirm;

typedef struct packet_attack
{
    BYTE        header;
    DWORD       dwVID;
    DWORD       dwVictimVID;    // �� VID
    BYTE        bType;          // ���� ����
} TPacketGCAttack;

typedef struct packet_c2c
{
	BYTE		header;
	WORD		wSize;
} TPacketGCC2C;

typedef struct packetd_sync_position_element
{
    DWORD       dwVID;
    long        lX;
    long        lY;
} TPacketGCSyncPositionElement;

typedef struct packetd_sync_position
{
    BYTE        bHeader;
    WORD		wSize;
} TPacketGCSyncPosition;

typedef struct packet_ownership
{
    BYTE                bHeader;
    DWORD               dwOwnerVID;
    DWORD               dwVictimVID;
} TPacketGCOwnership;

#define	SKILL_MAX_NUM 255

typedef struct packet_skill_level
{
    BYTE        bHeader;
    BYTE        abSkillLevels[SKILL_MAX_NUM];
} TPacketGCSkillLevel;

typedef struct SPlayerSkill
{
	BYTE bMasterType;
	BYTE bLevel;
	time_t tNextRead;
} TPlayerSkill;

typedef struct packet_skill_level_new
{
	BYTE bHeader;
	TPlayerSkill skills[SKILL_MAX_NUM];
} TPacketGCSkillLevelNew;

// fly
typedef struct packet_fly
{
    BYTE        bHeader;
    BYTE        bType;
    DWORD       dwStartVID;
    DWORD       dwEndVID;
} TPacketGCCreateFly;

enum EPVPModes
{
	PVP_MODE_NONE,
    PVP_MODE_AGREE,
    PVP_MODE_FIGHT,
    PVP_MODE_REVENGE,
};

typedef struct packet_duel_start
{
    BYTE	header ;
    WORD	wSize ;	// DWORD�� �? ���� = (wSize - sizeof(TPacketGCPVPList)) / 4
} TPacketGCDuelStart ;

typedef struct packet_pvp
{
	BYTE		header;
	DWORD		dwVIDSrc;
	DWORD		dwVIDDst;
	BYTE		bMode;
#ifdef ___DECORUM___
	bool		bIsDecored;
#endif	
} TPacketGCPVP;

typedef struct packet_skill_cooltime_end
{
	BYTE		header;
	BYTE		bSkill;
} TPacketGCSkillCoolTimeEnd;

typedef struct packet_warp
{
	BYTE			bHeader;
	LONG			lX;
	LONG			lY;
	LONG			lAddr;
	WORD			wPort;
} TPacketGCWarp;

typedef struct packet_party_invite
{
    BYTE header;
    DWORD leader_pid;
} TPacketGCPartyInvite;

typedef struct packet_party_add
{
    BYTE header;
    DWORD pid;
    char name[CHARACTER_NAME_MAX_LEN+1];
} TPacketGCPartyAdd;

typedef struct packet_party_update
{
    BYTE header;
    DWORD pid;
    BYTE state;
    BYTE percent_hp;
	BYTE percent_sp;
    short affects[PARTY_AFFECT_SLOT_MAX_NUM];
} TPacketGCPartyUpdate;

typedef struct packet_party_remove
{
    BYTE header;
    DWORD pid;
} TPacketGCPartyRemove;

typedef TPacketCGSafeboxCheckout TPacketGCSafeboxCheckout;
typedef TPacketCGSafeboxCheckin TPacketGCSafeboxCheckin;

typedef struct packet_safebox_wrong_password
{
    BYTE        bHeader;
} TPacketGCSafeboxWrongPassword;

typedef struct packet_safebox_size
{
	BYTE bHeader;
	BYTE bSize;
} TPacketGCSafeboxSize;

typedef struct packet_safebox_money_change
{
    BYTE bHeader;
    DWORD dwMoney;
} TPacketGCSafeboxMoneyChange;

typedef struct command_empire
{
    BYTE        bHeader;
    BYTE        bEmpire;
} TPacketCGEmpire;

typedef struct packet_empire
{
    BYTE        bHeader;
    BYTE        bEmpire;
} TPacketGCEmpire;

enum
{
	FISHING_SUBHEADER_GC_START,
	FISHING_SUBHEADER_GC_STOP,
	FISHING_SUBHEADER_GC_REACT,
	FISHING_SUBHEADER_GC_SUCCESS,
	FISHING_SUBHEADER_GC_FAIL,
    FISHING_SUBHEADER_GC_FISH,
};

typedef struct packet_fishing
{
    BYTE header;
    BYTE subheader;
    DWORD info;
    BYTE dir;
} TPacketGCFishing;

typedef struct paryt_parameter
{
    BYTE        bHeader;
    BYTE        bDistributeMode;
} TPacketGCPartyParameter;

//////////////////////////////////////////////////////////////////////////
// Guild

enum
{
    GUILD_SUBHEADER_GC_LOGIN,
	GUILD_SUBHEADER_GC_LOGOUT,
	GUILD_SUBHEADER_GC_LIST,
	GUILD_SUBHEADER_GC_GRADE,
	GUILD_SUBHEADER_GC_ADD,
	GUILD_SUBHEADER_GC_REMOVE,
	GUILD_SUBHEADER_GC_GRADE_NAME,
	GUILD_SUBHEADER_GC_GRADE_AUTH,
	GUILD_SUBHEADER_GC_INFO,
	GUILD_SUBHEADER_GC_COMMENTS,
    GUILD_SUBHEADER_GC_CHANGE_EXP,
    GUILD_SUBHEADER_GC_CHANGE_MEMBER_GRADE,
	GUILD_SUBHEADER_GC_SKILL_INFO,
	GUILD_SUBHEADER_GC_CHANGE_MEMBER_GENERAL,
	GUILD_SUBHEADER_GC_GUILD_INVITE,
    GUILD_SUBHEADER_GC_WAR,
    GUILD_SUBHEADER_GC_GUILD_NAME,
    GUILD_SUBHEADER_GC_GUILD_WAR_LIST,
    GUILD_SUBHEADER_GC_GUILD_WAR_END_LIST,
    GUILD_SUBHEADER_GC_WAR_POINT,
	GUILD_SUBHEADER_GC_MONEY_CHANGE,
};

typedef struct packet_guild
{
    BYTE header;
    WORD size;
    BYTE subheader;
} TPacketGCGuild;

// SubHeader - Grade
enum
{
    GUILD_AUTH_ADD_MEMBER       = (1 << 0),
    GUILD_AUTH_REMOVE_MEMBER    = (1 << 1),
    GUILD_AUTH_NOTICE           = (1 << 2),
    GUILD_AUTH_SKILL            = (1 << 3),
};

typedef struct packet_guild_sub_grade
{
	char grade_name[GUILD_GRADE_NAME_MAX_LEN+1]; // 8+1 �����?, ���� ���� �̸�
	BYTE auth_flag;
} TPacketGCGuildSubGrade;

typedef struct packet_guild_sub_member
{
	DWORD pid;
	BYTE byGrade;
	BYTE byIsGeneral;
	BYTE byJob;
	BYTE byLevel;
	DWORD dwOffer;
	BYTE byNameFlag;
	// if NameFlag is TRUE, name is sent from server.
	//    char szName[CHARACTER_ME_MAX_LEN+1];
} TPacketGCGuildSubMember;

typedef struct packet_guild_sub_info
{
    WORD member_count;
    WORD max_member_count;
	DWORD guild_id;
    DWORD master_pid;
    DWORD exp;
    BYTE level;
    char name[GUILD_NAME_MAX_LEN+1];
	DWORD gold;
	BYTE hasLand;

} TPacketGCGuildInfo;

enum EGuildWarState
{
    GUILD_WAR_NONE,
    GUILD_WAR_SEND_DECLARE,
    GUILD_WAR_REFUSE,
    GUILD_WAR_RECV_DECLARE,
    GUILD_WAR_WAIT_START,
    GUILD_WAR_CANCEL,
    GUILD_WAR_ON_WAR,
    GUILD_WAR_END,

    GUILD_WAR_DURATION = 2*60*60, // 2�ð�
};

typedef struct packet_guild_war
{
	DWORD       dwGuildSelf;
	DWORD       dwGuildOpp;
	BYTE        bType;
	BYTE        bWarState;
#ifdef __IMPROVED_GUILD_WAR__
	int		iMaxPlayer;
	int		iMaxScore;
	DWORD	flags;
	int		custom_map_index;
#endif
} TPacketGCGuildWar;


typedef struct SPacketGuildWarPoint
{
    DWORD dwGainGuildID;
    DWORD dwOpponentGuildID;
    long lPoint;
} TPacketGuildWarPoint;

// SubHeader - Dungeon
enum
{
	DUNGEON_SUBHEADER_GC_TIME_ATTACK_START = 0,
	DUNGEON_SUBHEADER_GC_DESTINATION_POSITION = 1,
};

typedef struct packet_dungeon
{
	BYTE		bHeader;
    WORD		size;
    BYTE		subheader;
} TPacketGCDungeon;

// Private Shop
typedef struct SPacketGCShopSign
{
    BYTE        bHeader;
    DWORD       dwVID;
    char        szSign[SHOP_SIGN_MAX_LEN + 1];
} TPacketGCShopSign;

typedef struct SPacketGCTime
{
    BYTE        bHeader;
    time_t      time;
} TPacketGCTime;

enum
{
    WALKMODE_RUN,
    WALKMODE_WALK,
};

typedef struct SPacketGCWalkMode
{
    BYTE        header;
    DWORD       vid;
    BYTE        mode;
} TPacketGCWalkMode;

typedef struct SPacketGCChangeSkillGroup
{
    BYTE        header;
    BYTE        skill_group;
} TPacketGCChangeSkillGroup;

struct TMaterial
{
    DWORD vnum;
    DWORD count;
};

typedef struct SRefineTable
{
    DWORD src_vnum;
    DWORD result_vnum;
    short material_count;
    int cost; // �ҿ� ���?
    int prob; // Ȯ��
    TMaterial materials[REFINE_MATERIAL_MAX_NUM];
} TRefineTable;

typedef struct SPacketGCRefineInformation
{
	BYTE			header;
	BYTE			pos;
	TRefineTable	refine_table;
} TPacketGCRefineInformation;

typedef struct SPacketGCRefineInformationNew
{
	BYTE			header;
	BYTE			type;
	BYTE			pos;
	TRefineTable	refine_table;
} TPacketGCRefineInformationNew;

enum SPECIAL_EFFECT
{
	SE_NONE,
	SE_HPUP_RED,
	SE_SPUP_BLUE,
	SE_SPEEDUP_GREEN,
	SE_DXUP_PURPLE,
	SE_CRITICAL,
	SE_PENETRATE,
	SE_BLOCK,
	SE_DODGE,
	SE_CHINA_FIREWORK,
	SE_SPIN_TOP,
	SE_SUCCESS,
	SE_FAIL,
	SE_FR_SUCCESS,
	SE_LEVELUP_ON_14_FOR_GERMANY,	//������ 14�϶� ( �������� )
	SE_LEVELUP_UNDER_15_FOR_GERMANY,//������ 15�϶� ( �������� )
	SE_PERCENT_DAMAGE1,
	SE_PERCENT_DAMAGE2,
	SE_PERCENT_DAMAGE3,
	SE_AUTO_HPUP,
	SE_AUTO_SPUP,
	SE_EQUIP_RAMADAN_RING,			// �ʽ´��� ������ �����ϴ� ������ �ߵ��ϴ� ����Ʈ
	SE_EQUIP_HALLOWEEN_CANDY,		// �ҷ��� ������ ����(-_-;)�� ������ �ߵ��ϴ� ����Ʈ
	SE_EQUIP_HAPPINESS_RING,		// ũ�������� �ູ�� ������ �����ϴ� ������ �ߵ��ϴ� ����Ʈ
	SE_EQUIP_LOVE_PENDANT,		// �߷�Ÿ�� �����? �Ҵ�Ʈ(71145) ������ �� ����Ʈ (�ߵ�����Ʈ��, ��������Ʈ �ƴ�)
#ifdef ENABLE_DUEL_EFFECT
	SE_PVP_WIN,
	SE_PVP_OPEN1,
	SE_PVP_OPEN2,
	SE_PVP_BEGIN1,
	SE_PVP_BEGIN2,
#endif
#ifdef ENABLE_AGGREGATE_MONSTER_EFFECT
	SE_AGGREGATE_MONSTER_EFFECT,
#endif
#ifdef ENABLE_ACCE_COSTUME_SYSTEM
	SE_EFFECT_ACCE_SUCCEDED,
	SE_EFFECT_ACCE_EQUIP,
#endif

#ifdef ENABLE_OVER_KILL
	SE_OVER_KILL_START,
	SE_OVER_KILL_END = SE_OVER_KILL_START + 4,
#endif
#ifdef ENABLE_TALISMAN_EFFECT
	SE_EFFECT_TALISMAN_EQUIP_FIRE,
	SE_EFFECT_TALISMAN_EQUIP_ICE,
	SE_EFFECT_TALISMAN_EQUIP_WIND,
	SE_EFFECT_TALISMAN_EQUIP_EARTH,
	SE_EFFECT_TALISMAN_EQUIP_DARK,
	SE_EFFECT_TALISMAN_EQUIP_ELEC,
#endif
#ifdef ENABLE_SYSTEM_RUNE
	SE_RUNA_WHITE_EFFECT,
	SE_RUNA_RED_EFFECT,
	SE_RUNA_BLUE_EFFECT,
	SE_RUNA_YELLOW_EFFECT,
	SE_RUNA_BLACK_EFFECT,
	SE_RUNA_GREEN_EFFECT,
#endif
};

typedef struct SPacketGCSpecialEffect
{
    BYTE header;
    BYTE type;
    DWORD vid;
} TPacketGCSpecialEffect;

typedef struct SPacketGCNPCPosition
{
    BYTE header;
	WORD size;
    WORD count;
} TPacketGCNPCPosition;

struct TNPCPosition
{
    BYTE bType;
    char name[CHARACTER_NAME_MAX_LEN+1];
    long x;
    long y;
};

typedef struct SPacketGCChangeName
{
    BYTE header;
    DWORD pid;
    char name[CHARACTER_NAME_MAX_LEN+1];
} TPacketGCChangeName;

enum EBlockAction
{
    BLOCK_EXCHANGE              = (1 << 0),
    BLOCK_PARTY_INVITE          = (1 << 1),
    BLOCK_GUILD_INVITE          = (1 << 2),
    BLOCK_WHISPER               = (1 << 3),
    BLOCK_MESSENGER_INVITE      = (1 << 4),
    BLOCK_PARTY_REQUEST         = (1 << 5),
};

typedef struct packet_china_matrixd_card
{
	BYTE	bHeader;
	DWORD	dwRows;
	DWORD	dwCols;
} TPacketGCChinaMatrixCard;

typedef struct packet_runup_matrixd_quiz
{
	BYTE	bHeader;
	char	szQuiz[RUNUP_MATRIX_QUIZ_MAX_LEN + 1];
} TPacketGCRunupMatrixQuiz;

typedef struct packet_newcibn_passpod_request
{
	BYTE	bHeader;
} TPacketGCNEWCIBNPasspodRequest;

typedef struct packet_newcibn_passpod_failure
{
	BYTE	bHeader;
	char	szMessage[NEWCIBN_PASSPOD_FAILURE_MAX_LEN + 1];
} TPacketGCNEWCIBNPasspodFailure;

typedef struct packet_login_key
{
	BYTE	bHeader;
	DWORD	dwLoginKey;
} TPacketGCLoginKey;

typedef struct packet_auth_success
{
    BYTE        bHeader;
    DWORD       dwLoginKey;
    BYTE        bResult;
} TPacketGCAuthSuccess;

typedef struct packet_auth_success_openid
{
    BYTE        bHeader;
    DWORD       dwLoginKey;
    BYTE        bResult;
	char		login[ID_MAX_NUM + 1];
} TPacketGCAuthSuccessOpenID;

typedef struct packet_channel
{
    BYTE header;
    BYTE channel;
} TPacketGCChannel;

typedef struct SEquipmentItemSet
{
	DWORD   vnum;
	short    count;
	long    alSockets[ITEM_SOCKET_SLOT_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_SLOT_MAX_NUM];
} TEquipmentItemSet;

typedef struct pakcet_view_equip
{
    BYTE header;
	DWORD dwVID;
	TEquipmentItemSet equips[WEAR_MAX_NUM];
} TPacketGCViewEquip;

typedef struct
{
    DWORD       dwID;
    long        x, y;
    long        width, height;
    DWORD       dwGuildID;
} TLandPacketElement;

typedef struct packet_land_list
{
    BYTE        header;
    WORD        size;
} TPacketGCLandList;

typedef struct
{
    BYTE        bHeader;
    long        lID;
    char        szTargetName[32+1];
} TPacketGCTargetCreate;

enum
{
	CREATE_TARGET_TYPE_NONE,
	CREATE_TARGET_TYPE_LOCATION,
	CREATE_TARGET_TYPE_CHARACTER,
};

typedef struct
{
	BYTE		bHeader;
	long		lID;
	char		szTargetName[32+1];
	DWORD		dwVID;
	BYTE		byType;
} TPacketGCTargetCreateNew;

typedef struct
{
    BYTE        bHeader;
    long        lID;
    long        lX, lY;
} TPacketGCTargetUpdate;

typedef struct
{
    BYTE        bHeader;
    long        lID;
} TPacketGCTargetDelete;

typedef struct
{
    DWORD       dwType;
    BYTE        bPointIdxApplyOn;
    long        lApplyValue;
    DWORD       dwFlag;
    long        lDuration;
    long        lSPCost;
} TPacketAffectElement;

typedef struct
{
    BYTE bHeader;
    TPacketAffectElement elem;
} TPacketGCAffectAdd;

typedef struct
{
    BYTE bHeader;
    DWORD dwType;
    BYTE bApplyOn;
} TPacketGCAffectRemove;

typedef struct packet_mall_open
{
	BYTE bHeader;
	BYTE bSize;
} TPacketGCMallOpen;

typedef struct packet_lover_info
{
	BYTE bHeader;
	char szName[CHARACTER_NAME_MAX_LEN + 1];
	BYTE byLovePoint;
} TPacketGCLoverInfo;

typedef struct packet_love_point_update
{
	BYTE bHeader;
	BYTE byLovePoint;
} TPacketGCLovePointUpdate;

typedef struct packet_dig_motion
{
    BYTE header;
    DWORD vid;
    DWORD target_vid;
	BYTE count;
} TPacketGCDigMotion;

typedef struct SPacketGCOnTime
{
    BYTE header;
    int ontime;     // sec
} TPacketGCOnTime;

typedef struct SPacketGCResetOnTime
{
    BYTE header;
} TPacketGCResetOnTime;

typedef struct SPacketGCPanamaPack
{
    BYTE    bHeader;
    char    szPackName[256];
    BYTE    abIV[32];
} TPacketGCPanamaPack;

typedef struct SPacketGCHybridCryptKeys
{
private:
	SPacketGCHybridCryptKeys() : m_pStream(NULL) {}

public:
	SPacketGCHybridCryptKeys(int iStreamSize) : iKeyStreamLen(iStreamSize)
	{
		m_pStream = new BYTE[iStreamSize];
	}
	~SPacketGCHybridCryptKeys()
	{
		if( m_pStream )
		{
			delete[] m_pStream;
			m_pStream = NULL;
		}
	}
	static int GetFixedHeaderSize()
	{
		return sizeof(BYTE)+sizeof(WORD)+sizeof(int);
	}

	BYTE	bHeader;
	WORD    wDynamicPacketSize;
	int		iKeyStreamLen;
	BYTE*	m_pStream;

} TPacketGCHybridCryptKeys;


typedef struct SPacketGCHybridSDB
{
private:
	SPacketGCHybridSDB() : m_pStream(NULL) {}

public:
	SPacketGCHybridSDB(int iStreamSize) : iSDBStreamLen(iStreamSize)
	{
		m_pStream = new BYTE[iStreamSize];
	}
	~SPacketGCHybridSDB()
	{
		delete[] m_pStream;
		m_pStream = NULL;
	}
	static int GetFixedHeaderSize()
	{
		return sizeof(BYTE)+sizeof(WORD)+sizeof(int);
	}

	BYTE	bHeader;
	WORD    wDynamicPacketSize;
	int		iSDBStreamLen;
	BYTE*	m_pStream;

} TPacketGCHybridSDB;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Client To Client

typedef struct packet_state
{
	BYTE			bHeader;
	BYTE			bFunc;
	BYTE			bArg;
	BYTE			bRot;
	DWORD			dwVID;
	DWORD			dwTime;
	TPixelPosition	kPPos;
} TPacketCCState;

typedef struct packet_hs_check_req
{
	BYTE	bHeader;
	AHNHS_TRANS_BUFFER	Req;
} TPacketHSCheck;

//NOTE : recv/send�� �������� ����, bPacketData�� ���븸 �ٲ��?.
typedef struct packet_xtrap_verify
{
	BYTE	bHeader;
	BYTE	bPacketData[128];

} TPacketXTrapCSVerify;
// AUTOBAN
typedef struct packet_autoban_quiz
{
    BYTE bHeader;
	BYTE bDuration;
    BYTE bCaptcha[64*32];
    char szQuiz[256];
} TPacketGCAutoBanQuiz;
// END_OF_AUTOBAN

#ifdef _IMPROVED_PACKET_ENCRYPTION_
struct TPacketKeyAgreement
{
	static const int MAX_DATA_LEN = 256;
	BYTE bHeader;
	WORD wAgreedLength;
	WORD wDataLength;
	BYTE data[MAX_DATA_LEN];
};

struct TPacketKeyAgreementCompleted
{
	BYTE bHeader;
	BYTE data[3]; // dummy (not used)
};
#endif // _IMPROVED_PACKET_ENCRYPTION_

#ifdef __AUCTION__
// Argument�� �뵵�� cmd�� ���� �ٸ���.
typedef struct SPacketCGAuctionCmd
{
	BYTE bHeader;
	BYTE cmd;
	int arg1;
	int arg2;
	int arg3;
	int arg4;
} TPacketCGAuctionCmd;

typedef struct SPacketGCAuctionItemListPack
{
	BYTE bHeader;
	BYTE bNumbers;
} TPacketGCAuctionItemListPack;

#endif

typedef struct SPacketGCSpecificEffect
{
	BYTE header;
	DWORD vid;
	char effect_file[128];
} TPacketGCSpecificEffect;

typedef struct SPacketCGSoulStoneUse {
	BYTE		header;
	DWORD		skillIndex;
	TItemPos	Cell;
	bool		all;
} TPacketCGSoulStoneUse;


// ��ȥ��
enum EDragonSoulRefineWindowRefineType
{
	DragonSoulRefineWindow_UPGRADE,
	DragonSoulRefineWindow_IMPROVEMENT,
	DragonSoulRefineWindow_REFINE,
};

enum EPacketCGDragonSoulSubHeaderType
{
	DS_SUB_HEADER_OPEN,
	DS_SUB_HEADER_CLOSE,
	DS_SUB_HEADER_DO_UPGRADE,
	DS_SUB_HEADER_DO_IMPROVEMENT,
	DS_SUB_HEADER_DO_REFINE,
	DS_SUB_HEADER_REFINE_FAIL,
	DS_SUB_HEADER_REFINE_FAIL_MAX_REFINE,
	DS_SUB_HEADER_REFINE_FAIL_INVALID_MATERIAL,
	DS_SUB_HEADER_REFINE_FAIL_NOT_ENOUGH_MONEY,
	DS_SUB_HEADER_REFINE_FAIL_NOT_ENOUGH_MATERIAL,
	DS_SUB_HEADER_REFINE_FAIL_TOO_MUCH_MATERIAL,
	DS_SUB_HEADER_REFINE_SUCCEED,
};

typedef struct SPacketCGDragonSoulRefine
{
	SPacketCGDragonSoulRefine() : header (HEADER_CG_DRAGON_SOUL_REFINE)
	{}
	BYTE header;
	BYTE bSubType;
	TItemPos ItemGrid[DS_REFINE_WINDOW_MAX_NUM];
} TPacketCGDragonSoulRefine;

typedef struct SPacketGCDragonSoulRefine
{
	SPacketGCDragonSoulRefine() : header(HEADER_GC_DRAGON_SOUL_REFINE)
	{}
	BYTE header;
	BYTE bSubType;
	TItemPos Pos;
} TPacketGCDragonSoulRefine;

typedef struct SChannelStatus
{
	short nPort;
	BYTE bStatus;
} TChannelStatus;

#ifdef ENABLE_CHANGELOOK_SYSTEM
enum EChangeLookInfo
{
	CL_WINDOW_MAX_MATERIALS = 3,
	CL_CLEAN_ATTR_VALUE0 = 8,
	CL_SCROLL_VNUM = 72326,
};

enum
{
	HEADER_CG_CL = 213,
	HEADER_GC_CL = 218,
	CL_SUBHEADER_OPEN = 0,
	CL_SUBHEADER_CLOSE,
	CL_SUBHEADER_ADD,
	CL_SUBHEADER_REMOVE,
	CL_SUBHEADER_REFINE,
};

typedef struct SPacketChangeLook
{
	BYTE	header;
	BYTE	subheader;
	DWORD	dwCost;
	BYTE	bPos;
	TItemPos	tPos;
} TPacketChangeLook;

typedef struct SChangeLookMaterial
{
	BYTE	bHere;
	WORD	wCell;
} TChangeLookMaterial;
#endif

#ifdef ENABLE_PRIVATESHOP_SEARCH_SYSTEM
typedef struct SPacketGCShopSearchItemInfo
{
	BYTE	bHeader;
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
} TPacketGCShopSearchItemInfo;

#include "../GameLib/ItemData.h"

typedef struct SPacketCGShopSearchInfo
{
	BYTE	bHeader;

	// BYTE	bJob;
	BYTE	bItemType;
	BYTE	bItemSubType;
	// BYTE	bMinLevel;
	// BYTE	bMaxLevel;
	// BYTE	bMinRefine;
	// BYTE	bMaxRefine;

	long	lMinGold;
	long	lMaxGold;

#ifdef ENABLE_CHEQUE_SYSTEM
	int	bMinCheque;
	int	bMaxCheque;
#endif

	char	szItemName[CItemData::ITEM_NAME_MAX_LEN + 1];
} TPacketCGShopSearchInfo;

typedef struct SPacketCGS
{
	BYTE	bHeader;
	DWORD	dwShopVID;
	BYTE	bItemPos;
} TPacketCGShopSearchBuyItem;
#endif


#ifdef ENABLE_ACCE_COSTUME_SYSTEM
enum EAcceInfo
{
	ACCE_ABSORPTION_SOCKET = 0,
	ACCE_ABSORBED_SOCKET = 1,
	ACCE_CLEAN_ATTR_VALUE0 = 7,
	ACCE_WINDOW_MAX_MATERIALS = 2,
};
enum
{
	HEADER_CG_ACCE = 211,
	HEADER_GC_ACCE = 215,
	ACCE_SUBHEADER_GC_OPEN = 0,
	ACCE_SUBHEADER_GC_CLOSE,
	ACCE_SUBHEADER_GC_ADDED,
	ACCE_SUBHEADER_GC_REMOVED,
	ACCE_SUBHEADER_CG_REFINED,
	ACCE_SUBHEADER_CG_CLOSE = 0,
	ACCE_SUBHEADER_CG_ADD,
	ACCE_SUBHEADER_CG_REMOVE,
	ACCE_SUBHEADER_CG_REFINE,
};
typedef struct SPacketAcce
{
	BYTE	header;
	BYTE	subheader;
	bool	bWindow;
	DWORD	dwPrice;
	BYTE	bPos;
	TItemPos	tPos;
	DWORD	dwItemVnum;
	DWORD	dwMinAbs;
	DWORD	dwMaxAbs;
} TPacketAcce;
typedef struct SAcceMaterial
{
	BYTE	bHere;
	WORD	wCell;
} TAcceMaterial;
typedef struct SAcceResult
{
	DWORD	dwItemVnum;
	DWORD	dwMinAbs;
	DWORD	dwMaxAbs;
} TAcceResult;
#endif
// @fixme007 length 2
typedef struct packet_unk_213
{
	BYTE bHeader;
	BYTE bUnk2;
} TPacketGCUnk213;

#ifdef ENABLE_SWITCHBOT
enum ECGSwitchbotSubheader
{
	SUBHEADER_CG_SWITCHBOT_START,
	SUBHEADER_CG_SWITCHBOT_STOP,
};

struct TPacketCGSwitchbot
{
	BYTE header;
	int size;
	BYTE subheader;
	BYTE slot;
};

enum EGCSwitchbotSubheader
{
	SUBHEADER_GC_SWITCHBOT_UPDATE,
	SUBHEADER_GC_SWITCHBOT_UPDATE_ITEM,
	SUBHEADER_GC_SWITCHBOT_SEND_ATTRIBUTE_INFORMATION,
};

struct TPacketGCSwitchbot
{
	BYTE header;
	int size;
	BYTE subheader;
	BYTE slot;
};

struct TSwitchbotUpdateItem
{
	BYTE	slot;
	BYTE	vnum;
	BYTE	count;
	long	alSockets[ITEM_SOCKET_SLOT_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_SLOT_MAX_NUM];
};
#endif

#ifdef ENABLE_CUBE_RENEWAL_WORLDARD

enum
{
	CUBE_RENEWAL_SUB_HEADER_OPEN_RECEIVE,
	CUBE_RENEWAL_SUB_HEADER_CLEAR_DATES_RECEIVE,
	CUBE_RENEWAL_SUB_HEADER_DATES_RECEIVE,
	CUBE_RENEWAL_SUB_HEADER_DATES_LOADING,

	CUBE_RENEWAL_SUB_HEADER_MAKE_ITEM,
	CUBE_RENEWAL_SUB_HEADER_CLOSE,
};


typedef struct  packet_send_cube_renewal
{
	BYTE header;
	BYTE subheader;
	DWORD index_item;
	DWORD count_item;
	DWORD index_item_improve;
}TPacketCGCubeRenewalSend;



typedef struct dates_cube_renewal
{
	DWORD npc_vnum;
	DWORD index;

	DWORD	vnum_reward;
	int		count_reward;

	bool 	item_reward_stackable;

	DWORD	vnum_material_1;
	int		count_material_1;

	DWORD	vnum_material_2;
	int		count_material_2;

	DWORD	vnum_material_3;
	int		count_material_3;

	DWORD	vnum_material_4;
	int		count_material_4;

	DWORD	vnum_material_5;
	int		count_material_5;

	int 	gold;
	int 	percent;

	char 	category[100];
}TInfoDateCubeRenewal;

typedef struct packet_receive_cube_renewal
{
	packet_receive_cube_renewal() : header(HEADER_GC_CUBE_RENEWAL)
	{}

	BYTE header;
	BYTE subheader;
	TInfoDateCubeRenewal	date_cube_renewal;
}TPacketGCCubeRenewalReceive;

#ifdef ENABLE_ATLAS_BOSS
typedef struct SPacketGCBossPosition
{
	BYTE	bHeader;
	WORD	wSize;
	WORD	wCount;
} TPacketGCBossPosition;

struct TBossPosition
{
	BYTE	bType;
#ifdef ENABLE_MULTI_NAMES
	DWORD	szName;
#else
	char	szName[CHARACTER_NAME_MAX_LEN + 1];
#endif
	long	lX;
	long	lY;
	long	lTime;
};
#endif

#endif

#ifdef ENABLE_6_7_BONUS_NEW_SYSTEM

enum
{
	BONUS_67_NEW_SUB_HEADER_CLOSE = 0,
	BONUS_67_NEW_SUB_HEADER_OPEN = 1,
	BONUS_67_NEW_SUB_HEADER_COMB_SKILLBOOK = 2,

	BONUS_67_NEW_SUB_HEADER_GET_FRAGMENT,
	BONUS_67_NEW_SUB_HEADER_ADD,

	BONUS_67_NEW_SUB_HEADER_FRAGMENT_RECEIVE,
};

typedef struct packet_send_67_bonus_system
{
	BYTE header;
	BYTE subheader;
	int  cell[c_skillbook_slot_max];
	int  regist_slot;
	int  count_fragment;
	int  cell_additive;
	int  count_additive;


} TPacketCG67BonusSend;

typedef struct packet_receive_67_bonus_new
{
	packet_receive_67_bonus_new() : header(HEADER_GC_67_BONUS_NEW)
	{}

	BYTE header;
	BYTE subheader;
	DWORD vnum_fragment;

}TPacketGC67BonusReceive;

#endif

#ifdef WON_EXCHANGE

enum EWonExchangeCGSubHeader
{
	WON_EXCHANGE_CG_SUBHEADER_SELL,
	WON_EXCHANGE_CG_SUBHEADER_BUY
};

typedef struct SPacketCGWonExchange
{
	SPacketCGWonExchange() :
		bHeader(HEADER_CG_WON_EXCHANGE)
	{}
	BYTE bHeader;
	BYTE bSubHeader;
	WORD wValue;
} TPacketCGWonExchange;

#endif

#ifdef ___DECORUM___
typedef struct SPacketGCDecorumStat
{
	BYTE		bHeader;
	DWORD		m_dwVid;
	DWORD		m_dwDecorum;
	BYTE		m_bLegue;
	DWORD		m_dwKill;
	DWORD		m_dwDeath;
	DWORD		m_dwPromotion;
	DWORD		m_dwDemotion;
	DWORD		m_dwBlock;
	DWORD		m_adwDecorumArena[ARENA_MAX_NUM][2];
	DWORD		m_adwDuel[2];
} TPacketGCDecorumStat;

typedef struct SPacketGCDecorumBase
{
	BYTE		bHeader;
	DWORD		dwDecorum;
	BYTE		bLegue;
	DWORD		dwPromotion;
	DWORD		dwDemotion;
	DWORD		dwBlock;
} TPacketGCDecorumBase;

typedef struct SPacketGCDecorumBattle
{
	BYTE		bHeader;
	BYTE		bBattleType;
	DWORD		dwBattleData[2];
} TPacketGCDecorumBattle;

typedef struct SPacketGCDecorumKill
{
	BYTE		bHeader;
	DWORD		dwKill;
	DWORD		dwDeath;
} TPacketGCDecorumKills;
#endif

#ifdef ENABLE_ANTY_WAIT_HACK

typedef struct packet_anty_wh
{
	BYTE			bHeader;
	char			regex[255 + 1];
} TPacketGCAntyWH;
typedef struct packet_anty_wh_cg
{
	BYTE			bHeader;
	char			hash[32 + 1];
	char			file[32 + 1];
} TPacketCGAntyWH;
#endif

typedef struct SPacketSearchShopSystem
{
	BYTE	header;
	int		Founded;
	char	szSeller[CHARACTER_NAME_MAX_LEN + 1];
	DWORD	dwVid;
	DWORD	vnum;
	DWORD	count;
	int		iItemRefine;
	int		iItemPrice;
	BYTE	bItemPriceCheque;
	int		iItemLevel;
	long    alSockets[ITEM_SOCKET_SLOT_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_SLOT_MAX_NUM];
//#ifdef __CHANGELOOK_SYSTEM__
	DWORD	dwTransmutation;
//#endif
	int		index;
} TPacketGCShopSearchSystem;

#ifdef GUILD_WAR_COUNTER
enum
{
	GUILD_STATIC_LOAD,
	GUILD_STATIC_KILL_DEAD,
	GUILD_STATIC_DMG,
	GUILD_STATIC_ADD_MEMBER,
	GUILD_STATIC_SPY,
	GUILD_STATIC_UPDATE_ONLINE,
	GUILD_STATIC_USER_COUNT,
	GUILD_STATIC_UPDATE_OBSERVER,
	/* LOG */
	GUILD_STATIC_INFO,
	GUILD_STATIC_DATA,
	GUILD_STATIC_EVENT,
	/* LOG */
#ifdef ENABLE_GUILD_ONLINE_LIST
	GUILD_ONLINE_LIST,
	GUILD_ONLINE_UPDATE,
#endif

};
typedef struct war_static_
{
	BYTE	empire;
	char	name[CHARACTER_NAME_MAX_LEN + 1];
	BYTE	level;
	BYTE	race;
	BYTE	kill;
	BYTE	dead;
	long	skill_dmg;
	bool	is_leader;
	DWORD	guild_id;
	DWORD	pid;
	bool	spy;
	bool	online;
} war_static_ptr;
typedef struct SPacketGCGuildStatic
{
	BYTE	header;
	DWORD	size;
	BYTE	sub_index;
	BYTE	packet_size;
} TPacketGCGuildStatic;
typedef struct SPacketGCUpdateUser
{
	DWORD id0;
	int user0;
	DWORD id1;
	int user1;
	int observer;
} TPacketGCUpdateUser;
typedef struct SGuildReserve
{
	DWORD       dwID;
	DWORD       dwGuildFrom;
	DWORD       dwGuildTo;
	DWORD       dwTime;
	BYTE        bType;
	long        lWarPrice;
	long        lInitialScore;
	bool        bStarted;
	DWORD	dwBetFrom;
	DWORD	dwBetTo;
	long	lPowerFrom;
	long	lPowerTo;
	long	lHandicap;
#ifdef __IMPROVED_GUILD_WAR__
	int		iMaxPlayer;
	int		iMaxScore;
	DWORD	flags;
	int		custom_map_index;
#endif
#ifdef GUILD_WAR_COUNTER
	char	guild1_name[GUILD_NAME_MAX_LEN + 1];
	char	guild2_name[GUILD_NAME_MAX_LEN + 1];
	char date[22];
	DWORD winner;
#endif
} TGuildWarReserve;
#endif


#ifdef ENABLE_HUNTING_SYSTEM
typedef struct SPacketCGHuntingAction
{
	BYTE	bHeader;
	BYTE	bAction;
	DWORD	dValue;
} TPacketGCHuntingAction;

typedef struct SPacketCGOpenWindowHuntingMain
{
	BYTE	bHeader;
	DWORD	dLevel;
	DWORD	dMonster;
	DWORD	dCurCount;
	DWORD	dDestCount;
	DWORD	dMoneyMin;
	DWORD	dMoneyMax;
	DWORD	dExpMin;
	DWORD	dExpMax;
	DWORD	dRaceItem;
	DWORD	dRaceItemCount;
} TPacketGCOpenWindowHuntingMain;

typedef struct SPacketCGOpenWindowHuntingSelect
{
	BYTE	bHeader;
	DWORD	dLevel;
	BYTE	bType;
	DWORD	dMonster;
	DWORD	dCount;
	DWORD	dMoneyMin;
	DWORD	dMoneyMax;
	DWORD	dExpMin;
	DWORD	dExpMax;
	DWORD	dRaceItem;
	DWORD	dRaceItemCount;
} TPacketGCOpenWindowHuntingSelect;

typedef struct SPacketGCOpenWindowReward
{
	BYTE	bHeader;
	DWORD	dLevel;
	DWORD	dReward;
	DWORD	dRewardCount;
	DWORD	dRandomReward;
	DWORD	dRandomRewardCount;
	DWORD	dMoney;
	BYTE	bExp;
} TPacketGCOpenWindowReward;

typedef struct SPacketGCUpdateHunting
{
	BYTE	bHeader;
	DWORD	dCount;
} TPacketGCUpdateHunting;

typedef struct SPacketGCReciveRandomItems
{
	BYTE	bHeader;
	BYTE	bWindow;
	DWORD	dItemVnum;
	DWORD	dItemCount;
} TPacketGCReciveRandomItems;
#endif

#ifdef ENABLE_ANTI_MULTIPLE_FARM
enum EAntiFarmSubHeader
{
	AF_SH_SENDING_DATA,
	AF_SH_SEND_STATUS_UPDATE,
	AF_SH_SENDING_CONFIRM_DATA,
};

typedef struct SAntiFarmPlayerInfo
{
	char	szName[CHARACTER_NAME_MAX_LEN + 1];
	DWORD	dwPID;
	bool	bDropStatus;
} TAntiFarmPlayerInfo;

typedef struct SSendAntiFarmInfo
{
	BYTE	header;
	int		size;
	BYTE	subheader;
} TSendAntiFarmInfo;
#endif

#ifdef ENABLE_NEW_FISHING_SYSTEM
enum {
	FISHING_SUBHEADER_NEW_START = 0,
	FISHING_SUBHEADER_NEW_STOP = 1,
	FISHING_SUBHEADER_NEW_CATCH = 2,
	FISHING_SUBHEADER_NEW_CATCH_FAIL = 3,
	FISHING_SUBHEADER_NEW_CATCH_SUCCESS = 4,
	FISHING_SUBHEADER_NEW_CATCH_FAILED = 5,
};

typedef struct SPacketFishingNew
{
	BYTE header;
	BYTE subheader;
	DWORD vid;
	int dir;
	BYTE need;
	BYTE count;
} TPacketFishingNew;
#endif

#ifdef ENABLE_ENTITY_PRELOADING
/*** HEADER_GC_PRELOAD_ENTITIES ***/
typedef struct packet_preload_entities
{
	uint8_t header;
	uint16_t size;
	uint16_t count;
} TPacketGCPreloadEntities;
#endif


#if defined(ENABLE_CSHIELD) && !defined(_DEBUG)
typedef struct SPacketGCCShield
{
	BYTE	header;
	bool	start;
	DWORD	num;
} TPacketGCCShield;

typedef struct SPacketCGCShield
{
	BYTE	header;
	char	key[128];
} TPacketCGCShield;
#endif


#pragma pack(pop)

