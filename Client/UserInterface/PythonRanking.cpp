#include "StdAfx.h"
#include "PythonRanking.h"
#include "AbstractPlayer.h"
#include <memory>
#include <string>

CPythonRanking::CPythonRanking()
{
	vRankingContainer.reserve(RANKING_MAX_NUM);
}

CPythonRanking::~CPythonRanking()
{
	vRankingContainer.clear();
}

void CPythonRanking::RegisterRankingData(const char* name, int level, BYTE job, BYTE empire, const char* guild)
{
	vRankingContainer.emplace_back(std::make_shared<SRankingData>(name, level, job, empire, guild));
}

void CPythonRanking::ClearRankData()
{
	vRankingContainer.clear();
	vRankingContainer.reserve(RANKING_MAX_NUM);
}

size_t CPythonRanking::GetRankCount() const
{
	return vRankingContainer.size();
}

CPythonRanking::SRankingData* CPythonRanking::GetRankByLine(std::uint16_t RankIndex) const
{
	return (RankIndex >= GetRankCount()) ? nullptr : vRankingContainer.at(RankIndex).get();
}

std::uint16_t CPythonRanking::GetRankMyLine() const
{
	auto it = std::find_if(vRankingContainer.begin(), vRankingContainer.end(), 
		[](const std::shared_ptr<SRankingData>& r) { return 0 == r->sName.compare(IAbstractPlayer::GetSingleton().GetName()); });

	return (it == vRankingContainer.end()) ? 0 : (std::distance(vRankingContainer.begin(), it) + 1);
}

/*Module*/

PyObject * rankingGetRankCount(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonRanking::Instance().GetRankCount());
}

PyObject * rankingGetRankByLine(PyObject * poSelf, PyObject * poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BadArgument();

	const CPythonRanking::SRankingData* Rank = CPythonRanking::Instance().GetRankByLine(iIndex);
	if (Rank == nullptr)
		return Py_BuildException("Failed to find rank by index %d", iIndex);

	return Py_BuildValue("siiis", Rank->sName.c_str(), Rank->iLevel, Rank->bJob, Rank->bEmpire, Rank->sGuild.c_str());
}

PyObject* rankingGetRankMyLine(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonRanking::Instance().GetRankMyLine());
}

PyObject* rankingClear(PyObject* poSelf, PyObject* poArgs)
{
	CPythonRanking::Instance().ClearRankData();
	return Py_BuildNone();
}

void initranking()
{
	static PyMethodDef s_methods[] =
	{
		{ "GetRankCount", rankingGetRankCount, METH_VARARGS },
		{ "GetRankByLine", rankingGetRankByLine, METH_VARARGS },
		{ "GetRankMyLine", rankingGetRankMyLine, METH_VARARGS },
		{ "RankClear", rankingClear, METH_VARARGS },
		{ NULL, NULL, NULL }
	};

	PyObject * poModule = Py_InitModule("ranking", s_methods);
	PyModule_AddIntConstant(poModule, "RANK_PAGE_MAX_NUM", CPythonRanking::RANK_PAGE_MAX_NUM);
	PyModule_AddIntConstant(poModule, "RANK_SHOW_COUNT", CPythonRanking::RANK_SHOW_COUNT);
}
