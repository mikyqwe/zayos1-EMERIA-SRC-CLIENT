#pragma once

#include "../eterLib/FuncObject.h"
#include "../eterlib/NetStream.h"
#include "../eterLib/NetPacketHeaderMap.h"

#include "InsultChecker.h"

#include "packet.h"
#ifdef ENABLE_SWITCHBOT
#include "PythonSwitchbot.h"
#endif
#include "Locale_inc.h"
class CInstanceBase;
class CNetworkActorManager;
struct SNetworkActorData;
struct SNetworkUpdateActorData;

class CPythonNetworkStream : public CNetworkStream, public CSingleton<CPythonNetworkStream>
{
	public:
		enum
		{
			SERVER_COMMAND_LOG_OUT = 0,
			SERVER_COMMAND_RETURN_TO_SELECT_CHARACTER = 1,
			SERVER_COMMAND_QUIT = 2,

			MAX_ACCOUNT_PLAYER
		};

		enum
		{
			ERROR_NONE,
			ERROR_UNKNOWN,
			ERROR_CONNECT_MARK_SERVER,
			ERROR_LOAD_MARK,
			ERROR_MARK_WIDTH,
			ERROR_MARK_HEIGHT,

			// MARK_BUG_FIX
			ERROR_MARK_UPLOAD_NEED_RECONNECT,
			ERROR_MARK_CHECK_NEED_RECONNECT,
			// END_OF_MARK_BUG_FIX
		};

		enum
		{
			ACCOUNT_CHARACTER_SLOT_ID,
			ACCOUNT_CHARACTER_SLOT_NAME,
			ACCOUNT_CHARACTER_SLOT_RACE,
			ACCOUNT_CHARACTER_SLOT_LEVEL,
			ACCOUNT_CHARACTER_SLOT_STR,
			ACCOUNT_CHARACTER_SLOT_DEX,
			ACCOUNT_CHARACTER_SLOT_HTH,
			ACCOUNT_CHARACTER_SLOT_INT,
			ACCOUNT_CHARACTER_SLOT_PLAYTIME,
			ACCOUNT_CHARACTER_SLOT_FORM,
			ACCOUNT_CHARACTER_SLOT_ADDR,
			ACCOUNT_CHARACTER_SLOT_PORT,
			ACCOUNT_CHARACTER_SLOT_GUILD_ID,
			ACCOUNT_CHARACTER_SLOT_GUILD_NAME,
			ACCOUNT_CHARACTER_SLOT_CHANGE_NAME_FLAG,
			ACCOUNT_CHARACTER_SLOT_HAIR,
#ifdef ENABLE_AURA_SYSTEM
			ACCOUNT_CHARACTER_SLOT_AURA,
#endif
#ifdef ENABLE_ACCE_COSTUME_SYSTEM
			ACCOUNT_CHARACTER_SLOT_ACCE,
#endif
		};

		enum
		{
			PHASE_WINDOW_LOGO,
			PHASE_WINDOW_LOGIN,
			PHASE_WINDOW_SELECT,
			PHASE_WINDOW_CREATE,
			PHASE_WINDOW_LOAD,
			PHASE_WINDOW_GAME,
			PHASE_WINDOW_EMPIRE,
			PHASE_WINDOW_NUM,
		};

		bool IsLoadingPhase() { return (m_strPhase == "Loading"); }
		
	public:
		CPythonNetworkStream();
		virtual ~CPythonNetworkStream();

		bool SendSpecial(int nLen, void * pvBuf);

#ifdef WON_EXCHANGE
		bool SendWonExchangeSellPacket(WORD wValue);
		bool SendWonExchangeBuyPacket(WORD wValue);
#endif
		void StartGame();
		void Warp(LONG lGlobalX, LONG lGlobalY);

		void NotifyHack(const char* c_szMsg);
		void SetWaitFlag();

#ifdef ENABLE_DISCORD_RPC
		void Discord_Start();
		void Discord_Close();
		void Discord_Update(const bool login, const bool select, const bool loading);
		const char * GetStaffRank();
		const char * GetRaceIcon();
		const char * GetRaceName();
		const char * GetEmpireIcon();
		const char * GetEmpireName();
#endif

		void SendEmoticon(UINT eEmoticon);

		void ExitApplication();
		void ExitGame();
		void LogOutGame();
		void AbsoluteExitGame();
		void AbsoluteExitApplication();

		void EnableChatInsultFilter(bool isEnable);
		bool IsChatInsultIn(const char* c_szMsg);
		bool IsInsultIn(const char* c_szMsg);

		DWORD GetGuildID();

		UINT UploadMark(const char* c_szImageFileName);
		UINT UploadSymbol(const char* c_szImageFileName);

		bool LoadInsultList(const char* c_szInsultListFileName);
		bool LoadConvertTable(DWORD dwEmpireID, const char* c_szFileName);

		UINT		GetAccountCharacterSlotDatau(UINT iSlot, UINT eType);
		const char* GetAccountCharacterSlotDataz(UINT iSlot, UINT eType);

