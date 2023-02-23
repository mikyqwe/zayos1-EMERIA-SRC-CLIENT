#pragma once

#include "InstanceBase.h"
#include "Locale_inc.h"
struct SNetworkActorData
{
	std::string m_stName;
#ifdef ENABLE_MULTI_LANGUAGE_SYSTEM
	std::string m_stLanguage;
#endif
	CAffectFlagContainer	m_kAffectFlags;

	BYTE	m_bType;
	DWORD	m_dwVID;
	DWORD	m_dwStateFlags;
	DWORD	m_dwEmpireID;
	DWORD	m_dwRace;
	DWORD	m_dwMovSpd;
	DWORD	m_dwAtkSpd;
	FLOAT	m_fRot;
	LONG	m_lCurX;
	LONG	m_lCurY;
	LONG	m_lSrcX;
	LONG	m_lSrcY;
	LONG	m_lDstX;
	LONG	m_lDstY;


	DWORD	m_dwServerSrcTime;
	DWORD	m_dwClientSrcTime;
	DWORD	m_dwDuration;

	DWORD	m_dwArmor;
	DWORD	m_dwWeapon;
	DWORD	m_dwHair;
#ifdef ENABLE_NEW_ARROW_SYSTEM
	DWORD	m_dwArrowType;
#endif
#ifdef ENABLE_ACCE_COSTUME_SYSTEM
	DWORD	m_dwAcce;
#endif
#ifdef ENABLE_AURA_SYSTEM
	DWORD	m_dwAura;
#endif
	DWORD	m_dwOwnerVID;

	short	m_sAlignment;
	BYTE	m_byPKMode;
	DWORD	m_dwMountVnum;
#ifdef GUILD_RANK_EFFECT
	DWORD	guildRank;
#endif
	DWORD	m_dwGuildID;
	DWORD	m_dwLevel;
#ifdef ENABLE_EFFECT_COSTUME_SYSTEM
	DWORD	m_dwEquipEffect;
#endif
#ifdef ___DECORUM___
	DWORD	m_dwDecorumPoint;
#endif
	SNetworkActorData();

	void SetDstPosition(DWORD dwServerTime, LONG lDstX, LONG lDstY, DWORD dwDuration);
	void SetPosition(LONG lPosX, LONG lPosY);
	void UpdatePosition();

	// NETWORK_ACTOR_DATA_COPY
	SNetworkActorData(const SNetworkActorData& src);
	void operator=(const SNetworkActorData& src);
	void __copy__(const SNetworkActorData& src);
	// END_OF_NETWORK_ACTOR_DATA_COPY

};

struct SNetworkMoveActorData
{
	DWORD	m_dwVID;
	DWORD	m_dwTime;
	LONG	m_lPosX;
	LONG	m_lPosY;
	float	m_fRot;
	DWORD	m_dwFunc;
	DWORD	m_dwArg;
	DWORD	m_dwDuration;

	SNetworkMoveActorData()
	{
		m_dwVID=0;
		m_dwTime=0;
		m_fRot=0.0f;
		m_lPosX=0;
		m_lPosY=0;
		m_dwFunc=0;
		m_dwArg=0;
		m_dwDuration=0;
	}
};




struct SNetworkUpdateActorData
{
	DWORD m_dwVID;
	DWORD m_dwGuildID;
	DWORD m_dwArmor;
	DWORD m_dwWeapon;
	DWORD m_dwHair;
#ifdef ENABLE_ACCE_COSTUME_SYSTEM
	DWORD	m_dwAcce;
#endif
#ifdef ENABLE_AURA_SYSTEM
	DWORD	m_dwAura;
#endif
#ifdef ENABLE_NEW_ARROW_SYSTEM
	DWORD m_dwArrowType;
#endif
	DWORD m_dwMovSpd;
	DWORD m_dwAtkSpd;
#ifdef NEW_PET_SYSTEM
	DWORD	m_dwLevel;
#endif
	short m_sAlignment;
	BYTE m_byPKMode;
	DWORD m_dwMountVnum;
#ifdef GUILD_RANK_EFFECT
	DWORD	guildRank;
#endif
	DWORD m_dwStateFlags; // ���� Create ���� ���̴� ������
#ifdef ENABLE_EFFECT_COSTUME_SYSTEM
	DWORD	m_dwEquipEffect;
#endif
#ifdef ___DECORUM___
	DWORD m_dwDecorumPoint;
#endif	
	CAffectFlagContainer m_kAffectFlags;
	SNetworkUpdateActorData()
	{
		m_dwGuildID=0;
		m_dwVID=0;
		m_dwArmor=0;
		m_dwWeapon=0;
		m_dwHair=0;
#ifdef ENABLE_ACCE_COSTUME_SYSTEM
		m_dwAcce = 0;
#endif
#ifdef ENABLE_AURA_SYSTEM
		m_dwAura = 0;
#endif
#ifdef ENABLE_NEW_ARROW_SYSTEM
		m_dwArrowType = 0;
#endif
#ifdef ENABLE_EFFECT_COSTUME_SYSTEM
		m_dwEquipEffect=0;
#endif
#ifdef ___DECORUM___
		m_dwDecorumPoint = 0;
#endif
		m_dwMovSpd=0;
		m_dwAtkSpd=0;
		m_sAlignment=0;
		m_byPKMode=0;
		m_dwMountVnum=0;
#ifdef GUILD_RANK_EFFECT
		guildRank = 0;
#endif
		m_dwStateFlags=0;
		m_kAffectFlags.Clear();
	}
};

class CPythonCharacterManager;

class CNetworkActorManager : public CReferenceObject
{
	public:
		CNetworkActorManager();
		virtual ~CNetworkActorManager();

		void Destroy();

		void SetMainActorVID(DWORD dwVID);

		void RemoveActor(DWORD dwVID);
		void AppendActor(const SNetworkActorData& c_rkNetActorData);
		void UpdateActor(const SNetworkUpdateActorData& c_rkNetUpdateActorData);
		void MoveActor(const SNetworkMoveActorData& c_rkNetMoveActorData);

		void SyncActor(DWORD dwVID, LONG lPosX, LONG lPosY);
		void SetActorOwner(DWORD dwOwnerVID, DWORD dwVictimVID);

		void Update();

	protected:
		void __OLD_Update();

		void __UpdateMainActor();

		bool __IsVisiblePos(LONG lPosX, LONG lPosY);
		bool __IsVisibleActor(const SNetworkActorData& c_rkNetActorData);
		bool __IsMainActorVID(DWORD dwVID);

		void __RemoveAllGroundItems();
		void __RemoveAllActors();
		void __RemoveDynamicActors();
		void __RemoveCharacterManagerActor(SNetworkActorData& rkNetActorData);

		SNetworkActorData* __FindActorData(DWORD dwVID);

		CInstanceBase* __AppendCharacterManagerActor(SNetworkActorData& rkNetActorData);
		CInstanceBase* __FindActor(SNetworkActorData& rkNetActorData);
		CInstanceBase* __FindActor(SNetworkActorData& rkNetActorData, LONG lDstX, LONG lDstY);

		CPythonCharacterManager& __GetCharacterManager();

	protected:
		DWORD m_dwMainVID;

		LONG m_lMainPosX;
		LONG m_lMainPosY;

		std::map<DWORD, SNetworkActorData> m_kNetActorDict;
};
