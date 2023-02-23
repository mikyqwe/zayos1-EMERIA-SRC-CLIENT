#include "StdAfx.h"
#include "../eterLib/GrpMath.h"
#include "../UserInterface/PythonBackground.h"
#include "../UserInterface/Locale_inc.h"
#include "ActorInstance.h"

void CActorInstance::__InitializeCollisionData()
{
	m_canSkipCollision=false;
}

void CActorInstance::EnableSkipCollision()
{
	m_canSkipCollision=true;
}

void CActorInstance::DisableSkipCollision()
{
	m_canSkipCollision=false;
}

bool CActorInstance::CanSkipCollision()
{
	return m_canSkipCollision;
}

void CActorInstance::UpdatePointInstance()
{
	TCollisionPointInstanceListIterator itor;
	for (itor = m_DefendingPointInstanceList.begin(); itor != m_DefendingPointInstanceList.end(); ++itor)
		UpdatePointInstance(&(*itor));
}

void CActorInstance::UpdatePointInstance(TCollisionPointInstance * pPointInstance)
{
	if (!pPointInstance)
	{
		assert(!"CActorInstance::UpdatePointInstance - pPointInstance is NULL"); // ���۷����� ��ü�Ͻÿ�
		return;
	}

	D3DXMATRIX matBone;

	if (pPointInstance->isAttached)
	{
		if (pPointInstance->dwModelIndex>=m_LODControllerVector.size())
		{
			//Tracenf("CActorInstance::UpdatePointInstance - rInstance.dwModelIndex=%d >= m_LODControllerVector.size()=%d",
			//		pPointInstance->dwModelIndex>m_LODControllerVector.size());
			return;
		}

		CGrannyLODController* pGrnLODController=m_LODControllerVector[pPointInstance->dwModelIndex];
		if (!pGrnLODController)
		{
			//Tracenf("CActorInstance::UpdatePointInstance - m_LODControllerVector[pPointInstance->dwModelIndex=%d] is NULL", pPointInstance->dwModelIndex);
			return;
		}

		CGrannyModelInstance * pModelInstance = pGrnLODController->GetModelInstance();
		if (!pModelInstance)
		{
			//Tracenf("CActorInstance::UpdatePointInstance - pGrnLODController->GetModelInstance() is NULL");
			return;
		}

		D3DXMATRIX * pmatBone = (D3DXMATRIX *)pModelInstance->GetBoneMatrixPointer(pPointInstance->dwBoneIndex);
		matBone = *(D3DXMATRIX *)pModelInstance->GetCompositeBoneMatrixPointer(pPointInstance->dwBoneIndex);
		matBone._41 = pmatBone->_41;
		matBone._42 = pmatBone->_42;
		matBone._43 = pmatBone->_43;
		matBone *= m_worldMatrix;
	}
	else
	{
		matBone = m_worldMatrix;
	}

	// Update Collsion Sphere
	CSphereCollisionInstanceVector::const_iterator sit = pPointInstance->c_pCollisionData->SphereDataVector.begin();
	CDynamicSphereInstanceVector::iterator dit=pPointInstance->SphereInstanceVector.begin();
	for (;sit!=pPointInstance->c_pCollisionData->SphereDataVector.end();++sit,++dit)
	{
		const TSphereData & c = sit->GetAttribute();//c_pCollisionData->SphereDataVector[j].GetAttribute();

		D3DXMATRIX matPoint;
		D3DXMatrixTranslation(&matPoint, c.v3Position.x, c.v3Position.y, c.v3Position.z);
		matPoint = matPoint * matBone;

		dit->v3LastPosition = dit->v3Position;
		dit->v3Position.x = matPoint._41;
		dit->v3Position.y = matPoint._42;
		dit->v3Position.z = matPoint._43;
	}
}