		// SUPPORT_BGM
		const char*		GetFieldMusicFileName();
		float			GetFieldMusicVolume();
		// END_OF_SUPPORT_BGM

		bool IsSelectedEmpire();

		void ToggleGameDebugInfo();

		void SetMarkServer(const char* c_szAddr, UINT uPort);
		void ConnectLoginServer(const char* c_szAddr, UINT uPort);
		void ConnectGameServer(UINT iChrSlot);

		void SetLoginInfo(const char* c_szID, const char* c_szPassword);
		void SetLoginKey(DWORD dwLoginKey);
		void ClearLoginInfo( void );

		void SetHandler(PyObject* poHandler);
		void SetPhaseWindow(UINT ePhaseWnd, PyObject* poPhaseWnd);
		void ClearPhaseWindow(UINT ePhaseWnd, PyObject* poPhaseWnd);
		void SetServerCommandParserWindow(PyObject* poPhaseWnd);

		bool SendSyncPositionElementPacket(DWORD dwVictimVID, DWORD dwVictimX, DWORD dwVictimY);
		bool SendAttackPacket(UINT uMotAttack, DWORD dwVIDVictim);

		bool SendCharacterStatePacket(const TPixelPosition& c_rkPPosDst, float fDstRot, UINT eFunc, UINT uArg);
		bool SendUseSkillPacket(DWORD dwSkillIndex, DWORD dwTargetVID=0);
		bool SendTargetPacket(DWORD dwVID);

		// OLDCODE:
		bool SendCharacterStartWalkingPacket(float fRotation, long lx, long ly);
		bool SendCharacterEndWalkingPacket(float fRotation, long lx, long ly);
		bool SendCharacterCheckWalkingPacket(float fRotation, long lx, long ly);

		bool SendCharacterPositionPacket(BYTE iPosition);
#if defined(__BL_SOUL_ROULETTE__)
		bool SoulRoulette(const BYTE option);
		bool RecvSoulRoulette();
#endif
#ifdef ENABLE_SORT_INVEN
		bool SortInven(BYTE option);
#endif
		bool SendItemUsePacket(TItemPos pos);
		bool SendItemUseToItemPacket(TItemPos source_pos, TItemPos target_pos);
		bool SendItemDropPacket(TItemPos pos, DWORD elk);
#ifdef NEW_ADD_INVENTORY
		bool Envanter_paketi(/*TItemPos pos*/);
#endif
		bool SendItemDropPacketNew(TItemPos pos, DWORD elk, DWORD count);
		bool SendItemMovePacket(TItemPos pos, TItemPos change_pos, short num);
		bool SendItemPickUpPacket(DWORD vid);
#ifdef ENABLE_SELL_ITEM
		bool SendItemSellPacket(TItemPos pos, DWORD elk);
#endif
#ifdef INGAME_WIKI
		bool SendWikiRequestInfo(unsigned long long retID, DWORD vnum, bool isMob);
		void ToggleWikiWindow();
#endif
		bool SendQuickSlotAddPacket(BYTE wpos, BYTE type, BYTE pos);
		bool SendQuickSlotDelPacket(BYTE wpos);
		bool SendQuickSlotMovePacket(BYTE wpos, BYTE change_pos);

#ifdef ENABLE_BUY_BONUS_CHANGER_IN_SWITCH_BOT
		void SendBuyBonusChangerItemPacket();
#endif

		// PointReset �� �ӽ�
		bool SendPointResetPacket();

		// Shop
		bool SendShopEndPacket();
		bool SendShopBuyPacket(BYTE byCount);
		bool SendShopSellPacket(BYTE bySlot);
#ifdef ENABLE_SPECIAL_STORAGE_SYSTEM
		bool SendShopSellPacketNew(BYTE bySlot, short byCount, BYTE byType);
#else
		bool SendShopSellPacketNew(BYTE bySlot, BYTE byCount);
#endif

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
		bool SendOfflineShopEndPacket();
		bool SendDestroyOfflineShop();
		bool SendOfflineShopBuyPacket(BYTE byCount);
#ifdef ENABLE_CHEQUE_SYSTEM
		bool SendAddOfflineShopItemPacket(TItemPos item_pos, int llPrice, int bCheque);
#else
		bool SendAddOfflineShopItemPacket(TItemPos item_pos, int llPrice);
#endif
		bool SendRefreshOfflineShopMoney();
#endif
		// Exchange
		bool SendExchangeStartPacket(DWORD vid);
		bool SendExchangeItemAddPacket(TItemPos ItemPos, BYTE byDisplayPos);
		bool SendExchangeElkAddPacket(DWORD elk);
		bool SendExchangeItemDelPacket(BYTE pos);
#ifdef ENABLE_GUILD_REQUEST
		bool SendGuildRequest(DWORD guildIndex, BYTE requestIndex);
		bool SendGuildRequestPage(BYTE tabIndex, int pageIndex);
		bool SendGuildRequestName(BYTE tabIndex, const char* guildName);
		bool RecvGuildRequest();
#endif
#ifdef ENABLE_EVENT_MANAGER
		bool RecvEventManager();
#endif
		bool SendExchangeAcceptPacket();
		bool SendExchangeExitPacket();
#ifdef ENABLE_CHEQUE_SYSTEM
		bool SendExchangeChequeAddPacket(int cheque);
#endif
		// Quest
		bool SendScriptAnswerPacket(int iAnswer);
		bool SendScriptButtonPacket(unsigned int iIndex);
		bool SendAnswerMakeGuildPacket(const char * c_szName);
		bool SendQuestInputStringPacket(const char * c_szString);
		bool SendQuestConfirmPacket(BYTE byAnswer, DWORD dwPID);

