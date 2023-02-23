#include "StdAfx.h"
#include "InstanceBase.h"
#include "PythonBackground.h"
#include "PythonNonPlayer.h"
#include "PythonPlayer.h"
#include "PythonCharacterManager.h"
#include "AbstractPlayer.h"
#include "AbstractApplication.h"
#include "packet.h"

#include "../eterlib/StateManager.h"
#include "../gamelib/ItemManager.h"
#include "../gamelib/RaceManager.h"
#include "../gamelib/GameLibDefines.h"
#include "Locale_inc.h"
#ifdef WJ_SHOW_MOB_INFO
#include "PythonSystem.h"
#include "PythonTextTail.h"
#endif

BOOL HAIR_COLOR_ENABLE=FALSE;
BOOL USE_ARMOR_SPECULAR=FALSE;
BOOL RIDE_HORSE_ENABLE=TRUE;
const float c_fDefaultRotationSpeed = 1200.0f;
const float c_fDefaultHorseRotationSpeed = 1500.0f;
// #define ENABLE_NO_MOUNT_CHECK

bool IsWall(unsigned race)
{
	switch (race)
	{
		case 14201:
		case 14202:
		case 14203:
		case 14204:
			return true;
			break;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////

CInstanceBase::SHORSE::SHORSE()
{
	__Initialize();
}

CInstanceBase::SHORSE::~SHORSE()
{
	assert(m_pkActor==NULL);
}

void CInstanceBase::SHORSE::__Initialize()
{
	m_isMounting=false;
	m_pkActor=NULL;
}

void CInstanceBase::SHORSE::SetAttackSpeed(UINT uAtkSpd)
{
	if (!IsMounting())
		return;

	CActorInstance& rkActor=GetActorRef();
	rkActor.SetAttackSpeed(uAtkSpd/100.0f);
}

void CInstanceBase::SHORSE::SetMoveSpeed(UINT uMovSpd)
{
	if (!IsMounting())
		return;

	CActorInstance& rkActor=GetActorRef();
	rkActor.SetMoveSpeed(uMovSpd/100.0f);
}

void CInstanceBase::SHORSE::Create(const TPixelPosition& c_rkPPos, UINT eRace, UINT eHitEffect)
{
	assert(NULL==m_pkActor && "CInstanceBase::SHORSE::Create - ALREADY MOUNT");

	m_pkActor=new CActorInstance;

	CActorInstance& rkActor=GetActorRef();
	rkActor.SetEventHandler(CActorInstance::IEventHandler::GetEmptyPtr());
	if (!rkActor.SetRace(eRace))
	{
		delete m_pkActor;
		m_pkActor=NULL;
		return;
	}

	rkActor.SetShape(0);
	rkActor.SetBattleHitEffect(eHitEffect);
	rkActor.SetAlphaValue(0.0f);
	rkActor.BlendAlphaValue(1.0f, 0.5f);
	rkActor.SetMoveSpeed(1.0f);
	rkActor.SetAttackSpeed(1.0f);
	rkActor.SetMotionMode(CRaceMotionData::MODE_GENERAL);
	rkActor.Stop();
	rkActor.RefreshActorInstance();

	rkActor.SetCurPixelPosition(c_rkPPos);

	m_isMounting=true;
}

void CInstanceBase::SHORSE::Destroy()
{
	if (m_pkActor)
	{
		m_pkActor->Destroy();
		delete m_pkActor;
	}

	__Initialize();
}

CActorInstance& CInstanceBase::SHORSE::GetActorRef()
{
	assert(NULL!=m_pkActor && "CInstanceBase::SHORSE::GetActorRef");
	return *m_pkActor;
}

CActorInstance* CInstanceBase::SHORSE::GetActorPtr()
{
	return m_pkActor;
}

enum eMountType {MOUNT_TYPE_NONE=0, MOUNT_TYPE_NORMAL=1, MOUNT_TYPE_COMBAT=2, MOUNT_TYPE_MILITARY=3};
eMountType GetMountLevelByVnum(DWORD dwMountVnum, bool IsNew)
{
	if (!dwMountVnum)
		return MOUNT_TYPE_NONE;

	return MOUNT_TYPE_MILITARY;
	/*
	switch (dwMountVnum)
	{
		// ### YES SKILL
		// @fixme116 begin
		case 20107: // normal military horse (no guild)
		case 20108: // normal military horse (guild member)
		case 20109: // normal military horse (guild master)
			if (IsNew)
				return MOUNT_TYPE_NONE;
		// @fixme116 end
		// Classic
		case 20110: // Classic Boar
		case 20111: // Classic Wolf
		case 20112: // Classic Tiger
		case 20113: // Classic Lion
		case 20114: // White Lion
		// Special Lv2
		case 20115: // Wild Battle Boar
		case 20116: // Fight Wolf
		case 20117: // Storm Tiger
		case 20118: // Battle Lion (bugged)
		case 20205: // Wild Battle Boar (alternative)
		case 20206: // Fight Wolf (alternative)
		case 20207: // Storm Tiger (alternative)
		case 20208: // Battle Lion (bugged) (alternative)
		// Royal Tigers
		case 20120: // blue
		case 20121: // dark red
		case 20122: // gold
		case 20123: // green
		case 20124: // pied
		case 20125: // white
		// Royal mounts (Special Lv3)
		case 20209: // Royal Boar
		case 20210: // Royal Wolf
		case 20211: // Royal Tiger
		case 20212: // Royal Lion
		//
		case 20215: // Rudolph m Lv3 (yes skill, yes atk)
		case 20218: // Rudolph f Lv3 (yes skill, yes atk)
		case 20225: // Dyno Lv3 (yes skill, yes atk)
		case 20230: // Turkey Lv3 (yes skill, yes atk)
		case 20226: // Inferno (yes skill, yes atk)
			return MOUNT_TYPE_MILITARY;

		// ### NO SKILL YES ATK
		// @fixme116 begin
		case 20104: // normal combat horse (no guild)
		case 20105: // normal combat horse (guild member)
		case 20106: // normal combat horse (guild master)
			if (IsNew)
				return MOUNT_TYPE_NONE;
		// @fixme116 end
		case 20119: // Black Horse (no skill, yes atk)
		case 20214: // Rudolph m Lv2 (no skill, yes atk)
		case 20217: // Rudolph f Lv2 (no skill, yes atk)
		case 20219: // Equus Porphyreus (no skill, yes atk)
		case 20220: // Comet (no skill, yes atk)
		case 20221: // Polar Predator (no skill, yes atk)
		case 20222: // Armoured Panda (no skill, yes atk)
		case 20224: // Dyno Lv2 (no skill, yes atk)
		case 20226: // Nightmare (no skill, yes atk)
		case 20227: // Unicorn (no skill, yes atk)
		case 20229: // Turkey Lv2 (no skill, yes atk)
		case 20231: // Leopard (no skill, yes atk)
		case 20232: // Black Panther (no skill, yes atk)
			return MOUNT_TYPE_COMBAT;

		// ### NO SKILL NO ATK
		// @fixme116 begin
		case 20101: // normal beginner horse (no guild)
		case 20102: // normal beginner horse (guild member)
		case 20103: // normal beginner horse (guild master)
			if (IsNew)
				return MOUNT_TYPE_NONE;
		// @fixme116 end
		case 20213: // Rudolph m Lv1 (no skill, no atk)
		case 20216: // Rudolph f Lv1 (no skill, no atk)
		// Special Lv1
		case 20201: // Boar Lv1 (no skill, no atk)
		case 20202: // Wolf Lv1 (no skill, no atk)
		case 20203: // Tiger Lv1 (no skill, no atk)
		case 20204: // Lion Lv1 (no skill, no atk)
		//
		case 20223: // Dyno Lv1 (no skill, no atk)
		case 20228: // Turkey Lv1 (no skill, no atk)
			return MOUNT_TYPE_NORMAL;

		default:
			return MOUNT_TYPE_NONE;

	}
	*/
}

UINT CInstanceBase::SHORSE::GetLevel()
{
	if (m_pkActor)
	{
#ifndef ENABLE_NO_MOUNT_CHECK
		return static_cast<UINT>(GetMountLevelByVnum(m_pkActor->GetRace(), false));
#else
		return (m_pkActor->GetRace()) ? MOUNT_TYPE_MILITARY : MOUNT_TYPE_NONE;
#endif
	}
	return 0;
}

bool CInstanceBase::SHORSE::IsNewMount()
{
#ifndef ENABLE_NO_MOUNT_CHECK
	if (m_pkActor)
	{
		DWORD dwMountVnum = m_pkActor->GetRace();
		eMountType mountType = GetMountLevelByVnum(dwMountVnum, true);
		return (mountType != MOUNT_TYPE_NONE) && (mountType != MOUNT_TYPE_NORMAL);
	}
#endif
	return false;
}
bool CInstanceBase::SHORSE::CanUseSkill()
{
	// ����ų�� ���� ������ 3 �̻��̾�߸￿ ��.
	if (IsMounting())
		return 2 < GetLevel();

	return true;
}

bool CInstanceBase::SHORSE::CanAttack()
{
	if (IsMounting())
		if (GetLevel()<=1)
			return false;

	return true;
}

bool CInstanceBase::SHORSE::IsMounting()
{
	return m_isMounting;
}

void CInstanceBase::SHORSE::Deform()
{
	if (!IsMounting())
		return;

	CActorInstance& rkActor=GetActorRef();
	rkActor.INSTANCEBASE_Deform();
}

void CInstanceBase::SHORSE::Render()
{
	if (!IsMounting())
		return;

	CActorInstance& rkActor=GetActorRef();
	rkActor.Render();
}

void CInstanceBase::__AttachHorseSaddle()
{
	if (!IsMountingHorse())
		return;
	m_kHorse.m_pkActor->AttachModelInstance(CRaceData::PART_MAIN, "saddle", m_GraphicThingInstance, CRaceData::PART_MAIN);
}

void CInstanceBase::__DetachHorseSaddle()
{
	if (!IsMountingHorse())
		return;
	m_kHorse.m_pkActor->DetachModelInstance(CRaceData::PART_MAIN, m_GraphicThingInstance, CRaceData::PART_MAIN);
}

//////////////////////////////////////////////////////////////////////////////////////

void CInstanceBase::BlockMovement()
{
	m_GraphicThingInstance.BlockMovement();
}

bool CInstanceBase::IsBlockObject(const CGraphicObjectInstance& c_rkBGObj)
{
	return m_GraphicThingInstance.IsBlockObject(c_rkBGObj);
}

bool CInstanceBase::AvoidObject(const CGraphicObjectInstance& c_rkBGObj)
{
	return m_GraphicThingInstance.AvoidObject(c_rkBGObj);
}

///////////////////////////////////////////////////////////////////////////////////

bool __ArmorVnumToShape(int iVnum, DWORD * pdwShape)
{
	*pdwShape = iVnum;

	/////////////////////////////////////////

	if (0 == iVnum || 1 == iVnum)
		return false;

	if (!USE_ARMOR_SPECULAR)
		return false;

	CItemData * pItemData;
	if (!CItemManager::Instance().GetItemDataPointer(iVnum, &pItemData))
		return false;

	enum
	{
		SHAPE_VALUE_SLOT_INDEX = 3,
	};

	*pdwShape = pItemData->GetValue(SHAPE_VALUE_SLOT_INDEX);

	return true;
}

// 2004.07.05.myevan.�ý�ź�� ���̴� ����
class CActorInstanceBackground : public IBackground
{
	public:
		CActorInstanceBackground() {}
		virtual ~CActorInstanceBackground() {}
		bool IsBlock(int x, int y)
		{
			CPythonBackground& rkBG=CPythonBackground::Instance();
			return rkBG.isAttrOn(x, y, CTerrainImpl::ATTRIBUTE_BLOCK);
		}
};

static CActorInstanceBackground gs_kActorInstBG;

bool CInstanceBase::LessRenderOrder(CInstanceBase* pkInst)
{
	int nMainAlpha=(__GetAlphaValue() < 1.0f) ? 1 : 0;
	int nTestAlpha=(pkInst->__GetAlphaValue() < 1.0f) ? 1 : 0;
	if (nMainAlpha < nTestAlpha)
		return true;
	if (nMainAlpha > nTestAlpha)
		return false;

	if (GetRace()<pkInst->GetRace())
		return true;
	if (GetRace()>pkInst->GetRace())
		return false;

	if (GetShape()<pkInst->GetShape())
		return true;

	if (GetShape()>pkInst->GetShape())
		return false;

	UINT uLeftLODLevel=__LessRenderOrder_GetLODLevel();
	UINT uRightLODLevel=pkInst->__LessRenderOrder_GetLODLevel();
	if (uLeftLODLevel<uRightLODLevel)
		return true;
	if (uLeftLODLevel>uRightLODLevel)
		return false;

	if (m_awPart[CRaceData::PART_WEAPON]<pkInst->m_awPart[CRaceData::PART_WEAPON])
		return true;

	return false;
}

UINT CInstanceBase::__LessRenderOrder_GetLODLevel()
{
	CGrannyLODController* pLODCtrl=m_GraphicThingInstance.GetLODControllerPointer(0);
	if (!pLODCtrl)
		return 0;

	return pLODCtrl->GetLODLevel();
}

bool CInstanceBase::__Background_GetWaterHeight(const TPixelPosition& c_rkPPos, float* pfHeight)
{
	long lHeight;
	if (!CPythonBackground::Instance().GetWaterHeight(int(c_rkPPos.x), int(c_rkPPos.y), &lHeight))
		return false;

	*pfHeight = float(lHeight);

	return true;
}

bool CInstanceBase::__Background_IsWaterPixelPosition(const TPixelPosition& c_rkPPos)
{
	return CPythonBackground::Instance().isAttrOn(c_rkPPos.x, c_rkPPos.y, CTerrainImpl::ATTRIBUTE_WATER);
}

const float PC_DUST_RANGE = 2000.0f;
const float NPC_DUST_RANGE = 1000.0f;

DWORD CInstanceBase::ms_dwUpdateCounter=0;
DWORD CInstanceBase::ms_dwRenderCounter=0;
DWORD CInstanceBase::ms_dwDeformCounter=0;

CDynamicPool<CInstanceBase> CInstanceBase::ms_kPool;

bool CInstanceBase::__IsInDustRange()
{
	if (!__IsExistMainInstance())
		return false;

	CInstanceBase* pkInstMain=__GetMainInstancePtr();

	float fDistance=NEW_GetDistanceFromDestInstance(*pkInstMain);

	if (IsPC())
	{
		if (fDistance<=PC_DUST_RANGE)
			return true;
	}

	if (fDistance<=NPC_DUST_RANGE)
		return true;

	return false;
}

void CInstanceBase::__EnableSkipCollision()
{
	if (__IsMainInstance())
	{
		TraceError("CInstanceBase::__EnableSkipCollision - You should not skip your own collisions!!");
		return;
	}
	m_GraphicThingInstance.EnableSkipCollision();
}

void CInstanceBase::__DisableSkipCollision()
{
	m_GraphicThingInstance.DisableSkipCollision();
}

DWORD CInstanceBase::__GetShadowMapColor(float x, float y)
{
	CPythonBackground& rkBG=CPythonBackground::Instance();
	return rkBG.GetShadowMapColor(x, y);
}

float CInstanceBase::__GetBackgroundHeight(float x, float y)
{
	CPythonBackground& rkBG=CPythonBackground::Instance();
	return rkBG.GetHeight(x, y);
}

#ifdef __MOVIE_MODE__

BOOL CInstanceBase::IsMovieMode()
{
#ifdef ENABLE_CANSEEHIDDENTHING_FOR_GM
	if (IsAffect(AFFECT_INVISIBILITY) && !__MainCanSeeHiddenThing())
		return true;
#else
	if (IsAffect(AFFECT_INVISIBILITY))
		return true;
#endif

	return false;
}

#endif

BOOL CInstanceBase::IsInvisibility()
{
#ifdef ENABLE_CANSEEHIDDENTHING_FOR_GM
	if (IsAffect(AFFECT_INVISIBILITY) && !__MainCanSeeHiddenThing())
		return true;
#else
	if (IsAffect(AFFECT_INVISIBILITY))
		return true;
#endif

	return false;
}

BOOL CInstanceBase::IsParalysis()
{
	return m_GraphicThingInstance.IsParalysis();
}

BOOL CInstanceBase::IsGameMaster()
{
	if (m_kAffectFlagContainer.IsSet(AFFECT_YMIR))
		return true;
	return false;
}

void CInstanceBase::RenderAllAttachingEffect()
{
	m_GraphicThingInstance.RenderAllAttachingEffect();
}

void CInstanceBase::SetCanSelect(bool value)
{
	m_CanSelect = value;
}

BOOL CInstanceBase::IsSameEmpire(CInstanceBase& rkInstDst)
{
	if (0 == rkInstDst.m_dwEmpireID)
		return TRUE;

	if (IsGameMaster())
		return TRUE;

	if (rkInstDst.IsGameMaster())
		return TRUE;

	if (rkInstDst.m_dwEmpireID==m_dwEmpireID)
		return TRUE;

	return FALSE;
}

DWORD CInstanceBase::GetEmpireID()
{
	return m_dwEmpireID;
}

DWORD CInstanceBase::GetGuildID()
{
	return m_dwGuildID;
}

int CInstanceBase::GetAlignment()
{
	return m_sAlignment;
}

UINT CInstanceBase::GetAlignmentGrade()
{
	if (m_sAlignment >= 12000)
		return 0;
	else if (m_sAlignment >= 8000)
		return 1;
	else if (m_sAlignment >= 4000)
		return 2;
	else if (m_sAlignment >= 1000)
		return 3;
	else if (m_sAlignment >= 0)
		return 4;
	else if (m_sAlignment > -4000)
		return 5;
	else if (m_sAlignment > -8000)
		return 6;
	else if (m_sAlignment > -12000)
		return 7;

	return 8;
}

int CInstanceBase::GetAlignmentType()
{
	switch (GetAlignmentGrade())
	{
		case 0:
		case 1:
		case 2:
		case 3:
		{
			return ALIGNMENT_TYPE_WHITE;
			break;
		}

		case 5:
		case 6:
		case 7:
		case 8:
		{
			return ALIGNMENT_TYPE_DARK;
			break;
		}
	}

	return ALIGNMENT_TYPE_NORMAL;
}


BYTE CInstanceBase::GetPKMode()
{
	return m_byPKMode;
}

bool CInstanceBase::IsKiller()
{
	return m_isKiller;
}

bool CInstanceBase::IsPartyMember()
{
	return m_isPartyMember;
}

BOOL CInstanceBase::IsInSafe()
{
	const TPixelPosition& c_rkPPosCur=m_GraphicThingInstance.NEW_GetCurPixelPositionRef();
	if (CPythonBackground::Instance().isAttrOn(c_rkPPosCur.x, c_rkPPosCur.y, CTerrainImpl::ATTRIBUTE_BANPK))
		return TRUE;

	return FALSE;
}

float CInstanceBase::CalculateDistanceSq3d(const TPixelPosition& c_rkPPosDst)
{
	const TPixelPosition& c_rkPPosSrc=m_GraphicThingInstance.NEW_GetCurPixelPositionRef();
	return SPixelPosition_CalculateDistanceSq3d(c_rkPPosSrc, c_rkPPosDst);
}

void CInstanceBase::OnSelected()
{
#ifdef __MOVIE_MODE__
	if (!__IsExistMainInstance())
		return;
#endif
	if (!m_CanSelect)
		return;

	if (IsStoneDoor())
		return;

	if (IsDead())
		return;

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	// if (IsOfflineShopNPC())
	// __AttachOfflineShopSelectEffect();
	// else
	__AttachSelectEffect();
#else
	__AttachSelectEffect();
#endif
}

void CInstanceBase::OnUnselected()
{
#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	// if (IsOfflineShopNPC())
	// __DetachOfflineShopSelectEffect();
	// else
	__DetachSelectEffect();
#else
	__DetachSelectEffect();
#endif
}

#ifdef ENABLE_HIDE_PET
BOOL CInstanceBase::IsPet()
{
	if (GetRace() >= 34001 && GetRace() <= 34999)
		return true;
	return false;
}
#endif
#ifdef ENABLE_HIDE_MOUNT
BOOL CInstanceBase::IsMount()
{
	if (GetRace() >= 20110 && GetRace() <= 20299)
		return true;
	return false;
}
#endif
#ifdef ENABLE_HIDE_SHOPS
BOOL CInstanceBase::IsShops()
{
	if (GetRace() >= 30000 && GetRace() <= 30007)
		return true;
	return false;
}
#endif

void CInstanceBase::OnTargeted()
{
#ifdef __MOVIE_MODE__
	if (!__IsExistMainInstance())
		return;
#endif

	if (IsStoneDoor())
		return;

	if (IsDead())
		return;

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	// if (IsOfflineShopNPC())
	// __AttachOfflineShopTargetEffect();
	// else
	__AttachTargetEffect();
#else
	__AttachTargetEffect();
#endif
}

void CInstanceBase::OnUntargeted()
{
#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	// if (IsOfflineShopNPC())
	// __DetachOfflineShopTargetEffect();
	// else
	__DetachTargetEffect();
#else
	__DetachTargetEffect();
#endif
}

void CInstanceBase::DestroySystem()
{
	ms_kPool.Clear();
}

void CInstanceBase::CreateSystem(UINT uCapacity)
{
	ms_kPool.Create(uCapacity);

	memset(ms_adwCRCAffectEffect, 0, sizeof(ms_adwCRCAffectEffect));

	ms_fDustGap=250.0f;
	ms_fHorseDustGap=500.0f;
}

CInstanceBase* CInstanceBase::New()
{
	return ms_kPool.Alloc();
}

void CInstanceBase::Delete(CInstanceBase* pkInst)
{
	pkInst->Destroy();
	ms_kPool.Free(pkInst);
}

void CInstanceBase::SetMainInstance()
{
	CPythonCharacterManager& rkChrMgr=CPythonCharacterManager::Instance();

	DWORD dwVID=GetVirtualID();
	rkChrMgr.SetMainInstance(dwVID);

	m_GraphicThingInstance.SetMainInstance();
}

CInstanceBase* CInstanceBase::__GetMainInstancePtr()
{
	CPythonCharacterManager& rkChrMgr=CPythonCharacterManager::Instance();
	return rkChrMgr.GetMainInstancePtr();
}

void CInstanceBase::__ClearMainInstance()
{
	CPythonCharacterManager& rkChrMgr=CPythonCharacterManager::Instance();
	rkChrMgr.ClearMainInstance();
}

/* ���� �÷��̾� ĳ�������� ����.*/
bool CInstanceBase::__IsMainInstance()
{
	if (this==__GetMainInstancePtr())
		return true;

	return false;
}

bool CInstanceBase::__IsExistMainInstance()
{
	if(__GetMainInstancePtr())
		return true;
	else
		return false;
}

bool CInstanceBase::__MainCanSeeHiddenThing()
{
#ifdef ENABLE_CANSEEHIDDENTHING_FOR_GM
	CInstanceBase * pInstance = __GetMainInstancePtr();
	return (pInstance) ? TRUE == pInstance->IsGameMaster() : false;
#else
	return false;
#endif
}

float CInstanceBase::__GetBowRange()
{
	float fRange = 2500.0f - 100.0f;

	if (__IsMainInstance())
	{
		IAbstractPlayer& rPlayer=IAbstractPlayer::GetSingleton();
		fRange += float(rPlayer.GetStatus(POINT_BOW_DISTANCE));
	}

	return fRange;
}

CInstanceBase* CInstanceBase::__FindInstancePtr(DWORD dwVID)
{
	CPythonCharacterManager& rkChrMgr=CPythonCharacterManager::Instance();
	return rkChrMgr.GetInstancePtr(dwVID);
}

bool CInstanceBase::__FindRaceType(DWORD dwRace, BYTE* pbType)
{
	CPythonNonPlayer& rkNonPlayer=CPythonNonPlayer::Instance();
	return rkNonPlayer.GetInstanceType(dwRace, pbType);
}

bool CInstanceBase::Create(const SCreateData& c_rkCreateData)
{
	//IAbstractApplication::GetSingleton().SkipRenderBuffering(300);	//@MentaLFix Game freezes when a character is loaded [SpeedUp]

	SetInstanceType(c_rkCreateData.m_bType);


#ifdef NEW_PET_SYSTEM
	if (!SetRace(c_rkCreateData.m_dwRace))
		return false;

	if(c_rkCreateData.m_dwRace == 34064 || c_rkCreateData.m_dwRace == 34041 || c_rkCreateData.m_dwRace == 34045 || c_rkCreateData.m_dwRace == 34049 || c_rkCreateData.m_dwRace == 34053)
	{
		if(c_rkCreateData.m_dwLevel >= 81)
			SetRace(c_rkCreateData.m_dwRace+1);
	}
#else
	if (!SetRace(c_rkCreateData.m_dwRace))
		return false;
#endif

	SetVirtualID(c_rkCreateData.m_dwVID);
#ifdef ENABLE_DOG_MODE
	SetOriginalRace(c_rkCreateData.m_dwRace);
#endif

	if (c_rkCreateData.m_isMain)
		SetMainInstance();

	if (IsGuildWall())
	{
		unsigned center_x;
		unsigned center_y;

		c_rkCreateData.m_kAffectFlags.ConvertToPosition(&center_x, &center_y);

		float center_z = __GetBackgroundHeight(center_x, center_y);
		NEW_SetPixelPosition(TPixelPosition(float(c_rkCreateData.m_lPosX), float(c_rkCreateData.m_lPosY), center_z));
	}
	else
	{
		SCRIPT_SetPixelPosition(float(c_rkCreateData.m_lPosX), float(c_rkCreateData.m_lPosY));
	}

	if (0 != c_rkCreateData.m_dwMountVnum)
		MountHorse(c_rkCreateData.m_dwMountVnum);

	SetArmor(c_rkCreateData.m_dwArmor);

	if (IsPC())
	{
		SetHair(c_rkCreateData.m_dwHair);
#ifdef ENABLE_AURA_SYSTEM
		SetAura(c_rkCreateData.m_dwAura);
#endif
#ifdef ENABLE_ACCE_COSTUME_SYSTEM
		SetAcce(c_rkCreateData.m_dwAcce);
#endif
#ifdef ENABLE_NEW_ARROW_SYSTEM
		SetWeapon(c_rkCreateData.m_dwWeapon, c_rkCreateData.m_dwArrowType);
#else
		SetWeapon(c_rkCreateData.m_dwWeapon);
#endif
	}

	__Create_SetName(c_rkCreateData);

#if defined(WJ_SHOW_MOB_INFO) && defined(ENABLE_SHOW_MOBLEVEL)
	if (IsEnemy() && CPythonSystem::Instance().IsShowMobLevel())
		m_dwLevel = CPythonNonPlayer::Instance().GetMonsterLevel(GetRace());
	else
		m_dwLevel = c_rkCreateData.m_dwLevel;
#else
	m_dwLevel = c_rkCreateData.m_dwLevel;
#endif

	m_dwGuildID = c_rkCreateData.m_dwGuildID;
	m_dwEmpireID = c_rkCreateData.m_dwEmpireID;

#ifdef ENABLE_EFFECT_COSTUME_SYSTEM
	SetEquipEffect(c_rkCreateData.m_dwEquipEffect);
#endif
#ifdef ___DECORUM___
	ChangeDecorum(c_rkCreateData.m_dwDecorumPoint);
#endif
	SetVirtualNumber(c_rkCreateData.m_dwRace);
	SetRotation(c_rkCreateData.m_fRot);

	SetAlignment(c_rkCreateData.m_sAlignment);

#ifdef GUILD_RANK_EFFECT
	SetRankAffect(c_rkCreateData.guildRank);
#endif

#ifdef NEW_PET_SYSTEM
	SetLevelText(c_rkCreateData.m_dwLevel);
#endif
	SetPKMode(c_rkCreateData.m_byPKMode);

	SetMoveSpeed(c_rkCreateData.m_dwMovSpd);
	SetAttackSpeed(c_rkCreateData.m_dwAtkSpd);
	
#ifdef NEW_PET_SYSTEM
	if(m_dwRace == 34064 || m_dwRace == 34041 || m_dwRace == 34045 || m_dwRace == 34049 ||m_dwRace == 34053){
		float scale = c_rkCreateData.m_dwLevel * 0.005f + 0.75f;
		m_GraphicThingInstance.SetScaleNew(scale, scale, scale);
	}
	else
		m_GraphicThingInstance.SetScaleNew(1.0f,1.0f,1.0f);
#endif

	// NOTE : Dress �� �԰� ������ Alpha �� ���� �ʴ´�.
	if (!IsWearingDress())
	{
		// NOTE : �ݵ�￿ Affect ���� ���ʿ� �־�￿ ��
		m_GraphicThingInstance.SetAlphaValue(0.0f);
		m_GraphicThingInstance.BlendAlphaValue(1.0f, 0.5f);
	}

#ifdef ENABLE_MOUNT_SCALE
	if (IsMount())
		SetMountScale();
#endif

	if (!IsGuildWall())
	{
		SetAffectFlagContainer(c_rkCreateData.m_kAffectFlags);
	}

	// NOTE : �ݵ�￿ Affect ���� �Ŀ� �ؾ� ��

	if (c_rkCreateData.m_dwStateFlags & ADD_CHARACTER_STATE_SPAWN)
	{
		if (IsAffect(AFFECT_SPAWN))
			__AttachEffect(EFFECT_SPAWN_APPEAR);

		if (IsPC())
		{
			Refresh(CRaceMotionData::NAME_WAIT, true);
		}
		else
		{
			Refresh(CRaceMotionData::NAME_SPAWN, false);
		}
	}
	else
	{
		Refresh(CRaceMotionData::NAME_WAIT, true);
	}

	__AttachEmpireEffect(c_rkCreateData.m_dwEmpireID);

	RegisterBoundingSphere();

	AttachTextTail();
	RefreshTextTail();

	uint32_t dwRank = CPythonNonPlayer::Instance().GetMonsterRank(c_rkCreateData.m_dwRace);
	uint32_t dwType = CPythonNonPlayer::Instance().GetMonsterType(c_rkCreateData.m_dwRace);
	if (dwRank >= 4 && dwType == 0)
		__AttachBossEffect();

	if (c_rkCreateData.m_dwStateFlags & ADD_CHARACTER_STATE_DEAD)
		m_GraphicThingInstance.DieEnd();

	SetStateFlags(c_rkCreateData.m_dwStateFlags);

	m_GraphicThingInstance.SetBattleHitEffect(ms_adwCRCAffectEffect[EFFECT_HIT]);

	if (!IsPC())
	{
		DWORD dwBodyColor = CPythonNonPlayer::Instance().GetMonsterColor(c_rkCreateData.m_dwRace);
		if (0 != dwBodyColor)
		{
			SetModulateRenderMode();
			SetAddColor(dwBodyColor);
		}
	}

	__AttachHorseSaddle();

	// ��￿ �ɺ��� ���� �ӽ� �ڵ�, ���� ��ġ�� ã�� ��
	const int c_iGuildSymbolRace = 14200;
	if (c_iGuildSymbolRace == GetRace())
	{
		std::string strFileName = GetGuildSymbolFileName(m_dwGuildID);
		if (IsFile(strFileName.c_str()))
			m_GraphicThingInstance.ChangeMaterial(strFileName.c_str());
	}

#ifdef ENABLE_CANSEEHIDDENTHING_FOR_GM
	if (IsAffect(AFFECT_INVISIBILITY) && __MainCanSeeHiddenThing())
		m_GraphicThingInstance.BlendAlphaValue(0.5f, 0.5f);
#endif

	return true;
}


void CInstanceBase::__Create_SetName(const SCreateData& c_rkCreateData)
{
	if (IsGoto())
	{
		SetNameString("", 0);
#ifdef ENABLE_MULTI_LANGUAGE_SYSTEM
		SetLanguageString("", 0);
#endif
		return;
	}
	if (IsWarp())
	{
		__Create_SetWarpName(c_rkCreateData);
		return;
	}
#if defined(WJ_SHOW_MOB_INFO) && defined(ENABLE_SHOW_MOBAIFLAG)
	if (IsEnemy() && CPythonSystem::Instance().IsShowMobAIFlag() && CPythonNonPlayer::Instance().IsAggressive(GetRace()))
	{
		std::string strName = c_rkCreateData.m_stName;
		strName += "*";
		SetNameString(strName.c_str(), strName.length());
	}
	else
		SetNameString(c_rkCreateData.m_stName.c_str(), c_rkCreateData.m_stName.length());
		#ifdef ENABLE_MULTI_LANGUAGE_SYSTEM
		SetLanguageString(c_rkCreateData.m_stLanguage.c_str(), c_rkCreateData.m_stLanguage.length());
		#endif
#else
	SetNameString(c_rkCreateData.m_stName.c_str(), c_rkCreateData.m_stName.length());
	#ifdef ENABLE_MULTI_LANGUAGE_SYSTEM
	SetLanguageString(c_rkCreateData.m_stLanguage.c_str(), c_rkCreateData.m_stLanguage.length());
	#endif
#endif
}

void CInstanceBase::__Create_SetWarpName(const SCreateData& c_rkCreateData)
{
	const char * c_szName;
	if (CPythonNonPlayer::Instance().GetName(c_rkCreateData.m_dwRace, &c_szName))
	{
		std::string strName = c_szName;
		int iFindingPos = strName.find_first_of(" ", 0);
		if (iFindingPos > 0)
		{
			strName.resize(iFindingPos);
		}
		SetNameString(strName.c_str(), strName.length());
	}
	else
	{
		SetNameString(c_rkCreateData.m_stName.c_str(), c_rkCreateData.m_stName.length());
	}
}

void CInstanceBase::SetNameString(const char* c_szName, int len)
{
	m_stName.assign(c_szName, len);
}
#ifdef ENABLE_MULTI_LANGUAGE_SYSTEM
void CInstanceBase::SetLanguageString(const char* c_szLang, int len)
{
	m_stLanguage.assign(c_szLang, len);
}
#endif


bool CInstanceBase::SetRace(DWORD eRace)
{
	m_dwRace = eRace;

	if (!m_GraphicThingInstance.SetRace(eRace))
		return false;

	if (!__FindRaceType(m_dwRace, &m_eRaceType))
		m_eRaceType=CActorInstance::TYPE_PC;

	return true;
}

BOOL CInstanceBase::__IsChangableWeapon(int iWeaponID)
{
	// �巹�� �԰� �������� ���ɿ��� ���� ������ �ʰ�..
	if (IsWearingDress())
	{
		const int c_iBouquets[] =
		{
			50201,	// Bouquet for Assassin
			50202,	// Bouquet for Shaman
			50203,
			50204,
			0, // #0000545: [M2CN] ���� �巹���� ��￿ ���� ����
		};

		for (int i = 0; c_iBouquets[i] != 0; ++i)
			if (iWeaponID == c_iBouquets[i])
				return true;

		return false;
	}
	else
		return true;
}

BOOL CInstanceBase::IsWearingDress()
{
	const int c_iWeddingDressShape = 201;
	return c_iWeddingDressShape == m_eShape;
}

BOOL CInstanceBase::IsHoldingPickAxe()
{
	const int c_iPickAxeStart = 29101;
	const int c_iPickAxeEnd = 29110;
	return m_awPart[CRaceData::PART_WEAPON] >= c_iPickAxeStart && m_awPart[CRaceData::PART_WEAPON] <= c_iPickAxeEnd;
}

BOOL CInstanceBase::IsNewMount()
{
	return m_kHorse.IsNewMount();
}

BOOL CInstanceBase::IsMountingHorse()
{
	return m_kHorse.IsMounting();
}

void CInstanceBase::MountHorse(UINT eRace)
{
	m_kHorse.Destroy();
	m_kHorse.Create(m_GraphicThingInstance.NEW_GetCurPixelPositionRef(), eRace, ms_adwCRCAffectEffect[EFFECT_HIT]);

	SetMotionMode(CRaceMotionData::MODE_HORSE);
	SetRotationSpeed(c_fDefaultHorseRotationSpeed);

	m_GraphicThingInstance.MountHorse(m_kHorse.GetActorPtr());
	m_GraphicThingInstance.Stop();
	m_GraphicThingInstance.RefreshActorInstance();
}

void CInstanceBase::DismountHorse()
{
	m_kHorse.Destroy();
}

void CInstanceBase::GetInfo(std::string* pstInfo)
{
	char szInfo[256];
	sprintf(szInfo, "Inst - UC %d, RC %d Pool - %d ",
		ms_dwUpdateCounter,
		ms_dwRenderCounter,
		ms_kPool.GetCapacity()
	);

	pstInfo->append(szInfo);
}

void CInstanceBase::ResetPerformanceCounter()
{
	ms_dwUpdateCounter=0;
	ms_dwRenderCounter=0;
	ms_dwDeformCounter=0;
}

bool CInstanceBase::NEW_IsLastPixelPosition()
{
	return m_GraphicThingInstance.IsPushing();
}

const TPixelPosition& CInstanceBase::NEW_GetLastPixelPositionRef()
{
	return m_GraphicThingInstance.NEW_GetLastPixelPositionRef();
}

void CInstanceBase::NEW_SetDstPixelPositionZ(FLOAT z)
{
	m_GraphicThingInstance.NEW_SetDstPixelPositionZ(z);
}

void CInstanceBase::NEW_SetDstPixelPosition(const TPixelPosition& c_rkPPosDst)
{
	m_GraphicThingInstance.NEW_SetDstPixelPosition(c_rkPPosDst);
}

void CInstanceBase::NEW_SetSrcPixelPosition(const TPixelPosition& c_rkPPosSrc)
{
	m_GraphicThingInstance.NEW_SetSrcPixelPosition(c_rkPPosSrc);
}

const TPixelPosition& CInstanceBase::NEW_GetCurPixelPositionRef()
{
	return m_GraphicThingInstance.NEW_GetCurPixelPositionRef();
}

const TPixelPosition& CInstanceBase::NEW_GetDstPixelPositionRef()
{
	return m_GraphicThingInstance.NEW_GetDstPixelPositionRef();
}

const TPixelPosition& CInstanceBase::NEW_GetSrcPixelPositionRef()
{
	return m_GraphicThingInstance.NEW_GetSrcPixelPositionRef();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
void CInstanceBase::OnSyncing()
{
	m_GraphicThingInstance.__OnSyncing();
}

void CInstanceBase::OnWaiting()
{
	m_GraphicThingInstance.__OnWaiting();
}

void CInstanceBase::OnMoving()
{
	m_GraphicThingInstance.__OnMoving();
}

void CInstanceBase::ChangeGuild(DWORD dwGuildID)
{
	m_dwGuildID=dwGuildID;

	DetachTextTail();
	AttachTextTail();
	RefreshTextTail();
}

#ifdef ENABLE_EFFECT_COSTUME_SYSTEM
////////////////////////////////
//// Equip (Marks) Effect Handle 
void CInstanceBase::__SetEquipEffect()
{
	__ClearEquipEffect();

	if (IsInvisibility() && !__IsMainInstance())
		return;

	DWORD dwEquipEffect = EFFECT_EQUIPMENT + GetPart(CRaceData::PART_EFFECT);
#ifdef ___DECORUM___
	if (EFFECT_EQUIPMENT == dwEquipEffect)
	{
		BYTE bLegue = GetDecorumLegue();
		if (0 == bLegue)
			return;

		BYTE bLegueRank = (bLegue - 1) / 4;
		dwEquipEffect = EFFECT_DECORUM + bLegueRank;
	}
#endif
	if (dwEquipEffect)
		m_vecEquipEffect.push_back(__AttachEffect(dwEquipEffect));
}

void CInstanceBase::SetEquipEffect(DWORD dwEquipEffect)
{
	m_awPart[CRaceData::PART_EFFECT] = dwEquipEffect;
	__SetEquipEffect();
}

void CInstanceBase::__ClearEquipEffect()
{
	std::vector<DWORD>::iterator it = m_vecEquipEffect.begin();
	for (; it != m_vecEquipEffect.end(); it++)
	{
		if (*it)
			__DetachEffect(*it);
	}
	m_vecEquipEffect.clear();
}

#endif
#ifdef ___DECORUM___
void CInstanceBase::ChangeDecorum(DWORD dwDecorum)
{
	bool bChanged = m_dwDecorumPoint != dwDecorum;
	m_dwDecorumPoint = dwDecorum;

	if (!m_dwDecorumPoint && !bChanged)
		return;

	if (0 == GetPart(CRaceData::PART_EFFECT))
		__SetEquipEffect();

	RefreshTextTail();
}

BYTE CInstanceBase::GetDecorumLegue()
{
	DWORD aRankTable[LEGUE_MAX_LEVEL] = {
		0, 160, 250, 500, 750,
		1100, 1400, 1800, 2300, 3000,
		3500, 4100, 4700, 5500, 6300,
		7500, 9000, 12500, 15000, 17500,
		20000
	};

	BYTE bLevel = 0;
	for (int i = 0; i < LEGUE_MAX_LEVEL - 1; ++i)
	{
		
		if (m_dwDecorumPoint < aRankTable[i + 1])
			break;

		++bLevel;
	}
	if(__GetRaceType() != CActorInstance::TYPE_PC)
		return 0;

	return m_dwDecorumPoint > 0 ? bLevel = MAX(1, bLevel) : bLevel;
}
#endif

DWORD CInstanceBase::GetPart(CRaceData::EParts part)
{
	assert(part >= 0 && part < CRaceData::PART_MAX_NUM);
	return m_awPart[part];
}

DWORD CInstanceBase::GetShape()
{
	return m_eShape;
}

bool CInstanceBase::CanAct()
{
	return m_GraphicThingInstance.CanAct();
}

bool CInstanceBase::CanMove()
{
	return m_GraphicThingInstance.CanMove();
}

bool CInstanceBase::CanUseSkill()
{
	if (IsPoly())
		return false;

	if (IsWearingDress())
		return false;

	if (IsHoldingPickAxe())
		return false;

	if (!m_kHorse.CanUseSkill())
		return false;

	if (!m_GraphicThingInstance.CanUseSkill())
		return false;

	return true;
}

bool CInstanceBase::CanAttack()
{
	if (!m_kHorse.CanAttack())
		return false;

	if (IsWearingDress())
		return false;

	if (IsHoldingPickAxe())
		return false;

	return m_GraphicThingInstance.CanAttack();
}



bool CInstanceBase::CanFishing()
{
	return m_GraphicThingInstance.CanFishing();
}


BOOL CInstanceBase::IsBowMode()
{
	return m_GraphicThingInstance.IsBowMode();
}

BOOL CInstanceBase::IsHandMode()
{
	return m_GraphicThingInstance.IsHandMode();
}

BOOL CInstanceBase::IsFishingMode()
{
	if (CRaceMotionData::MODE_FISHING == m_GraphicThingInstance.GetMotionMode())
		return true;

	return false;
}

BOOL CInstanceBase::IsFishing()
{
	return m_GraphicThingInstance.IsFishing();
}

BOOL CInstanceBase::IsDead()
{
	return m_GraphicThingInstance.IsDead();
}

BOOL CInstanceBase::IsStun()
{
	return m_GraphicThingInstance.IsStun();
}

BOOL CInstanceBase::IsSleep()
{
	return m_GraphicThingInstance.IsSleep();
}


BOOL CInstanceBase::__IsSyncing()
{
	return m_GraphicThingInstance.__IsSyncing();
}



void CInstanceBase::NEW_SetOwner(DWORD dwVIDOwner)
{
	m_GraphicThingInstance.SetOwner(dwVIDOwner);
}

float CInstanceBase::GetLocalTime()
{
	return m_GraphicThingInstance.GetLocalTime();
}


void CInstanceBase::PushUDPState(DWORD dwCmdTime, const TPixelPosition& c_rkPPosDst, float fDstRot, UINT eFunc, UINT uArg)
{
}

DWORD	ELTimer_GetServerFrameMSec();

void CInstanceBase::PushTCPStateExpanded(DWORD dwCmdTime, const TPixelPosition& c_rkPPosDst, float fDstRot, UINT eFunc, UINT uArg, UINT uTargetVID)
{
	SCommand kCmdNew;
	kCmdNew.m_kPPosDst = c_rkPPosDst;
	kCmdNew.m_dwChkTime = dwCmdTime+100;
	kCmdNew.m_dwCmdTime = dwCmdTime;
	kCmdNew.m_fDstRot = fDstRot;
	kCmdNew.m_eFunc = eFunc;
	kCmdNew.m_uArg = uArg;
	kCmdNew.m_uTargetVID = uTargetVID;
	m_kQue_kCmdNew.push_back(kCmdNew);
}

void CInstanceBase::PushTCPState(DWORD dwCmdTime, const TPixelPosition& c_rkPPosDst, float fDstRot, UINT eFunc, UINT uArg)
{
	if (__IsMainInstance())
	{
		//assert(!"CInstanceBase::PushTCPState �÷��̾� �ڽſ��� �̵���Ŷ�� ���� �ȵȴ�!");
		TraceError("CInstanceBase::PushTCPState You can't send move packets to yourself!");
		return;
	}

	int nNetworkGap=ELTimer_GetServerFrameMSec()-dwCmdTime;

	m_nAverageNetworkGap=(m_nAverageNetworkGap*70+nNetworkGap*30)/100;

	/*
	if (m_dwBaseCmdTime == 0)
	{
		m_dwBaseChkTime = ELTimer_GetFrameMSec()-nNetworkGap;
		m_dwBaseCmdTime = dwCmdTime;

		Tracenf("VID[%d] ��Ʈ���� [%d]", GetVirtualID(), nNetworkGap);
	}
	*/

	//m_dwBaseChkTime-m_dwBaseCmdTime+ELTimer_GetServerMSec();

	SCommand kCmdNew;
	kCmdNew.m_kPPosDst = c_rkPPosDst;
	kCmdNew.m_dwChkTime = dwCmdTime+m_nAverageNetworkGap;//m_dwBaseChkTime + (dwCmdTime - m_dwBaseCmdTime);// + nNetworkGap;
	kCmdNew.m_dwCmdTime = dwCmdTime;
	kCmdNew.m_fDstRot = fDstRot;
	kCmdNew.m_eFunc = eFunc;
	kCmdNew.m_uArg = uArg;
	m_kQue_kCmdNew.push_back(kCmdNew);

	//int nApplyGap=kCmdNew.m_dwChkTime-ELTimer_GetServerFrameMSec();

	//if (nApplyGap<-500 || nApplyGap>500)
	//	Tracenf("VID[%d] NAME[%s] ��Ʈ���� [cur:%d ave:%d] �۵��ð� (%d)", GetVirtualID(), GetNameString(), nNetworkGap, m_nAverageNetworkGap, nApplyGap);
}

/*
CInstanceBase::TStateQueue::iterator CInstanceBase::FindSameState(TStateQueue& rkQuekStt, DWORD dwCmdTime, UINT eFunc, UINT uArg)
{
	TStateQueue::iterator i=rkQuekStt.begin();
	while (rkQuekStt.end()!=i)
	{
		SState& rkSttEach=*i;
		if (rkSttEach.m_dwCmdTime==dwCmdTime)
			if (rkSttEach.m_eFunc==eFunc)
				if (rkSttEach.m_uArg==uArg)
					break;
		++i;
	}

	return i;
}
*/

BOOL CInstanceBase::__CanProcessNetworkStatePacket()
{
	if (m_GraphicThingInstance.IsDead())
		return FALSE;
	if (m_GraphicThingInstance.IsKnockDown())
		return FALSE;
	if (m_GraphicThingInstance.IsUsingSkill())
		if (!m_GraphicThingInstance.CanCancelSkill())
			return FALSE;

	return TRUE;
}

BOOL CInstanceBase::__IsEnableTCPProcess(UINT eCurFunc)
{
	if (m_GraphicThingInstance.IsActEmotion())
	{
		return FALSE;
	}

	if (!m_bEnableTCPState)
	{
		if (FUNC_EMOTION != eCurFunc)
		{
			return FALSE;
		}
	}

	return TRUE;
}

void CInstanceBase::StateProcess()
{
	while (1)
	{
		if (m_kQue_kCmdNew.empty())
			return;

		DWORD dwDstChkTime = m_kQue_kCmdNew.front().m_dwChkTime;
		DWORD dwCurChkTime = ELTimer_GetServerFrameMSec();

		if (dwCurChkTime < dwDstChkTime)
			return;

		SCommand kCmdTop = m_kQue_kCmdNew.front();
		m_kQue_kCmdNew.pop_front();

		TPixelPosition kPPosDst = kCmdTop.m_kPPosDst;
		//DWORD dwCmdTime = kCmdTop.m_dwCmdTime;
		FLOAT fRotDst = kCmdTop.m_fDstRot;
		UINT eFunc = kCmdTop.m_eFunc;
		UINT uArg = kCmdTop.m_uArg;
		UINT uVID = GetVirtualID();
		UINT uTargetVID = kCmdTop.m_uTargetVID;

		TPixelPosition kPPosCur;
		NEW_GetPixelPosition(&kPPosCur);

		/*
		if (IsPC())
			Tracenf("%d cmd: vid=%d[%s] func=%d arg=%d  curPos=(%f, %f) dstPos=(%f, %f) rot=%f (time %d)",
			ELTimer_GetMSec(),
			uVID, m_stName.c_str(), eFunc, uArg,
			kPPosCur.x, kPPosCur.y,
			kPPosDst.x, kPPosDst.y, fRotDst, dwCmdTime-m_dwBaseCmdTime);
		*/

		TPixelPosition kPPosDir = kPPosDst - kPPosCur;
		float fDirLen = (float)sqrt(kPPosDir.x * kPPosDir.x + kPPosDir.y * kPPosDir.y);

		//Tracenf("�Ÿ� %f", fDirLen);

		if (!__CanProcessNetworkStatePacket())
		{
			Lognf(0, "vid=%d Skip State as unable to process IsDead=%d, IsKnockDown=%d", uVID, m_GraphicThingInstance.IsDead(), m_GraphicThingInstance.IsKnockDown());
			return;
		}

		if (!__IsEnableTCPProcess(eFunc))
		{
			return;
		}

		switch (eFunc)
		{
			case FUNC_WAIT:
			{
				//Tracenf("%s (%f, %f) -> (%f, %f) �����Ÿ� %f", GetNameString(), kPPosCur.x, kPPosCur.y, kPPosDst.x, kPPosDst.y, fDirLen);
				if (fDirLen > 1.0f)
				{
					//NEW_GetSrcPixelPositionRef() = kPPosCur;
					//NEW_GetDstPixelPositionRef() = kPPosDst;
					NEW_SetSrcPixelPosition(kPPosCur);
					NEW_SetDstPixelPosition(kPPosDst);

					__EnableSkipCollision();

					m_fDstRot = fRotDst;
					m_isGoing = TRUE;

					m_kMovAfterFunc.eFunc = FUNC_WAIT;

					if (!IsWalking())
						StartWalking();

					//Tracen("��ǥ����");
				}
				else
				{
					//Tracen("���� ����");

					m_isGoing = FALSE;

					if (!IsWaiting())
						EndWalking();

					SCRIPT_SetPixelPosition(kPPosDst.x, kPPosDst.y);
					SetAdvancingRotation(fRotDst);
					SetRotation(fRotDst);
				}
				break;
			}

			case FUNC_MOVE:
			{
				//NEW_GetSrcPixelPositionRef() = kPPosCur;
				//NEW_GetDstPixelPositionRef() = kPPosDst;
				NEW_SetSrcPixelPosition(kPPosCur);
				NEW_SetDstPixelPosition(kPPosDst);
				m_fDstRot = fRotDst;
				m_isGoing = TRUE;
				__EnableSkipCollision();
				//m_isSyncMov = TRUE;

				m_kMovAfterFunc.eFunc = FUNC_MOVE;

				if (!IsWalking())
				{
					//Tracen("�Ȱ� ���� �ʾ� �ȱ� ����");
					StartWalking();
				}
				else
				{
					//Tracen("�̹� �ȴ��� ");
				}
				break;
			}

			case FUNC_COMBO:
			{
				if (fDirLen >= 50.0f)
				{
					NEW_SetSrcPixelPosition(kPPosCur);
					NEW_SetDstPixelPosition(kPPosDst);
					m_fDstRot=fRotDst;
					m_isGoing = TRUE;
					__EnableSkipCollision();

					m_kMovAfterFunc.eFunc = FUNC_COMBO;
					m_kMovAfterFunc.uArg = uArg;

					if (!IsWalking())
						StartWalking();
				}
				else
				{
					//Tracen("��￿ ���� ����");

					m_isGoing = FALSE;

					if (IsWalking())
						EndWalking();

					SCRIPT_SetPixelPosition(kPPosDst.x, kPPosDst.y);
					RunComboAttack(fRotDst, uArg);
				}
				break;
			}

			case FUNC_ATTACK:
			{
				if (fDirLen>=50.0f)
				{
					//NEW_GetSrcPixelPositionRef() = kPPosCur;
					//NEW_GetDstPixelPositionRef() = kPPosDst;
					NEW_SetSrcPixelPosition(kPPosCur);
					NEW_SetDstPixelPosition(kPPosDst);
					m_fDstRot = fRotDst;
					m_isGoing = TRUE;
					__EnableSkipCollision();
					//m_isSyncMov = TRUE;

					m_kMovAfterFunc.eFunc = FUNC_ATTACK;

					if (!IsWalking())
						StartWalking();

					//Tracen("�ʹ� �־ �̵� �� ����");
				}
				else
				{
					//Tracen("�븻 ���� ����");

					m_isGoing = FALSE;

					if (IsWalking())
						EndWalking();

					SCRIPT_SetPixelPosition(kPPosDst.x, kPPosDst.y);
					BlendRotation(fRotDst);

					RunNormalAttack(fRotDst);

					//Tracen("������ ������ ���� ����");
				}
				break;
			}

			case FUNC_MOB_SKILL:
			{
				if (fDirLen >= 50.0f)
				{
					NEW_SetSrcPixelPosition(kPPosCur);
					NEW_SetDstPixelPosition(kPPosDst);
					m_fDstRot = fRotDst;
					m_isGoing = TRUE;
					__EnableSkipCollision();

					m_kMovAfterFunc.eFunc = FUNC_MOB_SKILL;
					m_kMovAfterFunc.uArg = uArg;

					if (!IsWalking())
						StartWalking();
				}
				else
				{
					m_isGoing = FALSE;

					if (IsWalking())
						EndWalking();

					SCRIPT_SetPixelPosition(kPPosDst.x, kPPosDst.y);
					BlendRotation(fRotDst);

					m_GraphicThingInstance.InterceptOnceMotion(CRaceMotionData::NAME_SPECIAL_1 + uArg);
				}
				break;
			}

			case FUNC_EMOTION:
			{
				if (fDirLen>100.0f)
				{
					NEW_SetSrcPixelPosition(kPPosCur);
					NEW_SetDstPixelPosition(kPPosDst);
					m_fDstRot = fRotDst;
					m_isGoing = TRUE;

					if (__IsMainInstance())
						__EnableSkipCollision();

					m_kMovAfterFunc.eFunc = FUNC_EMOTION;
					m_kMovAfterFunc.uArg = uArg;
					m_kMovAfterFunc.uArgExpanded = uTargetVID;
					m_kMovAfterFunc.kPosDst = kPPosDst;

					if (!IsWalking())
						StartWalking();
				}
				else
				{
					__ProcessFunctionEmotion(uArg, uTargetVID, kPPosDst);
				}
				break;
			}

			default:
			{
				if (eFunc & FUNC_SKILL)
				{
					if (fDirLen >= 50.0f)
					{
						//NEW_GetSrcPixelPositionRef() = kPPosCur;
						//NEW_GetDstPixelPositionRef() = kPPosDst;
						NEW_SetSrcPixelPosition(kPPosCur);
						NEW_SetDstPixelPosition(kPPosDst);
						m_fDstRot = fRotDst;
						m_isGoing = TRUE;
						//m_isSyncMov = TRUE;
						__EnableSkipCollision();

						m_kMovAfterFunc.eFunc = eFunc;
						m_kMovAfterFunc.uArg = uArg;

						if (!IsWalking())
							StartWalking();

						//Tracen("�ʹ� �־ �̵� �� ����");
					}
					else
					{
						//Tracen("��ų ����");

						m_isGoing = FALSE;

						if (IsWalking())
							EndWalking();

						SCRIPT_SetPixelPosition(kPPosDst.x, kPPosDst.y);
						SetAdvancingRotation(fRotDst);
						SetRotation(fRotDst);

						NEW_UseSkill(0, eFunc & 0x7f, uArg&0x0f, (uArg>>4) ? true : false);
						//Tracen("������ ������ ���� ����");
					}
				}
				break;
			}
		}
	}
}


void CInstanceBase::MovementProcess()
{
	TPixelPosition kPPosCur;
	NEW_GetPixelPosition(&kPPosCur);

	// ������ ��ǥ���̹Ƿ� y�� -ȭ�ؼ� ���Ѵ�.

	TPixelPosition kPPosNext;
	{
		const D3DXVECTOR3 & c_rkV3Mov = m_GraphicThingInstance.GetMovementVectorRef();

		kPPosNext.x = kPPosCur.x + (+c_rkV3Mov.x);
		kPPosNext.y = kPPosCur.y + (-c_rkV3Mov.y);
		kPPosNext.z = kPPosCur.z + (+c_rkV3Mov.z);
	}

	TPixelPosition kPPosDeltaSC = kPPosCur - NEW_GetSrcPixelPositionRef();
	TPixelPosition kPPosDeltaSN = kPPosNext - NEW_GetSrcPixelPositionRef();
	TPixelPosition kPPosDeltaSD = NEW_GetDstPixelPositionRef() - NEW_GetSrcPixelPositionRef();

	float fCurLen = sqrtf(kPPosDeltaSC.x * kPPosDeltaSC.x + kPPosDeltaSC.y * kPPosDeltaSC.y);
	float fNextLen = sqrtf(kPPosDeltaSN.x * kPPosDeltaSN.x + kPPosDeltaSN.y * kPPosDeltaSN.y);
	float fTotalLen = sqrtf(kPPosDeltaSD.x * kPPosDeltaSD.x + kPPosDeltaSD.y * kPPosDeltaSD.y);
	float fRestLen = fTotalLen - fCurLen;

	if (__IsMainInstance())
	{
		if (m_isGoing && IsWalking())
		{
			float fDstRot = NEW_GetAdvancingRotationFromPixelPosition(NEW_GetSrcPixelPositionRef(), NEW_GetDstPixelPositionRef());

			SetAdvancingRotation(fDstRot);

			if (fRestLen<=0.0)
			{
				if (IsWalking())
					EndWalking();

				//Tracen("��ǥ ���� ����");

				m_isGoing = FALSE;

				BlockMovement();

				if (FUNC_EMOTION == m_kMovAfterFunc.eFunc)
				{
					DWORD dwMotionNumber = m_kMovAfterFunc.uArg;
					DWORD dwTargetVID = m_kMovAfterFunc.uArgExpanded;
					__ProcessFunctionEmotion(dwMotionNumber, dwTargetVID, m_kMovAfterFunc.kPosDst);
					m_kMovAfterFunc.eFunc = FUNC_WAIT;
					return;
				}
			}
		}
	}
	else
	{
		if (m_isGoing && IsWalking())
		{
			float fDstRot = NEW_GetAdvancingRotationFromPixelPosition(NEW_GetSrcPixelPositionRef(), NEW_GetDstPixelPositionRef());

			SetAdvancingRotation(fDstRot);

			// ���� ���Ͻð� �ʾ� �ʹ� ���� �̵��ߴٸ�..
			if (fRestLen < -100.0f)
			{
				NEW_SetSrcPixelPosition(kPPosCur);

				float fDstRot = NEW_GetAdvancingRotationFromPixelPosition(kPPosCur, NEW_GetDstPixelPositionRef());
				SetAdvancingRotation(fDstRot);
				//Tracenf("VID %d ���� ���⼳�� (%f, %f) %f rest %f", GetVirtualID(), kPPosCur.x, kPPosCur.y, fDstRot, fRestLen);

				// �̵����̶�￿ �������� ���߰� �Ѵ�
				if (FUNC_MOVE == m_kMovAfterFunc.eFunc)
				{
					m_kMovAfterFunc.eFunc = FUNC_WAIT;
				}
			}
			// �����ߴٸ�...
			else if (fCurLen <= fTotalLen && fTotalLen <= fNextLen)
			{
				if (m_GraphicThingInstance.IsDead() || m_GraphicThingInstance.IsKnockDown())
				{
					__DisableSkipCollision();

					//Tracen("��￿ ���¶� ���� ��ŵ");

					m_isGoing = FALSE;

					//Tracen("�ൿ �Ҵ� ���¶� ���� ���� ��ŵ");
				}
				else
				{
					switch (m_kMovAfterFunc.eFunc)
					{
						case FUNC_ATTACK:
						{
							if (IsWalking())
								EndWalking();

							__DisableSkipCollision();
							m_isGoing = FALSE;

							BlockMovement();
							SCRIPT_SetPixelPosition(NEW_GetDstPixelPositionRef().x, NEW_GetDstPixelPositionRef().y);
							SetAdvancingRotation(m_fDstRot);
							SetRotation(m_fDstRot);

							RunNormalAttack(m_fDstRot);
							break;
						}

						case FUNC_COMBO:
						{
							if (IsWalking())
								EndWalking();

							__DisableSkipCollision();
							m_isGoing = FALSE;

							BlockMovement();
							SCRIPT_SetPixelPosition(NEW_GetDstPixelPositionRef().x, NEW_GetDstPixelPositionRef().y);
							RunComboAttack(m_fDstRot, m_kMovAfterFunc.uArg);
							break;
						}

						case FUNC_EMOTION:
						{
							m_isGoing = FALSE;
							m_kMovAfterFunc.eFunc = FUNC_WAIT;
							__DisableSkipCollision();
							BlockMovement();

							DWORD dwMotionNumber = m_kMovAfterFunc.uArg;
							DWORD dwTargetVID = m_kMovAfterFunc.uArgExpanded;
							__ProcessFunctionEmotion(dwMotionNumber, dwTargetVID, m_kMovAfterFunc.kPosDst);
							break;
						}

						case FUNC_MOVE:
						{
							break;
						}

						case FUNC_MOB_SKILL:
						{
							if (IsWalking())
								EndWalking();

							__DisableSkipCollision();
							m_isGoing = FALSE;

							BlockMovement();
							SCRIPT_SetPixelPosition(NEW_GetDstPixelPositionRef().x, NEW_GetDstPixelPositionRef().y);
							SetAdvancingRotation(m_fDstRot);
							SetRotation(m_fDstRot);

							m_GraphicThingInstance.InterceptOnceMotion(CRaceMotionData::NAME_SPECIAL_1 + m_kMovAfterFunc.uArg);
							break;
						}

						default:
						{
							if (m_kMovAfterFunc.eFunc & FUNC_SKILL)
							{
								SetAdvancingRotation(m_fDstRot);
								BlendRotation(m_fDstRot);
								NEW_UseSkill(0, m_kMovAfterFunc.eFunc & 0x7f, m_kMovAfterFunc.uArg&0x0f, (m_kMovAfterFunc.uArg>>4) ? true : false);
							}
							else
							{
								//Tracenf("VID %d ��ų ���� (%f, %f) rot %f", GetVirtualID(), NEW_GetDstPixelPositionRef().x, NEW_GetDstPixelPositionRef().y, m_fDstRot);

								__DisableSkipCollision();
								m_isGoing = FALSE;

								BlockMovement();
								SCRIPT_SetPixelPosition(NEW_GetDstPixelPositionRef().x, NEW_GetDstPixelPositionRef().y);
								SetAdvancingRotation(m_fDstRot);
								BlendRotation(m_fDstRot);
								if (!IsWaiting())
								{
									EndWalking();
								}

								//Tracenf("VID %d ���� (%f, %f) rot %f IsWalking %d", GetVirtualID(), NEW_GetDstPixelPositionRef().x, NEW_GetDstPixelPositionRef().y, m_fDstRot, IsWalking());
							}
							break;
						}
					}

				}
			}

		}
	}

	if (IsWalking() || m_GraphicThingInstance.IsUsingMovingSkill())
	{
		float fRotation = m_GraphicThingInstance.GetRotation();
		float fAdvancingRotation = m_GraphicThingInstance.GetAdvancingRotation();
		int iDirection = GetRotatingDirection(fRotation, fAdvancingRotation);

		if (DEGREE_DIRECTION_SAME != m_iRotatingDirection)
		{
			if (DEGREE_DIRECTION_LEFT == iDirection)
			{
				fRotation = fmodf(fRotation + m_fRotSpd*m_GraphicThingInstance.GetSecondElapsed(), 360.0f);
			}
			else if (DEGREE_DIRECTION_RIGHT == iDirection)
			{
				fRotation = fmodf(fRotation - m_fRotSpd*m_GraphicThingInstance.GetSecondElapsed() + 360.0f, 360.0f);
			}

			if (m_iRotatingDirection != GetRotatingDirection(fRotation, fAdvancingRotation))
			{
				m_iRotatingDirection = DEGREE_DIRECTION_SAME;
				fRotation = fAdvancingRotation;
			}

			m_GraphicThingInstance.SetRotation(fRotation);
		}

		if (__IsInDustRange())
		{
			float fDustDistance = NEW_GetDistanceFromDestPixelPosition(m_kPPosDust);
			if (IsMountingHorse())
			{
				if (fDustDistance > ms_fHorseDustGap)
				{
					NEW_GetPixelPosition(&m_kPPosDust);
					__AttachEffect(EFFECT_HORSE_DUST);
				}
			}
			else
			{
				if (fDustDistance > ms_fDustGap)
				{
					NEW_GetPixelPosition(&m_kPPosDust);
					__AttachEffect(EFFECT_DUST);
				}
			}
		}
	}
}

void CInstanceBase::__ProcessFunctionEmotion(DWORD dwMotionNumber, DWORD dwTargetVID, const TPixelPosition & c_rkPosDst)
{
	if (IsWalking())
		EndWalkingWithoutBlending();

	__EnableChangingTCPState();
	SCRIPT_SetPixelPosition(c_rkPosDst.x, c_rkPosDst.y);

	CInstanceBase * pTargetInstance = CPythonCharacterManager::Instance().GetInstancePtr(dwTargetVID);
	if (pTargetInstance)
	{
		pTargetInstance->__EnableChangingTCPState();

		if (pTargetInstance->IsWalking())
			pTargetInstance->EndWalkingWithoutBlending();

		WORD wMotionNumber1 = HIWORD(dwMotionNumber);
		WORD wMotionNumber2 = LOWORD(dwMotionNumber);

		int src_job = RaceToJob(GetRace());
		int dst_job = RaceToJob(pTargetInstance->GetRace());

		NEW_LookAtDestInstance(*pTargetInstance);
		m_GraphicThingInstance.InterceptOnceMotion(wMotionNumber1 + dst_job);
		m_GraphicThingInstance.SetRotation(m_GraphicThingInstance.GetTargetRotation());
		m_GraphicThingInstance.SetAdvancingRotation(m_GraphicThingInstance.GetTargetRotation());

		pTargetInstance->NEW_LookAtDestInstance(*this);
		pTargetInstance->m_GraphicThingInstance.InterceptOnceMotion(wMotionNumber2 + src_job);
		pTargetInstance->m_GraphicThingInstance.SetRotation(pTargetInstance->m_GraphicThingInstance.GetTargetRotation());
		pTargetInstance->m_GraphicThingInstance.SetAdvancingRotation(pTargetInstance->m_GraphicThingInstance.GetTargetRotation());

		if (pTargetInstance->__IsMainInstance())
		{
			IAbstractPlayer & rPlayer=IAbstractPlayer::GetSingleton();
			rPlayer.EndEmotionProcess();
		}
	}

	if (__IsMainInstance())
	{
		IAbstractPlayer & rPlayer=IAbstractPlayer::GetSingleton();
		rPlayer.EndEmotionProcess();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Update & Deform & Render

int g_iAccumulationTime = 0;

void CInstanceBase::Update()
{
	++ms_dwUpdateCounter;

	StateProcess();
	m_GraphicThingInstance.PhysicsProcess();
	m_GraphicThingInstance.RotationProcess();
	m_GraphicThingInstance.ComboProcess();
	m_GraphicThingInstance.AccumulationMovement();

	if (m_GraphicThingInstance.IsMovement())
	{
		TPixelPosition kPPosCur;
		NEW_GetPixelPosition(&kPPosCur);

		DWORD dwCurTime=ELTimer_GetFrameMSec();
		//if (m_dwNextUpdateHeightTime<dwCurTime)
		{
			m_dwNextUpdateHeightTime=dwCurTime;
			kPPosCur.z = __GetBackgroundHeight(kPPosCur.x, kPPosCur.y);
			NEW_SetPixelPosition(kPPosCur);
		}

		// SetMaterialColor
		{
			DWORD dwMtrlColor=__GetShadowMapColor(kPPosCur.x, kPPosCur.y);
			m_GraphicThingInstance.SetMaterialColor(dwMtrlColor);
		}
	}

	m_GraphicThingInstance.UpdateAdvancingPointInstance();

#ifdef ENABLE_HIDE_PET
	if (IsPet())
	{
		if (CPythonSystem::Instance().IsHidePets())
		{
			if (!IsAffect(AFFECT_INVISIBILITY))
			{
				__SetAffect(AFFECT_INVISIBILITY, true);
				m_kAffectFlagContainer.Set(AFFECT_INVISIBILITY, true);
			}
		}
		else
		{
			if (IsAffect(AFFECT_INVISIBILITY))
			{
				__SetAffect(AFFECT_INVISIBILITY, false);
				m_kAffectFlagContainer.Set(AFFECT_INVISIBILITY, false);
			}
		}
	}
#endif
#ifdef ENABLE_HIDE_MOUNT
	if (IsMount())
	{
		if (CPythonSystem::Instance().IsHideMounts())
		{
			if (!IsAffect(AFFECT_INVISIBILITY))
			{
				__SetAffect(AFFECT_INVISIBILITY, true);
				m_kAffectFlagContainer.Set(AFFECT_INVISIBILITY, true);
			}
		}
		else
		{
			if (IsAffect(AFFECT_INVISIBILITY))
			{
				__SetAffect(AFFECT_INVISIBILITY, false);
				m_kAffectFlagContainer.Set(AFFECT_INVISIBILITY, false);
			}
		}
	}
#endif

#ifdef ENABLE_HIDE_SHOPS
	if (IsShops())
	{
		if (CPythonSystem::Instance().IsHideShops())
		{
			if (!IsAffect(AFFECT_INVISIBILITY))
			{
				__SetAffect(AFFECT_INVISIBILITY, true);
				m_kAffectFlagContainer.Set(AFFECT_INVISIBILITY, true);
			}
		}
		else
		{
			if (IsAffect(AFFECT_INVISIBILITY))
			{
				__SetAffect(AFFECT_INVISIBILITY, false);
				m_kAffectFlagContainer.Set(AFFECT_INVISIBILITY, false);
			}
		}
	}
#endif


	AttackProcess();
	MovementProcess();

	m_GraphicThingInstance.MotionProcess(IsPC());
	if (IsMountingHorse())
	{
		m_kHorse.m_pkActor->HORSE_MotionProcess(FALSE);
	}

	__ComboProcess();

	ProcessRemoveOldDamage();
	ProcessDamage();

}

void CInstanceBase::Transform()
{
	if (__IsSyncing())
	{
		//OnSyncing();
	}
	else
	{
		if (IsWalking() || m_GraphicThingInstance.IsUsingMovingSkill())
		{
			const D3DXVECTOR3& c_rv3Movment=m_GraphicThingInstance.GetMovementVectorRef();

			float len=(c_rv3Movment.x*c_rv3Movment.x)+(c_rv3Movment.y*c_rv3Movment.y);
			if (len>1.0f)
				OnMoving();
			else
				OnWaiting();
		}
	}

	m_GraphicThingInstance.INSTANCEBASE_Transform();
}


void CInstanceBase::Deform()
{
	// 2004.07.17.levites.isShow�� ViewFrustumCheck�� ����
	if (!__CanRender())
		return;

	++ms_dwDeformCounter;

	m_GraphicThingInstance.INSTANCEBASE_Deform();

	m_kHorse.Deform();
}

void CInstanceBase::RenderTrace()
{
	if (!__CanRender())
		return;

	m_GraphicThingInstance.RenderTrace();
}




void CInstanceBase::Render()
{
	// 2004.07.17.levites.isShow�� ViewFrustumCheck�� ����
	if (!__CanRender())
		return;

	++ms_dwRenderCounter;

	m_kHorse.Render();
	m_GraphicThingInstance.Render();

	if (CActorInstance::IsDirLine())
	{
		if (NEW_GetDstPixelPositionRef().x != 0.0f)
		{
			static CScreen s_kScreen;

			STATEMANAGER.SetTextureStageState(0, D3DTSS_COLORARG1,	D3DTA_DIFFUSE);
			STATEMANAGER.SetTextureStageState(0, D3DTSS_COLOROP,	D3DTOP_SELECTARG1);
			STATEMANAGER.SetTextureStageState(0, D3DTSS_ALPHAOP,	D3DTOP_DISABLE);
			STATEMANAGER.SaveRenderState(D3DRS_ZENABLE, FALSE);
			STATEMANAGER.SetRenderState(D3DRS_FOGENABLE, FALSE);
			STATEMANAGER.SetRenderState(D3DRS_LIGHTING, FALSE);

			TPixelPosition px;
			m_GraphicThingInstance.GetPixelPosition(&px);
			D3DXVECTOR3 kD3DVt3Cur(px.x, px.y, px.z);
			//D3DXVECTOR3 kD3DVt3Cur(NEW_GetSrcPixelPositionRef().x, -NEW_GetSrcPixelPositionRef().y, NEW_GetSrcPixelPositionRef().z);
			D3DXVECTOR3 kD3DVt3Dest(NEW_GetDstPixelPositionRef().x, -NEW_GetDstPixelPositionRef().y, NEW_GetDstPixelPositionRef().z);

			//printf("%s %f\n", GetNameString(), kD3DVt3Cur.y - kD3DVt3Dest.y);
			//float fdx = NEW_GetDstPixelPositionRef().x - NEW_GetSrcPixelPositionRef().x;
			//float fdy = NEW_GetDstPixelPositionRef().y - NEW_GetSrcPixelPositionRef().y;

			s_kScreen.SetDiffuseColor(0.0f, 0.0f, 1.0f);
			s_kScreen.RenderLine3d(kD3DVt3Cur.x, kD3DVt3Cur.y, px.z, kD3DVt3Dest.x, kD3DVt3Dest.y, px.z);
			STATEMANAGER.RestoreRenderState(D3DRS_ZENABLE);
			STATEMANAGER.SetRenderState(D3DRS_FOGENABLE, TRUE);
			STATEMANAGER.SetRenderState(D3DRS_LIGHTING, TRUE);
		}
	}
}

void CInstanceBase::RenderToShadowMap()
{
	if (IsDoor())
		return;

	if (IsBuilding())
		return;

	if (!__CanRender())
		return;

	if (!__IsExistMainInstance())
		return;

	CInstanceBase* pkInstMain=__GetMainInstancePtr();

	const float SHADOW_APPLY_DISTANCE = 2500.0f;

	float fDistance=NEW_GetDistanceFromDestInstance(*pkInstMain);
	if (fDistance>=SHADOW_APPLY_DISTANCE)
		return;

	m_GraphicThingInstance.RenderToShadowMap();
}

void CInstanceBase::RenderCollision()
{
	m_GraphicThingInstance.RenderCollisionData();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Setting & Getting Data

void CInstanceBase::SetVirtualID(DWORD dwVirtualID)
{
	m_GraphicThingInstance.SetVirtualID(dwVirtualID);
}

void CInstanceBase::SetVirtualNumber(DWORD dwVirtualNumber)
{
	m_dwVirtualNumber = dwVirtualNumber;
}

void CInstanceBase::SetInstanceType(int iInstanceType)
{
	m_GraphicThingInstance.SetActorType(iInstanceType);
}

void CInstanceBase::SetAlignment(short sAlignment)
{
	m_sAlignment = sAlignment;
	RefreshTextTailTitle();
}

#ifdef NEW_PET_SYSTEM
void CInstanceBase::SetLevelText(int sLevel)
{
	m_dwLevel = sLevel;
	UpdateTextTailLevel(sLevel);
}
#endif

void CInstanceBase::SetPKMode(BYTE byPKMode)
{
	if (m_byPKMode == byPKMode)
		return;

	m_byPKMode = byPKMode;

	if (__IsMainInstance())
	{
		IAbstractPlayer& rPlayer=IAbstractPlayer::GetSingleton();
		rPlayer.NotifyChangePKMode();
	}
}

void CInstanceBase::SetKiller(bool bFlag)
{
	if (m_isKiller == bFlag)
		return;

	m_isKiller = bFlag;
	RefreshTextTail();
}

void CInstanceBase::SetPartyMemberFlag(bool bFlag)
{
	m_isPartyMember = bFlag;
}

void CInstanceBase::SetStateFlags(DWORD dwStateFlags)
{
	if (dwStateFlags & ADD_CHARACTER_STATE_KILLER)
		SetKiller(TRUE);
	else
		SetKiller(FALSE);

	if (dwStateFlags & ADD_CHARACTER_STATE_PARTY)
		SetPartyMemberFlag(TRUE);
	else
		SetPartyMemberFlag(FALSE);
}

void CInstanceBase::SetComboType(UINT uComboType)
{
	m_GraphicThingInstance.SetComboType(uComboType);
}

#ifdef ENABLE_MULTI_LANGUAGE_SYSTEM
const char * CInstanceBase::GetLanguageString()
{
	return m_stLanguage.c_str();
}
#endif

const char * CInstanceBase::GetNameString()
{
	return m_stName.c_str();
}

#ifdef ENABLE_TEXT_LEVEL_REFRESH
void CInstanceBase::SetLevel(DWORD dwLevel)
{
	m_dwLevel = dwLevel;
}
#endif

#ifdef ENABLE_LEVEL_IN_TRADE
DWORD CInstanceBase::GetLevel()
{
	return m_dwLevel;
}
#endif

#if defined(WJ_SHOW_MOB_INFO) && defined(ENABLE_SHOW_MOBAIFLAG)
void CInstanceBase::MobInfoAiFlagRefresh()
{
	// set
	std::string strName = CPythonNonPlayer::Instance().GetMonsterName(GetRace());
	if (CPythonSystem::Instance().IsShowMobAIFlag() && CPythonNonPlayer::Instance().IsAggressive(GetRace()))
		strName += "*";
	SetNameString(strName.c_str(), strName.length());
	// refresh
	DetachTextTail();
	AttachTextTail();
	RefreshTextTail();
}
#endif
#if defined(WJ_SHOW_MOB_INFO) && defined(ENABLE_SHOW_MOBLEVEL)
void CInstanceBase::MobInfoLevelRefresh()
{
	// set
	if (CPythonSystem::Instance().IsShowMobLevel())
		m_dwLevel = CPythonNonPlayer::Instance().GetMonsterLevel(GetRace());
	else
		m_dwLevel = 0;
	// refresh
	if (m_dwLevel)
		UpdateTextTailLevel(m_dwLevel);
	else
		CPythonTextTail::Instance().DetachLevel(GetVirtualID());
}
#endif

#ifdef ENABLE_NEW_EXCHANGE_WINDOW
DWORD CInstanceBase::GetLevel()
{
	return m_dwLevel;
}
#endif

DWORD CInstanceBase::GetRace()
{
	return m_dwRace;
}


bool CInstanceBase::IsConflictAlignmentInstance(CInstanceBase& rkInstVictim)
{
	if (PK_MODE_PROTECT == rkInstVictim.GetPKMode())
		return false;

	switch (GetAlignmentType())
	{
		case ALIGNMENT_TYPE_NORMAL:
		case ALIGNMENT_TYPE_WHITE:
			if (ALIGNMENT_TYPE_DARK == rkInstVictim.GetAlignmentType())
				return true;
			break;
		case ALIGNMENT_TYPE_DARK:
			if (GetAlignmentType() != rkInstVictim.GetAlignmentType())
				return true;
			break;
	}

	return false;
}

void CInstanceBase::SetDuelMode(DWORD type)
{
	m_dwDuelMode = type;
}

DWORD CInstanceBase::GetDuelMode()
{
	return m_dwDuelMode;
}

bool CInstanceBase::IsAttackableInstance(CInstanceBase& rkInstVictim)
{
	if (__IsMainInstance())
	{
		CPythonPlayer& rkPlayer=CPythonPlayer::Instance();
		if(rkPlayer.IsObserverMode())
			return false;
	}

	if (GetVirtualID() == rkInstVictim.GetVirtualID())
		return false;

	if (IsStone())
	{
		if (rkInstVictim.IsPC())
			return true;
	}
	else if (IsPC())
	{
		if (rkInstVictim.IsStone())
			return true;

		if (rkInstVictim.IsPC())
		{
			if (GetDuelMode())
			{
				switch(GetDuelMode())
				{
				case DUEL_CANNOTATTACK:
					return false;
				case DUEL_START:
					if(__FindDUELKey(GetVirtualID(),rkInstVictim.GetVirtualID()))
						return true;
					else
						return false;
				}
			}
			if (PK_MODE_GUILD == GetPKMode())
				if (GetGuildID() == rkInstVictim.GetGuildID())
					return false;

			if (rkInstVictim.IsKiller())
				if (!IAbstractPlayer::GetSingleton().IsSamePartyMember(GetVirtualID(), rkInstVictim.GetVirtualID()))
					return true;

			if (PK_MODE_PROTECT != GetPKMode())
			{
				if (PK_MODE_FREE == GetPKMode())
				{
					if (PK_MODE_PROTECT != rkInstVictim.GetPKMode())
						if (!IAbstractPlayer::GetSingleton().IsSamePartyMember(GetVirtualID(), rkInstVictim.GetVirtualID()))
							return true;
				}
				if (PK_MODE_GUILD == GetPKMode())
				{
					if (PK_MODE_PROTECT != rkInstVictim.GetPKMode())
						if (!IAbstractPlayer::GetSingleton().IsSamePartyMember(GetVirtualID(), rkInstVictim.GetVirtualID()))
							if (GetGuildID() != rkInstVictim.GetGuildID())
								return true;
				}
			}

			if (IsSameEmpire(rkInstVictim))
			{
				if (IsPVPInstance(rkInstVictim))
					return true;

				if (PK_MODE_REVENGE == GetPKMode())
					if (!IAbstractPlayer::GetSingleton().IsSamePartyMember(GetVirtualID(), rkInstVictim.GetVirtualID()))
						if (IsConflictAlignmentInstance(rkInstVictim))
							return true;
			}
			else
			{
				return true;
			}
		}

		if (rkInstVictim.IsEnemy())
			return true;

		if (rkInstVictim.IsWoodenDoor())
			return true;
	}
	else if (IsEnemy())
	{
		if (rkInstVictim.IsPC())
			return true;

		if (rkInstVictim.IsBuilding())
			return true;
#ifdef BOSS_MARK_SYSTEM
		if (IsBoss())
			return NAMECOLOR_BOSS;
#endif	
	}
	else if (IsPoly())
	{
		if (rkInstVictim.IsPC())
			return true;

		if (rkInstVictim.IsEnemy())
			return true;
	}
	return false;
}

bool CInstanceBase::IsTargetableInstance(CInstanceBase& rkInstVictim)
{
	return rkInstVictim.CanPickInstance();
}

// 2004. 07. 07. [levites] - ��ų ����￿ Ÿ���� �ٲ�￿ ���� �ذ��� ���� �ڵ�
bool CInstanceBase::CanChangeTarget()
{
	return m_GraphicThingInstance.CanChangeTarget();
}

// 2004.07.17.levites.isShow�� ViewFrustumCheck�� ����
bool CInstanceBase::CanPickInstance()
{
	if (!__IsInViewFrustum())
		return false;

	if (IsDoor())
	{
		if (IsDead())
			return false;
	}

	if (IsPC())
	{
		if (IsAffect(AFFECT_EUNHYEONG))
		{
			if (!__MainCanSeeHiddenThing())
				return false;
		}
#ifdef ENABLE_CANSEEHIDDENTHING_FOR_GM
		if (IsAffect(AFFECT_REVIVE_INVISIBILITY) && !__MainCanSeeHiddenThing())
			return false;
#else
		if (IsAffect(AFFECT_REVIVE_INVISIBILITY))
			return false;
#endif
#ifdef ENABLE_CANSEEHIDDENTHING_FOR_GM
		if (IsAffect(AFFECT_INVISIBILITY) && !__MainCanSeeHiddenThing())
			return false;
#else
		if (IsAffect(AFFECT_INVISIBILITY))
			return false;
#endif

	}

	if (IsDead())
		return false;

	return true;
}

bool CInstanceBase::CanViewTargetHP(CInstanceBase& rkInstVictim)
{
#ifdef ENABLE_DEFENSAWESHIP
	if (rkInstVictim.IsHydraNPC())
		return true;
#endif
	if (rkInstVictim.IsStone())
		return true;
	else if (rkInstVictim.IsWoodenDoor())
	 	return true;
	else if (rkInstVictim.IsEnemy())
	 	return true;
	else if (rkInstVictim.IsPC())
		return true;
	return false;
}

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
BOOL CInstanceBase::IsOfflineShopNPC()
{
	if (GetVirtualNumber() == 30000)
		return true;

	return false;
}
#endif

BOOL CInstanceBase::IsPoly()
{
	return m_GraphicThingInstance.IsPoly();
}

BOOL CInstanceBase::IsPC()
{
	return m_GraphicThingInstance.IsPC();
}

BOOL CInstanceBase::IsNPC()
{
	return m_GraphicThingInstance.IsNPC();
}

#ifdef NEW_PET_SYSTEM
BOOL CInstanceBase::IsNewPet()
{
	return m_GraphicThingInstance.IsNewPet();
}
#endif

BOOL CInstanceBase::IsEnemy()
{
	return m_GraphicThingInstance.IsEnemy();
}

BOOL CInstanceBase::IsStone()
{
	return m_GraphicThingInstance.IsStone();
}

#ifdef BOSS_MARK_SYSTEM
BOOL CInstanceBase::IsBoss()
{
    if (GetRace() == 691)
        return TRUE;
    if (GetRace() == 692)
        return TRUE;
    if (GetRace() == 693)
        return TRUE;
    if (GetRace() == 791)
        return TRUE;
    if (GetRace() == 991)
        return TRUE;
    if (GetRace() == 992)
        return TRUE;
    if (GetRace() == 993)
        return TRUE;
    if (GetRace() == 1091)
        return TRUE;
    if (GetRace() == 1092)
        return TRUE;
    if (GetRace() == 1093)
        return TRUE;
    if (GetRace() == 1094)
        return TRUE;
    if (GetRace() == 1095)
        return TRUE;
    if (GetRace() == 2191)
        return TRUE;
    if (GetRace() == 1191)
        return TRUE;
    if (GetRace() == 1192)
        return TRUE;
    if (GetRace() == 1304)
        return TRUE;
    if (GetRace() == 1306)
        return TRUE;
    if (GetRace() == 1307)
        return TRUE;
    if (GetRace() == 1901)
        return TRUE;
    if (GetRace() == 1902)
        return TRUE;
    if (GetRace() == 1903)
        return TRUE;
    if (GetRace() == 2206)
        return TRUE;
    if (GetRace() == 2207)
        return TRUE;
    if (GetRace() == 2291)
        return TRUE;
    if (GetRace() == 2306)
        return TRUE;
    if (GetRace() == 2307)
        return TRUE;
    if (GetRace() == 2492)
        return TRUE;
    if (GetRace() == 2493)
        return TRUE;
    if (GetRace() == 2494)
        return TRUE;
    if (GetRace() == 2598)
        return TRUE;
    if (GetRace() == 3090)
        return TRUE;
    if (GetRace() == 3091)
        return TRUE;
    if (GetRace() == 3190)
        return TRUE;
    if (GetRace() == 3191)
        return TRUE;
    if (GetRace() == 3290)
        return TRUE;
    if (GetRace() == 3291)
        return TRUE;
    if (GetRace() == 3390)
        return TRUE;
    if (GetRace() == 3391)
        return TRUE;
    if (GetRace() == 3490)
        return TRUE;
    if (GetRace() == 3491)
        return TRUE;
    if (GetRace() == 3590)
        return TRUE;
    if (GetRace() == 3591)
        return TRUE;
    if (GetRace() == 3690)
        return TRUE;
    if (GetRace() == 3691)
        return TRUE;
    if (GetRace() == 3790)
        return TRUE;
    if (GetRace() == 3791)
        return TRUE;
    if (GetRace() == 3890)
        return TRUE;
    if (GetRace() == 3891)
        return TRUE;
    if (GetRace() == 5001)
        return TRUE;
    if (GetRace() == 5004)
        return TRUE;
    if (GetRace() == 5002)
        return TRUE;
    if (GetRace() == 5161)
        return TRUE;
    if (GetRace() == 5162)
        return TRUE;
    if (GetRace() == 5163)
        return TRUE;
    if (GetRace() == 6091)
        return TRUE;
    if (GetRace() == 6191)
        return TRUE;

    return FALSE;
}
#endif

BOOL CInstanceBase::IsGuildWall()	//IsBuilding ���ǹ���ü IsGuildWall�� ���庮��(���� ����)
{
	return IsWall(m_dwRace);
}


BOOL CInstanceBase::IsResource()
{
	switch (m_dwVirtualNumber)
	{
		case 20047:
		case 20048:
		case 20049:
		case 20050:
		case 20051:
		case 20052:
		case 20053:
		case 20054:
		case 20055:
		case 20056:
		case 20057:
		case 20058:
		case 20059:
		case 30301:
		case 30302:
		case 30303:
		case 30304:
		case 30305:
		case 30306:
			return TRUE;
	}

	return FALSE;
}

BOOL CInstanceBase::IsWarp()
{
	return m_GraphicThingInstance.IsWarp();
}

BOOL CInstanceBase::IsGoto()
{
	return m_GraphicThingInstance.IsGoto();
}

BOOL CInstanceBase::IsObject()
{
	return m_GraphicThingInstance.IsObject();
}

BOOL CInstanceBase::IsBuilding()
{
	return m_GraphicThingInstance.IsBuilding();
}

BOOL CInstanceBase::IsDoor()
{
	return m_GraphicThingInstance.IsDoor();
}

BOOL CInstanceBase::IsWoodenDoor()
{
	if (m_GraphicThingInstance.IsDoor())
	{
		int vnum = GetVirtualNumber();
		if (vnum == 13000) // ������
			return true;
		else if (vnum >= 30111 && vnum <= 30119) // ��͹￿
			return true;
		else
			return false;
	}
	else
	{
		return false;
	}
}

BOOL CInstanceBase::IsStoneDoor()
{
	return m_GraphicThingInstance.IsDoor() && 13001 == GetVirtualNumber();
}

BOOL CInstanceBase::IsFlag()
{
	if (GetRace() == 20035)
		return TRUE;
	if (GetRace() == 20036)
		return TRUE;
	if (GetRace() == 20037)
		return TRUE;

	return FALSE;
}

BOOL CInstanceBase::IsForceVisible()
{
	if (IsAffect(AFFECT_SHOW_ALWAYS))
		return TRUE;

	if (IsObject() || IsBuilding() || IsDoor() )
		return TRUE;

	return FALSE;
}

int	CInstanceBase::GetInstanceType()
{
	return m_GraphicThingInstance.GetActorType();
}

DWORD CInstanceBase::GetVirtualID()
{
	return m_GraphicThingInstance.GetVirtualID();
}

DWORD CInstanceBase::GetVirtualNumber()
{
	return m_dwVirtualNumber;
}

// 2004.07.17.levites.isShow�� ViewFrustumCheck�� ����
bool CInstanceBase::__IsInViewFrustum()
{
	return m_GraphicThingInstance.isShow();
}

#ifdef RENDER_TARGET
bool CInstanceBase::__CanRender()
{
	if (IsAlwaysRender())
	{
		return true;
	}

	if (!__IsInViewFrustum())
		return false;

#ifdef ENABLE_CANSEEHIDDENTHING_FOR_GM
	if (IsAffect(AFFECT_INVISIBILITY) && !__MainCanSeeHiddenThing())
		return false;
#else
	if (IsAffect(AFFECT_INVISIBILITY))
		return false;
#endif

	return true;
}
#else
bool CInstanceBase::__CanRender()
{
	if (!__IsInViewFrustum())
		return false;

#ifdef ENABLE_CANSEEHIDDENTHING_FOR_GM
	if (IsAffect(AFFECT_INVISIBILITY) && !__MainCanSeeHiddenThing())
		return false;
#else
	if (IsAffect(AFFECT_INVISIBILITY))
		return false;
#endif

	return true;
}
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////
// Graphic Control

bool CInstanceBase::IntersectBoundingBox()
{
	float u, v, t;
	return m_GraphicThingInstance.Intersect(&u, &v, &t);
}

bool CInstanceBase::IntersectDefendingSphere()
{
	return m_GraphicThingInstance.IntersectDefendingSphere();
}

float CInstanceBase::GetDistance(CInstanceBase * pkTargetInst)
{
	TPixelPosition TargetPixelPosition;
	pkTargetInst->m_GraphicThingInstance.GetPixelPosition(&TargetPixelPosition);
	return GetDistance(TargetPixelPosition);
}

float CInstanceBase::GetDistance(const TPixelPosition & c_rPixelPosition)
{
	TPixelPosition PixelPosition;
	m_GraphicThingInstance.GetPixelPosition(&PixelPosition);

	float fdx = PixelPosition.x - c_rPixelPosition.x;
	float fdy = PixelPosition.y - c_rPixelPosition.y;

	return sqrtf((fdx*fdx) + (fdy*fdy));
}

CActorInstance& CInstanceBase::GetGraphicThingInstanceRef()
{
	return m_GraphicThingInstance;
}

CActorInstance* CInstanceBase::GetGraphicThingInstancePtr()
{
	return &m_GraphicThingInstance;
}

void CInstanceBase::RefreshActorInstance()
{
	m_GraphicThingInstance.RefreshActorInstance();
}

void CInstanceBase::Refresh(DWORD dwMotIndex, bool isLoop)
{
	RefreshState(dwMotIndex, isLoop);
}

void CInstanceBase::RestoreRenderMode()
{
	m_GraphicThingInstance.RestoreRenderMode();
}

void CInstanceBase::SetAddRenderMode()
{
	m_GraphicThingInstance.SetAddRenderMode();
}

void CInstanceBase::SetModulateRenderMode()
{
	m_GraphicThingInstance.SetModulateRenderMode();
}

void CInstanceBase::SetRenderMode(int iRenderMode)
{
	m_GraphicThingInstance.SetRenderMode(iRenderMode);
}

void CInstanceBase::SetAddColor(const D3DXCOLOR & c_rColor)
{
	m_GraphicThingInstance.SetAddColor(c_rColor);
}

void CInstanceBase::__SetBlendRenderingMode()
{
	m_GraphicThingInstance.SetBlendRenderMode();
}

void CInstanceBase::__SetAlphaValue(float fAlpha)
{
	m_GraphicThingInstance.SetAlphaValue(fAlpha);
}

float CInstanceBase::__GetAlphaValue()
{
	return m_GraphicThingInstance.GetAlphaValue();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Part

void CInstanceBase::SetHair(DWORD eHair)
{
	if (!HAIR_COLOR_ENABLE)
		return;

	if (IsPC()==false)
		return;
	m_awPart[CRaceData::PART_HAIR] = eHair;
	m_GraphicThingInstance.SetHair(eHair);
}

void CInstanceBase::ChangeHair(DWORD eHair)
{
	if (!HAIR_COLOR_ENABLE)
		return;

	if (IsPC()==false)
		return;

	if (GetPart(CRaceData::PART_HAIR)==eHair)
		return;

	SetHair(eHair);

	//int type = m_GraphicThingInstance.GetMotionMode();

	RefreshState(CRaceMotionData::NAME_WAIT, true);
	//RefreshState(type, true);
}

void CInstanceBase::SetArmor(DWORD dwArmor)
{
	DWORD dwShape;
	if (__ArmorVnumToShape(dwArmor, &dwShape))
	{
		CItemData * pItemData;
		if (CItemManager::Instance().GetItemDataPointer(dwArmor, &pItemData))
		{
			float fSpecularPower = pItemData->GetSpecularPowerf();
			SetShape(dwShape, fSpecularPower);
			__GetRefinedEffect(pItemData);
#ifdef ENABLE_SHINING_SYSTEM
			__GetShiningEffect(pItemData);
#endif
			return;
		}
		else
		{
			__ClearArmorRefineEffect();
#ifdef ENABLE_SHINING_SYSTEM
			__ClearArmorShiningEffect();
#endif
		}
	}

	SetShape(dwArmor);
}

#ifdef ENABLE_ACCE_COSTUME_SYSTEM
void CInstanceBase::SetAcce(DWORD dwAcce)
{
	if (!IsPC())
		return;
	if (IsPoly())
		return;
	dwAcce += 85000;
	ClearAcceEffect();
	float fSpecular = 65.0f;
	if (dwAcce > 86000)
	{
		dwAcce -= 1000;
		fSpecular += 35;
		m_dwAcceEffect = EFFECT_REFINED + EFFECT_ACCE;
		__EffectContainer_AttachEffect(m_dwAcceEffect);
	}
	fSpecular /= 100.0f;
	m_awPart[CRaceData::PART_ACCE] = dwAcce;
	CItemData * pItemData = NULL;
	CItemManager::Instance().GetItemDataPointer(dwAcce, &pItemData);
		m_GraphicThingInstance.AttachAcce(pItemData, fSpecular);
#ifdef ENABLE_OBJ_SCALLING
	DWORD dwRace = GetRace(), dwPos = RaceToJob(dwRace), dwSex = RaceToSex(dwRace);
	dwPos += 1;
	if (dwSex == 0)
		dwPos += 5;
	float fScaleX, fScaleY, fScaleZ, fPositionX, fPositionY, fPositionZ;
	if (pItemData && pItemData->GetItemScale(dwPos, fScaleX, fScaleY, fScaleZ, fPositionX, fPositionY, fPositionZ))
	{
		m_GraphicThingInstance.SetScale(fScaleX, fScaleY, fScaleZ, true);
		if (m_kHorse.IsMounting())
			fPositionZ += 10.0f;
		m_GraphicThingInstance.SetScalePosition(fPositionX, fPositionY, fPositionZ);
	}
#endif
}
void CInstanceBase::ChangeAcce(DWORD dwAcce)
{
	if (!IsPC())
		return;
	SetAcce(dwAcce);
}
void CInstanceBase::ClearAcceEffect()
{
	if (!m_dwAcceEffect)
		return;
	__EffectContainer_DetachEffect(m_dwAcceEffect);
	m_dwAcceEffect = 0;
}
#endif
void CInstanceBase::SetShape(DWORD eShape, float fSpecular)
{
	if (IsPoly())
	{
		m_GraphicThingInstance.SetShape(0);
	}
	else
	{
		m_GraphicThingInstance.SetShape(eShape, fSpecular);
	}

	m_eShape = eShape;
}



DWORD CInstanceBase::GetWeaponType()
{
	DWORD dwWeapon = GetPart(CRaceData::PART_WEAPON);
	CItemData * pItemData;
	if (!CItemManager::Instance().GetItemDataPointer(dwWeapon, &pItemData))
		return CItemData::WEAPON_NONE;

	return pItemData->GetWeaponType();
}

#ifdef RENDER_SHINING
void CInstanceBase::SetEffect() {
	GetGraphicThingInstanceRef().RenderAllAttachingEffect();
	Refresh(CRaceMotionData::NAME_WAIT, true);
}	
#endif

/*
void CInstanceBase::SetParts(const WORD * c_pParts)
{
	if (IsPoly())
		return;

	if (__IsShapeAnimalWear())
		return;

	UINT eWeapon=c_pParts[CRaceData::PART_WEAPON];

	if (__IsChangableWeapon(eWeapon) == false)
			eWeapon = 0;

	if (eWeapon != m_GraphicThingInstance.GetPartItemID(CRaceData::PART_WEAPON))
	{
		m_GraphicThingInstance.AttachPart(CRaceData::PART_MAIN, CRaceData::PART_WEAPON, eWeapon);
		m_awPart[CRaceData::PART_WEAPON] = eWeapon;
	}

	__AttachHorseSaddle();
}
*/

void CInstanceBase::__ClearWeaponRefineEffect()
{
	if (m_swordRefineEffectRight)
	{
		__DetachEffect(m_swordRefineEffectRight);
		m_swordRefineEffectRight = 0;
	}
	if (m_swordRefineEffectLeft)
	{
		__DetachEffect(m_swordRefineEffectLeft);
		m_swordRefineEffectLeft = 0;
	}
}

void CInstanceBase::__ClearArmorRefineEffect()
{
	if (m_armorRefineEffect)
	{
		__DetachEffect(m_armorRefineEffect);
		m_armorRefineEffect = 0;
#ifdef GUILD_RANK_EFFECT
		rankIndex = 0;
		rankAffectIndex = 0;
#endif
	}
}

// #define ENABLE_SIMPLE_REFINED_EFFECT_CHECK
// #define USE_WEAPON_COSTUME_WITH_EFFECT
// #define USE_BODY_COSTUME_WITH_EFFECT
UINT CInstanceBase::__GetRefinedEffect(CItemData* pItem)
{
#ifdef ENABLE_SIMPLE_REFINED_EFFECT_CHECK
	DWORD refine = pItem->GetRefine();
#else
	DWORD refine = max(pItem->GetRefine() + pItem->GetSocketCount(),CItemData::ITEM_SOCKET_MAX_NUM) - CItemData::ITEM_SOCKET_MAX_NUM;
#endif
	switch (pItem->GetType())
	{
	case CItemData::ITEM_TYPE_WEAPON:
		__ClearWeaponRefineEffect();
		if (refine < 7)	//���� ���õ� 7 �̻� ����Ʈ�� �ֽ��ϴ�.
			return 0;
		switch(pItem->GetSubType())
		{
		case CItemData::WEAPON_DAGGER:
			m_swordRefineEffectRight = EFFECT_REFINED+EFFECT_SMALLSWORD_REFINED7+refine-7;
			m_swordRefineEffectLeft = EFFECT_REFINED+EFFECT_SMALLSWORD_REFINED7_LEFT+refine-7;
			break;
		case CItemData::WEAPON_FAN:
			m_swordRefineEffectRight = EFFECT_REFINED+EFFECT_FANBELL_REFINED7+refine-7;
			break;
		case CItemData::WEAPON_ARROW:
#ifdef ENABLE_NEW_ARROW_SYSTEM
		case CItemData::WEAPON_UNLIMITED_ARROW:
#endif
		case CItemData::WEAPON_BELL:
			m_swordRefineEffectRight = EFFECT_REFINED+EFFECT_SMALLSWORD_REFINED7+refine-7;
			break;
		case CItemData::WEAPON_BOW:
			m_swordRefineEffectRight = EFFECT_REFINED+EFFECT_BOW_REFINED7+refine-7;
			break;
#ifdef ENABLE_WOLFMAN_CHARACTER
		case CItemData::WEAPON_CLAW:
			m_swordRefineEffectRight = EFFECT_REFINED + EFFECT_SMALLSWORD_REFINED7 + refine - 7;
			m_swordRefineEffectLeft = EFFECT_REFINED + EFFECT_SMALLSWORD_REFINED7_LEFT + refine - 7;
			break;
#endif
		default:
			m_swordRefineEffectRight = EFFECT_REFINED+EFFECT_SWORD_REFINED7+refine-7;
		}
		if (m_swordRefineEffectRight)
			m_swordRefineEffectRight = __AttachEffect(m_swordRefineEffectRight);
		if (m_swordRefineEffectLeft)
			m_swordRefineEffectLeft = __AttachEffect(m_swordRefineEffectLeft);
		break;
	case CItemData::ITEM_TYPE_ARMOR:
		__ClearArmorRefineEffect();

		// ���� Ưȭ ����Ʈ
		if (pItem->GetSubType() == CItemData::ARMOR_BODY)
		{
			DWORD vnum = pItem->GetIndex();

			if (
				(12010 <= vnum && vnum <= 12049)
#ifdef ENABLE_WOLFMAN_CHARACTER
				|| (21080 <= vnum && vnum <= 21089)
#endif
			)
			{
				__AttachEffect(EFFECT_REFINED+EFFECT_BODYARMOR_SPECIAL);
				__AttachEffect(EFFECT_REFINED+EFFECT_BODYARMOR_SPECIAL2);
			}
#ifdef ENABLE_LVL115_ARMOR_EFFECT
			else if (20760 <= vnum && vnum <= 20959)
			{
				__AttachEffect(EFFECT_REFINED+EFFECT_BODYARMOR_SPECIAL3);
			}
#endif //ENABLE_LVL115_ARMOR_EFFECT
		}

		if (refine < 7)	//���� ���õ� 7 �̻� ����Ʈ�� �ֽ��ϴ�.
			return 0;

		if (pItem->GetSubType() == CItemData::ARMOR_BODY)
		{
			m_armorRefineEffect = EFFECT_REFINED+EFFECT_BODYARMOR_REFINED7+refine-7;
			__AttachEffect(m_armorRefineEffect);
		}
		break;
	case CItemData::ITEM_TYPE_COSTUME:
#ifdef ENABLE_WEAPON_COSTUME_SYSTEM
		if (pItem->GetSubType() == CItemData::COSTUME_WEAPON)
		{
			__ClearWeaponRefineEffect();

#ifdef USE_WEAPON_COSTUME_WITH_EFFECT
			switch(pItem->GetValue(3))
			{
				case CItemData::WEAPON_DAGGER:
					m_swordRefineEffectRight = EFFECT_REFINED+EFFECT_SMALLSWORD_REFINED9;
					m_swordRefineEffectLeft = EFFECT_REFINED+EFFECT_SMALLSWORD_REFINED9_LEFT;
					break;
				case CItemData::WEAPON_FAN:
					m_swordRefineEffectRight = EFFECT_REFINED+EFFECT_FANBELL_REFINED9;
					break;
				case CItemData::WEAPON_ARROW:
				case CItemData::WEAPON_BELL:
					m_swordRefineEffectRight = EFFECT_REFINED+EFFECT_SMALLSWORD_REFINED9;
					break;
				case CItemData::WEAPON_BOW:
					m_swordRefineEffectRight = EFFECT_REFINED+EFFECT_BOW_REFINED9;
					break;
#ifdef ENABLE_WOLFMAN_CHARACTER
				case CItemData::WEAPON_CLAW:
					m_swordRefineEffectRight = EFFECT_REFINED + EFFECT_SMALLSWORD_REFINED9;
					m_swordRefineEffectLeft = EFFECT_REFINED + EFFECT_SMALLSWORD_REFINED9_LEFT;
					break;
#endif
				default:
					m_swordRefineEffectRight = EFFECT_REFINED+EFFECT_SWORD_REFINED9;
			}
			if (m_swordRefineEffectRight)
				m_swordRefineEffectRight = __AttachEffect(m_swordRefineEffectRight);
			if (m_swordRefineEffectLeft)
				m_swordRefineEffectLeft = __AttachEffect(m_swordRefineEffectLeft);
#endif //USE_WEAPON_COSTUME_WITH_EFFECT
			break;
		}
#endif
#ifdef USE_BODY_COSTUME_WITH_EFFECT
		if (pItem->GetSubType() == CItemData::COSTUME_BODY)
		{
			__ClearArmorRefineEffect();

			// lvl80 armor's effect (blue smoke+bubbles)
			// {
				// __AttachEffect(EFFECT_REFINED+EFFECT_BODYARMOR_SPECIAL);
				// __AttachEffect(EFFECT_REFINED+EFFECT_BODYARMOR_SPECIAL2);
			// }

			// lvl105 armor's effect (sparkles)
			// {
				// __AttachEffect(EFFECT_REFINED+EFFECT_BODYARMOR_SPECIAL3);
			// }

			m_armorRefineEffect = EFFECT_REFINED+EFFECT_BODYARMOR_REFINED9;
			__AttachEffect(m_armorRefineEffect);
			break;
		}
#endif //USE_BODY_COSTUME_WITH_EFFECT
		break;
	}
	return 0;
}

#ifdef ENABLE_NEW_ARROW_SYSTEM
bool CInstanceBase::SetWeapon(DWORD eWeapon, DWORD eArrow)
{
	if (IsPoly())
		return false;

	if (__IsShapeAnimalWear())
		return false;

	m_awPart[CRaceData::PART_ARROW_TYPE] = eArrow;
	if (__IsChangableWeapon(eWeapon) == false)
		eWeapon = 0;

	m_GraphicThingInstance.AttachWeapon(eWeapon);
	m_awPart[CRaceData::PART_WEAPON] = eWeapon;

	CItemData* pItemData;
	if (CItemManager::Instance().GetItemDataPointer(eWeapon, &pItemData))
	{ //
		__GetRefinedEffect(pItemData);
#ifdef ENABLE_SHINING_SYSTEM
		__GetShiningEffect(pItemData);
#endif
	} //
	else
	{ //
		__ClearWeaponRefineEffect();
#ifdef ENABLE_SHINING_SYSTEM
		__ClearWeaponShiningEffect();
#endif
	} //	
	return true;
}
#else
bool CInstanceBase::SetWeapon(DWORD eWeapon)
{
	if (IsPoly())
		return false;

	if (__IsShapeAnimalWear())
		return false;

	if (__IsChangableWeapon(eWeapon) == false)
		eWeapon = 0;

	m_GraphicThingInstance.AttachWeapon(eWeapon);
	m_awPart[CRaceData::PART_WEAPON] = eWeapon;

	CItemData* pItemData;
	if (CItemManager::Instance().GetItemDataPointer(eWeapon, &pItemData))
	{ //
		__GetRefinedEffect(pItemData);
#ifdef ENABLE_SHINING_SYSTEM
		__GetShiningEffect(pItemData);
#endif
	} //
	else
	{ //
		__ClearWeaponRefineEffect();
#ifdef ENABLE_SHINING_SYSTEM
		__ClearWeaponShiningEffect();
#endif
	} //
	return true;
}
#endif

#ifdef ENABLE_NEW_ARROW_SYSTEM
void CInstanceBase::ChangeWeapon(DWORD eWeapon, DWORD eArrow)
{
	CItemData* pItemData;
	m_awPart[CRaceData::PART_ARROW_TYPE] = eArrow;

	if (CItemManager::Instance().GetItemDataPointer(eWeapon, &pItemData))
	{
		if (!m_kHorse.IsMounting())
		{
			if (pItemData->GetSubType() == CItemData::WEAPON_BOW)
			{
				if (eArrow == CItemData::WEAPON_UNLIMITED_ARROW)
					SetMotionMode(CRaceMotionData::MODE_BOW_SPECIAL);
				else
					SetMotionMode(CRaceMotionData::MODE_BOW);
			}
			else if (pItemData->GetType() == CItemData::ITEM_TYPE_COSTUME && pItemData->GetValue(3) == CItemData::WEAPON_BOW)
			{
				if (eArrow == CItemData::WEAPON_UNLIMITED_ARROW)
					SetMotionMode(CRaceMotionData::MODE_BOW_SPECIAL);
				else
					SetMotionMode(CRaceMotionData::MODE_BOW);
			}
		}
		else
		{
			if (pItemData->GetSubType() == CItemData::WEAPON_BOW)
			{
				if (eArrow == CItemData::WEAPON_UNLIMITED_ARROW)
					SetMotionMode(CRaceMotionData::MODE_HORSE_BOW_SPECIAL);
				else
					SetMotionMode(CRaceMotionData::MODE_HORSE_BOW);
			}
			else if (pItemData->GetType() == CItemData::ITEM_TYPE_COSTUME && pItemData->GetValue(3) == CItemData::WEAPON_BOW)
			{
				if (eArrow == CItemData::WEAPON_UNLIMITED_ARROW)
					SetMotionMode(CRaceMotionData::MODE_HORSE_BOW_SPECIAL);
				else
					SetMotionMode(CRaceMotionData::MODE_HORSE_BOW);
			}
		}
	}

	if (eWeapon == m_GraphicThingInstance.GetPartItemID(CRaceData::PART_WEAPON))
		return;

	if (SetWeapon(eWeapon, eArrow))
		RefreshState(CRaceMotionData::NAME_WAIT, true);
}
#else
void CInstanceBase::ChangeWeapon(DWORD eWeapon)
{
	CItemData* pItemData;
	if (eWeapon == m_GraphicThingInstance.GetPartItemID(CRaceData::PART_WEAPON))
		return;

	if (SetWeapon(eWeapon))
		RefreshState(CRaceMotionData::NAME_WAIT, true);
}
#endif

bool CInstanceBase::ChangeArmor(DWORD dwArmor)
{
	DWORD eShape;
	__ArmorVnumToShape(dwArmor, &eShape);

	if (GetShape()==eShape)
		return false;

	CAffectFlagContainer kAffectFlagContainer;
	kAffectFlagContainer.CopyInstance(m_kAffectFlagContainer);

	DWORD dwVID = GetVirtualID();
	DWORD dwRace = GetRace();
	DWORD eHair = GetPart(CRaceData::PART_HAIR);
#ifdef ENABLE_ACCE_COSTUME_SYSTEM
	DWORD dwAcce = GetPart(CRaceData::PART_ACCE);
#endif
#ifdef ENABLE_AURA_SYSTEM
	DWORD eAura = GetPart(CRaceData::PART_AURA);
#endif
	DWORD eWeapon = GetPart(CRaceData::PART_WEAPON);

	float fRot = GetRotation();
	float fAdvRot = GetAdvancingRotation();

	if (IsWalking())
		EndWalking();

	// 2004.07.25.myevan.����Ʈ �� �ٴ� ����
	//////////////////////////////////////////////////////
	__ClearAffects();
	//////////////////////////////////////////////////////

	if (!SetRace(dwRace))
	{
		TraceError("CPythonCharacterManager::ChangeArmor - SetRace VID[%d] Race[%d] ERROR", dwVID, dwRace);
		return false;
	}

	SetArmor(dwArmor);
	SetHair(eHair);
#ifdef ENABLE_ACCE_COSTUME_SYSTEM
	SetAcce(dwAcce);
#endif
#ifdef ENABLE_AURA_SYSTEM
	SetAura(eAura);
#endif
	SetWeapon(eWeapon);


	SetRotation(fRot);
	SetAdvancingRotation(fAdvRot);

	__AttachHorseSaddle();

	RefreshState(CRaceMotionData::NAME_WAIT, TRUE);

	// 2004.07.25.myevan.����Ʈ �� �ٴ� ����
	/////////////////////////////////////////////////
	SetAffectFlagContainer(kAffectFlagContainer);
	/////////////////////////////////////////////////

	CActorInstance::IEventHandler& rkEventHandler=GetEventHandlerRef();
	rkEventHandler.OnChangeShape();

	return true;
}

bool CInstanceBase::__IsShapeAnimalWear()
{
	if (100 == GetShape() ||
		101 == GetShape() ||
		102 == GetShape() ||
		103 == GetShape())
		return true;

	return false;
}

DWORD CInstanceBase::__GetRaceType()
{
	return m_eRaceType;
}


void CInstanceBase::RefreshState(DWORD dwMotIndex, bool isLoop)
{
	DWORD dwPartItemID = m_GraphicThingInstance.GetPartItemID(CRaceData::PART_WEAPON);

	BYTE byItemType = 0xff;
	BYTE bySubType = 0xff;

	CItemManager & rkItemMgr = CItemManager::Instance();
	CItemData * pItemData;

	if (rkItemMgr.GetItemDataPointer(dwPartItemID, &pItemData))
	{
		byItemType = pItemData->GetType();
		bySubType = pItemData->GetWeaponType();
	}

	if (IsPoly())
	{
		SetMotionMode(CRaceMotionData::MODE_GENERAL);
	}
	else if (IsWearingDress())
	{
		SetMotionMode(CRaceMotionData::MODE_WEDDING_DRESS);
	}
	else if (IsHoldingPickAxe())
	{
		if (m_kHorse.IsMounting())
		{
			SetMotionMode(CRaceMotionData::MODE_HORSE);
		}
		else
		{
			SetMotionMode(CRaceMotionData::MODE_GENERAL);
		}
	}
	else if (CItemData::ITEM_TYPE_ROD == byItemType)
	{
		if (m_kHorse.IsMounting())
		{
			SetMotionMode(CRaceMotionData::MODE_HORSE);
		}
		else
		{
			SetMotionMode(CRaceMotionData::MODE_FISHING);
		}
	}
	else if (m_kHorse.IsMounting())
	{
		switch (bySubType)
		{
			case CItemData::WEAPON_SWORD:
				SetMotionMode(CRaceMotionData::MODE_HORSE_ONEHAND_SWORD);
				break;

			case CItemData::WEAPON_TWO_HANDED:
				SetMotionMode(CRaceMotionData::MODE_HORSE_TWOHAND_SWORD); // Only Warrior
				break;

			case CItemData::WEAPON_DAGGER:
				SetMotionMode(CRaceMotionData::MODE_HORSE_DUALHAND_SWORD); // Only Assassin
				break;

			case CItemData::WEAPON_FAN:
				SetMotionMode(CRaceMotionData::MODE_HORSE_FAN); // Only Shaman
				break;

			case CItemData::WEAPON_BELL:
				SetMotionMode(CRaceMotionData::MODE_HORSE_BELL); // Only Shaman
				break;

			case CItemData::WEAPON_BOW:
#ifdef ENABLE_NEW_ARROW_SYSTEM
			{
				if (m_awPart[CRaceData::PART_ARROW_TYPE] == CItemData::WEAPON_UNLIMITED_ARROW)
					SetMotionMode(CRaceMotionData::MODE_BOW_SPECIAL);
				else
					SetMotionMode(CRaceMotionData::MODE_BOW);
			}
#else
				SetMotionMode(CRaceMotionData::MODE_BOW);
#endif
				break;
#ifdef ENABLE_WOLFMAN_CHARACTER
			case CItemData::WEAPON_CLAW:
				SetMotionMode(CRaceMotionData::MODE_HORSE_CLAW); // Only Wolfman
				break;
#endif
			default:
				SetMotionMode(CRaceMotionData::MODE_HORSE);
				break;
		}
	}
	else
	{
		switch (bySubType)
		{
			case CItemData::WEAPON_SWORD:
				SetMotionMode(CRaceMotionData::MODE_ONEHAND_SWORD);
				break;

			case CItemData::WEAPON_TWO_HANDED:
				SetMotionMode(CRaceMotionData::MODE_TWOHAND_SWORD); // Only Warrior
				break;

			case CItemData::WEAPON_DAGGER:
				SetMotionMode(CRaceMotionData::MODE_DUALHAND_SWORD); // Only Assassin
				break;

			case CItemData::WEAPON_BOW:
				SetMotionMode(CRaceMotionData::MODE_BOW);
				break;

			case CItemData::WEAPON_FAN:
				SetMotionMode(CRaceMotionData::MODE_FAN); // Only Shaman
				break;

			case CItemData::WEAPON_BELL:
				SetMotionMode(CRaceMotionData::MODE_BELL); // Only Shaman
				break;
#ifdef ENABLE_WOLFMAN_CHARACTER
			case CItemData::WEAPON_CLAW:
				SetMotionMode(CRaceMotionData::MODE_CLAW); // Only Wolfman
				break;
#endif
			case CItemData::WEAPON_ARROW:
#ifdef ENABLE_NEW_ARROW_SYSTEM
			case CItemData::WEAPON_UNLIMITED_ARROW:
#endif
			default:
				SetMotionMode(CRaceMotionData::MODE_GENERAL);
				break;
		}
	}

	if (isLoop)
		m_GraphicThingInstance.InterceptLoopMotion(dwMotIndex);
	else
		m_GraphicThingInstance.InterceptOnceMotion(dwMotIndex);

	RefreshActorInstance();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Device

void CInstanceBase::RegisterBoundingSphere()
{
	// Stone �� ��￿ DeforomNoSkin �� �ϸ�
	// �����ϴ� �ִϸ��̼� ���� ��￿ �ִϸ��̼���
	// �ٿ�￿ �ڽ��� ������ ���� �ø��� ����￿ �̷�����￿ �ʴ´�.
	if (!IsStone())
	{
		m_GraphicThingInstance.DeformNoSkin();
	}

	m_GraphicThingInstance.RegisterBoundingSphere();
}

bool CInstanceBase::CreateDeviceObjects()
{
	return m_GraphicThingInstance.CreateDeviceObjects();
}

void CInstanceBase::DestroyDeviceObjects()
{
	m_GraphicThingInstance.DestroyDeviceObjects();
}

void CInstanceBase::Destroy()
{
	DetachTextTail();

	DismountHorse();

	m_kQue_kCmdNew.clear();

	__EffectContainer_Destroy();
	__StoneSmoke_Destroy();

	if (__IsMainInstance())
		__ClearMainInstance();

	m_GraphicThingInstance.Destroy();

	__Initialize();
}

void CInstanceBase::__InitializeRotationSpeed()
{
	SetRotationSpeed(c_fDefaultRotationSpeed);
}

void CInstanceBase::__Warrior_Initialize()
{
	m_kWarrior.m_dwGeomgyeongEffect=0;
}

void CInstanceBase::__Initialize()
{
	__Warrior_Initialize();
	__StoneSmoke_Inialize();
	__EffectContainer_Initialize();
	__InitializeRotationSpeed();

	SetEventHandler(CActorInstance::IEventHandler::GetEmptyPtr());

	m_kAffectFlagContainer.Clear();

	m_dwLevel = 0;
	m_dwGuildID = 0;
	m_dwEmpireID = 0;

	m_eType = 0;
	m_eRaceType = 0;
	m_eShape = 0;
	m_dwRace = 0;
	m_dwVirtualNumber = 0;

	m_dwBaseCmdTime=0;
	m_dwBaseChkTime=0;
	m_dwSkipTime=0;

	m_GraphicThingInstance.Initialize();

	m_dwAdvActorVID=0;
	m_dwLastDmgActorVID=0;

	m_nAverageNetworkGap=0;
	m_dwNextUpdateHeightTime=0;

	// Moving by keyboard
	m_iRotatingDirection = DEGREE_DIRECTION_SAME;
	m_CanSelect = true;
	// Moving by mouse
	m_isTextTail = FALSE;
	m_isGoing = FALSE;
	NEW_SetSrcPixelPosition(TPixelPosition(0, 0, 0));
	NEW_SetDstPixelPosition(TPixelPosition(0, 0, 0));

	m_kPPosDust = TPixelPosition(0, 0, 0);


	m_kQue_kCmdNew.clear();

	m_dwLastComboIndex = 0;

	m_swordRefineEffectRight = 0;
	m_swordRefineEffectLeft = 0;
	m_armorRefineEffect = 0;
#ifdef ENABLE_SHINING_SYSTEM
	__ClearWeaponShiningEffect(false);
	__ClearArmorShiningEffect(false);
#endif
#ifdef ENABLE_ACCE_COSTUME_SYSTEM
	m_dwAcceEffect = 0;
#endif
#ifdef ENABLE_AURA_SYSTEM
	m_auraRefineEffect = 0;
#endif
#ifdef ENABLE_EFFECT_COSTUME_SYSTEM
	m_vecEquipEffect.clear();
#endif
	m_sAlignment = 0;
	m_byPKMode = 0;
	m_isKiller = false;
	m_isPartyMember = false;

	m_bEnableTCPState = TRUE;

#ifdef ___DECORUM___
	m_dwDecorumPoint = 0;
#endif

	m_stName = "";
#ifdef ENABLE_MULTI_LANGUAGE_SYSTEM
	m_stLanguage = "";
#endif

	memset(m_awPart, 0, sizeof(m_awPart));
	memset(m_adwCRCAffectEffect, 0, sizeof(m_adwCRCAffectEffect));
	//memset(m_adwCRCEmoticonEffect, 0, sizeof(m_adwCRCEmoticonEffect));
	memset(&m_kMovAfterFunc, 0, sizeof(m_kMovAfterFunc));

	m_bDamageEffectType = false;
	m_dwDuelMode = DUEL_NONE;
	m_dwEmoticonTime = 0;
#ifdef RENDER_TARGET
	m_IsAlwaysRender = false;
#endif
#ifdef ENABLE_DOG_MODE
	m_dwOriginalRace = 0;
#endif
}

CInstanceBase::CInstanceBase()
{
	__Initialize();
}

CInstanceBase::~CInstanceBase()
{
	Destroy();
}

#ifdef ENABLE_DOG_MODE
DWORD CInstanceBase::GetOriginalRace()
{
	return m_dwOriginalRace;
}

void CInstanceBase::SetOriginalRace(DWORD race)
{
	m_dwOriginalRace = race;
}

void CInstanceBase::ChangeRace(DWORD eRace, DWORD eShape)
{
	m_dwRace = eRace;

	if (!m_GraphicThingInstance.SetRace(eRace))
		return;

	m_GraphicThingInstance.SetShape(eShape, 0.0f);

	m_GraphicThingInstance.RefreshActorInstance();
	Refresh(CRaceMotionData::NAME_WAIT, false);
}
#endif

#ifdef RENDER_TARGET
bool CInstanceBase::IsAlwaysRender()
{
	return m_IsAlwaysRender;
}

void CInstanceBase::SetAlwaysRender(bool val)
{
	m_IsAlwaysRender = val;
}
#endif


void CInstanceBase::GetBoundBox(D3DXVECTOR3 * vtMin, D3DXVECTOR3 * vtMax)
{
	m_GraphicThingInstance.GetBoundBox(vtMin, vtMax);
}

#ifdef ENABLE_AURA_SYSTEM
void CInstanceBase::ChangeAura(DWORD eAura)
{
	if (IsPC() == false)
		return;

	SetAura(eAura);
}

static DWORD GetAuraVnum(DWORD aura)
{
	if (aura >= 1 && aura <= 9)
		return 49001;
	else if (aura >= 10 && aura <= 49)
		return 49002;
	else if (aura >= 50 && aura <= 99)
		return 49003;
	else if (aura >= 100 && aura <= 249)
		return 49004;
	else if (aura >= 250 && aura <= 499)
		return 49005;
	else if (aura == 500)
		return 49006;
	return 0;
}

#ifdef RENDER_SHINING
void CInstanceBase::SetAuraRender(DWORD AuraNum)
{
	if (!IsPC() || IsPoly() || __IsShapeAnimalWear())
		return;

	switch(AuraNum)
	{
		case 49001:
			m_auraRefineEffect = EFFECT_REFINED + EFFECT_AURA_0;
			__EffectContainer_AttachEffect(m_auraRefineEffect);
			break;
		case 49002:
			m_auraRefineEffect = EFFECT_REFINED + EFFECT_AURA_1;
			__EffectContainer_AttachEffect(m_auraRefineEffect);
			break;
		case 49003:
			m_auraRefineEffect = EFFECT_REFINED + EFFECT_AURA_2;
			__EffectContainer_AttachEffect(m_auraRefineEffect);
			break;
		case 49004:
			m_auraRefineEffect = EFFECT_REFINED + EFFECT_AURA_3;
			__EffectContainer_AttachEffect(m_auraRefineEffect);
			break;
		case 49005:
			m_auraRefineEffect = EFFECT_REFINED + EFFECT_AURA_4;
			__EffectContainer_AttachEffect(m_auraRefineEffect);
			break;
		case 49006:
			m_auraRefineEffect = EFFECT_REFINED + EFFECT_AURA_5;
			__EffectContainer_AttachEffect(m_auraRefineEffect);
			break;
	}

}
#endif

void CInstanceBase::SetAura(DWORD eAura)
{
	if (!IsPC() || IsPoly() || __IsShapeAnimalWear())
		return;
	CItemData* pItemData;
	if (eAura == 0 || !CItemManager::Instance().GetItemDataPointer(GetAuraVnum(eAura), &pItemData))
	{
		if (m_auraRefineEffect)
		{
			__DetachEffect(m_auraRefineEffect);
			m_auraRefineEffect = 0;
		}
		m_awPart[CRaceData::PART_AURA] = 0;
		return;
	}
	else if (m_awPart[CRaceData::PART_AURA] == eAura)
	{
		if (m_auraRefineEffect == 0){
			goto GIVE_EFFECT;
		}
		else
		{
			return;
		}
	}

GIVE_EFFECT:
	if (m_auraRefineEffect)
	{
		__DetachEffect(m_auraRefineEffect);
		m_auraRefineEffect = 0;
	}
	m_awPart[CRaceData::PART_AURA] = eAura;
	DWORD dwRace = GetRace(), dwPos = RaceToJob2(dwRace), dwSex = RaceToSex2(dwRace);
	dwPos += 1;
	if (dwSex == 0)
		dwPos += 5;
	D3DXVECTOR3 scalePos = pItemData->GetItemScalePosition(dwPos);
	if (m_kHorse.IsMounting())
		scalePos.z += 15.0f;
	m_auraRefineEffect = m_GraphicThingInstance.AttachEffectByID(NULL, "Bip01 Spine2", pItemData->GetAuraEffectID(), &scalePos);
}
#endif

#ifdef ENABLE_SHINING_SYSTEM
void CInstanceBase::__GetShiningEffect(CItemData* pItem)
{
	bool removeRefineEffect = false;

	CItemData::TItemShiningTable shiningTable = pItem->GetItemShiningTable();

#if defined(ENABLE_COSTUME_SYSTEM) && defined(ENABLE_WEAPON_COSTUME_SYSTEM)
	if (pItem->GetType() == CItemData::ITEM_TYPE_WEAPON || (pItem->GetType() == CItemData::ITEM_TYPE_COSTUME && pItem->GetSubType() == CItemData::COSTUME_WEAPON))
	{
		BYTE bSubType = pItem->GetType() == CItemData::ITEM_TYPE_COSTUME && pItem->GetSubType() == CItemData::COSTUME_WEAPON ? pItem->GetValue(3) : pItem->GetSubType();
#else
	if (pItem->GetType() == CItemData::ITEM_TYPE_WEAPON)
	{
		BYTE bSubType = pItem->GetSubType();
#endif

		__ClearWeaponShiningEffect();

		if (shiningTable.Any() && removeRefineEffect)
		{
			__ClearWeaponRefineEffect();
		}

		for (int i = 0; i < CItemData::ITEM_SHINING_MAX_COUNT; i++)
		{
			if (strcmp(shiningTable.szShinings[i], ""))
			{
				if (bSubType == CItemData::WEAPON_BOW)
				{
					__AttachWeaponShiningEffect(i, shiningTable.szShinings[i], "PART_WEAPON_LEFT");
				}
				else
				{
#ifdef ENABLE_WOLFMAN
					bool twoSidedWeapon = bSubType == CItemData::WEAPON_DAGGER || (IsMountingHorse() && bSubType == CItemData::WEAPON_FAN) || bSubType == CItemData::WEAPON_CLAW;
#else
					bool twoSidedWeapon = bSubType == CItemData::WEAPON_DAGGER || (IsMountingHorse() && bSubType == CItemData::WEAPON_FAN);
#endif
					if (twoSidedWeapon)
					{
						__AttachWeaponShiningEffect(i, shiningTable.szShinings[i], "PART_WEAPON_LEFT");
					}


					__AttachWeaponShiningEffect(i, shiningTable.szShinings[i], "PART_WEAPON");
				}
			}
		}
	}

#if defined(ENABLE_COSTUME_SYSTEM)
	if ((pItem->GetType() == CItemData::ITEM_TYPE_ARMOR && pItem->GetSubType() == CItemData::ARMOR_BODY) || (pItem->GetType() == CItemData::ITEM_TYPE_COSTUME && pItem->GetSubType() == CItemData::COSTUME_BODY))
#else
	if (pItem->GetType() == CItemData::ITEM_TYPE_ARMOR && pItem->GetSubType() == CItemData::ARMOR_BODY)
#endif
	{
		__ClearArmorShiningEffect();

		if (shiningTable.Any() && removeRefineEffect)
		{
			__ClearArmorRefineEffect();
		}

		for (int i = 0; i < CItemData::ITEM_SHINING_MAX_COUNT; i++)
		{
			if (strcmp(shiningTable.szShinings[i], ""))
			{
				__AttachArmorShiningEffect(i, shiningTable.szShinings[i]);
			}
		}
	}
	}

void CInstanceBase::__AttachWeaponShiningEffect(int effectIndex, const char* effectFileName, const char* boneName)
{
	if (IsAffect(AFFECT_INVISIBILITY))
	{
		return;
	}

	if (effectIndex >= CItemData::ITEM_SHINING_MAX_COUNT)
	{
		return;
	}

	CEffectManager::Instance().RegisterEffect(effectFileName, false, false);

	if (!strcmp(boneName, "PART_WEAPON"))
	{
		const char* c_szRightBoneName;
		m_GraphicThingInstance.GetAttachingBoneName(CRaceData::PART_WEAPON, &c_szRightBoneName);

		if (strcmp(c_szRightBoneName, ""))
		{
			m_weaponShiningEffects[0][effectIndex] = m_GraphicThingInstance.AttachEffectByName(0, c_szRightBoneName, effectFileName);
		}
	}
	else if (!strcmp(boneName, "PART_WEAPON_LEFT"))
	{
		const char* c_szLeftBoneName;
		m_GraphicThingInstance.GetAttachingBoneName(CRaceData::PART_WEAPON_LEFT, &c_szLeftBoneName);

		if (strcmp(c_szLeftBoneName, ""))
		{
			m_weaponShiningEffects[1][effectIndex] = m_GraphicThingInstance.AttachEffectByName(0, c_szLeftBoneName, effectFileName);
		}
	}
	else
	{
		Tracef("Invalid partname for getting attaching bone name. %s - %s", effectFileName, boneName);
	}
}

void CInstanceBase::__AttachArmorShiningEffect(int effectIndex, const char* effectFileName, const char* boneName)
{
	if (IsAffect(AFFECT_INVISIBILITY))
	{
		return;
	}

	if (effectIndex >= CItemData::ITEM_SHINING_MAX_COUNT)
	{
		return;
	}

	if (!strcmp(boneName, ""))
	{
		Tracef("Empty bone name for attaching armor shining. Effect Index: %i, EffectFileName: %s", effectIndex, effectFileName);
		return;
	}

	CEffectManager::Instance().RegisterEffect(effectFileName, false, false);
	m_armorShiningEffects[effectIndex] = m_GraphicThingInstance.AttachEffectByName(0, boneName, effectFileName);
}

void CInstanceBase::__ClearWeaponShiningEffect(bool detaching)
{
	if (detaching)
	{
		for (int i = 0; i < CItemData::ITEM_SHINING_MAX_COUNT; i++)
		{
			if (m_weaponShiningEffects[0][i])
			{
				__DetachEffect(m_weaponShiningEffects[0][i]);
			}

			if (m_weaponShiningEffects[1][i])
			{
				__DetachEffect(m_weaponShiningEffects[1][i]);
			}
		}
	}

#ifdef __cplusplus < 199711L
	memset(&m_weaponShiningEffects, 0, sizeof(m_weaponShiningEffects));
#else
	m_weaponShiningEffects = {};
#endif
}

void CInstanceBase::__ClearArmorShiningEffect(bool detaching)
{
	if (detaching)
	{
		for (int i = 0; i < CItemData::ITEM_SHINING_MAX_COUNT; i++)
		{
			__DetachEffect(m_armorShiningEffects[i]);
		}
	}

#ifdef __cplusplus < 199711L
	memset(&m_armorShiningEffects, 0, sizeof(m_armorShiningEffects));
#else
	m_armorShiningEffects = {};
#endif
}
#endif

#ifdef ENABLE_MOUNT_SCALE
float CInstanceBase::GetMountScale()
{
	if (m_dwRace >= 20110 && m_dwRace <= 20125 
		//|| m_dwRace >= 20127 && m_dwRace <= 20248 
		|| m_dwRace >= 20601 && m_dwRace <= 20602 
		|| m_dwRace >= 817 && m_dwRace <= 821 
		|| m_dwRace >= 35000 && m_dwRace <= 35003 
		|| m_dwRace >= 49763 && m_dwRace <= 49767 
		|| m_dwRace >= 30245 && m_dwRace <= 30246 
		|| m_dwRace >= 20263 && m_dwRace <= 20264 
		|| m_dwRace >= 20267 && m_dwRace <= 20274)
		return 60.0;

	// 100 <- is max model height. if u write 50 model will be scale %50.
	// special index for scale.
	std::map<DWORD, float> m_scaleData = {
		//{MOBNUM, SCALEPERCENT},
		//{20213,15.0},
	};

	auto it = m_scaleData.find(m_dwRace);
	if (it != m_scaleData.end())
		return it->second;
	return 60.0;//default for unkown mounts.
}
void CInstanceBase::SetMountScale()
{
	float scaleSize = GetMountScale();
	float fScale = static_cast<float>(scaleSize / 100.0f);
	m_GraphicThingInstance.SetScaleWorld(fScale, fScale, fScale);
}
#endif

#ifdef GUILD_RANK_EFFECT
void CInstanceBase::SetRankAffect(DWORD newRankIndex)
{
	rankIndex = newRankIndex;

	//TraceError("player guild rank index %u", rankIndex);
	if (rankIndex == 1)
	{
		if (rankAffectIndex)
		{
			__DetachEffect(rankAffectIndex);
			rankAffectIndex = 0;
		}

		if (!rankAffectIndex)
			rankAffectIndex = __AttachEffect(EFFECT_GUILD_RANK_1);
	}
	
	else if (rankIndex == 2)
	{
		if (rankAffectIndex)
		{
			__DetachEffect(rankAffectIndex);
			rankAffectIndex = 0;
		}
		rankAffectIndex = __AttachEffect(EFFECT_GUILD_RANK_2);
	}

	else if (rankIndex == 3)
	{
		if (rankAffectIndex)
		{
			__DetachEffect(rankAffectIndex);
			rankAffectIndex = 0;
		}

		rankAffectIndex = __AttachEffect(EFFECT_GUILD_RANK_3);
	}
	else
	{
		if (rankAffectIndex)
		{
			__DetachEffect(rankAffectIndex);
			rankAffectIndex = 0;
		}
	}
}
#endif

#ifdef ENABLE_DEFENSAWESHIP
BOOL CInstanceBase::IsHydraNPC()
{
	switch (m_dwVirtualNumber)
	{
	case 6501:
		return TRUE;
	}
	return FALSE;
}
#endif

float CInstanceBase::GetBaseHeight()
{
	CActorInstance* pkHorse = m_kHorse.GetActorPtr();
	if (!m_kHorse.IsMounting() || !pkHorse)
		return 0.0f;

	DWORD dwHorseVnum = m_kHorse.m_pkActor->GetRace();
	if ((dwHorseVnum >= 20101 && dwHorseVnum <= 20109) ||
		(dwHorseVnum == 20029 || dwHorseVnum == 20030))
		return 100.0f;

	float fRaceHeight = CRaceManager::instance().GetRaceHeight(dwHorseVnum);
	if (fRaceHeight == 0.0f)
		return 100.0f;
	else
		return fRaceHeight;
}