void CActorInstance::UpdateAdvancingPointInstance()
{
	// ���� ���� ��� ����� �̵����� ������ ���� �ʱ� ������ ���� ���� ���;� �Ѵ� - [levites]
	D3DXVECTOR3 v3Movement = m_v3Movement;
	if (m_pkHorse)
		v3Movement = m_pkHorse->m_v3Movement;

	// ���� ������Ʈ ���� �ʾƵ� �ȴ� - [levites]
	if (m_pkHorse)
		m_pkHorse->UpdateAdvancingPointInstance();

	D3DXMATRIX matPoint;
	D3DXMATRIX matCenter;

	TCollisionPointInstanceListIterator itor = m_BodyPointInstanceList.begin();
	for (; itor != m_BodyPointInstanceList.end(); ++itor)
	{
		TCollisionPointInstance & rInstance = *itor;

		if (rInstance.isAttached)
		{
			if (rInstance.dwModelIndex>=m_LODControllerVector.size())
			{
				Tracenf("CActorInstance::UpdateAdvancingPointInstance - rInstance.dwModelIndex=%d >= m_LODControllerVector.size()=%d",
					rInstance.dwModelIndex, m_LODControllerVector.size());
				continue;
			}

			CGrannyLODController* pGrnLODController=m_LODControllerVector[rInstance.dwModelIndex];
			if (!pGrnLODController)
			{
				Tracenf("CActorInstance::UpdateAdvancingPointInstance - m_LODControllerVector[rInstance.dwModelIndex=%d] is NULL", rInstance.dwModelIndex);
				continue;
			}

			CGrannyModelInstance * pModelInstance = pGrnLODController->GetModelInstance();
			if (!pModelInstance)
			{
				//Tracenf("CActorInstance::UpdateAdvancingPointInstance - pGrnLODController->GetModelInstance() is NULL");
				continue;
			}

			matCenter = *(D3DXMATRIX *)pModelInstance->GetBoneMatrixPointer(rInstance.dwBoneIndex);
			matCenter *= m_worldMatrix;
		}
		else
		{
			matCenter = m_worldMatrix;
		}

		// Update Collision Sphere
		const NRaceData::TCollisionData * c_pCollisionData = rInstance.c_pCollisionData;
		if (c_pCollisionData)
		{
			for (DWORD j = 0; j < c_pCollisionData->SphereDataVector.size(); ++j)
			{
				const TSphereData & c = c_pCollisionData->SphereDataVector[j].GetAttribute();
				CDynamicSphereInstance & rSphereInstance = rInstance.SphereInstanceVector[j];

				D3DXMatrixTranslation(&matPoint, c.v3Position.x, c.v3Position.y, c.v3Position.z);
				matPoint = matPoint * matCenter;

				rSphereInstance.v3LastPosition.x = matPoint._41;
				rSphereInstance.v3LastPosition.y = matPoint._42;
				rSphereInstance.v3LastPosition.z = matPoint._43;
				rSphereInstance.v3Position = rSphereInstance.v3LastPosition;
				rSphereInstance.v3Position += v3Movement;
			}
		}
	}
}

bool CActorInstance::CheckCollisionDetection(const CDynamicSphereInstanceVector * c_pAttackingSphereVector, D3DXVECTOR3 * pv3Position)
{
	if (!c_pAttackingSphereVector)
	{
		assert(!"CActorInstance::CheckCollisionDetection - c_pAttackingSphereVector is NULL"); // ���۷����� ��ü�Ͻÿ�
		return false;
	}

	TCollisionPointInstanceListIterator itor;
	for (itor = m_DefendingPointInstanceList.begin(); itor != m_DefendingPointInstanceList.end(); ++itor)
	{
		const CDynamicSphereInstanceVector * c_pDefendingSphereVector = &(*itor).SphereInstanceVector;

		for (DWORD i = 0; i < c_pAttackingSphereVector->size(); ++i)
		for (DWORD j = 0; j < c_pDefendingSphereVector->size(); ++j)
		{
			const CDynamicSphereInstance & c_rAttackingSphere = c_pAttackingSphereVector->at(i);
			const CDynamicSphereInstance & c_rDefendingSphere = c_pDefendingSphereVector->at(j);

			if (DetectCollisionDynamicSphereVSDynamicSphere(c_rAttackingSphere, c_rDefendingSphere))
			{
				// FIXME : �� ���� ������ ã�Ƴ��� ������ �ٲ�� �Ѵ�.
				*pv3Position = (c_rAttackingSphere.v3Position + c_rDefendingSphere.v3Position) / 2.0f;
				return true;
			}
		}
	}

	return false;
}