		// Event
		bool SendOnClickPacket(DWORD vid);

		// Fly
		bool SendFlyTargetingPacket(DWORD dwTargetVID, const TPixelPosition& kPPosTarget);
		bool SendAddFlyTargetingPacket(DWORD dwTargetVID, const TPixelPosition& kPPosTarget);
		bool SendShootPacket(UINT uSkill);

		// Command
		bool ClientCommand(const char * c_szCommand);
		void ServerCommand(char * c_szCommand);

		// Emoticon
		void RegisterEmoticonString(const char * pcEmoticonString);

		// Party
		bool SendPartyInvitePacket(DWORD dwVID);
		bool SendPartyInviteAnswerPacket(DWORD dwLeaderVID, BYTE byAccept);
		bool SendPartyRemovePacket(DWORD dwPID);
		bool SendPartySetStatePacket(DWORD dwVID, BYTE byState, BYTE byFlag);
		bool SendPartyUseSkillPacket(BYTE bySkillIndex, DWORD dwVID);
		bool SendPartyParameterPacket(BYTE byDistributeMode);

		// SafeBox
		bool SendSafeBoxMoneyPacket(BYTE byState, DWORD dwMoney);
		bool SendSafeBoxCheckinPacket(TItemPos InventoryPos, BYTE bySafeBoxPos);
		bool SendSafeBoxCheckoutPacket(BYTE bySafeBoxPos, TItemPos InventoryPos);
		bool SendSafeBoxItemMovePacket(BYTE bySourcePos, BYTE byTargetPos, short byCount);

		// Mall
		bool SendMallCheckoutPacket(BYTE byMallPos, TItemPos InventoryPos);

		// Guild
		bool SendGuildAddMemberPacket(DWORD dwVID);
		bool SendGuildRemoveMemberPacket(DWORD dwPID);
		bool SendGuildChangeGradeNamePacket(BYTE byGradeNumber, const char * c_szName);
		bool SendGuildChangeGradeAuthorityPacket(BYTE byGradeNumber, BYTE byAuthority);
		bool SendGuildOfferPacket(DWORD dwExperience);
		bool SendGuildPostCommentPacket(const char * c_szMessage);
		bool SendGuildDeleteCommentPacket(DWORD dwIndex);
		bool SendGuildRefreshCommentsPacket(DWORD dwHighestIndex);
		bool SendGuildChangeMemberGradePacket(DWORD dwPID, BYTE byGrade);
		bool SendGuildUseSkillPacket(DWORD dwSkillID, DWORD dwTargetVID);
		bool SendGuildChangeMemberGeneralPacket(DWORD dwPID, BYTE byFlag);
		bool SendGuildInvitePacket(DWORD dwVID);
		bool SendGuildInviteAnswerPacket(DWORD dwGuildID, BYTE byAnswer);
		bool SendGuildChargeGSPPacket(DWORD dwMoney);
		bool SendGuildDepositMoneyPacket(DWORD dwMoney);

		bool SendGuildWithdrawMoneyPacket(DWORD dwMoney);
#if defined(BL_REMOTE_SHOP)
		bool SendRemoteShopPacket(BYTE bIndex);
#endif
#ifdef NEW_PET_SYSTEM
		bool PetSetNamePacket(const char * petname);
#endif

		// Mall
		bool RecvMallOpenPacket();
		bool RecvMallItemSetPacket();
		bool RecvMallItemDelPacket();
		// Search Item
		bool RecvSInfoSearchItem();

		// Lover
		bool RecvLoverInfoPacket();
		bool RecvLovePointUpdatePacket();

		// Dig
		bool RecvDigMotionPacket();
#if defined(__BL_KILL_BAR__)
		bool RecvKillBar();
#endif
#if defined(__BL_RANKING__)
		bool RecvRanking();
#endif
#if defined(GUILD_RANK_SYSTEM)
		bool RecvGuildRanking();
#endif
		// Fishing
		bool SendFishingPacket(int iRotation);
#ifdef ENABLE_NEW_FISHING_SYSTEM
		bool SendFishingPacketNew(int r, int i);
		bool RecvFishingNew();
#endif
		bool SendGiveItemPacket(DWORD dwTargetVID, TItemPos ItemPos, int iItemCount);

		// Private Shop
		bool SendBuildPrivateShopPacket(const char * c_szName, const std::vector<TShopItemTable> & c_rSellingItemStock);
#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
		bool SendBuildOfflineShopPacket(const char * c_szName, const std::vector<TShopItemTable> & c_rSellingItemStock, BYTE bTime);
#endif

