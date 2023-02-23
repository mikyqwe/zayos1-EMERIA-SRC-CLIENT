#include "StdAfx.h"
#include "PythonGuild.h"
#include "AbstractPlayer.h"
#include "MarkManager.h"

std::map<DWORD, DWORD> g_GuildSkillSlotToIndexMap;

void CPythonGuild::EnableGuild()
{
	m_bGuildEnable = TRUE;
}

void CPythonGuild::SetGuildMoney(DWORD dwMoney)
{
	m_GuildInfo.dwGuildMoney = dwMoney;
}

void CPythonGuild::SetGuildEXP(BYTE byLevel, DWORD dwEXP)
{
	m_GuildInfo.dwGuildLevel = byLevel;
	m_GuildInfo.dwCurrentExperience = dwEXP;
}

void CPythonGuild::SetGradeData(BYTE byGradeNumber, TGuildGradeData & rGuildGradeData)
{
	m_GradeDataMap[byGradeNumber] = rGuildGradeData;
}

void CPythonGuild::SetGradeName(BYTE byGradeNumber, const char * c_szName)
{
	if (!__IsGradeData(byGradeNumber))
		return;

	TGuildGradeData & rGradeData = m_GradeDataMap.find(byGradeNumber)->second;
	rGradeData.strName = c_szName;
}

void CPythonGuild::SetGradeAuthority(BYTE byGradeNumber, BYTE byAuthority)
{
	if (!__IsGradeData(byGradeNumber))
		return;

	TGuildGradeData & rGradeData = m_GradeDataMap.find(byGradeNumber)->second;
	rGradeData.byAuthorityFlag = byAuthority;
}

void CPythonGuild::ClearComment()
{
	m_GuildBoardCommentVector.clear();
}

void CPythonGuild::RegisterComment(DWORD dwCommentID, const char * c_szName, const char * c_szComment)
{
	if (0 == strlen(c_szComment))
		return;

	TGuildBoardCommentData CommentData;
	CommentData.dwCommentID = dwCommentID;
	CommentData.strName = c_szName;
	CommentData.strComment = c_szComment;

	m_GuildBoardCommentVector.push_back(CommentData);
}

void CPythonGuild::RegisterMember(TGuildMemberData & rGuildMemberData)
{
	TGuildMemberData * pGuildMemberData;
	if (GetMemberDataPtrByPID(rGuildMemberData.dwPID, &pGuildMemberData))
	{
		pGuildMemberData->byGeneralFlag = rGuildMemberData.byGeneralFlag;
		pGuildMemberData->byGrade = rGuildMemberData.byGrade;
		pGuildMemberData->byLevel = rGuildMemberData.byLevel;
		pGuildMemberData->dwOffer = rGuildMemberData.dwOffer;
	}
	else
	{
		m_GuildMemberDataVector.push_back(rGuildMemberData);
	}

	__CalculateLevelAverage();
	__SortMember();
}

struct CPythonGuild_FFindGuildMemberByPID
{
	CPythonGuild_FFindGuildMemberByPID(DWORD dwSearchingPID_) : dwSearchingPID(dwSearchingPID_) {}
	int operator () (CPythonGuild::TGuildMemberData & rGuildMemberData)
	{
		return rGuildMemberData.dwPID == dwSearchingPID;
	}

	DWORD dwSearchingPID;
};

struct CPythonGuild_FFindGuildMemberByName
{
	CPythonGuild_FFindGuildMemberByName(const char * c_szSearchingName) : strSearchingName(c_szSearchingName) {}
	int operator () (CPythonGuild::TGuildMemberData & rGuildMemberData)
	{
		return 0 == strSearchingName.compare(rGuildMemberData.strName.c_str());
	}

	std::string strSearchingName;
};

void CPythonGuild::ChangeGuildMemberGrade(DWORD dwPID, BYTE byGrade)
{
	TGuildMemberData * pGuildMemberData;
	if (!GetMemberDataPtrByPID(dwPID, &pGuildMemberData))
		return;

	pGuildMemberData->byGrade = byGrade;
}

void CPythonGuild::ChangeGuildMemberGeneralFlag(DWORD dwPID, BYTE byFlag)
{
	TGuildMemberData * pGuildMemberData;
	if (!GetMemberDataPtrByPID(dwPID, &pGuildMemberData))
		return;

	pGuildMemberData->byGeneralFlag = byFlag;
}

void CPythonGuild::RemoveMember(DWORD dwPID)
{
	TGuildMemberDataVector::iterator itor;
	itor = std::find_if(	m_GuildMemberDataVector.begin(),
							m_GuildMemberDataVector.end(),
							CPythonGuild_FFindGuildMemberByPID(dwPID));

	if (m_GuildMemberDataVector.end() == itor)
		return;

	m_GuildMemberDataVector.erase(itor);
}

void CPythonGuild::RegisterGuildName(DWORD dwID, const char * c_szName)
{
	m_GuildNameMap.insert(std::make_pair(dwID, std::string(c_szName)));
}

BOOL CPythonGuild::IsMainPlayer(DWORD dwPID)
{
	TGuildMemberData * pGuildMemberData;
	if (!GetMemberDataPtrByPID(dwPID, &pGuildMemberData))
		return FALSE;

	IAbstractPlayer& rPlayer=IAbstractPlayer::GetSingleton();
	return 0 == pGuildMemberData->strName.compare(rPlayer.GetName());
}

BOOL CPythonGuild::IsGuildEnable()
{
	return m_bGuildEnable;
}

CPythonGuild::TGuildInfo & CPythonGuild::GetGuildInfoRef()
{
	return m_GuildInfo;
}

BOOL CPythonGuild::GetGradeDataPtr(DWORD dwGradeNumber, TGuildGradeData ** ppData)
{
	TGradeDataMap::iterator itor = m_GradeDataMap.find(dwGradeNumber);
	if (m_GradeDataMap.end() == itor)
		return FALSE;

	*ppData = &(itor->second);

	return TRUE;
}

const CPythonGuild::TGuildBoardCommentDataVector & CPythonGuild::GetGuildBoardCommentVector()
{
	return m_GuildBoardCommentVector;
}