bool CActorInstance::CreateCollisionInstancePiece(DWORD dwAttachingModelIndex, const NRaceData::TAttachingData * c_pAttachingData, TCollisionPointInstance * pPointInstance)
{
	if (!c_pAttachingData)
	{
		assert(!"CActorInstance::CreateCollisionInstancePiece - c_pAttachingData is NULL"); // ���۷����� ��ü�Ͻÿ�
		return false;
	}

	if (!c_pAttachingData->pCollisionData)
	{
		assert(!"CActorInstance::CreateCollisionInstancePiece - c_pAttachingData->pCollisionData is NULL"); // ���۷����� ��ü�Ͻÿ�
		return false;
	}

	if (!pPointInstance)
	{
		assert(!"CActorInstance::CreateCollisionInstancePiece - pPointInstance is NULL"); // ���۷����� ��ü�Ͻÿ�
		return false;
	}

	pPointInstance->dwModelIndex = dwAttachingModelIndex;
	pPointInstance->isAttached = FALSE;
	pPointInstance->dwBoneIndex = 0;
	pPointInstance->c_pCollisionData = c_pAttachingData->pCollisionData;

	if (c_pAttachingData->isAttaching)
	{
		int iAttachingBoneIndex;

		CGrannyModelInstance * pModelInstance = m_LODControllerVector[dwAttachingModelIndex]->GetModelInstance();

		if (pModelInstance && pModelInstance->GetBoneIndexByName(c_pAttachingData->strAttachingBoneName.c_str(),
												&iAttachingBoneIndex))
		{
			pPointInstance->isAttached = TRUE;
			pPointInstance->dwBoneIndex = iAttachingBoneIndex;
		}
		else
		{
			//TraceError("CActorInstance::CreateCollisionInstancePiece: Cannot get matrix of bone %s ModelInstance 0x%p",	c_pAttachingData->strAttachingBoneName.c_str(), pModelInstance);
			pPointInstance->isAttached = TRUE;
			pPointInstance->dwBoneIndex = 0;
		}
	}


	const CSphereCollisionInstanceVector & c_rSphereDataVector = c_pAttachingData->pCollisionData->SphereDataVector;

	pPointInstance->SphereInstanceVector.clear();
	pPointInstance->SphereInstanceVector.reserve(c_rSphereDataVector.size());

	CSphereCollisionInstanceVector::const_iterator it;
	CDynamicSphereInstance dsi;

	dsi.v3LastPosition = D3DXVECTOR3(0.0f,0.0f,0.0f);
	dsi.v3Position = D3DXVECTOR3(0.0f,0.0f,0.0f);
	for (it = c_rSphereDataVector.begin(); it!=c_rSphereDataVector.end(); ++it)
	{
		const TSphereData & c_rSphereData = it->GetAttribute();
		dsi.fRadius = c_rSphereData.fRadius;
		pPointInstance->SphereInstanceVector.push_back(dsi);
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CActorInstance::__SplashAttackProcess(CActorInstance & rVictim)
{
	D3DXVECTOR3 v3Distance(rVictim.m_x - m_x, rVictim.m_z - m_z, rVictim.m_z - m_z);
	float fDistance = D3DXVec3LengthSq(&v3Distance);
	if (fDistance >= 1000.0f*1000.0f)
		return FALSE;

	// Check Distance
	if (!__IsInSplashTime())
		return FALSE;

	const CRaceMotionData::TMotionAttackingEventData * c_pAttackingEvent = m_kSplashArea.c_pAttackingEvent;
	const NRaceData::TAttackData & c_rAttackData = c_pAttackingEvent->AttackData;
	THittedInstanceMap & rHittedInstanceMap = m_kSplashArea.HittedInstanceMap;

	// NOTE : �̹� ���ȴٸ� ���� �� ����
	if (rHittedInstanceMap.end() != rHittedInstanceMap.find(&rVictim))
	{
		return FALSE;
	}

	// NOTE : Snipe ����̰�..
	if (NRaceData::ATTACK_TYPE_SNIPE == c_rAttackData.iAttackType)
	{
		// Target �� PC ���..
		if (__IsFlyTargetPC())
			// �ٸ� ��ü�� ���� �� ����
			if (!__IsSameFlyTarget(&rVictim))
				return FALSE;

/*
		if (IsFlyTargetObject())
		{
			CActorInstance * pActorInstance = (CActorInstance *)m_kFlyTarget.GetFlyTarget();

			// NOTE : Target �� PC �϶��� �Ѹ� ���� �� �ִ�.
			if (pActorInstance->IsPC())
				if (&rVictim != pActorInstance)
					return FALSE;
		}
*/
	}

	D3DXVECTOR3 v3HitPosition;
	if (rVictim.CheckCollisionDetection(&m_kSplashArea.SphereInstanceVector, &v3HitPosition))
	{
		rHittedInstanceMap.insert(std::make_pair(&rVictim, GetLocalTime()+c_rAttackData.fInvisibleTime));

		int iCurrentHitCount = rHittedInstanceMap.size();
		int iMaxHitCount = (0 == c_rAttackData.iHitLimitCount ? 16 : c_rAttackData.iHitLimitCount);
		//Tracef(" ------------------- Splash Hit : %d\n", iCurrentHitCount);

		if (iCurrentHitCount > iMaxHitCount)
		{
			//Tracef(" ------------------- OVER FLOW :: Splash Hit Count : %d\n", iCurrentHitCount);
			return FALSE;
		}

		NEW_SetAtkPixelPosition(NEW_GetCurPixelPositionRef());
		__ProcessDataAttackSuccess(c_rAttackData, rVictim, v3HitPosition, m_kSplashArea.uSkill, m_kSplashArea.isEnableHitProcess);
		return TRUE;
	}

	return FALSE;
}

BOOL CActorInstance::__NormalAttackProcess(CActorInstance & rVictim)
{
	// Check Distance
	// NOTE - �ϴ� ���� üũ�� �ϰ� ����
	D3DXVECTOR3 v3Distance(rVictim.m_x - m_x, rVictim.m_z - m_z, rVictim.m_z - m_z);
	float fDistance = D3DXVec3LengthSq(&v3Distance);

	extern bool IS_HUGE_RACE(unsigned int vnum);
	if (IS_HUGE_RACE(rVictim.GetRace()))
	{
		if (fDistance >= 500.0f*500.0f)
			return FALSE;
	}
	else
	{
		if (fDistance >= 300.0f*300.0f)
			return FALSE;
	}

	if (!isValidAttacking())
		return FALSE;

	const float c_fAttackRadius = 20.0f;
	const NRaceData::TMotionAttackData * pad = m_pkCurRaceMotionData->GetMotionAttackDataPointer();

	const float motiontime = GetAttackingElapsedTime();

	NRaceData::THitDataContainer::const_iterator itorHitData = pad->HitDataContainer.begin();
	for (; itorHitData != pad->HitDataContainer.end(); ++itorHitData)
	{
		const NRaceData::THitData & c_rHitData = *itorHitData;

		// NOTE : �̹� �¾Ҵ��� üũ
		THitDataMap::iterator itHitData = m_HitDataMap.find(&c_rHitData);
		if (itHitData != m_HitDataMap.end())
		{
			THittedInstanceMap & rHittedInstanceMap = itHitData->second;

			THittedInstanceMap::iterator itInstance;
			if ((itInstance=rHittedInstanceMap.find(&rVictim)) != rHittedInstanceMap.end())
			{
				if (pad->iMotionType==NRaceData::MOTION_TYPE_COMBO || itInstance->second > GetLocalTime())
					continue;
			}
		}

		NRaceData::THitTimePositionMap::const_iterator range_start, range_end;
		range_start = c_rHitData.mapHitPosition.lower_bound(motiontime-CTimer::Instance().GetElapsedSecond());
		range_end = c_rHitData.mapHitPosition.upper_bound(motiontime);
		float c = cosf(D3DXToRadian(GetRotation()));
		float s = sinf(D3DXToRadian(GetRotation()));

		for(;range_start!=range_end;++range_start)
		{
			const CDynamicSphereInstance& dsiSrc=range_start->second;

			CDynamicSphereInstance dsi;
			dsi = dsiSrc;
			dsi.fRadius = c_fAttackRadius;
			{
				D3DXVECTOR3 v3SrcDir=dsiSrc.v3Position-dsiSrc.v3LastPosition;
				v3SrcDir*=__GetReachScale();

				const D3DXVECTOR3& v3Src = dsiSrc.v3LastPosition+v3SrcDir;
				D3DXVECTOR3& v3Dst = dsi.v3Position;
				v3Dst.x = v3Src.x * c - v3Src.y * s;
				v3Dst.y = v3Src.x * s + v3Src.y * c;
				v3Dst += GetPosition();
			}
			{
				const D3DXVECTOR3& v3Src = dsiSrc.v3LastPosition;
				D3DXVECTOR3& v3Dst = dsi.v3LastPosition;
				v3Dst.x = v3Src.x * c - v3Src.y * s;
				v3Dst.y = v3Src.x * s + v3Src.y * c;
				v3Dst += GetPosition();
			}


			TCollisionPointInstanceList::iterator cpit;
			for(cpit = rVictim.m_DefendingPointInstanceList.begin(); cpit!=rVictim.m_DefendingPointInstanceList.end();++cpit)
			{
				int index = 0;
				const CDynamicSphereInstanceVector & c_DefendingSphereVector = cpit->SphereInstanceVector;
				CDynamicSphereInstanceVector::const_iterator dsit;
				for(dsit = c_DefendingSphereVector.begin(); dsit!= c_DefendingSphereVector.end();++dsit, ++index)
				{
					const CDynamicSphereInstance& sub = *dsit;
					if (DetectCollisionDynamicZCylinderVSDynamicZCylinder(dsi, sub))
					{
						THitDataMap::iterator itHitData = m_HitDataMap.find(&c_rHitData);
						if (itHitData == m_HitDataMap.end())
						{
							THittedInstanceMap HittedInstanceMap;
							HittedInstanceMap.insert(std::make_pair(&rVictim, GetLocalTime()+pad->fInvisibleTime));
							//HittedInstanceMap.insert(make_pair(&rVictim, GetLocalTime()+HIT_COOL_TIME));
							m_HitDataMap.insert(std::make_pair(&c_rHitData, HittedInstanceMap));

							//Tracef(" ----------- First Hit\n");
						}
						else
						{
							itHitData->second.insert(std::make_pair(&rVictim, GetLocalTime()+pad->fInvisibleTime));
							//itHitData->second.insert(make_pair(&rVictim, GetLocalTime()+HIT_COOL_TIME));

							//Tracef(" ----------- Next Hit : %d\n", itHitData->second.size());

							int iCurrentHitCount = itHitData->second.size();
							// NOTE : ���� ������ 16���� �Ѱ�
							if (NRaceData::MOTION_TYPE_COMBO == pad->iMotionType || NRaceData::MOTION_TYPE_NORMAL == pad->iMotionType)
							{
								if (iCurrentHitCount > 16)
								{
									//Tracef(" Type NORMAL :: Overflow - Can't process, skip\n");
									return FALSE;
								}
							}
							else
							{
								if (iCurrentHitCount > pad->iHitLimitCount)
								{
									//Tracef(" Type SKILL :: Overflow - Can't process, skip\n");
									return FALSE;
								}
							}
						}

						D3DXVECTOR3 v3HitPosition = (GetPosition() + rVictim.GetPosition()) *0.5f;

						// #0000780: [M2KR] ���� Ÿ�ݱ� ����
						extern bool IS_HUGE_RACE(unsigned int vnum);
						if (IS_HUGE_RACE(rVictim.GetRace()))
						{
							v3HitPosition = (GetPosition() + sub.v3Position) * 0.5f;
						}

						__ProcessDataAttackSuccess(*pad, rVictim, v3HitPosition, m_kCurMotNode.uSkill);
						return TRUE;
					}
				}
			}
		}
	}

	return FALSE;
}

BOOL CActorInstance::AttackingProcess(CActorInstance & rVictim)
{
	if (rVictim.__isInvisible())
		return FALSE;

	if (__SplashAttackProcess(rVictim))
		return TRUE;

	if (__NormalAttackProcess(rVictim))
		return TRUE;

	return FALSE;
}

BOOL CActorInstance::TestPhysicsBlendingCollision(CActorInstance & rVictim)
{
	if (rVictim.IsDead())
		return FALSE;

	TPixelPosition kPPosLast;
	GetBlendingPosition( &kPPosLast );

	D3DXVECTOR3 v3Distance = D3DXVECTOR3(rVictim.m_x - kPPosLast.x, rVictim.m_y - kPPosLast.y, rVictim.m_z - kPPosLast.z);
	float fDistance = D3DXVec3LengthSq(&v3Distance);
	if (fDistance > 800.0f*800.0f)
		return FALSE;

	// NOTE : ���� ���϶��� Defending Sphere�� Collision Check�� �մϴ�.
	// NOTE : Wait�� ���� �Ǵ� ���߿� �հ� ���� ������ �־.. - [levites]
	TCollisionPointInstanceList * pMainList;
	TCollisionPointInstanceList * pVictimList;
	if (isAttacking() || IsWaiting())
	{
		pMainList = &m_DefendingPointInstanceList;
		pVictimList = &rVictim.m_DefendingPointInstanceList;
	}
	else
	{
		pMainList = &m_BodyPointInstanceList;
		pVictimList = &rVictim.m_BodyPointInstanceList;
	}

	TPixelPosition kPDelta;
	m_PhysicsObject.GetLastPosition(&kPDelta);

	D3DXVECTOR3 prevLastPosition, prevPosition;
	const int nSubCheckCount = 50;

	TCollisionPointInstanceListIterator itorMain = pMainList->begin();
	TCollisionPointInstanceListIterator itorVictim = pVictimList->begin();
	for (; itorMain != pMainList->end(); ++itorMain)
	{
		for (; itorVictim != pVictimList->end(); ++itorVictim)
		{
			CDynamicSphereInstanceVector & c_rMainSphereVector = (*itorMain).SphereInstanceVector;
			CDynamicSphereInstanceVector & c_rVictimSphereVector = (*itorVictim).SphereInstanceVector;

			for (DWORD i = 0; i < c_rMainSphereVector.size(); ++i)
			{
				CDynamicSphereInstance & c_rMainSphere = c_rMainSphereVector[i];
				//adjust main sphere center
				prevLastPosition = c_rMainSphere.v3LastPosition;
				prevPosition	 = c_rMainSphere.v3Position;

				c_rMainSphere.v3LastPosition = prevPosition;

				for( int i = 1; i <= nSubCheckCount; ++ i )
				{
					c_rMainSphere.v3Position = prevPosition + (float)(i/(float)nSubCheckCount) * kPDelta;

					for (DWORD j = 0; j < c_rVictimSphereVector.size(); ++j)
					{
						CDynamicSphereInstance & c_rVictimSphere = c_rVictimSphereVector[j];

						if (DetectCollisionDynamicSphereVSDynamicSphere(c_rMainSphere, c_rVictimSphere))
						{
							BOOL bResult = GetVector3Distance(c_rMainSphere.v3Position, c_rVictimSphere.v3Position) <= GetVector3Distance(c_rMainSphere.v3LastPosition, c_rVictimSphere.v3Position);

							c_rMainSphere.v3LastPosition = prevLastPosition;
							c_rMainSphere.v3Position	 = prevPosition;

							return bResult;
						}
					}
				}

				//restore
				c_rMainSphere.v3LastPosition = prevLastPosition;
				c_rMainSphere.v3Position	 = prevPosition;
			}
		}
	}

	return FALSE;
}

#define ENABLE_PETS_WITHOUT_COLLISIONS
#define ENABLE_SHOPS_WITHOUT_COLLISIONS
#define ENABLE_MOUNTS_WITHOUT_COLLISIONS
#include "../UserInterface/Locale_inc.h"
BOOL CActorInstance::TestActorCollision(CActorInstance & rVictim)
{
#ifdef ENABLE_STOP_COLISSION_GLOBAL
	/*********************************************************************
	* date		: 2016.02.16
	* function	: Stop Colission
	* developer	: VegaS
	* skype		: sacadatt.amazon
	* description : Checks if the victim is one of the examples below you can easily configure. If the victim was found
					success as vnum site / breed ve you could go through it no longer block.
	*/
	/************
	* The first value is the minimum value and the second value is the maximum value of pet vnum (mob_proto) - change 34051 with your max vnum of pet */
	int pListPet[2] = { 34032, 34033 };
	/************
	* You can add whatever you like vnum of npc or monster (mob_proto) */
	int pListGlobal[] = { 36040, 36038, 36039, 20119, 20122, 20123, 20214, 20219, 11000, 11001, 9003, 20016, 9002, 9001, 20008, 20007, 20001, 20349, 9007, 9006, 20406
	, 20021, 20088, 20018, 20011, 20013, 20015, 20041, 20006, 20017, 20084, 20095, 20094
	, 20036, 30035, 20037, 101, 102, 103, 104, 105, 106, 107, 108, 109
	, 110, 111, 112, 113, 114, 115, 171, 172, 173, 174, 175, 176
	, 177, 178, 179, 180, 181, 182, 183, 184, 185, 191, 192, 193, 351, 352, 353, 354, 391, 392, 393, 394, 395, 396, 397, 398
	, 194, 8001, 8002, 8003, 8004, 8004, 8005, 8006, 8007, 8008, 8009, 8010
	, 8011, 8012, 8013, 8014, 8015, 8016, 8017, 8018, 8019, 8020, 8021, 8022 
	, 8023, 8024, 8025, 8026, 8035, 8036, 8037, 8038, 8039, 8040, 301, 302 
	, 303, 304, 401, 402, 403, 404, 405, 406, 433, 434, 435, 454, 501, 502, 503, 504, 551, 552, 553, 554
	, 591, 601, 602, 603, 604, 631, 632, 633, 634, 635, 636, 637, 6001, 6002, 6003, 6004, 6005, 6006, 6007, 6008, 6009, 2501,
	2502, 2503, 2504, 2505, 2506, 2507, 2508, 2509, 2510, 2511, 2512, 2513, 2514, 2542, 2543, 2544, 2545, 2546, 2547, 2301,
	2302, 2303, 2304, 2305, 2306, 2311, 2312, 2313, 2314, 2315, 2401, 2402, 2403, 2404, 2411, 2412, 2413, 2414, 2415,
	601, 602, 603, 604, 631, 632, 633, 634, 635, 636, 637, 651, 652, 653, 654, 655, 656, 657, 2101, 2102, 2103, 2104, 2105, 2106, 2107, 2108,
	2131, 2132, 2133, 2134, 2135, 2151, 2152, 2153, 2154, 2155, 2156, 2157, 2158, 2595, 1101, 1102, 1103, 1104, 1105, 1106, 1107,
	1131, 1132, 1133, 1134, 1135, 1136, 1137, 1151, 1152, 1153, 1154, 1155, 1156, 1157, 1171, 1172, 1173, 1174, 1175, 1176, 1177,
	1301, 1302, 1303, 1304, 1305, 701, 702, 703, 704, 705, 706, 707, 731, 732, 733, 734, 735, 736, 737, 751, 752, 753, 754, 755, 756, 757,
	771, 772, 773, 774, 775, 776, 777, 792, 793, 794, 795, 796, 2001, 2002, 2003, 2004, 2005, 2031, 2032, 2033, 2034, 2035, 2036, 2051, 2052,
	2053, 2054, 2055, 2061, 2062, 2063, 2064, 2065, 2071, 2072, 2073, 2074, 2076, 2201, 2202, 2203, 2204, 2205, 2206, 2231, 2232, 2233, 2234, 2235,
	4111, 4112, 4113, 4114, 4115, 4116, 4117, 4118, 4119, 4120, 4121, 4122, 4123, 4124, 4125, 4126, 4127, 4128, 4129, 4130, 4131, 4132, 4133, 4134,
	4135, 4136, 4137, 4138, 4139, 4140, 4141, 4142, 4143, 4144, 4145, 4146, 4147, 4148, 4149, 4150, 4151, 4152, 6101, 6102, 6103, 6104, 6105, 6106, 6107,
	6108, 6109, 36001, 36002, 36003, 36004, 36005, 36006, 36007, 5121, 5122, 5123, 5124, 5125, 5126, 1001, 1002, 1003, 1004, 1031, 1032, 1033, 1034, 1035, 1036, 1037, 1038, 1039, 1040, 1041};
	/************
	* You can add what mapname you want for enable this stop collission global like pet / npc */
	const char* strMapListGlobal[] = { "metin2_map_a1", "metin2_map_a3", "metin2_map_b1", "metin2_map_b3", "metin2_map_c1", "metin2_map_c3",
									"season2/metin2_map_skipia_dungeon_01", "season2/metin2_map_skipia_dungeon_02", "metin2_map_duel", "metin2_map_devilscatacomb", "metin2_map_n_flame_dungeon_01", "metin2_map_n_snow_dungeon_01", "plechito_desert_map_01", "plechito_pyramide_dungeon", "metin2_map_trent", "metin2_map_trent02",
									"metin2_map_skipia_dungeon_02", "map_n_threeway", "metin2_map_n_desert_01", "map_n_snowm_01", "metin2_map_milgyo", "metin2_map_spiderdungeon", "season1/metin2_map_spiderdungeon_02", "metin2_map_spiderdungeon_03", "metin2_map_n_flame_01", "map_a2", "metin2_map_skipia_dungeon_boss", "metin2_map_monkeydungeon_03", "plechito_lava_map_01", "metin2_map_deviltower1"};
	/************
	* Location name of the map where the event takes place ox */
	const char* strMapEventOx = "season1/metin2_map_oxevent";


	std::string stringName = CPythonBackground::Instance().GetWarpMapName();

	for (int i = 0; i < _countof(strMapListGlobal); i++)
	{
#ifdef ENABLE_STOP_COLLISION_PLAYER_OX
		if (strMapEventOx == stringName) // Check if u are place in map ox
		{
			if (0 <= rVictim.GetRace() && rVictim.GetRace() <= 7) // Check if the victim through which pass over a player (change 7 with 8 if u have wolfman)
				return FALSE;	// Stop collission for player --> You can go through players now successfully without lock yourself		
		}
#endif		
		if (strMapListGlobal[i] == stringName) // Check if you are in one of the maps listed in the global list
		{
			for (int i = 0; i < _countof(pListGlobal); i++)
			{
				if (rVictim.GetRace() == pListGlobal[i] || pListPet[0] <= rVictim.GetRace() && rVictim.GetRace() <= pListPet[1]) // Verify that the victim is npc vnum listed above, or if a pet.
					return FALSE;	// Stop collission for global vnum like a pet or npc							
			}
		}
	}
#endif		

/*
	if (m_pkHorse)
	{
		if (m_pkHorse->TestActorCollision(rVictim))
			return TRUE;

		return FALSE;
	}
*/

	if (rVictim.IsDead())
		return FALSE;

#ifdef ENABLE_PETS_WITHOUT_COLLISIONS
	if (rVictim.GetRace()>=34001 && rVictim.GetRace()<=34099)
		return FALSE;
#endif

#ifdef ENABLE_SHOPS_WITHOUT_COLLISIONS
	if (rVictim.GetRace()>=30000 && rVictim.GetRace()<=30001)
		return FALSE;
#endif

#ifdef ENABLE_MOUNTS_WITHOUT_COLLISIONS
	if (rVictim.GetRace()>=20101 && rVictim.GetRace()<=20299)
		return FALSE;
#endif

	// Check Distance
	// NOTE : ������ �ָ� üũ ����
	//        ������ ��ŵ�ó� ��� ������Ʈ�� ũ�Ⱑ Ŭ��� ������ ���� ������ ����
	//        ĳ���Ͱ� �ڽ��� Body Sphere Radius ���� �� ũ�� �̵��ߴ����� üũ�ϰ�,
	//        ���� �׷��� �ʴٸ� �Ÿ��� üũ�ؼ� �ɷ��ش�.
#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	if (rVictim.GetRace() == 30000)
		return FALSE;
#endif
	D3DXVECTOR3 v3Distance = D3DXVECTOR3(rVictim.m_x - m_x, rVictim.m_y - m_y, rVictim.m_z - m_z);
	float fDistance = D3DXVec3LengthSq(&v3Distance);
	if (fDistance > 800.0f*800.0f)
		return FALSE;

	// NOTE : ���� ���϶��� Defending Sphere�� Collision Check�� �մϴ�.
	// NOTE : Wait�� ���� �Ǵ� ���߿� �հ� ���� ������ �־.. - [levites]
	TCollisionPointInstanceList * pMainList;
	TCollisionPointInstanceList * pVictimList;
	if (isAttacking() || IsWaiting())
	{
		pMainList = &m_DefendingPointInstanceList;
		pVictimList = &rVictim.m_DefendingPointInstanceList;
	}
	else
	{
		pMainList = &m_BodyPointInstanceList;
		pVictimList = &rVictim.m_BodyPointInstanceList;
	}

	TCollisionPointInstanceListIterator itorMain = pMainList->begin();
	TCollisionPointInstanceListIterator itorVictim = pVictimList->begin();
	for (; itorMain != pMainList->end(); ++itorMain)
	for (; itorVictim != pVictimList->end(); ++itorVictim)
	{
		const CDynamicSphereInstanceVector & c_rMainSphereVector = (*itorMain).SphereInstanceVector;
		const CDynamicSphereInstanceVector & c_rVictimSphereVector = (*itorVictim).SphereInstanceVector;

		for (DWORD i = 0; i < c_rMainSphereVector.size(); ++i)
		for (DWORD j = 0; j < c_rVictimSphereVector.size(); ++j)
		{
			const CDynamicSphereInstance & c_rMainSphere = c_rMainSphereVector[i];
			const CDynamicSphereInstance & c_rVictimSphere = c_rVictimSphereVector[j];

			if (DetectCollisionDynamicSphereVSDynamicSphere(c_rMainSphere, c_rVictimSphere))
			{
				if (GetVector3Distance(c_rMainSphere.v3Position, c_rVictimSphere.v3Position) <=
					GetVector3Distance(c_rMainSphere.v3LastPosition, c_rVictimSphere.v3Position))
				{
					return TRUE;
				}
				return FALSE;
			}
		}
	}

	return FALSE;
}

bool CActorInstance::AvoidObject(const CGraphicObjectInstance& c_rkBGObj)
{
#ifdef __MOVIE_MODE__
	if (IsMovieMode())
		return false;
#endif

	if (this==&c_rkBGObj)
		return false;

	if (!__TestObjectCollision(&c_rkBGObj))
		return false;

	__AdjustCollisionMovement(&c_rkBGObj);
	return true;
}

bool CActorInstance::IsBlockObject(const CGraphicObjectInstance& c_rkBGObj)
{
	if (this==&c_rkBGObj)
		return false;

	if (!__TestObjectCollision(&c_rkBGObj))
		return false;

	return true;
}

void CActorInstance::BlockMovement()
{
	if (m_pkHorse)
	{
		m_pkHorse->__InitializeMovement();
		return;
	}

	__InitializeMovement();
}

BOOL CActorInstance::__TestObjectCollision(const CGraphicObjectInstance * c_pObjectInstance)
{
	if (m_pkHorse)
	{
		if (m_pkHorse->__TestObjectCollision(c_pObjectInstance))
			return TRUE;

		return FALSE;
	}

	if (m_canSkipCollision)
		return FALSE;

	if (m_v3Movement.x == 0.0f && m_v3Movement.y == 0.0f && m_v3Movement.z == 0.0f)
		return FALSE;

	TCollisionPointInstanceListIterator itorMain = m_BodyPointInstanceList.begin();
	for (; itorMain != m_BodyPointInstanceList.end(); ++itorMain)
	{
		const CDynamicSphereInstanceVector & c_rMainSphereVector = (*itorMain).SphereInstanceVector;
		for (DWORD i = 0; i < c_rMainSphereVector.size(); ++i)
		{
			const CDynamicSphereInstance & c_rMainSphere = c_rMainSphereVector[i];

			if (c_pObjectInstance->MovementCollisionDynamicSphere(c_rMainSphere))
			{
				//const D3DXVECTOR3 & c_rv3Position = c_pObjectInstance->GetPosition();
				//if (GetVector3Distance(c_rMainSphere.v3Position, c_rv3Position) <
				//	GetVector3Distance(c_rMainSphere.v3LastPosition, c_rv3Position))
				{
					return TRUE;
				}

				//return FALSE;
			}
		}
	}

	return FALSE;
}


bool CActorInstance::TestCollisionWithDynamicSphere(const CDynamicSphereInstance & dsi)
{
	TCollisionPointInstanceListIterator itorMain = m_BodyPointInstanceList.begin();
	for (; itorMain != m_BodyPointInstanceList.end(); ++itorMain)
	{
		const CDynamicSphereInstanceVector & c_rMainSphereVector = (*itorMain).SphereInstanceVector;
		for (DWORD i = 0; i < c_rMainSphereVector.size(); ++i)
		{
			const CDynamicSphereInstance & c_rMainSphere = c_rMainSphereVector[i];

			if (DetectCollisionDynamicSphereVSDynamicSphere(c_rMainSphere, dsi))
			{
				return true;
			}
		}
	}

	return false;
}