		// Refine
		bool SendRefinePacket(BYTE byPos, BYTE byType);
		bool SendSelectItemPacket(DWORD dwItemPos);
#if defined(__BL_MAILBOX__)
		bool RecvMailboxProcess();
		bool RecvMailbox();
		bool RecvMailboxAddData();
		bool RecvMailboxAll();
		bool RecvMailboxUnread();
		bool SendPostDelete(const BYTE Index);
		bool SendPostGetItems(const BYTE Index);
		bool SendPostWriteConfirm(const char* szName);
		bool SendPostWrite(const char* szName, const char* szTitle, const char* szMessage, const TItemPos& pos, const int iYang, const int iWon);
		bool SendMailBoxClose();
		bool SendPostAllDelete();
		bool SendPostAllGetItems();
		bool RequestPostAddData(const BYTE ButtonIndex, const BYTE DataIndex);
#endif
		// Client Version
		bool SendClientVersionPacket();

		// CRC Report
		bool __SendCRCReportPacket();

		// ��ȫ�� ��ȭ
		bool SendDragonSoulRefinePacket(BYTE bRefineType, TItemPos* pos);

		// Handshake
		bool RecvHandshakePacket();
		bool RecvHandshakeOKPacket();
#ifdef ENABLE_6_7_BONUS_NEW_SYSTEM
		bool SendSkillBookCombinationPacket(PyObject* cell, int index_skillbook_combi);
		bool SendRegistFragmentPacket(int cell);
		bool SendAddAttrCombPacket(int index_comb);
		bool SendAddAttrPacket(int cell, int count_fragment, int cell_additive, int count_additive);
		bool RecVBonus67NewPacket();
#endif
		bool RecvHybridCryptKeyPacket();
		bool RecvHybridCryptSDBPacket();
#ifdef _IMPROVED_PACKET_ENCRYPTION_
		bool RecvKeyAgreementPacket();
		bool RecvKeyAgreementCompletedPacket();

#endif
#ifdef ENABLE_CUBE_RENEWAL_WORLDARD
		bool CubeRenewalMakeItem(int index_item, int count_item, int index_item_improve);
		bool CubeRenewalClose();
		bool RecvCubeRenewalPacket();
#endif
		// ETC
		DWORD GetMainActorVID();
		DWORD GetMainActorRace();
		DWORD GetMainActorEmpire();
		DWORD GetMainActorSkillGroup();
		void SetEmpireID(DWORD dwEmpireID);
		DWORD GetEmpireID();
		void __TEST_SetSkillGroupFake(int iIndex);
#ifdef __ULTIMATE_TOOLTIP__
		bool 	SendChestDropInfo(WORD wInventoryCell, DWORD item_vnum = 0);
		bool 	RecvChestDropInfo();
#endif
#ifdef ENABLE_ACCE_COSTUME_SYSTEM
		bool	SendAcceClosePacket();
		bool	SendAcceAddPacket(TItemPos tPos, BYTE bPos);
		bool	SendAcceRemovePacket(BYTE bPos);
		bool	SendAcceRefinePacket();
#endif
#ifdef ENABLE_CHANGELOOK_SYSTEM
		bool	SendClClosePacket();
		bool	SendClAddPacket(TItemPos tPos, BYTE bPos);
		bool	SendClRemovePacket(BYTE bPos);
		bool	SendClRefinePacket();
#endif
	//////////////////////////////////////////////////////////////////////////
	// Phase ����
	//////////////////////////////////////////////////////////////////////////
	public:
		void SetOffLinePhase();
		void SetHandShakePhase();
		void SetLoginPhase();
		void SetSelectPhase();
		void SetLoadingPhase();
		void SetGamePhase();
		void ClosePhase();

		// Login Phase
		bool SendLoginPacket(const char * c_szName, const char * c_szPassword);
		bool SendLoginPacketNew(const char * c_szName, const char * c_szPassword);
		bool SendChinaMatrixCardPacket(const char * c_szMatrixCardString);
		bool SendRunupMatrixAnswerPacket(const char * c_szMatrixCardString);
		bool SendNEWCIBNPasspodAnswerPacket(const char * answer);
		bool SendDirectEnterPacket(const char * c_szName, const char * c_szPassword, UINT uChrSlot);

		bool SendEnterGame();

		// Select Phase
		bool SendSelectEmpirePacket(DWORD dwEmpireID);
		bool SendSelectCharacterPacket(BYTE account_Index);
		bool SendChangeNamePacket(BYTE index, const char *name);
#ifdef ENABLE_ANTI_MULTIPLE_FARM
		bool SendAntiFarmStatus(std::vector<DWORD> dwPIDs);
#endif
		bool SendCreateCharacterPacket(BYTE index, const char *name, BYTE job, BYTE shape, BYTE byStat1, BYTE byStat2, BYTE byStat3, BYTE byStat4);
		bool SendDestroyCharacterPacket(BYTE index, const char * szPrivateCode);
#ifdef ENABLE_MULTI_LANGUAGE_SYSTEM
		bool RequestFlag(const char* szName);
#endif