DWORD CPythonGuild::GetMemberCount()
{
	return m_GuildMemberDataVector.size();
}

BOOL CPythonGuild::GetMemberDataPtr(DWORD dwIndex, TGuildMemberData ** ppData)
{
	if (dwIndex >= m_GuildMemberDataVector.size())
		return FALSE;

	*ppData = &m_GuildMemberDataVector[dwIndex];

	return TRUE;
}

BOOL CPythonGuild::GetMemberDataPtrByPID(DWORD dwPID, TGuildMemberData ** ppData)
{
	TGuildMemberDataVector::iterator itor;
	itor = std::find_if(	m_GuildMemberDataVector.begin(),
							m_GuildMemberDataVector.end(),
							CPythonGuild_FFindGuildMemberByPID(dwPID));

	if (m_GuildMemberDataVector.end() == itor)
		return FALSE;

	*ppData = &(*itor);
	return TRUE;
}

BOOL CPythonGuild::GetMemberDataPtrByName(const char * c_szName, TGuildMemberData ** ppData)
{
	TGuildMemberDataVector::iterator itor;
	itor = std::find_if(	m_GuildMemberDataVector.begin(),
							m_GuildMemberDataVector.end(),
							CPythonGuild_FFindGuildMemberByName(c_szName));

	if (m_GuildMemberDataVector.end() == itor)
		return FALSE;

	*ppData = &(*itor);
	return TRUE;
}

DWORD CPythonGuild::GetGuildMemberLevelSummary()
{
	return m_dwMemberLevelSummary;
}

DWORD CPythonGuild::GetGuildMemberLevelAverage()
{
	return m_dwMemberLevelAverage;
}

DWORD CPythonGuild::GetGuildExperienceSummary()
{
	return m_dwMemberExperienceSummary;
}

CPythonGuild::TGuildSkillData & CPythonGuild::GetGuildSkillDataRef()
{
	return m_GuildSkillData;
}

bool CPythonGuild::GetGuildName(DWORD dwID, std::string * pstrGuildName)
{
	if (m_GuildNameMap.end() == m_GuildNameMap.find(dwID))
		return false;

	*pstrGuildName = m_GuildNameMap[dwID];

	return true;
}

DWORD CPythonGuild::GetGuildID()
{
	return m_GuildInfo.dwGuildID;
}

BOOL CPythonGuild::HasGuildLand()
{
	return m_GuildInfo.bHasLand;
}


void CPythonGuild::StartGuildWar(DWORD dwEnemyGuildID)
{
	int i;

	for (i = 0; i < ENEMY_GUILD_SLOT_MAX_COUNT; ++i)
		if (dwEnemyGuildID == m_adwEnemyGuildID[i])
			return;

	for (i = 0; i < ENEMY_GUILD_SLOT_MAX_COUNT; ++i)
		if (0 == m_adwEnemyGuildID[i])
		{
			m_adwEnemyGuildID[i] = dwEnemyGuildID;
			break;
		}
}

void CPythonGuild::EndGuildWar(DWORD dwEnemyGuildID)
{
	for (int i = 0; i < ENEMY_GUILD_SLOT_MAX_COUNT; ++i)
		if (dwEnemyGuildID == m_adwEnemyGuildID[i])
			m_adwEnemyGuildID[i] = 0;
}

DWORD CPythonGuild::GetEnemyGuildID(DWORD dwIndex)
{
	if (dwIndex >= ENEMY_GUILD_SLOT_MAX_COUNT)
		return 0;

	return m_adwEnemyGuildID[dwIndex];
}

BOOL CPythonGuild::IsDoingGuildWar()
{
	for (int i = 0; i < ENEMY_GUILD_SLOT_MAX_COUNT; ++i)
		if (0 != m_adwEnemyGuildID[i])
		{
			return TRUE;
		}

	return FALSE;
}


void CPythonGuild::__CalculateLevelAverage()
{
	m_dwMemberLevelSummary = 0;
	m_dwMemberLevelAverage = 0;
	m_dwMemberExperienceSummary = 0;

	if (m_GuildMemberDataVector.empty())
		return;

	TGuildMemberDataVector::iterator itor;

	// Sum Level & Experience
	itor = m_GuildMemberDataVector.begin();
	for (; itor != m_GuildMemberDataVector.end(); ++itor)
	{
		TGuildMemberData & rGuildMemberData = *itor;
		m_dwMemberLevelSummary += rGuildMemberData.byLevel;
		m_dwMemberExperienceSummary += rGuildMemberData.dwOffer;
	}

	assert(!m_GuildMemberDataVector.empty());
	m_dwMemberLevelAverage = m_dwMemberLevelSummary / m_GuildMemberDataVector.size();
}

struct CPythonGuild_SLessMemberGrade
{
	bool operator() (CPythonGuild::TGuildMemberData & rleft, CPythonGuild::TGuildMemberData & rright)
	{
		if (rleft.byGrade < rright.byGrade)
			return true;

		return false;
	}
};

void CPythonGuild::__SortMember()
{
	std::sort(m_GuildMemberDataVector.begin(), m_GuildMemberDataVector.end(), CPythonGuild_SLessMemberGrade());
}

BOOL CPythonGuild::__IsGradeData(BYTE byGradeNumber)
{
	return m_GradeDataMap.end() != m_GradeDataMap.find(byGradeNumber);
}

void CPythonGuild::__Initialize()
{
	ZeroMemory(&m_GuildInfo, sizeof(m_GuildInfo));
	ZeroMemory(&m_GuildSkillData, sizeof(m_GuildSkillData));
	ZeroMemory(&m_adwEnemyGuildID, ENEMY_GUILD_SLOT_MAX_COUNT*sizeof(DWORD));
	m_GradeDataMap.clear();
	m_GuildMemberDataVector.clear();
	m_dwMemberLevelSummary = 0;
	m_dwMemberLevelAverage = 0;
	m_bGuildEnable = FALSE;
	m_GuildNameMap.clear();
#ifdef GUILD_WAR_COUNTER
	m_guild_war.clear();
#endif

}

void CPythonGuild::Destroy()
{
	__Initialize();
}

CPythonGuild::CPythonGuild()
{
	__Initialize();
}
CPythonGuild::~CPythonGuild()
{
}

#ifdef GUILD_WAR_COUNTER
bool sortByKill(war_static_ptr& a, war_static_ptr& b)
{
	return (a.kill >= b.kill && a.dead <= b.dead && a.skill_dmg >= b.skill_dmg);
}
war_static_ptr* CPythonGuild::GetStaticsPTR(const char* name)
{
	for (DWORD i = 0; i < m_guild_war.size(); ++i)
	{
		if (strstr(name, m_guild_war[i].name))
			return &m_guild_war[i];
	}
	return NULL;
}
war_static_ptr* CPythonGuild::GetStaticsPTR(DWORD pid)
{
	for (DWORD i = 0; i < m_guild_war.size(); ++i)
	{
		if (m_guild_war[i].pid == pid)
			return &m_guild_war[i];
	}
	return NULL;
}


int CPythonGuild::GetStaticsIndex(const char* name)
{
	for (DWORD i = 0; i < m_guild_war.size(); ++i)
	{
		if (strstr(name, m_guild_war[i].name))
			return i;
	}
	return -1;
}
int CPythonGuild::GetStaticsIndex(DWORD pid)
{
	for (DWORD i = 0; i < m_guild_war.size(); ++i)
	{
		if (m_guild_war[i].pid == pid)
			return i;
	}
	return -1;
}
void CPythonGuild::SetGuildWarStatic(BYTE sub_index, std::vector<war_static_ptr>& m_list)
{
	if (sub_index == GUILD_STATIC_LOAD)
	{
		m_guild_war.clear();
		for (DWORD i = 0; i < m_list.size(); ++i)
			m_guild_war.push_back(m_list[i]);
	}
	else
	{
		for (DWORD i = 0; i < m_list.size(); ++i)
		{
			int index = GetStaticsIndex(m_list[i].pid);
			if (index == -1)
				m_guild_war.push_back(m_list[i]);
			else
			{
				war_static_ptr& ptr = m_guild_war[index];
				memcpy(&ptr, &m_list[i], sizeof(ptr));
			}
		}
	}
	std::sort(m_guild_war.begin(), m_guild_war.end(), sortByKill);
}
war_static_ptr* CPythonGuild::StaticsIndexToPTR(UINT index)
{
	if (index >= m_guild_war.size())
		return NULL;
	return &m_guild_war[index];
}
void CPythonGuild::ClearStatistics()
{
	m_WarStatisticsInfo.clear();
	m_WarStatisticsData.clear();
}
/* LOG INFO ^_^ */
const TGuildWarReserve* CPythonGuild::WarStatisticsInfo(DWORD index, bool isIndex)
{
	if (isIndex)
	{
		if (index >= m_WarStatisticsInfo.size())
			return NULL;
		auto it = m_WarStatisticsInfo.begin();
		std::advance(it, index);
		return &it->second;
	}
	else
	{
		auto it = m_WarStatisticsInfo.find(index);
		if (it == m_WarStatisticsInfo.end())
			return NULL;
		return &it->second;
	}
	return NULL;
}
void CPythonGuild::SetWarStatisticsInfo(const std::vector<TGuildWarReserve>& m_vec)
{
	for (auto i = 0; i < m_vec.size(); ++i)
	{
		auto it = m_WarStatisticsInfo.find(m_vec[i].dwID);
		if (it != m_WarStatisticsInfo.end())
			memcpy(&it->second, &m_vec[i], sizeof(it->second));
		else
			m_WarStatisticsInfo.emplace(m_vec[i].dwID, m_vec[i]);
	}
}
void CPythonGuild::SetWarStatisticsInfo(const TGuildWarReserve& m_data)
{
	auto it = m_WarStatisticsInfo.find(m_data.dwID);
	if (it != m_WarStatisticsInfo.end())
		memcpy(&it->second, &m_data, sizeof(it->second));
	else
		m_WarStatisticsInfo.emplace(m_data.dwID, m_data);
}