		// Main Game Phase
		bool SendC2CPacket(DWORD dwSize, void * pData);
		bool SendChatPacket(const char * c_szChat, BYTE byType = CHAT_TYPE_TALKING);
		bool SendWhisperPacket(const char * name, const char * c_szChat);
		bool SendMobileMessagePacket(const char * name, const char * c_szChat);
		bool SendMessengerAddByVIDPacket(DWORD vid);
		bool SendMessengerAddByNamePacket(const char * c_szName);
		bool SendMessengerRemovePacket(const char * c_szKey, const char * c_szName);
		#ifdef ENABLE_MESSENGER_BLOCK
		bool SendMessengerAddBlockByVIDPacket(DWORD vid);
		bool SendMessengerAddBlockByNamePacket(const char * c_szName);
		bool SendMessengerRemoveBlockPacket(const char * c_szKey, const char * c_szName);
		#endif

		bool SendHackNotification(const char* c_szMsg, const char* c_szInfo, const char* c_szHwid);

	protected:
		bool OnProcess();	// State���� ������ �����Ѵ�.
		void OffLinePhase();
		void HandShakePhase();
		void LoginPhase();
		void SelectPhase();
		void LoadingPhase();
		void GamePhase();

		bool __IsNotPing();

		void __DownloadMark();
		void __DownloadSymbol(const std::vector<DWORD> & c_rkVec_dwGuildID);

		void __PlayInventoryItemUseSound(TItemPos uSlotPos);
		void __PlayInventoryItemDropSound(TItemPos uSlotPos);
		//void __PlayShopItemDropSound(UINT uSlotPos);
		void __PlaySafeBoxItemDropSound(UINT uSlotPos);
		void __PlayMallItemDropSound(UINT uSlotPos);

		bool __CanActMainInstance();

		enum REFRESH_WINDOW_TYPE
		{
			RefreshStatus = (1 << 0),
			RefreshAlignmentWindow = (1 << 1),
			RefreshCharacterWindow = (1 << 2),
			RefreshEquipmentWindow = (1 << 3),
			RefreshInventoryWindow = (1 << 4),
			RefreshExchangeWindow = (1 << 5),
			RefreshSkillWindow = (1 << 6),
			RefreshSafeboxWindow  = (1 << 7),
			RefreshMessengerWindow = (1 << 8),
			RefreshGuildWindowInfoPage = (1 << 9),
			RefreshGuildWindowBoardPage = (1 << 10),
			RefreshGuildWindowMemberPage = (1 << 11),
			RefreshGuildWindowMemberPageGradeComboBox = (1 << 12),
			RefreshGuildWindowSkillPage = (1 << 13),
			RefreshGuildWindowGradePage = (1 << 14),
			RefreshTargetBoard = (1 << 15),
			RefreshMallWindow = (1 << 16),
		};

		void __RefreshStatus();
		void __RefreshAlignmentWindow();
		void __RefreshCharacterWindow();
		void __RefreshEquipmentWindow();
		void __RefreshInventoryWindow();
		void __RefreshExchangeWindow();
		void __RefreshSkillWindow();
		void __RefreshSafeboxWindow();
		void __RefreshMessengerWindow();
		void __RefreshGuildWindowInfoPage();
		void __RefreshGuildWindowBoardPage();
		void __RefreshGuildWindowMemberPage();
		void __RefreshGuildWindowMemberPageGradeComboBox();
		void __RefreshGuildWindowSkillPage();
		void __RefreshGuildWindowGradePage();
		void __RefreshTargetBoardByVID(DWORD dwVID);
		void __RefreshTargetBoardByName(const char * c_szName);
		void __RefreshTargetBoard();
		void __RefreshMallWindow();
#ifdef ENABLE_AFFECT_FIX
		void __RefreshAffectWindow();
#endif
#ifdef ENABLE_PRIVATESHOP_SEARCH_SYSTEM
		void __RefreshShopSearchWindow();
#endif
		bool __SendHack(const char* c_szMsg);

	protected:
		bool RecvObserverAddPacket();
		bool RecvObserverRemovePacket();
		bool RecvObserverMovePacket();

		// Common
		bool RecvErrorPacket(int header);
		bool RecvPingPacket();
		bool RecvDefaultPacket(int header);
		bool RecvPhasePacket();

		// Login Phase
		bool __RecvLoginSuccessPacket3();
		bool __RecvLoginSuccessPacket4();
		bool __RecvLoginFailurePacket();
		bool __RecvEmpirePacket();
		bool __RecvChinaMatrixCardPacket();
		bool __RecvRunupMatrixQuizPacket();
		bool __RecvNEWCIBNPasspodRequestPacket();
		bool __RecvNEWCIBNPasspodFailurePacket();
		bool __RecvLoginKeyPacket();

		// Select Phase
		bool __RecvPlayerCreateSuccessPacket();
		bool __RecvPlayerCreateFailurePacket();
		bool __RecvPlayerDestroySuccessPacket();
		bool __RecvPlayerDestroyFailurePacket();
		bool __RecvPreserveItemPacket();
		bool __RecvPlayerPoints();
		bool __RecvChangeName();