/* LOG DATA ^_^ */
const war_static_ptr* CPythonGuild::WarStatisticsData(DWORD id, DWORD index)
{
	auto it = m_WarStatisticsData.find(id);
	if (it == m_WarStatisticsData.end())
		return NULL;
	return &it->second[index];
}
int CPythonGuild::WarStatisticsDataSize(DWORD id)
{
	auto it = m_WarStatisticsData.find(id);
	if (it == m_WarStatisticsData.end())
		return 0;
	return it->second.size();
}
void CPythonGuild::WarStatisticsDataRemove(DWORD id)
{
	auto it = m_WarStatisticsData.find(id);
	if (it == m_WarStatisticsData.end())
		return;
	it->second.clear();
}
void CPythonGuild::SetWarStatisticsData(DWORD id, const std::vector<war_static_ptr>& m_vec)
{
	auto it = m_WarStatisticsData.find(id);
	if (it != m_WarStatisticsData.end())
	{
		it->second.clear();
		it->second.resize(m_vec.size());
		memcpy(it->second.data(), &m_vec[0], sizeof(it->second));
	}
	else
		m_WarStatisticsData.emplace(id, m_vec);
}
void CPythonGuild::SetWarStatisticsData(DWORD id, const war_static_ptr& m_data)
{
	auto it = m_WarStatisticsData.find(id);
	if (it != m_WarStatisticsData.end())
		it->second.emplace_back(m_data);
	else
	{
		std::vector<war_static_ptr> m_vec;
		m_vec.clear();
		m_vec.emplace_back(m_data);
		m_WarStatisticsData.emplace(id, m_vec);
	}
}
PyObject* guildWarStatisticsInfo(PyObject* poSelf, PyObject* poArgs)
{
	int iID;
	if (!PyTuple_GetInteger(poArgs, 0, &iID))
		return Py_BuildException();
	bool iIndex;
	if (!PyTuple_GetBoolean(poArgs, 1, &iIndex))
		return Py_BuildException();
	auto warReserve = CPythonGuild::Instance().WarStatisticsInfo(iID, iIndex);
	if (warReserve)
#ifdef __IMPROVED_GUILD_WAR__
		return Py_BuildValue("iiiiiiiiisss", warReserve->dwID, warReserve->dwGuildFrom, warReserve->dwGuildTo, warReserve->dwTime, warReserve->bType, warReserve->iMaxPlayer, warReserve->iMaxScore, warReserve->flags, warReserve->winner, warReserve->guild1_name, warReserve->guild2_name, warReserve->date);
#else
		return Py_BuildValue("iiiiiisss", warReserve->dwID, warReserve->dwGuildFrom, warReserve->dwGuildTo, warReserve->dwTime, warReserve->bType, warReserve->winner, warReserve->guild1_name, warReserve->guild2_name, warReserve->date);
#endif
	else
#ifdef __IMPROVED_GUILD_WAR__
		return Py_BuildValue("iiiiiiiiisss", 0, 0, 0, 0, 0, 0, 0, 0, 0, "", "", "");
#else
		return Py_BuildValue("iiiiiisss", 0, 0, 0, 0, 0, 0, "", "", "");
#endif
	return Py_BuildNone();
}
PyObject* guildWarStatisticsInfoSize(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonGuild::Instance().WarStatisticsInfoSize());
}
/*
PyObject* guildStaticsSetLogInfo(PyObject* poSelf, PyObject* poArgs)
{
	int iWarID;
	if (!PyTuple_GetInteger(poArgs, 0, &iWarID))
		return Py_BuildException();
	int iGuildFrom;
	if (!PyTuple_GetInteger(poArgs, 1, &iGuildFrom))
		return Py_BuildException();
	int iGuildTo;
	if (!PyTuple_GetInteger(poArgs, 2, &iGuildTo))
		return Py_BuildException();
	int iTime;
	if (!PyTuple_GetInteger(poArgs, 3, &iTime))
		return Py_BuildException();
	int iType;
	if (!PyTuple_GetInteger(poArgs, 4, &iType))
		return Py_BuildException();
	int iMaxPlayer;
	if (!PyTuple_GetInteger(poArgs, 5, &iMaxPlayer))
		return Py_BuildException();
	int iMaxScore;
	if (!PyTuple_GetInteger(poArgs, 6, &iMaxScore))
		return Py_BuildException();
	int iFlags;
	if (!PyTuple_GetInteger(poArgs, 7, &iFlags))
		return Py_BuildException();
	int iWinner;
	if (!PyTuple_GetInteger(poArgs, 8, &iWinner))
		return Py_BuildException();
	char* iGuild1Name;
	if (!PyTuple_GetString(poArgs, 9, &iGuild1Name))
		return Py_BuildException();
	char* iGuild2Name;
	if (!PyTuple_GetString(poArgs, 10, &iGuild2Name))
		return Py_BuildException();
	char* iDate;
	if (!PyTuple_GetString(poArgs, 11, &iDate))
		return Py_BuildException();

	TGuildWarReserve p;
	p.dwID = iWarID;
	p.dwGuildFrom = iGuildFrom;
	p.dwGuildTo = iGuildTo;
	p.dwTime = iTime;
	p.bType = iType;
	p.iMaxPlayer = iMaxPlayer;
	p.iMaxScore = iMaxScore;
	p.flags = iFlags;
	p.winner = iWinner;
	strcpy(p.guild1_name, iGuild1Name);
	strcpy(p.guild2_name, iGuild2Name);
	strcpy(p.date, iDate);
	CPythonGuild::Instance().SetWarStatisticsInfo(p);
	return Py_BuildNone();
}
*/
PyObject* guildWarStatisticsDataSize(PyObject* poSelf, PyObject* poArgs)
{
	int iWarID;
	if (!PyTuple_GetInteger(poArgs, 0, &iWarID))
		return Py_BuildException();
	return Py_BuildValue("i", CPythonGuild::Instance().WarStatisticsDataSize(iWarID));
}
PyObject* guildWarStatisticsDataRemove(PyObject* poSelf, PyObject* poArgs)
{
	int iWarID;
	if (!PyTuple_GetInteger(poArgs, 0, &iWarID))
		return Py_BuildException();
	CPythonGuild::Instance().WarStatisticsDataRemove(iWarID);
	return Py_BuildNone();
}

PyObject* guildWarStatisticsDataSet(PyObject* poSelf, PyObject* poArgs)
{
	int iWarID;
	if (!PyTuple_GetInteger(poArgs, 0, &iWarID))
		return Py_BuildException();
	char* iName;
	if (!PyTuple_GetString(poArgs, 1, &iName))
		return Py_BuildException();
	int iLevel;
	if (!PyTuple_GetInteger(poArgs, 2, &iLevel))
		return Py_BuildException();
	int iRace;
	if (!PyTuple_GetInteger(poArgs, 3, &iRace))
		return Py_BuildException();
	int iEmpire;
	if (!PyTuple_GetInteger(poArgs, 4, &iEmpire))
		return Py_BuildException();
	int iIsLeader;
	if (!PyTuple_GetInteger(poArgs, 5, &iIsLeader))
		return Py_BuildException();
	int iKill;
	if (!PyTuple_GetInteger(poArgs, 6, &iKill))
		return Py_BuildException();
	int iDead;
	if (!PyTuple_GetInteger(poArgs, 7, &iDead))
		return Py_BuildException();
	int iSkillDmg;
	if (!PyTuple_GetInteger(poArgs, 8, &iSkillDmg))
		return Py_BuildException();
	int iGuildID;
	if (!PyTuple_GetInteger(poArgs, 9, &iGuildID))
		return Py_BuildException();
	int iSpy;
	if (!PyTuple_GetInteger(poArgs, 10, &iSpy))
		return Py_BuildException();
	int iOnline;
	if (!PyTuple_GetInteger(poArgs, 11, &iOnline))
		return Py_BuildException();
	int iPID;
	if (!PyTuple_GetInteger(poArgs, 12, &iPID))
		return Py_BuildException();

	war_static_ptr p;
	strcpy(p.name, iName);
	p.level = iLevel;
	p.race = iRace;
	p.empire = iEmpire;
	p.is_leader = iIsLeader;
	p.kill = iKill;
	p.dead = iDead;
	p.skill_dmg = iSkillDmg;
	p.guild_id = iGuildID;
	p.spy = iSpy;
	p.online = iOnline;
	p.pid = iPID;

	CPythonGuild::Instance().SetWarStatisticsData(iWarID, p);
	return Py_BuildNone();
}

PyObject* guildWarStatisticsData(PyObject* poSelf, PyObject* poArgs)
{
	int iWarID;
	if (!PyTuple_GetInteger(poArgs, 0, &iWarID))
		return Py_BuildException();
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 1, &iIndex))
		return Py_BuildException();
	auto warInfoPtr = CPythonGuild::Instance().WarStatisticsData(iWarID, iIndex);
	if (warInfoPtr)
		return Py_BuildValue("siiiiiiiiiii", warInfoPtr->name, warInfoPtr->level, warInfoPtr->race, warInfoPtr->empire, warInfoPtr->is_leader, warInfoPtr->kill, warInfoPtr->dead, warInfoPtr->skill_dmg
			, warInfoPtr->guild_id, warInfoPtr->spy, warInfoPtr->online, warInfoPtr->pid);
	else
		return Py_BuildValue("siiiiiiiiiii", "", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	return Py_BuildNone();
}
PyObject* guildWarStatisticsClear(PyObject* poSelf, PyObject* poArgs)
{
	CPythonGuild::Instance().ClearStatistics();
	return Py_BuildNone();
}
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

PyObject * guildIsGuildEnable(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonGuild::Instance().IsGuildEnable());
}

PyObject * guildGetGuildID(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonGuild::Instance().GetGuildID());
}

PyObject * guildHasGuildLand(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonGuild::Instance().HasGuildLand());
}

PyObject * guildGetGuildName(PyObject * poSelf, PyObject * poArgs)
{
	int iGuildID;
	if (!PyTuple_GetInteger(poArgs, 0, &iGuildID))
	{
		CPythonGuild::TGuildInfo & rGuildInfo = CPythonGuild::Instance().GetGuildInfoRef();
		return Py_BuildValue("s", rGuildInfo.szGuildName);
	}

	std::string strGuildName;
	if (!CPythonGuild::Instance().GetGuildName(iGuildID, &strGuildName))
		return Py_BuildValue("s", "Noname");

	return Py_BuildValue("s", strGuildName.c_str());
}

PyObject * guildGetGuildMasterName(PyObject * poSelf, PyObject * poArgs)
{
	CPythonGuild::TGuildInfo & rGuildInfo = CPythonGuild::Instance().GetGuildInfoRef();

	CPythonGuild::TGuildMemberData * pData;
	if (!CPythonGuild::Instance().GetMemberDataPtrByPID(rGuildInfo.dwMasterPID, &pData))
		return Py_BuildValue("s", "Noname");

	return Py_BuildValue("s", pData->strName.c_str());
}

PyObject * guildGetEnemyGuildName(PyObject * poSelf, PyObject * poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildValue("s", "");

	DWORD dwEnemyGuildID = CPythonGuild::Instance().GetEnemyGuildID(iIndex);

	std::string strEnemyGuildName;
	if (!CPythonGuild::Instance().GetGuildName(dwEnemyGuildID, &strEnemyGuildName))
		return Py_BuildValue("s", "");

	return Py_BuildValue("s", strEnemyGuildName.c_str());
}

PyObject * guildGetGuildMoney(PyObject * poSelf, PyObject * poArgs)
{
	CPythonGuild::TGuildInfo & rGuildInfo = CPythonGuild::Instance().GetGuildInfoRef();
	return Py_BuildValue("i", rGuildInfo.dwGuildMoney);
}

PyObject * guildGetGuildBoardCommentCount(PyObject * poSelf, PyObject * poArgs)
{
	const CPythonGuild::TGuildBoardCommentDataVector & rCommentVector = CPythonGuild::Instance().GetGuildBoardCommentVector();
	return Py_BuildValue("i", rCommentVector.size());
}

PyObject * guildGetGuildBoardCommentData(PyObject * poSelf, PyObject * poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildException();

	const CPythonGuild::TGuildBoardCommentDataVector & c_rCommentVector = CPythonGuild::Instance().GetGuildBoardCommentVector();
	if (DWORD(iIndex) >= c_rCommentVector.size())
		return Py_BuildValue("iss", 0, "Noname", "Noname");

	const CPythonGuild::TGuildBoardCommentData & c_rData = c_rCommentVector[iIndex];

	return Py_BuildValue("iss", c_rData.dwCommentID, c_rData.strName.c_str(), c_rData.strComment.c_str());
}

PyObject * guildGetGuildLevel(PyObject * poSelf, PyObject * poArgs)
{
	CPythonGuild::TGuildInfo & rGuildInfo = CPythonGuild::Instance().GetGuildInfoRef();
	return Py_BuildValue("i", rGuildInfo.dwGuildLevel);
}



PyObject * guildGetGuildExperience(PyObject * poSelf, PyObject * poArgs)
{
	CPythonGuild::TGuildInfo & rGuildInfo = CPythonGuild::Instance().GetGuildInfoRef();

	int GULID_MAX_LEVEL = 20;
	if (rGuildInfo.dwGuildLevel >= GULID_MAX_LEVEL)
		return Py_BuildValue("ii", 0, 0);

	unsigned lastExp = LocaleService_GetLastExp(rGuildInfo.dwGuildLevel);

	return Py_BuildValue("ii", rGuildInfo.dwCurrentExperience, lastExp - rGuildInfo.dwCurrentExperience);
}

PyObject * guildGetGuildMemberCount(PyObject * poSelf, PyObject * poArgs)
{
	CPythonGuild::TGuildInfo & rGuildInfo = CPythonGuild::Instance().GetGuildInfoRef();
	return Py_BuildValue("ii", rGuildInfo.dwCurrentMemberCount, rGuildInfo.dwMaxMemberCount);
}

PyObject * guildGetGuildMemberLevelSummary(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonGuild::Instance().GetGuildMemberLevelSummary());
}

PyObject * guildGetGuildMemberLevelAverage(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonGuild::Instance().GetGuildMemberLevelAverage());
}

PyObject * guildGetGuildExperienceSummary(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonGuild::Instance().GetGuildExperienceSummary());
}