		// Loading Phase
		bool RecvMainCharacter();
		bool RecvMainCharacter2_EMPIRE();
		bool RecvMainCharacter3_BGM();
		bool RecvMainCharacter4_BGM_VOL();

		void __SetFieldMusicFileName(const char* musicName);
		void __SetFieldMusicFileInfo(const char* musicName, float vol);
		// END_OF_SUPPORT_BGM

		// Main Game Phase
#ifdef ENABLE_ENTITY_PRELOADING
		bool RecvPreloadEntitiesPacket();
#endif

		bool RecvWarpPacket();
		bool RecvPVPPacket();
		bool RecvDuelStartPacket();
        bool RecvGlobalTimePacket();
		bool RecvCharacterAppendPacket();
		bool RecvCharacterAdditionalInfo();
		bool RecvCharacterAppendPacketNew();
		bool RecvCharacterUpdatePacket();
		bool RecvCharacterUpdatePacketNew();
		bool RecvCharacterDeletePacket();
		bool RecvChatPacket();
#ifdef GUILD_WAR_COUNTER
		bool RecvGuildWarPacket();
#endif
		bool RecvOwnerShipPacket();
		bool RecvSyncPositionPacket();
		bool RecvWhisperPacket();
		bool RecvPointChange();					// Alarm to python
		bool RecvChangeSpeedPacket();

		bool RecvStunPacket();
		bool RecvDeadPacket();
		bool RecvCharacterMovePacket();

		bool RecvItemSetPacket();					// Alarm to python
		bool RecvItemSetPacket2();					// Alarm to python
		bool RecvItemUsePacket();					// Alarm to python
		bool RecvItemUpdatePacket();				// Alarm to python
		bool RecvItemGroundAddPacket();
		bool RecvItemGroundDelPacket();
		bool RecvItemOwnership();
#if defined(__BL_DROP_DESTROY_TIME__)
		bool RecvItemDestroyTime();
#endif
		bool RecvQuickSlotAddPacket();				// Alarm to python
		bool RecvQuickSlotDelPacket();				// Alarm to python
		bool RecvQuickSlotMovePacket();				// Alarm to python

		bool RecvCharacterPositionPacket();
		bool RecvMotionPacket();

		bool RecvShopPacket();
		bool RecvShopSignPacket();
		bool RecvExchangePacket();
#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
		bool RecvOfflineShopPacket();
		bool RecvOfflineShopSignPacket();
#endif
		// Quest
		bool RecvScriptPacket();
		bool RecvQuestInfoPacket();
		bool RecvQuestConfirmPacket();
		bool RecvRequestMakeGuild();

		// Skill
		bool RecvSkillLevel();
		bool RecvSkillLevelNew();
		bool RecvSkillCoolTimeEnd();

		// Target
		bool RecvTargetPacket();
		bool RecvViewEquipPacket();
		bool RecvDamageInfoPacket();
#ifdef ENABLE_NEW_DETAILS_GUI
		bool RecvKillLOG();
#endif

#ifdef TARGET_INFORMATION_SYSTEM
		bool RecvTargetInfoPacket();
		public:
			bool SendTargetInfoLoadPacket(DWORD dwVID);
		protected:
#endif
		// Mount
		bool RecvMountPacket();

		// Fly
		bool RecvCreateFlyPacket();
		bool RecvFlyTargetingPacket();
		bool RecvAddFlyTargetingPacket();

		// Messenger
		bool RecvMessenger();

		// Guild
		bool RecvGuild();

		// Party
		bool RecvPartyInvite();
#if defined(__BL_PARTY_POSITION__)
		bool RecvPartyPositionInfo();
#endif
		bool RecvPartyAdd();
		bool RecvPartyUpdate();
		bool RecvPartyRemove();
		bool RecvPartyLink();
		bool RecvPartyUnlink();
		bool RecvPartyParameter();

		// SafeBox
		bool RecvSafeBoxSetPacket();
		bool RecvSafeBoxDelPacket();
		bool RecvSafeBoxWrongPasswordPacket();
		bool RecvSafeBoxSizePacket();
		bool RecvSafeBoxMoneyChangePacket();
#if defined(ENABLE_DUNGEON_INFO_SYSTEM)
		// Dungeon Information
public:
		bool SendDungeonInfo(BYTE bAction, BYTE bIndex = 0, BYTE bRankType = 0);

protected:
		bool RecvDungeonInfo();
		bool RecvDungeonRanking();
#endif
		// Fishing
		bool RecvFishing();

		// Dungeon
		bool RecvDungeon();

		// Time
		bool RecvTimePacket();

		// WalkMode
		bool RecvWalkModePacket();

		// ChangeSkillGroup
		bool RecvChangeSkillGroupPacket();

		// Refine
		bool RecvRefineInformationPacket();
		bool RecvRefineInformationPacketNew();

		// Use Potion
		bool RecvSpecialEffect();