PyObject * guildGetGuildSkillPoint(PyObject * poSelf, PyObject * poArgs)
{
	const CPythonGuild::TGuildSkillData & c_rSkillData = CPythonGuild::Instance().GetGuildSkillDataRef();
	return Py_BuildValue("i", c_rSkillData.bySkillPoint);
}

PyObject * guildGetDragonPowerPoint(PyObject * poSelf, PyObject * poArgs)
{
	const CPythonGuild::TGuildSkillData & c_rSkillData = CPythonGuild::Instance().GetGuildSkillDataRef();
	return Py_BuildValue("ii", c_rSkillData.wGuildPoint, c_rSkillData.wMaxGuildPoint);
}

PyObject * guildGetGuildSkillLevel(PyObject * poSelf, PyObject * poArgs)
{
	assert(FALSE && !"guildGetGuildSkillLevel - The function is not used.");

	int iSkillIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iSkillIndex))
		return Py_BuildException();

	const CPythonGuild::TGuildSkillData & c_rSkillData = CPythonGuild::Instance().GetGuildSkillDataRef();
	return Py_BuildValue("i", c_rSkillData.bySkillLevel[iSkillIndex]);
}

PyObject * guildGetSkillLevel(PyObject * poSelf, PyObject * poArgs)
{
	int iSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Py_BuildException();

	std::map<DWORD, DWORD>::iterator itor = g_GuildSkillSlotToIndexMap.find(iSlotIndex);

	if (g_GuildSkillSlotToIndexMap.end() == itor)
		return Py_BuildValue("i", 0);

	DWORD dwSkillIndex = itor->second;
	assert(dwSkillIndex < CPythonGuild::GUILD_SKILL_MAX_NUM);

	const CPythonGuild::TGuildSkillData & c_rSkillData = CPythonGuild::Instance().GetGuildSkillDataRef();
	return Py_BuildValue("i", c_rSkillData.bySkillLevel[dwSkillIndex]);
}

PyObject * guildGetSkillMaxLevelNew(PyObject * poSelf, PyObject * poArgs)
{
	int iSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Py_BuildException();

	std::map<DWORD, DWORD>::iterator itor = g_GuildSkillSlotToIndexMap.find(iSlotIndex);

	if (g_GuildSkillSlotToIndexMap.end() == itor)
		return Py_BuildValue("i", 0);

	DWORD dwSkillIndex = itor->second;
	assert(dwSkillIndex < CPythonGuild::GUILD_SKILL_MAX_NUM);

	const CPythonGuild::TGuildSkillData & c_rSkillData = CPythonGuild::Instance().GetGuildSkillDataRef();
	return Py_BuildValue("i", c_rSkillData.bySkillLevel[dwSkillIndex]);
}

PyObject * guildSetSkillIndex(PyObject * poSelf, PyObject * poArgs)
{
	int iSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Py_BuildException();
	int iSkillIndex;
	if (!PyTuple_GetInteger(poArgs, 1, &iSkillIndex))
		return Py_BuildException();

	g_GuildSkillSlotToIndexMap.insert(std::make_pair(iSlotIndex, iSkillIndex));

	return Py_BuildNone();
}

PyObject * guildGetSkillIndex(PyObject * poSelf, PyObject * poArgs)
{
	int iSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Py_BuildException();

	std::map<DWORD, DWORD>::iterator itor = g_GuildSkillSlotToIndexMap.find(iSlotIndex);

	if (g_GuildSkillSlotToIndexMap.end() == itor)
		return Py_BuildValue("i", 0);

	DWORD dwSkillIndex = itor->second;
	return Py_BuildValue("i", dwSkillIndex);
}

PyObject * guildGetGradeData(PyObject * poSelf, PyObject * poArgs)
{
	int iGradeNumber;
	if (!PyTuple_GetInteger(poArgs, 0, &iGradeNumber))
		return Py_BuildException();

	CPythonGuild::TGuildGradeData * pData;
	if (!CPythonGuild::Instance().GetGradeDataPtr(iGradeNumber, &pData))
		return Py_BuildValue("si", "?", 0);

	return Py_BuildValue("si", pData->strName.c_str(), pData->byAuthorityFlag);
}

PyObject * guildGetGradeName(PyObject * poSelf, PyObject * poArgs)
{
	int iGradeNumber;
	if (!PyTuple_GetInteger(poArgs, 0, &iGradeNumber))
		return Py_BuildException();

	CPythonGuild::TGuildGradeData * pData;
	if (!CPythonGuild::Instance().GetGradeDataPtr(iGradeNumber, &pData))
		return Py_BuildValue("s", "?");

	return Py_BuildValue("s", pData->strName.c_str());
}


PyObject * guildGetMemberCount(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonGuild::Instance().GetMemberCount());
}

PyObject * guildGetMemberData(PyObject * poSelf, PyObject * poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildException();

	CPythonGuild::TGuildMemberData * pData;
	if (!CPythonGuild::Instance().GetMemberDataPtr(iIndex, &pData))
		return Py_BuildValue("isiiiii", -1, "", 0, 0, 0, 0, 0);

	return Py_BuildValue("isiiiii", pData->dwPID, pData->strName.c_str(), pData->byGrade, pData->byJob, pData->byLevel, pData->dwOffer, pData->byGeneralFlag);
}

PyObject * guildMemberIndexToPID(PyObject * poSelf, PyObject * poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildException();

	CPythonGuild::TGuildMemberData * pData;
	if (!CPythonGuild::Instance().GetMemberDataPtr(iIndex, &pData))
		return Py_BuildValue("i", -1);

	return Py_BuildValue("i", pData->dwPID);
}

PyObject * guildIsMember(PyObject * poSelf, PyObject * poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildException();

	CPythonGuild::TGuildMemberData * pData;
	if (CPythonGuild::Instance().GetMemberDataPtr(iIndex, &pData))
		return Py_BuildValue("i", TRUE);

	return Py_BuildValue("i", FALSE);
}