		// �������� ������ ����Ʈ �ߵ� ��Ŷ.
		bool RecvSpecificEffect();

		// ��ȥ�� ����
		bool RecvDragonSoulRefine();
#ifdef INGAME_WIKI
		bool RecvWikiPacket();
#endif

		// MiniMap Info
		bool RecvNPCList();
#ifdef ENABLE_ATLAS_BOSS
		bool RecvBossList();
#endif
		bool RecvLandPacket();
		bool RecvTargetCreatePacket();
		bool RecvTargetCreatePacketNew();
		bool RecvTargetUpdatePacket();
		bool RecvTargetDeletePacket();

		// Affect
		bool RecvAffectAddPacket();
		bool RecvAffectRemovePacket();

		// Channel
		bool RecvChannelPacket();
#ifdef ENABLE_ANTI_MULTIPLE_FARM
		bool RecvAntiFarmInformation();
#endif

#ifdef ENABLE_ACCE_COSTUME_SYSTEM
		bool	RecvAccePacket(bool bReturn = false);
#endif

#ifdef ENABLE_CHANGELOOK_SYSTEM
		bool	RecvChangeLookPacket();
#endif
		//Security
		bool RecvHSCheckRequest();
		bool RecvXTrapVerifyRequest();

		// @fixme007
		bool RecvUnk213();

#ifdef ___DECORUM___
		bool RecvDecorumStat();
		bool RecvDecorumBase();
		bool RecvDecorumBattle();
		bool RecvDecorumKills();
#endif
#ifdef SYSTEM_PDA
	public:
		bool SendSoulStoneUsePacket(DWORD dwSkillIndex, WORD wItemPos, bool bUseAll);
#endif
#ifdef ENABLE_MAINTENANCE_SYSTEM
		// Maintenance
	protected:
		bool RecvMaintenancePacket();
#endif		
	protected:
		// �̸�Ƽ��
		bool ParseEmoticon(const char * pChatMsg, DWORD * pdwEmoticon);

		// ���̽����� ������ �ݵ�
		void OnConnectFailure();
		void OnScriptEventStart(int iSkin, int iIndex);

		void OnRemoteDisconnect();
		void OnDisconnect();

		void SetGameOnline();
		void SetGameOffline();
		BOOL IsGameOnline();

#ifdef ENABLE_HUNTING_SYSTEM
	public:
		bool SendHuntingAction(BYTE bAction, DWORD dValue);

	protected:
		bool RecvHuntingOpenWindowMain();
		bool RecvHuntingOpenWindowSelect();
		bool RecvHuntingOpenWindowReward();
		bool RecvHuntingUpdate();
		bool RecvHuntingRandomItems();
#endif

	protected:
		bool CheckPacket(TPacketHeader * pRetHeader);

		void __InitializeGamePhase();
		void __InitializeMarkAuth();
		void __GlobalPositionToLocalPosition(LONG& rGlobalX, LONG& rGlobalY);
		void __LocalPositionToGlobalPosition(LONG& rLocalX, LONG& rLocalY);

		bool __IsPlayerAttacking();
		bool __IsEquipItemInSlot(TItemPos Cell);

		void __ShowMapName(LONG lLocalX, LONG lLocalY);

		void __LeaveOfflinePhase() {}
		void __LeaveHandshakePhase() {}
		void __LeaveLoginPhase() {}
		void __LeaveSelectPhase() {}
		void __LeaveLoadingPhase() {}
		void __LeaveGamePhase();

		void __ClearNetworkActorManager();

		void __ClearSelectCharacterData();

		// DELETEME
		//void __SendWarpPacket();

		void __ConvertEmpireText(DWORD dwEmpireID, char* szText);

		void __RecvCharacterAppendPacket(SNetworkActorData * pkNetActorData);
		void __RecvCharacterUpdatePacket(SNetworkUpdateActorData * pkNetUpdateActorData);

		void __FilterInsult(char* szLine, UINT uLineLen);

		void __SetGuildID(DWORD id);

	protected:
		TPacketGCHandshake m_HandshakeData;
		DWORD m_dwChangingPhaseTime;
		DWORD m_dwBindupRetryCount;
		DWORD m_dwMainActorVID;
		DWORD m_dwMainActorRace;
		DWORD m_dwMainActorEmpire;
		DWORD m_dwMainActorSkillGroup;
		BOOL m_isGameOnline;
		BOOL m_isStartGame;

		DWORD m_dwGuildID;
		DWORD m_dwEmpireID;

		struct SServerTimeSync
		{
			DWORD m_dwChangeServerTime;
			DWORD m_dwChangeClientTime;
		} m_kServerTimeSync;

		void __ServerTimeSync_Initialize();
		//DWORD m_dwBaseServerTime;
		//DWORD m_dwBaseClientTime;

		DWORD m_dwLastGamePingTime;

		std::string	m_stID;
		std::string	m_stPassword;
		std::string	m_strLastCommand;
		std::string	m_strPhase;
		DWORD m_dwLoginKey;
		BOOL m_isWaitLoginKey;

		std::string m_stMarkIP;