PyObject * guildIsMemberByName(PyObject * poSelf, PyObject * poArgs)
{
	char * szName;
	if (!PyTuple_GetString(poArgs, 0, &szName))
		return Py_BuildException();

	CPythonGuild::TGuildMemberData * pData;
	if (CPythonGuild::Instance().GetMemberDataPtrByName(szName, &pData))
		return Py_BuildValue("i", TRUE);

	return Py_BuildValue("i", FALSE);
}

PyObject * guildMainPlayerHasAuthority(PyObject * poSelf, PyObject * poArgs)
{
	int iAuthority;
	if (!PyTuple_GetInteger(poArgs, 0, &iAuthority))
		return Py_BuildException();

	IAbstractPlayer& rPlayer=IAbstractPlayer::GetSingleton();
	const char * c_szMainPlayerName = rPlayer.GetName();

	CPythonGuild::TGuildMemberData * pMemberData;
	if (!CPythonGuild::Instance().GetMemberDataPtrByName(c_szMainPlayerName, &pMemberData))
		return Py_BuildValue("i", FALSE);

	CPythonGuild::TGuildGradeData * pGradeData;
	if (!CPythonGuild::Instance().GetGradeDataPtr(pMemberData->byGrade, &pGradeData))
		return Py_BuildValue("i", FALSE);

	return Py_BuildValue("i", iAuthority == (pGradeData->byAuthorityFlag & iAuthority));
}

PyObject * guildDestroy(PyObject * poSelf, PyObject * poArgs)
{
	CPythonGuild::Instance().Destroy();
	g_GuildSkillSlotToIndexMap.clear();
	return Py_BuildNone();
}


PyObject * guildGuildIDToMarkID(PyObject * poSelf, PyObject * poArgs)
{
	int guild_id;
	if (!PyTuple_GetInteger(poArgs, 0, &guild_id))
		return Py_BuildException();

	return Py_BuildValue("i", CGuildMarkManager::Instance().GetMarkID(guild_id));
}

PyObject * guildGetMarkImageFilenameByMarkID(PyObject * poSelf, PyObject * poArgs)
{
	int markID;

	if (!PyTuple_GetInteger(poArgs, 0, &markID))
		return Py_BuildException();

	std::string imagePath;
	CGuildMarkManager::Instance().GetMarkImageFilename(markID / CGuildMarkImage::MARK_TOTAL_COUNT, imagePath);
	return Py_BuildValue("s", imagePath.c_str());
}

PyObject * guildGetMarkIndexByMarkID(PyObject * poSelf, PyObject * poArgs)
{
	int markID;

	if (!PyTuple_GetInteger(poArgs, 0, &markID))
		return Py_BuildException();

	return Py_BuildValue("i", markID % CGuildMarkImage::MARK_TOTAL_COUNT);
}

#ifdef GUILD_WAR_COUNTER
#include "PythonPlayer.h"
PyObject* guildStaticsSize(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonGuild::Instance().GetGuildWarStaticSize());
}
PyObject* guildStaticsIndexToPID(PyObject* poSelf, PyObject* poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildException();
	war_static_ptr* ptr = CPythonGuild::Instance().StaticsIndexToPTR(iIndex);
	return Py_BuildValue("i", ptr ? ptr->pid : -1);

}
PyObject* guildStaticsPIDToData(PyObject* poSelf, PyObject* poArgs)
{
	int iPID;
	if (!PyTuple_GetInteger(poArgs, 0, &iPID))
		return Py_BuildException();
	int iSelf;
	if (!PyTuple_GetInteger(poArgs, 1, &iSelf))
		return Py_BuildException();

	CPythonGuild& guild_ptr = CPythonGuild::Instance();

	war_static_ptr* ptr = iSelf ? guild_ptr.GetStaticsPTR(CPythonPlayer::Instance().GetName()) : guild_ptr.GetStaticsPTR(iPID);
	if (ptr)
		return Py_BuildValue("siiiiiiiiii", ptr->name, ptr->level, ptr->race, ptr->empire, ptr->is_leader, ptr->kill, ptr->dead, ptr->skill_dmg, ptr->guild_id, ptr->spy, ptr->online);
	else
		return Py_BuildValue("siiiiiiiiii", "", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}
PyObject* guildStaticsNameToPID(PyObject* poSelf, PyObject* poArgs)
{
	char* iName;
	if (!PyTuple_GetString(poArgs, 0, &iName))
		return Py_BuildException();

	CPythonGuild& guild_ptr = CPythonGuild::Instance();
	war_static_ptr* ptr = guild_ptr.GetStaticsPTR(iName);
	return Py_BuildValue("i", ptr ? ptr->pid : -1);
}
PyObject* guildStaticsTest(PyObject* poSelf, PyObject* poArgs)
{
	char* iName;
	if (!PyTuple_GetString(poArgs, 0, &iName))
		return Py_BuildException();
	int iLevel;
	if (!PyTuple_GetInteger(poArgs, 1, &iLevel))
		return Py_BuildException();
	int iRace;
	if (!PyTuple_GetInteger(poArgs, 2, &iRace))
		return Py_BuildException();
	int iEmpire;
	if (!PyTuple_GetInteger(poArgs, 3, &iEmpire))
		return Py_BuildException();
	int iIsLeader;
	if (!PyTuple_GetInteger(poArgs, 4, &iIsLeader))
		return Py_BuildException();
	int iKill;
	if (!PyTuple_GetInteger(poArgs, 5, &iKill))
		return Py_BuildException();
	int iDead;
	if (!PyTuple_GetInteger(poArgs, 6, &iDead))
		return Py_BuildException();
	int iSkillDmg;
	if (!PyTuple_GetInteger(poArgs, 7, &iSkillDmg))
		return Py_BuildException();
	int iGuildID;
	if (!PyTuple_GetInteger(poArgs, 8, &iGuildID))
		return Py_BuildException();
	int iSpy;
	if (!PyTuple_GetInteger(poArgs, 9, &iSpy))
		return Py_BuildException();
	int iOnline;
	if (!PyTuple_GetInteger(poArgs, 10, &iOnline))
		return Py_BuildException();
	int iPID;
	if (!PyTuple_GetInteger(poArgs, 11, &iPID))
		return Py_BuildException();

	war_static_ptr p;
	strcpy(p.name, iName);
	p.level = iLevel;
	p.race = iRace;
	p.empire = iEmpire;
	p.is_leader = iIsLeader;
	p.kill = iKill;
	p.dead = iDead;
	p.skill_dmg = iSkillDmg;
	p.guild_id = iGuildID;
	p.spy = iSpy;
	p.online = iOnline;
	p.pid = iPID;
	std::vector<war_static_ptr> m_data;
	m_data.clear();
	m_data.push_back(p);
	CPythonGuild::Instance().SetGuildWarStatic(1, m_data);
	return Py_BuildNone();
}
#endif

void initguild()
{
	static PyMethodDef s_methods[] =
	{
		// Enable
		{ "IsGuildEnable",					guildIsGuildEnable,					METH_VARARGS },
		{ "GuildIDToMarkID",				guildGuildIDToMarkID,				METH_VARARGS },
		{ "GetMarkImageFilenameByMarkID",	guildGetMarkImageFilenameByMarkID,	METH_VARARGS },
		{ "GetMarkIndexByMarkID",			guildGetMarkIndexByMarkID,			METH_VARARGS },

		// GuildInfo
		{ "GetGuildID",						guildGetGuildID,					METH_VARARGS },
		{ "HasGuildLand",					guildHasGuildLand,					METH_VARARGS },
		{ "GetGuildName",					guildGetGuildName,					METH_VARARGS },
		{ "GetGuildMasterName",				guildGetGuildMasterName,			METH_VARARGS },
		{ "GetEnemyGuildName",				guildGetEnemyGuildName,				METH_VARARGS },
		{ "GetGuildMoney",					guildGetGuildMoney,					METH_VARARGS },

		// BoardPage
		{ "GetGuildBoardCommentCount",		guildGetGuildBoardCommentCount,		METH_VARARGS },
		{ "GetGuildBoardCommentData",		guildGetGuildBoardCommentData,		METH_VARARGS },

		// MemberPage
		{ "GetGuildLevel",					guildGetGuildLevel,					METH_VARARGS },
		{ "GetGuildExperience",				guildGetGuildExperience,			METH_VARARGS },
		{ "GetGuildMemberCount",			guildGetGuildMemberCount,			METH_VARARGS },
		{ "GetGuildMemberLevelSummary",		guildGetGuildMemberLevelSummary,	METH_VARARGS },
		{ "GetGuildMemberLevelAverage",		guildGetGuildMemberLevelAverage,	METH_VARARGS },
		{ "GetGuildExperienceSummary",		guildGetGuildExperienceSummary,		METH_VARARGS },

		// SkillPage
		{ "GetGuildSkillPoint",				guildGetGuildSkillPoint,			METH_VARARGS },
		{ "GetDragonPowerPoint",			guildGetDragonPowerPoint,			METH_VARARGS },
		{ "GetGuildSkillLevel",				guildGetGuildSkillLevel,			METH_VARARGS },
		{ "GetSkillLevel",					guildGetSkillLevel,					METH_VARARGS },
		{ "GetSkillMaxLevelNew",			guildGetSkillMaxLevelNew,			METH_VARARGS },

		{ "SetSkillIndex",					guildSetSkillIndex,					METH_VARARGS },
		{ "GetSkillIndex",					guildGetSkillIndex,					METH_VARARGS },

		// GradePage
		{ "GetGradeData",					guildGetGradeData,					METH_VARARGS },
		{ "GetGradeName",					guildGetGradeName,					METH_VARARGS },

		// About Member
		{ "GetMemberCount",					guildGetMemberCount,				METH_VARARGS },
		{ "GetMemberData",					guildGetMemberData,					METH_VARARGS },
		{ "MemberIndexToPID",				guildMemberIndexToPID,				METH_VARARGS },
		{ "IsMember",						guildIsMember,						METH_VARARGS },
		{ "IsMemberByName",					guildIsMemberByName,				METH_VARARGS },
		{ "MainPlayerHasAuthority",			guildMainPlayerHasAuthority,		METH_VARARGS },

		// Guild
		{ "Destroy",						guildDestroy,						METH_VARARGS },
#ifdef GUILD_WAR_COUNTER
		{ "StaticsSize",			guildStaticsSize,						METH_VARARGS },
		{ "StaticsIndexToPID",			guildStaticsIndexToPID,						METH_VARARGS },
		{ "StaticsPIDToData",			guildStaticsPIDToData,						METH_VARARGS },
		{ "StaticsNameToPID",			guildStaticsNameToPID,						METH_VARARGS },
		{ "StaticsTest",			guildStaticsTest,						METH_VARARGS },
		{ "WarStatisticsInfo",			guildWarStatisticsInfo,						METH_VARARGS },
		//{ "StaticsSetLogInfo",		guildStaticsSetLogInfo,						METH_VARARGS },
		{ "WarStatisticsInfoSize",		guildWarStatisticsInfoSize,					METH_VARARGS },

		{ "WarStatisticsData",			guildWarStatisticsData,						METH_VARARGS },
		{ "WarStatisticsDataSet",		guildWarStatisticsDataSet,						METH_VARARGS },
		{ "WarStatisticsDataSize",		guildWarStatisticsDataSize,						METH_VARARGS },
		{ "WarStatisticsDataRemove",		guildWarStatisticsDataRemove,						METH_VARARGS },

		{ "WarStatisticsClear",			guildWarStatisticsClear,							METH_VARARGS },
#endif
		{ NULL,								NULL,								NULL },
	};

	PyObject * poModule = Py_InitModule("guild", s_methods);
	PyModule_AddIntConstant(poModule, "AUTH_ADD_MEMBER", GUILD_AUTH_ADD_MEMBER);
	PyModule_AddIntConstant(poModule, "AUTH_REMOVE_MEMBER", GUILD_AUTH_REMOVE_MEMBER);
	PyModule_AddIntConstant(poModule, "AUTH_NOTICE", GUILD_AUTH_NOTICE);
	PyModule_AddIntConstant(poModule, "AUTH_SKILL", GUILD_AUTH_SKILL);
	PyModule_AddIntConstant(poModule, "ENEMY_GUILD_SLOT_MAX_COUNT", CPythonGuild::ENEMY_GUILD_SLOT_MAX_COUNT);
}