		CFuncObject<CPythonNetworkStream>	m_phaseProcessFunc;
		CFuncObject<CPythonNetworkStream>	m_phaseLeaveFunc;

		PyObject*							m_poHandler;
		PyObject*							m_apoPhaseWnd[PHASE_WINDOW_NUM];
		PyObject*							m_poSerCommandParserWnd;
#ifdef ENABLE_NEW_FISHING_SYSTEM
		bool m_phaseWindowGame;
#endif
		TSimplePlayerInformation			m_akSimplePlayerInfo[PLAYER_PER_ACCOUNT4];
		DWORD								m_adwGuildID[PLAYER_PER_ACCOUNT4];
		std::string							m_astrGuildName[PLAYER_PER_ACCOUNT4];
		bool m_bSimplePlayerInfo;

		CRef<CNetworkActorManager>			m_rokNetActorMgr;

		bool m_isRefreshStatus;
		bool m_isRefreshCharacterWnd;
		bool m_isRefreshEquipmentWnd;
		bool m_isRefreshInventoryWnd;
		bool m_isRefreshExchangeWnd;
		bool m_isRefreshSkillWnd;
		bool m_isRefreshSafeboxWnd;
		bool m_isRefreshMallWnd;
		bool m_isRefreshMessengerWnd;
		bool m_isRefreshGuildWndInfoPage;
		bool m_isRefreshGuildWndBoardPage;
		bool m_isRefreshGuildWndMemberPage;
		bool m_isRefreshGuildWndMemberPageGradeComboBox;
		bool m_isRefreshGuildWndSkillPage;
		bool m_isRefreshGuildWndGradePage;
#ifdef ENABLE_AFFECT_FIX
		bool m_isRefreshAffectWindow;
#endif
#ifdef ENABLE_PRIVATESHOP_SEARCH_SYSTEM
		bool m_isRefreshShopSearchWnd;
#endif
		// Emoticon
		std::vector<std::string> m_EmoticonStringVector;

		struct STextConvertTable
		{
			char acUpper[26];
			char acLower[26];
			BYTE aacHan[5000][2];
		} m_aTextConvTable[3];



		struct SMarkAuth
		{
			CNetworkAddress m_kNetAddr;
			DWORD m_dwHandle;
			DWORD m_dwRandomKey;
		} m_kMarkAuth;



		DWORD m_dwSelectedCharacterIndex;

		CInsultChecker m_kInsultChecker;

		bool m_isEnableChatInsultFilter;
		bool m_bComboSkillFlag;

		std::deque<std::string> m_kQue_stHack;

	private:
		struct SDirectEnterMode
		{
			bool m_isSet;
			DWORD m_dwChrSlotIndex;
		} m_kDirectEnterMode;

		void __DirectEnterMode_Initialize();
		void __DirectEnterMode_Set(UINT uChrSlotIndex);
		bool __DirectEnterMode_IsSet();

	public:
		bool DirectEnterMode_IsSet() { return __DirectEnterMode_IsSet(); }

	public:
		DWORD EXPORT_GetBettingGuildWarValue(const char* c_szValueName);

	private:
		struct SBettingGuildWar
		{
			DWORD m_dwBettingMoney;
			DWORD m_dwObserverCount;
		} m_kBettingGuildWar;

		CInstanceBase * m_pInstTarget;

		void __BettingGuildWar_Initialize();
		void __BettingGuildWar_SetObserverCount(UINT uObserverCount);
		void __BettingGuildWar_SetBettingMoney(UINT uBettingMoney);
#if defined(ENABLE_CSHIELD) && !defined(_DEBUG)
	public:
		bool SendCShieldPacket(bool start, DWORD num);

	protected:
		bool RecvCShieldPacket();
#endif
#ifdef ENABLE_SWITCHBOT
	public:
		bool RecvSwitchbotPacket();

		bool SendSwitchbotStartPacket(BYTE slot, std::vector<CPythonSwitchbot::TSwitchbotAttributeAlternativeTable> alternatives);
		bool SendSwitchbotStopPacket(BYTE slot);
#endif
#ifdef ENABLE_ANTY_WAIT_HACK
	public:
		bool RecvAntyWaitHack();
		void LoadMotion(std::string name, const char * race);
		std::map<std::string, std::string> GetMotions() { return motions; }
	protected:
		std::map<std::string, std::string> motions;
#endif

#ifdef ENABLE_PRIVATESHOP_SEARCH_SYSTEM
	public:
#ifdef ENABLE_CHEQUE_SYSTEM
		bool SendShopSearchInfo(BYTE bItemType, BYTE bItemSubType,long lMinGold, long lMaxGold, int bMinCheque, int bMaxCheque, const char* c_szItemName, bool bNameOnly);
#else                                           
		bool SendShopSearchInfo(BYTE bItemType, BYTE bItemSubType,long lMinGold, long lMaxGold, const char* c_szItemName, bool bNameOnly);
#endif
		bool SendShopSerchBuyItem(DWORD dwShopVID, BYTE bItemPos);
		bool RecvShopSearchItemInfo();
#endif
};
