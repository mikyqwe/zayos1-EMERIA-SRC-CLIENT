#include "StdAfx.h"
#include "../eterBase/CRC32.h"
#ifdef ENABLE_TWEENY_LIBRARY
#include "../EterLib/StateManager.h"
#endif
#include "PythonWindow.h"
#include "PythonSlotWindow.h"
#include "PythonWindowManager.h"
//#include "../UserInterface/PythonApplication.h"
//#include "../UserInterface/PythonApplication.cpp"
#ifdef INGAME_WIKI
#include "../eterLib/CWikiRenderTargetManager.h"
#endif
#include "../EterLib/CRenderTargetManager.h"
#ifdef ENABLE_WINDOW_SLIDE_EFFECT
#include "../EterLib/GrpScreen.h"
#endif
#ifdef ENABLE_FPS
#include "../GameLib/GameType.h"
#endif

#ifdef ENABLE_NEW_FISHING_SYSTEM
#include <stdlib.h>
float listFishLeft[MAX_FISHING_WAYS][3] = {
					{56.0f, 36.0f, -46.11f},
					{28.3f, 125.0f, -106.95f},
					{24.3f, 79.0f, -80.08f},
					{52.3f, 182.0f, -141.68f},
					{85.0f, 93.0f, -82.47f},
					{202.7f, 125.7f, -68.11f},
};

float listFishRight[MAX_FISHING_WAYS][3] = {
					{244.0f, 170.0f, 122.62f},
					{223.0f, 113.0f, 97.73f},
					{230.0f, 177.0f, 120.44f},
					{167.3f, 89.7f, 45.28f},
					{222.0f, 141.7f, 112.08f},
					{253.0f, 152.0f, 116.17f},
};
#endif

BOOL g_bOutlineBoxEnable = FALSE;

namespace UI
{

	CWindow::CWindow(PyObject * ppyObject) :
		m_x(0),
		m_y(0),
		m_lWidth(0),
		m_lHeight(0),
		m_poHandler(ppyObject),
		m_bShow(false),
		m_pParent(NULL),
		m_dwFlag(0),
		m_isUpdatingChildren(FALSE),
#ifdef INGAME_WIKI
		m_isInsideRender(false)
#endif
	{
#ifdef _DEBUG
		static DWORD DEBUG_dwGlobalCounter=0;
		DEBUG_dwCounter=DEBUG_dwGlobalCounter++;

		m_strName = "!!debug";
#endif
		//assert(m_poHandler != NULL);
		m_HorizontalAlign = HORIZONTAL_ALIGN_LEFT;
		m_VerticalAlign = VERTICAL_ALIGN_TOP;
		m_rect.bottom = m_rect.left = m_rect.right = m_rect.top = 0;
		m_limitBiasRect.bottom = m_limitBiasRect.left = m_limitBiasRect.right = m_limitBiasRect.top = 0;
#ifdef ENABLE_TWEENY_LIBRARY
		D3DXMatrixIdentity(&m_matScaling);
#endif
#ifdef INGAME_WIKI
		memset(&m_renderBox, 0, sizeof(m_renderBox));
#endif
#ifdef ENABLE_WINDOW_SLIDE_EFFECT
		m_bUseSlideEffect = false;
		m_bSpecialWindow = false;
		m_dScalingFactor = 1.0;
		m_dLastSlidingTime = 0.0;
		m_dSlidingTime = DEFAULT_SLIDING_TIME;
		m_sliding_status = sliding_status_t::none;
#endif
	}

	CWindow::~CWindow()
	{
	}

	DWORD CWindow::Type()
	{
		static DWORD s_dwType = GetCRC32("CWindow", strlen("CWindow"));
		return (s_dwType);
	}

	BOOL CWindow::IsType(DWORD dwType)
	{
		return OnIsType(dwType);
	}

	BOOL CWindow::OnIsType(DWORD dwType)
	{
		if (CWindow::Type() == dwType)
			return TRUE;

		return FALSE;
	}

	struct FClear
	{
		void operator () (CWindow* pWin)
		{
			if (UI::CWindowManager::Instance().GetActivateWindow() == pWin)
				UI::CWindowManager::Instance().DeactivateWindow();

			pWin->Clear();
		}
	};

	void CWindow::Clear()
	{
		// FIXME : ChildrenÀ» Áï½Ã DeleteÇÏÁö´Â ¾Ê´Â´Ù.
		//         ¾îÂ÷ÇÇ PythonÂÊ¿¡¼­ Destroy°¡ ÇÏ³ª¾¿ ´Ù½Ã È£Ãâ µÉ °ÍÀÌ¹Ç·Î..
		//         ÇÏÁö¸¸ ¸¸¾àÀ» À§ÇØ ¸µÅ©´Â ²÷¾î ³õ´Â´Ù.
		//         ´õ ÁÁÀº ÇüÅÂ´Â ÀÖ´Â°¡? - [levites]
		std::for_each(m_pChildList.begin(), m_pChildList.end(), FClear());
		m_pChildList.clear();

		m_pParent = NULL;
		DestroyHandle();
		Hide();
	}

	void CWindow::DestroyHandle()
	{
		m_poHandler = NULL;
	}

	void CWindow::Show()
	{
#ifdef ENABLE_TWEENY_LIBRARY
		if (IsFlag(FLAG_ANIMATED_BOARD))
		{
			if (!this->m_bShow)
			{
				this->m_bShow = true;

				m_sizeAnimation = tweeny::from(0.0f)
					.to(1.0f)
					.during(175)
					// Adalet - Not: Yukarıda verilen değerler scale during ve speed içeriyor
					.via(tweeny::easing::cubicOut)
					// Not:cubicOut kısımlarını kütüphane içerisindeki tweeny-3.3.0.h dosyasındaki ''enum class enumerated'' bloğunda bulunan setlerle değiştirip yeni animasyon elde edebilirsiniz.
					.onStep([this](tweeny::tween<float>& t, float scale) {
					if (t.progress() == 0.0f)
					{
						SetScale(0.0f, 0.0f);
						//SetAllAlpha(0.0f);
					}

					SetScale(scale, scale);
					//SetAllAlpha(scale);

					if (t.progress() == 1.0f)
					{
						SetScale(1.0f, 1.0f);
						//SetAllAlpha(1.0f);

						return true;
					}
					else
					{
						return false;
					}
				});
				m_sizeAnimation.value().step(0.0f);
			}
			else
			{
				this->m_bShow = true;
				SetScale(1.0f, 1.0f);
			}
		}
		else
		{
			m_bShow = true;
		}
#else
		m_bShow = true;
#endif
	}

	void CWindow::Hide()
	{
#ifdef ENABLE_TWEENY_LIBRARY
		if (IsFlag(FLAG_ANIMATED_BOARD))
		{
			if (m_bShow)
			{
				m_sizeAnimation = tweeny::from(1.0f)
					.to(0.0f)
					.during(120)
					// Adalet - Not: Yukarıda verilen değerler scale during ve speed içeriyor
					.via(tweeny::easing::cubicOut)
					// Adalet - Not:cubicOut kısımlarını kütüphane içerisindeki tweeny-3.3.0.h dosyasındaki ''enum class enumerated'' bloğunda bulunan setlerle değiştirip yeni animasyon elde edebilirsiniz.
					.onStep([this](tweeny::tween<float>& t, float scale) {
					if (t.progress() == 0.0f)
					{
						SetScale(1.0f, 1.0f);
						//SetAllAlpha(1.0f);
					}

					SetScale(scale, scale);
					//SetAllAlpha(scale);

					if (t.progress() == 1.0f)
					{
						SetScale(0.0f, 0.0f);
						//SetAllAlpha(0.0f);

						m_bShow = false;
						return true;
					}
					else
					{
						return false;
					}
				});
				m_sizeAnimation.value().step(0.0f);
			}
			else
				m_bShow = false;
		}
		else
		{
			m_bShow = false;
		}
#else
		m_bShow = false;
#endif
	}

#ifdef ENABLE_TWEENY_LIBRARY
#include <directxmath/DirectXMath.h> // Yeni kütüphane directxmath for Nileya - Adalet
	void CWindow::SetScale(float fx, float fy)
	{
		m_v2Scale.x = fx;
		m_v2Scale.y = fy;
		UpdateRect();

		using namespace DirectX;

		XMFLOAT2 vCenter(m_x + (m_lWidth / 2), m_y + (m_lHeight / 2));
		XMStoreFloat4x4((XMFLOAT4X4*)&m_matScaling, XMMatrixTransformation2D(XMLoadFloat2((XMFLOAT2*)&vCenter), NULL, XMLoadFloat2((XMFLOAT2*)&m_v2Scale), XMLoadFloat2((XMFLOAT2*)&vCenter), NULL, XMVectorZero()));

	}
#endif

#ifdef INGAME_WIKI
	void CWindow::OnHideWithChilds()
	{
		OnHide();
		std::for_each(m_pChildList.begin(), m_pChildList.end(), std::mem_fn(&CWindow::OnHideWithChilds));
	}

	void CWindow::OnHide()
	{
		PyCallClassMemberFunc(m_poHandler, "OnHide", BuildEmptyTuple());
	}
#endif

	// NOTE : IsShow´Â "ÀÚ½ÅÀÌ º¸ÀÌ´Â°¡?" ÀÌÁö¸¸, __IsShowingÀº "ÀÚ½ÅÀÌ ±×·ÁÁö°í ÀÖ´Â°¡?" ¸¦ Ã¼Å©ÇÑ´Ù
	//        ÀÚ½ÅÀº Show Áö¸¸ Tree À§ÂÊÀÇ Parent Áß ÇÏ³ª´Â Hide ÀÏ ¼ö ÀÖÀ¸¹Ç·Î.. - [levites]
	bool CWindow::IsRendering()
	{
		if (!IsShow())
			return false;

		if (!m_pParent)
			return true;

		return m_pParent->IsRendering();
	}

	void CWindow::__RemoveReserveChildren()
	{
		if (m_pReserveChildList.empty())
			return;

		TWindowContainer::iterator it;
		for(it = m_pReserveChildList.begin(); it != m_pReserveChildList.end(); ++it)
		{
			m_pChildList.remove(*it);
		}
		m_pReserveChildList.clear();
	}

	void CWindow::Update()
	{
		if (!IsShow())
			return;

		__RemoveReserveChildren();

		OnUpdate();
#ifdef ENABLE_TWEENY_LIBRARY
		if (m_sizeAnimation)
			m_sizeAnimation.value().step(static_cast<uint32_t>(CTimer::instance().GetElapsedMilliecond()));
#endif
		m_isUpdatingChildren = TRUE;
		TWindowContainer::iterator it;
		for(it = m_pChildList.begin(); it != m_pChildList.end();)
		{
			TWindowContainer::iterator it_next = it;
			++it_next;
			(*it)->Update();
			it = it_next;
		}
		m_isUpdatingChildren = FALSE;
	}

#ifdef INGAME_WIKI
	bool CWindow::IsShow()
	{
		if (!m_bShow)
			return false;

		if (m_isInsideRender)
			if (m_renderBox.left + m_renderBox.right >= m_lWidth || m_renderBox.top + m_renderBox.bottom >= m_lHeight)
				return false;

		return true;
	}
#endif

	void CWindow::Render()
	{
		if (!IsShow())
			return;
#ifdef ENABLE_WINDOW_SLIDE_EFFECT
		float oldSliding = 1.0f;
		if (IsSlidingEffectEnabled() && IsSlidingRunning()) {
			oldSliding = grp::SetSliding(m_dScalingFactor);
			grp::SetParentCoord(m_rect.left, m_rect.top, m_lxStoring, m_lyStoring);
		}
#endif
		OnRender();
#ifdef ENABLE_WINDOW_SLIDE_EFFECT
		const auto scaling = GetParentSlidingFactor();
		const auto width = m_rect.right - m_rect.left;
		const auto height = m_rect.bottom - m_rect.top;
		const auto xoff = (width * (1.0 - scaling)) / 2;
		const auto yoff = (height * (1.0 - scaling)) / 2;
#endif
#ifdef ENABLE_TWEENY_LIBRARY
		D3DXMATRIX currentMat;
		STATEMANAGER.GetTransform(D3DTS_WORLD, &currentMat);

		D3DXMATRIX newMat;
		D3DXMatrixMultiply(&newMat, &currentMat, &m_matScaling);
		STATEMANAGER.SetTransform(D3DTS_WORLD, &newMat);
#endif
		if (g_bOutlineBoxEnable)
		{
			CPythonGraphic::Instance().SetDiffuseColor(1.0f, 1.0f, 1.0f);
#ifdef ENABLE_WINDOW_SLIDE_EFFECT
			auto tempSliding = grp::SetSliding(1.0f);
			CPythonGraphic::Instance().RenderBox2d(m_rect.left + xoff, m_rect.top + yoff, m_rect.right - xoff, m_rect.bottom - yoff);
			grp::SetSliding(tempSliding);
#else
			CPythonGraphic::Instance().RenderBox2d(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
#endif
		}

		std::for_each(m_pChildList.begin(), m_pChildList.end(), std::mem_fn(&CWindow::Render));
#ifdef ENABLE_TWEENY_LIBRARY
		STATEMANAGER.SetTransform(D3DTS_WORLD, &currentMat);
#endif
#ifdef INGAME_WIKI
		OnAfterRender();
#endif
#ifdef ENABLE_WINDOW_SLIDE_EFFECT
		if (IsSlidingEffectEnabled() && IsSlidingRunning()) {
			grp::SetSliding(oldSliding);
		}
#endif
	}

	void CWindow::OnUpdate()
	{
#ifdef ENABLE_WINDOW_SLIDE_EFFECT
		__UpdateSlidingEffect();
#endif
		if (!m_poHandler)
			return;

		if (!IsShow())
			return;

		static PyObject* poFuncName_OnUpdate = PyString_InternFromString("OnUpdate");

		//PyCallClassMemberFunc(m_poHandler, "OnUpdate", BuildEmptyTuple());
		PyCallClassMemberFunc_ByPyString(m_poHandler, poFuncName_OnUpdate, BuildEmptyTuple());

	}

	void CWindow::OnRender()
	{
		if (!m_poHandler)
			return;

		if (!IsShow())
			return;

		//PyCallClassMemberFunc(m_poHandler, "OnRender", BuildEmptyTuple());
		PyCallClassMemberFunc(m_poHandler, "OnRender", BuildEmptyTuple());
	}

#ifdef INGAME_WIKI
	void CWindow::OnAfterRender()
	{
		if (!m_poHandler)
			return;

		if (!IsShow())
			return;

		PyCallClassMemberFunc(m_poHandler, "OnAfterRender", BuildEmptyTuple());
	}
#endif

	void CWindow::SetName(const char * c_szName)
	{
		m_strName = c_szName;
	}

	void CWindow::SetSize(long width, long height)
	{
		m_lWidth = width;
		m_lHeight = height;

		m_rect.right = m_rect.left + m_lWidth;
		m_rect.bottom = m_rect.top + m_lHeight;
#ifdef INGAME_WIKI
		if (m_isInsideRender)
			UpdateRenderBoxRecursive();
#endif
	}

	void CWindow::SetHorizontalAlign(DWORD dwAlign)
	{
		m_HorizontalAlign = (EHorizontalAlign)dwAlign;
		UpdateRect();
	}

	void CWindow::SetVerticalAlign(DWORD dwAlign)
	{
		m_VerticalAlign = (EVerticalAlign)dwAlign;
		UpdateRect();
	}

	void CWindow::SetPosition(long x, long y)
	{
		m_x = x;
		m_y = y;

		UpdateRect();
		//Add after:
#ifdef INGAME_WIKI
		if (m_isInsideRender)
			UpdateRenderBoxRecursive();
#endif
	}

#ifdef INGAME_WIKI
	void CWindow::UpdateRenderBoxRecursive()
	{
		UpdateRenderBox();
		for (auto it = m_pChildList.begin(); it != m_pChildList.end(); ++it)
			(*it)->UpdateRenderBoxRecursive();
	}
#endif

	void CWindow::GetPosition(long * plx, long * ply)
	{
		*plx = m_x;
		*ply = m_y;
	}

	long CWindow::UpdateRect()
	{
		m_rect.top		= m_y;
		if (m_pParent)
		{
			switch (m_VerticalAlign)
			{
				case VERTICAL_ALIGN_BOTTOM:
					m_rect.top = m_pParent->GetHeight() - m_rect.top;
					break;
				case VERTICAL_ALIGN_CENTER:
					m_rect.top = (m_pParent->GetHeight() - GetHeight()) / 2 + m_rect.top;
					break;
			}
			m_rect.top += m_pParent->m_rect.top;
		}
		m_rect.bottom	= m_rect.top + m_lHeight;

#if defined( _USE_CPP_RTL_FLIP )
		if( m_pParent == NULL ) {
			m_rect.left		= m_x;
			m_rect.right	= m_rect.left + m_lWidth;
		} else {
			if( m_pParent->IsFlag(UI::CWindow::FLAG_RTL) == true ) {
				m_rect.left = m_pParent->GetWidth() - m_lWidth - m_x;
				switch (m_HorizontalAlign)
				{
					case HORIZONTAL_ALIGN_RIGHT:
						m_rect.left = - m_x;
						break;
					case HORIZONTAL_ALIGN_CENTER:
						m_rect.left = m_pParent->GetWidth() / 2 - GetWidth() - m_x;
						break;
				}
				m_rect.left += m_pParent->m_rect.left;
				m_rect.right = m_rect.left + m_lWidth;
			} else {
				m_rect.left		= m_x;
				switch (m_HorizontalAlign)
				{
					case HORIZONTAL_ALIGN_RIGHT:
						m_rect.left = m_pParent->GetWidth() - m_rect.left;
						break;
					case HORIZONTAL_ALIGN_CENTER:
						m_rect.left = (m_pParent->GetWidth() - GetWidth()) / 2 + m_rect.left;
						break;
				}
				m_rect.left += m_pParent->m_rect.left;
				m_rect.right = m_rect.left + m_lWidth;
			}
		}
#else
		m_rect.left		= m_x;
		if (m_pParent)
		{
			switch (m_HorizontalAlign)
			{
				case HORIZONTAL_ALIGN_RIGHT:
					m_rect.left = ::abs(m_pParent->GetWidth()) - m_rect.left;
					break;
				case HORIZONTAL_ALIGN_CENTER:
					m_rect.left = m_pParent->GetWidth() / 2 - GetWidth() / 2 + m_rect.left;
					break;
			}
			m_rect.left += 0L < m_pParent->GetWidth() ? m_pParent->m_rect.left : m_pParent->m_rect.right + ::abs(m_pParent->GetWidth());
		}
		m_rect.right = m_rect.left + m_lWidth;
#endif
		std::for_each(m_pChildList.begin(), m_pChildList.end(), std::mem_fn(&CWindow::UpdateRect));

		OnChangePosition();

		return 1;
	}

	void CWindow::GetLocalPosition(long & rlx, long & rly)
	{
		rlx = rlx - m_rect.left;
		rly = rly - m_rect.top;
	}

	void CWindow::GetMouseLocalPosition(long & rlx, long & rly)
	{
		CWindowManager::Instance().GetMousePosition(rlx, rly);
		rlx = rlx - m_rect.left;
		rly = rly - m_rect.top;
	}

	void CWindow::AddChild(CWindow * pWin)
	{
		m_pChildList.push_back(pWin);
		pWin->m_pParent = this;
#ifdef INGAME_WIKI
		if (m_isInsideRender && !pWin->m_isInsideRender)
			pWin->SetInsideRender(m_isInsideRender);
#endif
	}

#ifdef INGAME_WIKI
	void CWindow::SetInsideRender(BOOL flag)
	{
		if (!m_pParent || m_isInsideRender && m_pParent->m_isInsideRender)
			return;

		if (m_isInsideRender && flag)
			return;

		m_isInsideRender = flag;
		UpdateRenderBox();
		for (auto it = m_pChildList.begin(); it != m_pChildList.end(); ++it)
			(*it)->SetInsideRender(m_isInsideRender);
	}

	void CWindow::GetRenderBox(RECT* box)
	{
		memcpy(box, &m_renderBox, sizeof(RECT));
	}

	void CWindow::UpdateTextLineRenderBox()
	{
		int width, height;
		((CTextLine*)this)->GetTextSize(&width, &height);

		int pWidth = m_pParent->GetWidth();
		int pHeight = m_pParent->GetHeight();

		if (m_x - m_pParent->m_renderBox.left < 0)
			m_renderBox.left = -m_x + m_pParent->m_renderBox.left;
		else
			m_renderBox.left = 0;

		if (m_y - m_pParent->m_renderBox.top < 0)
			m_renderBox.top = -m_y + m_pParent->m_renderBox.top;
		else
			m_renderBox.top = 0;

		if (m_x + width > pWidth - m_pParent->m_renderBox.right)
			m_renderBox.right = m_x + width - pWidth + m_pParent->m_renderBox.right;
		else
			m_renderBox.right = 0;

		if (m_y + height > pHeight - m_pParent->m_renderBox.bottom)
			m_renderBox.bottom = m_y + height - pHeight + m_pParent->m_renderBox.bottom;
		else
			m_renderBox.bottom = 0;
	}

	void CWindow::UpdateRenderBox()
	{
		if (!m_isInsideRender || !m_pParent)
			memset(&m_renderBox, 0, sizeof(m_renderBox));
		else
		{
			int width = m_lWidth;
			int height = m_lHeight;
			int pWidth = m_pParent->GetWidth();
			int pHeight = m_pParent->GetHeight();

			if (m_x - m_pParent->m_renderBox.left < 0)
				m_renderBox.left = -m_x + m_pParent->m_renderBox.left;
			else
				m_renderBox.left = 0;

			if (m_y - m_pParent->m_renderBox.top < 0)
				m_renderBox.top = -m_y + m_pParent->m_renderBox.top;
			else
				m_renderBox.top = 0;

			if (m_x + width > pWidth - m_pParent->m_renderBox.right)
				m_renderBox.right = m_x + width - pWidth + m_pParent->m_renderBox.right;
			else
				m_renderBox.right = 0;

			if (m_y + height > pHeight - m_pParent->m_renderBox.bottom)
				m_renderBox.bottom = m_y + height - pHeight + m_pParent->m_renderBox.bottom;
			else
				m_renderBox.bottom = 0;
		}

		OnUpdateRenderBox();
	}
#endif

	CWindow * CWindow::GetRoot()
	{
		if (m_pParent)
			if (m_pParent->IsWindow())
				return m_pParent->GetRoot();

		return this;
	}

	CWindow * CWindow::GetParent()
	{
		return m_pParent;
	}

#ifdef INGAME_WIKI
	bool CWindow::IsChild(CWindow* pWin, bool bCheckRecursive)
#else
	bool CWindow::IsChild(CWindow* pWin)
#endif
	{
		std::list<CWindow *>::iterator itor = m_pChildList.begin();

		while (itor != m_pChildList.end())
		{
			if (*itor == pWin)
				return true;
#ifdef INGAME_WIKI
			if (bCheckRecursive)
				if ((*itor)->IsChild(pWin, true))
					return true;
#endif

			++itor;
		}

		return false;
	}

#ifdef ENABLE_WINDOW_SLIDE_EFFECT
	double CWindow::GetParentSlidingFactor() {
		if (!GetParent())
			return m_dScalingFactor;
		return m_dScalingFactor * GetParent()->GetParentSlidingFactor();
	}
#endif

	void CWindow::DeleteChild(CWindow* pWin)
	{
		if (m_isUpdatingChildren)
		{
			m_pReserveChildList.push_back(pWin);
		}
		else
		{
			if (UI::CWindowManager::Instance().GetActivateWindow() == pWin)
				UI::CWindowManager::Instance().DeactivateWindow();

			m_pChildList.remove(pWin);
		}
	}


	void CWindow::SetTop(CWindow* pWin)
	{
		if (!pWin->IsFlag(CWindow::FLAG_FLOAT))
			return;

		TWindowContainer::iterator itor = std::find(m_pChildList.begin(), m_pChildList.end(), pWin);
		if (m_pChildList.end() != itor)
		{
			m_pChildList.push_back(*itor);
			m_pChildList.erase(itor);

			pWin->OnTop();
		}
		else
		{
			TraceError(" CWindow::SetTop - Failed to find child window\n");
		}
	}

	void CWindow::OnMouseDrag(long lx, long ly)
	{
		PyCallClassMemberFunc(m_poHandler, "OnMouseDrag", Py_BuildValue("(ii)", lx, ly));
	}

	void CWindow::OnMoveWindow(long lx, long ly)
	{
		PyCallClassMemberFunc(m_poHandler, "OnMoveWindow", Py_BuildValue("(ii)", lx, ly));
	}

	void CWindow::OnSetFocus()
	{
		//PyCallClassMemberFunc(m_poHandler, "OnSetFocus", BuildEmptyTuple());
		PyCallClassMemberFunc(m_poHandler, "OnSetFocus", BuildEmptyTuple());
	}

	void CWindow::OnKillFocus()
	{
		PyCallClassMemberFunc(m_poHandler, "OnKillFocus", BuildEmptyTuple());
	}

	void CWindow::OnMouseOverIn()
	{
		PyCallClassMemberFunc(m_poHandler, "OnMouseOverIn", BuildEmptyTuple());
	}

	void CWindow::OnMouseOverOut()
	{
		PyCallClassMemberFunc(m_poHandler, "OnMouseOverOut", BuildEmptyTuple());
	}

	void CWindow::OnMouseOver()
	{
	}

	void CWindow::OnDrop()
	{
		PyCallClassMemberFunc(m_poHandler, "OnDrop", BuildEmptyTuple());
	}

	void CWindow::OnTop()
	{
		PyCallClassMemberFunc(m_poHandler, "OnTop", BuildEmptyTuple());
	}

	void CWindow::OnIMEUpdate()
	{
		PyCallClassMemberFunc(m_poHandler, "OnIMEUpdate", BuildEmptyTuple());
	}

	BOOL CWindow::RunIMETabEvent()
	{
		if (!IsRendering())
			return FALSE;

		if (OnIMETabEvent())
			return TRUE;

		TWindowContainer::reverse_iterator itor;
		for (itor = m_pChildList.rbegin(); itor != m_pChildList.rend(); ++itor)
		{
			CWindow * pWindow = *itor;

			if (pWindow->RunIMETabEvent())
				return TRUE;
		}

		return FALSE;
	}

	BOOL CWindow::RunIMEReturnEvent()
	{
		if (!IsRendering())
			return FALSE;

		if (OnIMEReturnEvent())
			return TRUE;

		TWindowContainer::reverse_iterator itor;
		for (itor = m_pChildList.rbegin(); itor != m_pChildList.rend(); ++itor)
		{
			CWindow * pWindow = *itor;

			if (pWindow->RunIMEReturnEvent())
				return TRUE;
		}

		return FALSE;
	}

	BOOL CWindow::RunIMEKeyDownEvent(int ikey)
	{
		if (!IsRendering())
			return FALSE;

		if (OnIMEKeyDownEvent(ikey))
			return TRUE;

		TWindowContainer::reverse_iterator itor;
		for (itor = m_pChildList.rbegin(); itor != m_pChildList.rend(); ++itor)
		{
			CWindow * pWindow = *itor;

			if (pWindow->RunIMEKeyDownEvent(ikey))
				return TRUE;
		}

		return FALSE;
	}

	CWindow * CWindow::RunKeyDownEvent(int ikey)
	{
		if (OnKeyDown(ikey))
			return this;

		TWindowContainer::reverse_iterator itor;
		for (itor = m_pChildList.rbegin(); itor != m_pChildList.rend(); ++itor)
		{
			CWindow * pWindow = *itor;

			if (pWindow->IsShow())
			{
				CWindow * pProcessedWindow = pWindow->RunKeyDownEvent(ikey);
				if (NULL != pProcessedWindow)
				{
					return pProcessedWindow;
				}
			}
		}

		return NULL;
	}

	BOOL CWindow::RunKeyUpEvent(int ikey)
	{
		if (OnKeyUp(ikey))
			return TRUE;

		TWindowContainer::reverse_iterator itor;
		for (itor = m_pChildList.rbegin(); itor != m_pChildList.rend(); ++itor)
		{
			CWindow * pWindow = *itor;

			if (pWindow->IsShow())
			if (pWindow->RunKeyUpEvent(ikey))
				return TRUE;
		}

		return FALSE;
	}

	BOOL CWindow::RunPressEscapeKeyEvent()
	{
		TWindowContainer::reverse_iterator itor;
		for (itor = m_pChildList.rbegin(); itor != m_pChildList.rend(); ++itor)
		{
			CWindow * pWindow = *itor;

			if (pWindow->IsShow())
			if (pWindow->RunPressEscapeKeyEvent())
				return TRUE;
		}

		if (OnPressEscapeKey())
			return TRUE;

		return FALSE;
	}

	BOOL CWindow::RunPressExitKeyEvent()
	{
		TWindowContainer::reverse_iterator itor;
		for (itor = m_pChildList.rbegin(); itor != m_pChildList.rend(); ++itor)
		{
			CWindow * pWindow = *itor;

			if (pWindow->RunPressExitKeyEvent())
				return TRUE;

			if (pWindow->IsShow())
			if (pWindow->OnPressExitKey())
				return TRUE;
		}

		return FALSE;
	}

	BOOL CWindow::OnMouseLeftButtonDown()
	{
		long lValue;
		if (PyCallClassMemberFunc(m_poHandler, "OnMouseLeftButtonDown", BuildEmptyTuple(), &lValue))
		if (0 != lValue)
			return TRUE;

		return FALSE;
	}

	BOOL CWindow::OnMouseLeftButtonUp()
	{
		PyCallClassMemberFunc(m_poHandler, "OnMouseLeftButtonUp", BuildEmptyTuple());
		return TRUE; // NOTE : ButtonUpÀº ¿¹¿Ü·Î ¹«Á¶°Ç TRUE
	}

	BOOL CWindow::OnMouseLeftButtonDoubleClick()
	{
		long lValue;
		if (PyCallClassMemberFunc(m_poHandler, "OnMouseLeftButtonDoubleClick", BuildEmptyTuple(), &lValue))
		if (0 != lValue)
			return TRUE;

		return FALSE;
	}

	BOOL CWindow::OnMouseRightButtonDown()
	{
		long lValue;
		if (PyCallClassMemberFunc(m_poHandler, "OnMouseRightButtonDown", BuildEmptyTuple(), &lValue))
		if (0 != lValue)
			return TRUE;

		return FALSE;
	}

	BOOL CWindow::OnMouseRightButtonUp()
	{
		long lValue;
		if (PyCallClassMemberFunc(m_poHandler, "OnMouseRightButtonUp", BuildEmptyTuple(), &lValue))
		if (0 != lValue)
			return TRUE;

		return FALSE;
	}

	BOOL CWindow::OnMouseRightButtonDoubleClick()
	{
		long lValue;
		if (PyCallClassMemberFunc(m_poHandler, "OnMouseRightButtonDoubleClick", BuildEmptyTuple(), &lValue))
		if (0 != lValue)
			return TRUE;

		return FALSE;
	}

	BOOL CWindow::OnMouseMiddleButtonDown()
	{
		long lValue;
		if (PyCallClassMemberFunc(m_poHandler, "OnMouseMiddleButtonDown", BuildEmptyTuple(), &lValue))
		if (0 != lValue)
			return TRUE;

		return FALSE;
	}

	BOOL CWindow::OnMouseMiddleButtonUp()
	{
		long lValue;
		if (PyCallClassMemberFunc(m_poHandler, "OnMouseMiddleButtonUp", BuildEmptyTuple(), &lValue))
		if (0 != lValue)
			return TRUE;

		return FALSE;
	}

	BOOL CWindow::RunMouseWheelEvent(long nLen)
	{
		bool bValue = false;
		if (PyCallClassMemberFunc(m_poHandler, "OnRunMouseWheel", Py_BuildValue("(l)", nLen), &bValue))
			return bValue;
	}
	
	BOOL CWindow::OnIMETabEvent()
	{
		long lValue;
		if (PyCallClassMemberFunc(m_poHandler, "OnIMETab", BuildEmptyTuple(), &lValue))
		if (0 != lValue)
			return TRUE;

		return FALSE;
	}

	BOOL CWindow::OnIMEReturnEvent()
	{
		long lValue;
		if (PyCallClassMemberFunc(m_poHandler, "OnIMEReturn", BuildEmptyTuple(), &lValue))
		if (0 != lValue)
			return TRUE;

		return FALSE;
	}

	BOOL CWindow::OnIMEKeyDownEvent(int ikey)
	{
		long lValue;
		if (PyCallClassMemberFunc(m_poHandler, "OnIMEKeyDown", Py_BuildValue("(i)", ikey), &lValue))
		if (0 != lValue)
			return TRUE;

		return FALSE;
	}

	BOOL CWindow::OnMouseWheel(int nLen)
	{
		long lValue;
		return PyCallClassMemberFunc(m_poHandler, "OnMouseWheel", Py_BuildValue("(i)", nLen), &lValue) && 0 != lValue;
	}

	BOOL CWindow::OnIMEChangeCodePage()
	{
		long lValue;
		if (PyCallClassMemberFunc(m_poHandler, "OnIMEChangeCodePage", BuildEmptyTuple(), &lValue))
		if (0 != lValue)
			return TRUE;

		return FALSE;
	}

	BOOL CWindow::OnIMEOpenCandidateListEvent()
	{
		long lValue;
		if (PyCallClassMemberFunc(m_poHandler, "OnIMEOpenCandidateList", BuildEmptyTuple(), &lValue))
		if (0 != lValue)
			return TRUE;

		return FALSE;
	}

	BOOL CWindow::OnIMECloseCandidateListEvent()
	{
		long lValue;
		if (PyCallClassMemberFunc(m_poHandler, "OnIMECloseCandidateList", BuildEmptyTuple(), &lValue))
		if (0 != lValue)
			return TRUE;

		return FALSE;
	}

	BOOL CWindow::OnIMEOpenReadingWndEvent()
	{
		long lValue;
		if (PyCallClassMemberFunc(m_poHandler, "OnIMEOpenReadingWnd", BuildEmptyTuple(), &lValue))
		if (0 != lValue)
			return TRUE;

		return FALSE;
	}

	BOOL CWindow::OnIMECloseReadingWndEvent()
	{
		long lValue;
		if (PyCallClassMemberFunc(m_poHandler, "OnIMECloseReadingWnd", BuildEmptyTuple(), &lValue))
		if (0 != lValue)
			return TRUE;

		return FALSE;
	}

	BOOL CWindow::OnKeyDown(int ikey)
	{
		long lValue;
		if (PyCallClassMemberFunc(m_poHandler, "OnKeyDown", Py_BuildValue("(i)", ikey), &lValue))
		if (0 != lValue)
			return TRUE;

		return FALSE;
	}

	BOOL CWindow::OnKeyUp(int ikey)
	{
		long lValue;
		if (PyCallClassMemberFunc(m_poHandler, "OnKeyUp", Py_BuildValue("(i)", ikey), &lValue))
		if (0 != lValue)
			return TRUE;

		return FALSE;
	}

	BOOL CWindow::OnPressEscapeKey()
	{
		long lValue;
		if (PyCallClassMemberFunc(m_poHandler, "OnPressEscapeKey", BuildEmptyTuple(), &lValue))
		if (0 != lValue)
			return TRUE;

		return FALSE;
	}

	BOOL CWindow::OnPressExitKey()
	{
		long lValue;
		if (PyCallClassMemberFunc(m_poHandler, "OnPressExitKey", BuildEmptyTuple(), &lValue))
		if (0 != lValue)
			return TRUE;

		return FALSE;
	}

	/////

#ifdef ENABLE_WINDOW_SLIDE_EFFECT
	void CWindow::SetSpecialWindow()
	{
		m_bSpecialWindow = true;
	}

	void CWindow::EnableSlidingEffect() {
		m_bUseSlideEffect = true;
	}

	void CWindow::DisableSlidingEffect() {
		m_bUseSlideEffect = false;
	}

	void CWindow::StartSlideIn() {
		if (m_sliding_status == sliding_status_t::sliding_in)
			return;

		if (m_sliding_status == sliding_status_t::sliding_out)
			StopSlideOut();

		m_dScalingFactor = 0.0;
		m_dScalingStep = 1.0 / m_dSlidingTime;
		m_dLastSlidingTime = GetSecondsInDouble();
		m_sliding_status = sliding_status_t::sliding_in;

		StorePosition();
	}

	void CWindow::StopSlideIn() {
		m_dScalingFactor = 1.0;
		m_sliding_status = sliding_status_t::none;
		RestorePosition();
	}

	void CWindow::StartSlideOut() {
		if (m_sliding_status == sliding_status_t::sliding_out)
			return;

		if (m_sliding_status == sliding_status_t::sliding_in)
			StopSlideIn();


		m_dScalingFactor = 1.0;
		m_dScalingStep = -(1.0 / m_dSlidingTime);
		m_dLastSlidingTime = GetSecondsInDouble();
		m_sliding_status = sliding_status_t::sliding_out;

		StorePosition();
	}

	void CWindow::StopSlideOut() {
		m_dScalingFactor = 1.0;
		m_dScalingStep = 0;
		m_sliding_status = sliding_status_t::none;
		//Hide(false);
		RestorePosition();
	}

	void CWindow::SetSlidingTime(double s) {
		m_dSlidingTime = s;
	}

	void CWindow::__UpdateSlidingEffect() {
		if (m_bUseSlideEffect) {
			if (m_dScalingFactor != 1.0 && m_dScalingStep > 0) {
				const auto timesec = GetSecondsInDouble() - m_dLastSlidingTime;
				const auto scalingup = m_dScalingStep * timesec;

				m_dScalingFactor = std::min<double>(m_dScalingFactor + scalingup, 1.0);
				m_dLastSlidingTime = GetSecondsInDouble();

				if (m_dScalingFactor == 1.0) {
					StopSlideIn();
					return;
				}

				__UpdateChildrenSliding();
			}

			else if (m_dScalingStep < 0 && m_dScalingFactor != 0.0) {
				const auto timesec = GetSecondsInDouble() - m_dLastSlidingTime;
				const auto scalingdown = m_dScalingStep * timesec;
				m_dScalingFactor += scalingdown;
				m_dScalingFactor = std::max<double>(m_dScalingFactor, 0.0);

				m_dLastSlidingTime = GetSecondsInDouble();

				if (m_dScalingFactor == 0.0) {
					StopSlideOut();
					return;
				}

				__UpdateChildrenSliding();
			}
		}
	}

	float CWindow::NormalizeXAlign(const float x) {
		float ParentWidth = GetParent() ?
			float(GetParent()->GetWidth()) : CWindowManager::Instance().GetScreenWidth();
		switch (m_HorizontalAlign) {
		case HORIZONTAL_ALIGN_CENTER: return x + ParentWidth / 2.0f;
		case HORIZONTAL_ALIGN_RIGHT:  return ParentWidth - x;
		default: return x;
		}
	};

	float CWindow::NormalizeYAlign(const float y) {
		float ParentHeight = GetParent() ?
			float(GetParent()->GetHeight()) : CWindowManager::Instance().GetScreenHeight();
		switch (m_VerticalAlign) {
		case VERTICAL_ALIGN_CENTER: return y + ParentHeight / 2.0f;
		case VERTICAL_ALIGN_BOTTOM: return ParentHeight - y;
		default: return y;
		}
	};
	float CWindow::ApplyXAlign(const float x) {
		float ParentWidth = GetParent() ?
			float(GetParent()->GetWidth()) : CWindowManager::Instance().GetScreenWidth();
		switch (m_HorizontalAlign) {
		case HORIZONTAL_ALIGN_CENTER: return x - ParentWidth / 2.0f;
		case HORIZONTAL_ALIGN_RIGHT:  return ParentWidth - x;
		default: return x;
		}
	};
	float CWindow::ApplyYAlign(const float y) {
		float ParentHeight = GetParent() ?
			float(GetParent()->GetHeight()) : CWindowManager::Instance().GetScreenHeight();
		switch (m_VerticalAlign) {
		case VERTICAL_ALIGN_CENTER: return y - ParentHeight / 2.0f;
		case VERTICAL_ALIGN_BOTTOM: return ParentHeight - y;
		default: return y;
		}
	};

	void CWindow::__UpdateChildrenSliding() {
		if (m_bUseSlideEffect) {
			auto offsetx = (GetWidth() / 2) * (1.0 - m_dScalingFactor);
			auto offsety = (GetHeight() / 2) * (1.0 - m_dScalingFactor);

			auto x = NormalizeXAlign(m_lxStoring) + offsetx;
			auto y = NormalizeYAlign(m_lyStoring) + offsety;

			SetPosition(ApplyXAlign(x), ApplyYAlign(y));
		}

		const auto scaling = GetParentSlidingFactor();

		for (auto& child : m_pChildList) {
			child->__SetSlidingPosition(scaling);
			child->__UpdateChildrenSliding();

			if (!m_bUseSlideEffect && (!IsType(CWindow::Type()) || m_bSpecialWindow)) {
				auto xmov = (GetSlidingWidth() * (1.0 - scaling)) / 2;
				auto ymov = (GetSlidingHeight() * (1.0 - scaling)) / 2;

				auto NormalizeParentMovement = [this, &scaling, &xmov, &ymov](CWindow* child) {
					auto x = child->NormalizeXAlign(child->GetPositionX()) + xmov;
					auto y = child->NormalizeYAlign(child->GetPositionY()) + ymov;
					child->SetPosition(child->ApplyXAlign(x), child->ApplyYAlign(y));
				};

				NormalizeParentMovement(child);
			}
		}
	}

	void CWindow::__SetSlidingPosition(const double scaling) {
		auto x = NormalizeXAlign(m_lxStoring);
		auto y = NormalizeYAlign(m_lyStoring);

		x -= (x * (1.0 - scaling)) + 0.5f;
		y -= (y * (1.0 - scaling)) + 0.5f;

		if (!IsType(CWindow::Type()) || m_bSpecialWindow) {
			x -= (GetSlidingWidth() * (1.0 - scaling)) / 2;
			y -= (GetSlidingHeight() * (1.0 - scaling)) / 2;
		}

		SetPosition(ApplyXAlign(x),
			ApplyYAlign(y));
	}

	void CWindow::StorePosition() {
		m_lxStoring = m_x;
		m_lyStoring = m_y;

		for (auto& child : m_pChildList)
			child->StorePosition();
	}

	void CWindow::RestorePosition() {
		SetPosition(m_lxStoring, m_lyStoring);
		for (auto& child : m_pChildList)
			child->RestorePosition();
	}

	void CWindow::SetStoredPositon(const long x, const long y)
	{
		m_lxStoring = x;
		m_lyStoring = y;

		for (auto& child : m_pChildList) {
			child->StorePosition();
		}
	}

	bool CWindow::IsSlidingRunning()
	{
		if (IsSlidingEffectEnabled()) {
			return m_sliding_status == sliding_status_t::sliding_out || m_sliding_status == sliding_status_t::sliding_in;
		} return (m_pParent && m_pParent->IsSlidingRunning());
	}
#endif

	bool CWindow::IsIn(long x, long y)
	{
		if (x >= m_rect.left && x <= m_rect.right)
			if (y >= m_rect.top && y <= m_rect.bottom)
				return true;

		return false;
	}

	bool CWindow::IsIn()
	{
		long lx, ly;
		UI::CWindowManager::Instance().GetMousePosition(lx, ly);

		return IsIn(lx, ly);
	}

	CWindow * CWindow::PickWindow(long x, long y)
	{
		std::list<CWindow *>::reverse_iterator ritor = m_pChildList.rbegin();
		for (; ritor != m_pChildList.rend(); ++ritor)
		{
			CWindow * pWin = *ritor;
			if (pWin->IsShow())
			{
				if (!pWin->IsFlag(CWindow::FLAG_IGNORE_SIZE))
				{
					if (!pWin->IsIn(x, y)) {
						if (0L <= pWin->GetWidth()) {
							continue;
						}
					}
				}

				CWindow * pResult = pWin->PickWindow(x, y);
				if (pResult)
					return pResult;
			}
		}

		if (IsFlag(CWindow::FLAG_NOT_PICK))
			return NULL;

		return (this);
	}

	CWindow * CWindow::PickTopWindow(long x, long y)
	{
		std::list<CWindow *>::reverse_iterator ritor = m_pChildList.rbegin();
		for (; ritor != m_pChildList.rend(); ++ritor)
		{
			CWindow * pWin = *ritor;
			if (pWin->IsShow())
				if (pWin->IsIn(x, y))
					if (!pWin->IsFlag(CWindow::FLAG_NOT_PICK))
						return pWin;
		}

		return NULL;
	}

#ifdef INGAME_WIKI
	void CWindow::iSetRenderingRect(int iLeft, int iTop, int iRight, int iBottom)
	{
		m_renderingRect.left = iLeft;
		m_renderingRect.top = iTop;
		m_renderingRect.right = iRight;
		m_renderingRect.bottom = iBottom;

		OnSetRenderingRect();
	}

	void CWindow::SetRenderingRect(float fLeft, float fTop, float fRight, float fBottom)
	{
		float fWidth = float(GetWidth());
		float fHeight = float(GetHeight());
		if (IsType(CTextLine::Type()))
		{
			int iWidth, iHeight;
			((CTextLine*)this)->GetTextSize(&iWidth, &iHeight);
			fWidth = float(iWidth);
			fHeight = float(iHeight);
		}

		iSetRenderingRect(fWidth * fLeft, fHeight * fTop, fWidth * fRight, fHeight * fBottom);
	}

	int CWindow::GetRenderingWidth()
	{
		return max(0, GetWidth() + m_renderingRect.right + m_renderingRect.left);
	}

	int CWindow::GetRenderingHeight()
	{
		return max(0, GetHeight() + m_renderingRect.bottom + m_renderingRect.top);
	}

	void CWindow::ResetRenderingRect(bool bCallEvent)
	{
		m_renderingRect.bottom = m_renderingRect.left = m_renderingRect.right = m_renderingRect.top = 0;

		if (bCallEvent)
			OnSetRenderingRect();
	}

	void CWindow::OnSetRenderingRect()
	{
	}
#endif

	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////

#ifdef RENDER_TARGET
	CUiRenderTarget::CUiRenderTarget(PyObject * ppyObject) : CWindow(ppyObject)
	{
		m_dwIndex = -1;
	}
	CUiRenderTarget::~CUiRenderTarget() = default;

	bool CUiRenderTarget::SetRenderTarget(uint8_t index)
	{
		if (!CRenderTargetManager::Instance().GetRenderTarget(index))
		{
			if (!CRenderTargetManager::Instance().CreateRenderTarget(index, GetWidth(), GetHeight()))
			{
				TraceError("CRenderTargetManager could not create the texture. w %d h %d", GetWidth(), GetHeight());
				return false;
			}
		}
		m_dwIndex = index;

		UpdateRect();
		return true;
	}

	void CUiRenderTarget::OnRender()
	{
		auto target = CRenderTargetManager::Instance().GetRenderTarget(m_dwIndex);
		if (!target)
		{
			TraceError("SetRenderingRect -> target empty!");
			return;
		}

		target->SetRenderingRect(&m_rect);

		target->RenderTexture();
	}
#endif

#ifdef INGAME_WIKI
	CUiWikiRenderTarget::~CUiWikiRenderTarget() = default;
	CUiWikiRenderTarget::CUiWikiRenderTarget(PyObject * ppyObject) :
		CWindow(ppyObject),
		m_dwIndex(-1) {}

	/*----------------------------
	--------PUBLIC CLASS FUNCTIONS
	-----------------------------*/

	bool CUiWikiRenderTarget::SetWikiRenderTargetModule(int iRenderTargetModule)
	{
		if (!CWikiRenderTargetManager::Instance().GetRenderTarget(iRenderTargetModule))
		{
			if (!CWikiRenderTargetManager::Instance().CreateRenderTarget(iRenderTargetModule, GetWidth(), GetHeight()))
			{
				TraceError("CWikiRenderTargetManager could not create the texture. w %d h %d", GetWidth(), GetHeight());
				return false;
			}
		}

		m_dwIndex = iRenderTargetModule;

		UpdateRect();
		return true;
	}

	void CUiWikiRenderTarget::OnUpdateRenderBox()
	{
		if (m_dwIndex == -1 /*(CPythonWikiRenderTarget::START_MODULE)*/)
			return;

		auto target = CWikiRenderTargetManager::Instance().GetRenderTarget(m_dwIndex);
		if (!target)
			return;

		target->SetRenderingBox(&m_renderBox);
	}

	/*----------------------------
	-----PROTECTED CLASS FUNCTIONS
	-----------------------------*/

	void CUiWikiRenderTarget::OnRender()
	{
		if (m_dwIndex == -1 /*(CPythonWikiRenderTarget::START_MODULE)*/)
			return;

		auto target = CWikiRenderTargetManager::Instance().GetRenderTarget(m_dwIndex);
		if (!target)
			return;

		target->SetRenderingRect(&m_rect);
		target->RenderTexture();
	}
#endif

	CBox::CBox(PyObject* ppyObject) : CWindow(ppyObject), m_dwColor(0xff000000) {}
	CBox::~CBox() = default;

	void CBox::SetColor(DWORD dwColor)
	{
		m_dwColor = dwColor;
	}

	void CBox::OnRender()
	{
		CPythonGraphic::Instance().SetDiffuseColor(m_dwColor);
#ifdef ENABLE_WINDOW_SLIDE_EFFECT
		const auto scaling = GetParentSlidingFactor();
		if (scaling != 1.0f) {
			const auto width = m_rect.right - m_rect.left;
			const auto height = m_rect.bottom - m_rect.top;
			const auto xoff = (width * (1.0 - scaling)) / 2;
			const auto yoff = (height * (1.0 - scaling)) / 2;
			auto oldSliding = grp::SetSliding(1.0f);
			CPythonGraphic::Instance().RenderBox2d(m_rect.left + xoff, m_rect.top + yoff, m_rect.right - xoff, m_rect.bottom - yoff);
			grp::SetSliding(oldSliding);
		}
		else {
			CPythonGraphic::Instance().RenderBox2d(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
		}
#else
		CPythonGraphic::Instance().RenderBox2d(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
#endif
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////

	CBar::CBar(PyObject* ppyObject) : CWindow(ppyObject), m_dwColor(0xff000000) {}
	CBar::~CBar() = default;

	void CBar::SetColor(DWORD dwColor)
	{
		m_dwColor = dwColor;
	}

	void CBar::OnRender()
	{
		CPythonGraphic::Instance().SetDiffuseColor(m_dwColor);
#ifdef ENABLE_WINDOW_SLIDE_EFFECT
		const auto scaling = GetParentSlidingFactor();

		if (scaling != 1.0f) {
			auto width = m_rect.right - m_rect.left;
			auto height = m_rect.bottom - m_rect.top;

			auto xoff = (width * (1.0 - scaling)) / 2;
			auto yoff = (height * (1.0 - scaling)) / 2;

			auto left = IsSlidingEffectEnabled() ? m_rect.left : m_rect.left + xoff;
			auto top = IsSlidingEffectEnabled() ? m_rect.top : m_rect.top + yoff;
			auto right = left + (width * scaling);
			auto bottom = top + (height * scaling);

			auto oldSliding = grp::SetSliding(1.0f);
			CPythonGraphic::Instance().RenderBar2d(left, top, right, bottom);
			grp::SetSliding(oldSliding);
		}
		else {
#ifdef INGAME_WIKI
			CPythonGraphic::Instance().RenderBar2d(m_rect.left + m_renderBox.left, m_rect.top + m_renderBox.top, m_rect.right - m_renderBox.right, m_rect.bottom - m_renderBox.bottom);
#else
			CPythonGraphic::Instance().RenderBar2d(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
#endif
		}
#else
#ifdef INGAME_WIKI
		CPythonGraphic::Instance().RenderBar2d(m_rect.left + m_renderBox.left, m_rect.top + m_renderBox.top, m_rect.right - m_renderBox.right, m_rect.bottom - m_renderBox.bottom);
#else
		CPythonGraphic::Instance().RenderBar2d(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
#endif
#endif
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////

	CLine::CLine(PyObject* ppyObject) : CWindow(ppyObject), m_dwColor(0xff000000) {}
	CLine::~CLine() = default;

	void CLine::SetColor(DWORD dwColor)
	{
		m_dwColor = dwColor;
	}

	void CLine::OnRender()
	{
		CPythonGraphic & rkpyGraphic = CPythonGraphic::Instance();
		rkpyGraphic.SetDiffuseColor(m_dwColor);
#ifdef ENABLE_WINDOW_SLIDE_EFFECT
		const auto scaling = GetParentSlidingFactor();
		if (scaling != 1.0f) {
			const auto width = m_rect.right - m_rect.left;
			const auto height = m_rect.bottom - m_rect.top;
			const auto xoff = (width * (1.0 - scaling)) / 2;
			const auto yoff = (height * (1.0 - scaling)) / 2;
			auto oldSliding = grp::SetSliding(1.0f);
			rkpyGraphic.RenderLine2d(m_rect.left + xoff, m_rect.top + yoff, m_rect.right - xoff, m_rect.bottom - yoff);
			grp::SetSliding(oldSliding);
		}
		else {
			rkpyGraphic.RenderLine2d(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
		}
#else
		rkpyGraphic.RenderLine2d(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
#endif
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////

	DWORD CBar3D::Type()
	{
		static DWORD s_dwType = GetCRC32("CBar3D", strlen("CBar3D"));
		return (s_dwType);
	}

	CBar3D::CBar3D(PyObject* ppyObject) : CWindow(ppyObject)
	{
		m_dwLeftColor = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
		m_dwRightColor = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
		m_dwCenterColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	}
	CBar3D::~CBar3D() = default;

	void CBar3D::SetColor(DWORD dwLeft, DWORD dwRight, DWORD dwCenter)
	{
		m_dwLeftColor = dwLeft;
		m_dwRightColor = dwRight;
		m_dwCenterColor = dwCenter;
	}

	void CBar3D::OnRender()
	{
		CPythonGraphic & rkpyGraphic = CPythonGraphic::Instance();
#ifdef ENABLE_WINDOW_SLIDE_EFFECT
		const auto scaling = GetParentSlidingFactor();
		
		if (scaling != 1.0)
		{
			const auto width = m_rect.right - m_rect.left;
			const auto height = m_rect.bottom - m_rect.top;
			const auto xoff = (width * (1.0 - scaling)) / 2;
			const auto yoff = (height * (1.0 - scaling)) / 2;

			auto center_color = D3DXCOLOR(m_dwCenterColor);
			auto border_color = D3DXCOLOR(m_dwLeftColor);
			auto right_color = D3DXCOLOR(m_dwRightColor);

			center_color.a = scaling * scaling;
			border_color.a = scaling * scaling;
			right_color.a = scaling * scaling;

			auto oldSliding = grp::SetSliding(1.0f);

			rkpyGraphic.SetDiffuseColor(center_color);
			rkpyGraphic.RenderBar2d(m_rect.left + xoff, m_rect.top + yoff, m_rect.right - xoff, m_rect.bottom - yoff);

			rkpyGraphic.SetDiffuseColor(border_color);
			rkpyGraphic.RenderLine2d(m_rect.left + xoff, m_rect.top + yoff, m_rect.right - xoff, m_rect.top + yoff);
			rkpyGraphic.RenderLine2d(m_rect.left + xoff, m_rect.top + yoff, m_rect.left + xoff, m_rect.bottom - yoff);

			rkpyGraphic.SetDiffuseColor(right_color);
			rkpyGraphic.RenderLine2d(m_rect.left + xoff, m_rect.bottom - yoff, m_rect.right - xoff, m_rect.bottom - yoff);
			rkpyGraphic.RenderLine2d(m_rect.right - xoff, m_rect.top + yoff, m_rect.right - xoff, m_rect.bottom - yoff);

			grp::SetSliding(oldSliding);
		}
		else
		{
			rkpyGraphic.SetDiffuseColor(m_dwCenterColor);
			rkpyGraphic.RenderBar2d(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
			rkpyGraphic.SetDiffuseColor(m_dwLeftColor);
			rkpyGraphic.RenderLine2d(m_rect.left, m_rect.top, m_rect.right, m_rect.top);
			rkpyGraphic.RenderLine2d(m_rect.left, m_rect.top, m_rect.left, m_rect.bottom);
			rkpyGraphic.SetDiffuseColor(m_dwRightColor);
			rkpyGraphic.RenderLine2d(m_rect.left, m_rect.bottom, m_rect.right, m_rect.bottom);
			rkpyGraphic.RenderLine2d(m_rect.right, m_rect.top, m_rect.right, m_rect.bottom);
		}
#else
		rkpyGraphic.SetDiffuseColor(m_dwCenterColor);
		rkpyGraphic.RenderBar2d(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
		rkpyGraphic.SetDiffuseColor(m_dwLeftColor);
		rkpyGraphic.RenderLine2d(m_rect.left, m_rect.top, m_rect.right, m_rect.top);
		rkpyGraphic.RenderLine2d(m_rect.left, m_rect.top, m_rect.left, m_rect.bottom);
		rkpyGraphic.SetDiffuseColor(m_dwRightColor);
		rkpyGraphic.RenderLine2d(m_rect.left, m_rect.bottom, m_rect.right, m_rect.bottom);
		rkpyGraphic.RenderLine2d(m_rect.right, m_rect.top, m_rect.right, m_rect.bottom);
#endif
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////

	CTextLine::CTextLine(PyObject * ppyObject) : CWindow(ppyObject)
	{
		m_TextInstance.SetColor(0.78f, 0.78f, 0.78f);
		m_TextInstance.SetHorizonalAlign(CGraphicTextInstance::HORIZONTAL_ALIGN_LEFT);
		m_TextInstance.SetVerticalAlign(CGraphicTextInstance::VERTICAL_ALIGN_TOP);
	}
	CTextLine::~CTextLine()
	{
		m_TextInstance.Destroy();
	}

#ifdef INGAME_WIKI
	DWORD CTextLine::Type()
	{
		static DWORD s_dwType = GetCRC32("CTextLine", strlen("CTextLine"));
		return (s_dwType);
	}
#endif

	void CTextLine::SetMax(int iMax)
	{
		m_TextInstance.SetMax(iMax);
	}
	void CTextLine::SetHorizontalAlign(int iType)
	{
		m_TextInstance.SetHorizonalAlign(iType);
	}
	void CTextLine::SetVerticalAlign(int iType)
	{
		m_TextInstance.SetVerticalAlign(iType);
	}
	void CTextLine::SetSecret(BOOL bFlag)
	{
		m_TextInstance.SetSecret(bFlag ? true : false);
	}
	void CTextLine::SetOutline(BOOL bFlag)
	{
		m_TextInstance.SetOutline(bFlag ? true : false);
	}
	void CTextLine::SetFeather(BOOL bFlag)
	{
		m_TextInstance.SetFeather(bFlag ? true : false);
	}
	void CTextLine::SetMultiLine(BOOL bFlag)
	{
		m_TextInstance.SetMultiLine(bFlag ? true : false);
	}
	void CTextLine::SetFontName(const char * c_szFontName)
	{
		std::string stFontName = c_szFontName;
		stFontName += ".fnt";

		CResourceManager& rkResMgr=CResourceManager::Instance();
		CResource* pkRes = rkResMgr.GetTypeResourcePointer(stFontName.c_str());
		CGraphicText* pkResFont=static_cast<CGraphicText*>(pkRes);
		m_TextInstance.SetTextPointer(pkResFont);
	}
	void CTextLine::SetFontColor(DWORD dwColor)
	{
		m_TextInstance.SetColor(dwColor);
	}
	void CTextLine::SetLimitWidth(float fWidth)
	{
		m_TextInstance.SetLimitWidth(fWidth);
	}
	void CTextLine::SetText(const char * c_szText)
	{
		OnSetText(c_szText);
	}
	void CTextLine::GetTextSize(int* pnWidth, int* pnHeight)
	{
		m_TextInstance.GetTextSize(pnWidth, pnHeight);
	}
	const char * CTextLine::GetText()
	{
		return m_TextInstance.GetValueStringReference().c_str();
	}
	void CTextLine::ShowCursor()
	{
		m_TextInstance.ShowCursor();
	}
	void CTextLine::HideCursor()
	{
		m_TextInstance.HideCursor();
	}

#ifdef INGAME_WIKI
	bool CTextLine::IsShowCursor()
	{
		return m_TextInstance.IsShowCursor();
	}
#endif

	int CTextLine::GetCursorPosition()
	{
		long lx, ly;
		CWindow::GetMouseLocalPosition(lx, ly);
		return m_TextInstance.PixelPositionToCharacterPosition(lx);
	}

	void CTextLine::OnSetText(const char * c_szText)
	{
		m_TextInstance.SetValue(c_szText);
		m_TextInstance.Update();
#ifdef INGAME_WIKI
		if (m_isInsideRender)
			UpdateRenderBoxRecursive();
#endif
	}

#ifdef INGAME_WIKI
	bool CTextLine::IsShow()
	{
		if (!m_bShow)
			return false;

		if (m_isInsideRender)
		{
			int cW, cH;
			GetTextSize(&cW, &cH);
			if (m_renderBox.left + m_renderBox.right >= cW || m_renderBox.top + m_renderBox.bottom >= cH)
				return false;
		}

		return true;
	}
#endif

	void CTextLine::OnUpdate()
	{
		if (IsShow())
			m_TextInstance.Update();
	}
	void CTextLine::OnRender()
	{
		if (IsShow())
#ifdef ENABLE_WINDOW_SLIDE_EFFECT
			m_TextInstance.Render(NULL, GetParentSlidingFactor());
#else
			m_TextInstance.Render();
#endif

	}


#ifdef INGAME_WIKI
	int CTextLine::GetRenderingWidth()
	{
		int iTextWidth, iTextHeight;
		GetTextSize(&iTextWidth, &iTextHeight);

		return iTextWidth + m_renderingRect.right + m_renderingRect.left;
	}

	int CTextLine::GetRenderingHeight()
	{
		int iTextWidth, iTextHeight;
		GetTextSize(&iTextWidth, &iTextHeight);

		return iTextHeight + m_renderingRect.bottom + m_renderingRect.top;
	}

	void CTextLine::OnSetRenderingRect()
	{
		int iTextWidth, iTextHeight;
		GetTextSize(&iTextWidth, &iTextHeight);

		m_TextInstance.iSetRenderingRect(m_renderingRect.left, -m_renderingRect.top, m_renderingRect.right, m_renderingRect.bottom);
	}
#endif

	void CTextLine::OnChangePosition()
	{
		// FOR_ARABIC_ALIGN
		//if (m_TextInstance.GetHorizontalAlign() == CGraphicTextInstance::HORIZONTAL_ALIGN_ARABIC)
		if( GetDefaultCodePage() == CP_ARABIC )
		{
			m_TextInstance.SetPosition(m_rect.right, m_rect.top);
		}
		else
		{
			m_TextInstance.SetPosition(m_rect.left, m_rect.top);
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////

	CNumberLine::CNumberLine(PyObject * ppyObject) : CWindow(ppyObject)
	{
		m_strPath = "d:/ymir work/ui/game/taskbar/";
		m_iHorizontalAlign = HORIZONTAL_ALIGN_LEFT;
		m_dwWidthSummary = 0;
	}
	CNumberLine::CNumberLine(CWindow * pParent) : CWindow(NULL)
	{
		m_strPath = "d:/ymir work/ui/game/taskbar/";
		m_iHorizontalAlign = HORIZONTAL_ALIGN_LEFT;
		m_dwWidthSummary = 0;

		m_pParent = pParent;
	}
	CNumberLine::~CNumberLine()
	{
		ClearNumber();
	}

	void CNumberLine::SetPath(const char * c_szPath)
	{
		m_strPath = c_szPath;
	}
	void CNumberLine::SetHorizontalAlign(int iType)
	{
		m_iHorizontalAlign = iType;
	}
	void CNumberLine::SetNumber(const char * c_szNumber)
	{
		if (0 == m_strNumber.compare(c_szNumber))
			return;

		ClearNumber();

		m_strNumber = c_szNumber;

		for (DWORD i = 0; i < m_strNumber.size(); ++i)
		{
			char cChar = m_strNumber[i];
			std::string strImageFileName;

			if (':' == cChar)
			{
				strImageFileName = m_strPath + "colon.sub";
			}
			else if ('?' == cChar)
			{
				strImageFileName = m_strPath + "questionmark.sub";
			}
			else if ('/' == cChar)
			{
				strImageFileName = m_strPath + "slash.sub";
			}
			else if ('%' == cChar)
			{
				strImageFileName = m_strPath + "percent.sub";
			}
			else if ('+' == cChar)
			{
				strImageFileName = m_strPath + "plus.sub";
			}
			else if ('m' == cChar)
			{
				strImageFileName = m_strPath + "m.sub";
			}
			else if ('g' == cChar)
			{
				strImageFileName = m_strPath + "g.sub";
			}
			else if ('p' == cChar)
			{
				strImageFileName = m_strPath + "p.sub";
			}
			else if (cChar >= '0' && cChar <= '9')
			{
				strImageFileName = m_strPath;
				strImageFileName += cChar;
				strImageFileName += ".sub";
			}
			else
				continue;

			if (!CResourceManager::Instance().IsFileExist(strImageFileName.c_str()))
				continue;

			CGraphicImage * pImage = (CGraphicImage *)CResourceManager::Instance().GetResourcePointer(strImageFileName.c_str());

			CGraphicImageInstance * pInstance = CGraphicImageInstance::New();
			pInstance->SetImagePointer(pImage);
			m_ImageInstanceVector.push_back(pInstance);

			m_dwWidthSummary += pInstance->GetWidth();
		}
	}

	void CNumberLine::ClearNumber()
	{
		for (auto ins : m_ImageInstanceVector)
		{
			CGraphicImageInstance::Delete(ins);
		} /* Clear number memory leak fix */

		m_ImageInstanceVector.clear();
		m_dwWidthSummary = 0;
		//m_dwMaxHeight = 0;
		m_strNumber = "";
	}

	void CNumberLine::OnRender()
	{
		for (DWORD i = 0; i < m_ImageInstanceVector.size(); ++i)
		{
#ifdef ENABLE_WINDOW_SLIDE_EFFECT
			m_ImageInstanceVector[i]->Render(GetParentSlidingFactor());
#else
			m_ImageInstanceVector[i]->Render();
#endif
		}
	}

	void CNumberLine::OnChangePosition()
	{
		int ix = m_x;
		int iy = m_y;

		if (m_pParent)
		{
			ix = m_rect.left;
			iy = m_rect.top;
		}

		switch (m_iHorizontalAlign)
		{
			case HORIZONTAL_ALIGN_LEFT:
				break;
			case HORIZONTAL_ALIGN_CENTER:
				ix -= int(m_dwWidthSummary) / 2;
				break;
			case HORIZONTAL_ALIGN_RIGHT:
				ix -= int(m_dwWidthSummary);
				break;
		}

		for (DWORD i = 0; i < m_ImageInstanceVector.size(); ++i)
		{
			m_ImageInstanceVector[i]->SetPosition(ix, iy);
			ix += m_ImageInstanceVector[i]->GetWidth();
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////

	CImageBox::CImageBox(PyObject * ppyObject) : CWindow(ppyObject)
		,m_fCoolTime(0.0f),
		m_fCoolTimeStart(0.0f),
		m_bCoolTimeInverse(false),
		m_bCoolTimeInvert(false),
		m_fRectCircle(99.0f)

	{
		m_pImageInstance = NULL;
	}
	CImageBox::~CImageBox()
	{
		OnDestroyInstance();
	}

	void CImageBox::OnCreateInstance()
	{
		OnDestroyInstance();

		m_pImageInstance = CGraphicImageInstance::New();
	}

	void CImageBox::SetCoolTime(float fCoolTime)
	{
		m_fCoolTime = fCoolTime;
	}

	void CImageBox::SetCoolTimeStart(float fCoolTimeStart)
	{
		m_fCoolTimeStart = fCoolTimeStart;
	}

	void CImageBox::SetCoolTimeInverse(bool bFlag)
	{
		m_bCoolTimeInverse = bFlag;
	}

	void CImageBox::SetCoolTimeInvert(bool bFlag)
	{
		m_bCoolTimeInvert = bFlag;
	}

	void CImageBox::SetRectCircle(float fRectCircle)
	{
		m_fRectCircle = fRectCircle;
	}

	void CImageBox::OnDestroyInstance()
	{
		if (m_pImageInstance)
		{
			CGraphicImageInstance::Delete(m_pImageInstance);
			m_pImageInstance=NULL;
		}
	}

	BOOL CImageBox::LoadImage(const char * c_szFileName)
	{
		if (!c_szFileName[0])
			return FALSE;

		OnCreateInstance();

		CResource * pResource = CResourceManager::Instance().GetResourcePointer(c_szFileName);
		if (!pResource)
			return FALSE;
		if (!pResource->IsType(CGraphicImage::Type()))
			return FALSE;

		m_pImageInstance->SetImagePointer(static_cast<CGraphicImage*>(pResource));
		if (m_pImageInstance->IsEmpty())
			return FALSE;

		SetSize(m_pImageInstance->GetWidth(), m_pImageInstance->GetHeight());
		UpdateRect();

		return TRUE;
	}
	
	void CImageBox::SetScale(float sx, float sy)
	{
		if (!m_pImageInstance)
			return;

		((CGraphicImageInstance*)m_pImageInstance)->SetScale(sx, sy);
		CWindow::SetSize(long(float(GetWidth())*sx), long(float(GetHeight())*sy));
	}

	void CImageBox::SetDiffuseColor(float fr, float fg, float fb, float fa)
	{
		if (!m_pImageInstance)
			return;

		m_pImageInstance->SetDiffuseColor(fr, fg, fb, fa);
	}

	int CImageBox::GetWidth()
	{
		if (!m_pImageInstance)
			return 0;

		return m_pImageInstance->GetWidth();
	}

	int CImageBox::GetHeight()
	{
		if (!m_pImageInstance)
			return 0;

		return m_pImageInstance->GetHeight();
	}

	void CImageBox::OnUpdate()
	{
	}
	void CImageBox::OnRender()
	{
		if (!m_pImageInstance)
			return;

		if (IsShow())
		{
			if (m_fRectCircle != 99.0f)
			{
				float fCoolTimePct = m_fRectCircle;
				if (m_bCoolTimeInvert)
					fCoolTimePct = (m_fRectCircle - CTimer::Instance().GetCurrentSecond()) / m_fRectCircle;
				m_pImageInstance->RenderRectCircle(fCoolTimePct, m_bCoolTimeInverse);
			}
			else if (m_fCoolTime != 0.0f)
			{
				float fCoolTimePct = (CTimer::Instance().GetCurrentSecond() - m_fCoolTimeStart) / m_fCoolTime;
				if (m_bCoolTimeInvert)
					fCoolTimePct = (m_fCoolTimeStart + m_fCoolTime - CTimer::Instance().GetCurrentSecond()) / m_fCoolTime;

				m_pImageInstance->RenderCoolTime(fCoolTimePct, m_bCoolTimeInverse);
			}
			else
#ifdef ENABLE_WINDOW_SLIDE_EFFECT
				m_pImageInstance->Render(GetParentSlidingFactor());
#else
				m_pImageInstance->Render();
#endif

		}

	}
	void CImageBox::OnChangePosition()
	{
		if (!m_pImageInstance)
			return;

		m_pImageInstance->SetPosition(m_rect.left, m_rect.top);
	}

#ifdef ENABLE_WINDOW_SLIDE_EFFECT
	DWORD CImageBox::Type() {
		static const std::string classname = __FUNCTION__;
		static const DWORD crc = GetCRC32(classname.c_str(), classname.length());
		return crc;
	}

	BOOL CImageBox::OnIsType(DWORD dwType) {
		return dwType == CImageBox::Type();
	}
#endif

	///////////////////////////////////////////////////////////////////////////////////////////////
	// MarkBox - ¸¶Å© Ãâ·Â¿ë UI À©µµ¿ì
	///////////////////////////////////////////////////////////////////////////////////////////////
	CMarkBox::CMarkBox(PyObject * ppyObject) : CWindow(ppyObject)
	{
		m_pMarkInstance = NULL;
	}

	CMarkBox::~CMarkBox()
	{
		OnDestroyInstance();
	}

	void CMarkBox::OnCreateInstance()
	{
		OnDestroyInstance();
		m_pMarkInstance = CGraphicMarkInstance::New();
	}

	void CMarkBox::OnDestroyInstance()
	{
		if (m_pMarkInstance)
		{
			CGraphicMarkInstance::Delete(m_pMarkInstance);
			m_pMarkInstance=NULL;
		}
	}

	void CMarkBox::LoadImage(const char * c_szFilename)
	{
		OnCreateInstance();

		m_pMarkInstance->SetImageFileName(c_szFilename);
		m_pMarkInstance->Load();
		SetSize(m_pMarkInstance->GetWidth(), m_pMarkInstance->GetHeight());

		UpdateRect();
	}

	void CMarkBox::SetScale(FLOAT fScale)
	{
		if (!m_pMarkInstance)
			return;

		m_pMarkInstance->SetScale(fScale);
	}

	void CMarkBox::SetIndex(UINT uIndex)
	{
		if (!m_pMarkInstance)
			return;

		m_pMarkInstance->SetIndex(uIndex);
	}

	void CMarkBox::SetDiffuseColor(float fr, float fg, float fb, float fa)
	{
		if (!m_pMarkInstance)
			return;

		m_pMarkInstance->SetDiffuseColor(fr, fg, fb, fa);
	}

	void CMarkBox::OnUpdate()
	{
	}

	void CMarkBox::OnRender()
	{
		if (!m_pMarkInstance)
			return;

		if (IsShow())
#ifdef ENABLE_WINDOW_SLIDE_EFFECT
			m_pMarkInstance->Render(GetParentSlidingFactor());
#else
			m_pMarkInstance->Render();
#endif
	}

	void CMarkBox::OnChangePosition()
	{
		if (!m_pMarkInstance)
			return;

		m_pMarkInstance->SetPosition(m_rect.left, m_rect.top);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////

	DWORD CExpandedImageBox::Type()
	{
		static DWORD s_dwType = GetCRC32("CExpandedImageBox", strlen("CExpandedImageBox"));
		return (s_dwType);
	}

	BOOL CExpandedImageBox::OnIsType(DWORD dwType)
	{
		if (CExpandedImageBox::Type() == dwType)
			return TRUE;

		return FALSE;
	}

	CExpandedImageBox::CExpandedImageBox(PyObject* ppyObject) : CImageBox(ppyObject) {}

	CExpandedImageBox::~CExpandedImageBox()
	{
		OnDestroyInstance();
	}

	void CExpandedImageBox::OnCreateInstance()
	{
		OnDestroyInstance();

		m_pImageInstance = CGraphicExpandedImageInstance::New();
	}
	void CExpandedImageBox::OnDestroyInstance()
	{
		if (m_pImageInstance)
		{
			CGraphicExpandedImageInstance::Delete((CGraphicExpandedImageInstance*)m_pImageInstance);
			m_pImageInstance=NULL;
		}
	}

	void CExpandedImageBox::SetScale(float fx, float fy)
	{
		if (!m_pImageInstance)
			return;

		((CGraphicExpandedImageInstance*)m_pImageInstance)->SetScale(fx, fy);
		CWindow::SetSize(long(float(GetWidth())*fx), long(float(GetHeight())*fy));
	}
	void CExpandedImageBox::SetOrigin(float fx, float fy)
	{
		if (!m_pImageInstance)
			return;

		((CGraphicExpandedImageInstance*)m_pImageInstance)->SetOrigin(fx, fy);
	}
	void CExpandedImageBox::SetRotation(float fRotation)
	{
		if (!m_pImageInstance)
			return;

		((CGraphicExpandedImageInstance*)m_pImageInstance)->SetRotation(fRotation);
	}
	void CExpandedImageBox::SetRenderingRect(float fLeft, float fTop, float fRight, float fBottom)
	{
		if (!m_pImageInstance)
			return;

		((CGraphicExpandedImageInstance*)m_pImageInstance)->SetRenderingRect(fLeft, fTop, fRight, fBottom);
	}

	void CExpandedImageBox::SetImageClipRect(float fLeft, float fTop, float fRight, float fBottom, bool bIsVertical)
	{
		if (!m_pImageInstance)
			return;

		const RECT& c_rRect = GetRect();

		float fDifLeft = (c_rRect.left < fLeft) ? -(float(fLeft - c_rRect.left) / float(GetWidth())) : 0.0f;
		float fDifTop = (c_rRect.top < fTop) ? -(float(fTop - c_rRect.top) / float(GetHeight())) : 0.0f;
		float fDifRight = (c_rRect.right > fRight) ? -(float(c_rRect.right - fRight) / float(GetWidth())) : 0.0f;
		float fDifBottom = (c_rRect.bottom > fBottom) ? -(float(c_rRect.bottom - fBottom) / float(GetHeight())) : 0.0f;

		if (bIsVertical)
			((CGraphicExpandedImageInstance*)m_pImageInstance)->SetRenderingRect(fLeft, fDifTop, fRight, fDifBottom);
		else
			((CGraphicExpandedImageInstance*)m_pImageInstance)->SetRenderingRect(fDifLeft, fDifTop, fDifRight, fDifBottom);
	}


	void CExpandedImageBox::SetRenderingMode(int iMode)
	{
		((CGraphicExpandedImageInstance*)m_pImageInstance)->SetRenderingMode(iMode);
	}

#ifdef INGAME_WIKI
	int CExpandedImageBox::GetRenderingWidth()
	{
		return CWindow::GetWidth() + m_renderingRect.right + m_renderingRect.left;
	}

	int CExpandedImageBox::GetRenderingHeight()
	{
		return CWindow::GetHeight() + m_renderingRect.bottom + m_renderingRect.top;
	}

	void CExpandedImageBox::OnSetRenderingRect()
	{
		if (!m_pImageInstance)
			return;

		((CGraphicExpandedImageInstance*)m_pImageInstance)->iSetRenderingRect(m_renderingRect.left, m_renderingRect.top, m_renderingRect.right, m_renderingRect.bottom);
	}

	void CExpandedImageBox::SetExpandedRenderingRect(float fLeftTop, float fLeftBottom, float fTopLeft, float fTopRight, float fRightTop, float fRightBottom, float fBottomLeft, float fBottomRight)
	{
		if (!m_pImageInstance)
			return;

		((CGraphicExpandedImageInstance*)m_pImageInstance)->SetExpandedRenderingRect(fLeftTop, fLeftBottom, fTopLeft, fTopRight, fRightTop, fRightBottom, fBottomLeft, fBottomRight);
	}

	void CExpandedImageBox::SetTextureRenderingRect(float fLeft, float fTop, float fRight, float fBottom)
	{
		if (!m_pImageInstance)
			return;

		((CGraphicExpandedImageInstance*)m_pImageInstance)->SetTextureRenderingRect(fLeft, fTop, fRight, fBottom);
	}

	DWORD CExpandedImageBox::GetPixelColor(DWORD x, DWORD y)
	{
		return ((CGraphicExpandedImageInstance*)m_pImageInstance)->GetPixelColor(x, y);
	}

	void CExpandedImageBox::OnUpdateRenderBox()
	{
		if (!m_pImageInstance)
			return;

		((CGraphicExpandedImageInstance*)m_pImageInstance)->SetRenderBox(m_renderBox);
	}
#endif

	void CExpandedImageBox::OnUpdate()
	{
	}
	void CExpandedImageBox::OnRender()
	{
		if (!m_pImageInstance)
			return;

		if (IsShow())
		{
			if (m_fRectCircle != 99.0f)
			{
				float fCoolTimePct = m_fRectCircle;
				if (m_bCoolTimeInvert)
					fCoolTimePct = (m_fRectCircle - CTimer::Instance().GetCurrentSecond()) / m_fRectCircle;
				((CGraphicExpandedImageInstance*)m_pImageInstance)->RenderRectCircle(fCoolTimePct, m_bCoolTimeInverse);
			}
			else if (m_fCoolTime != 0.0f)
			{
				float fCoolTimePct = (CTimer::Instance().GetCurrentSecond() - m_fCoolTimeStart) / m_fCoolTime;
				if (m_bCoolTimeInvert)
					fCoolTimePct = (m_fCoolTimeStart + m_fCoolTime - CTimer::Instance().GetCurrentSecond()) / m_fCoolTime;

				((CGraphicExpandedImageInstance*)m_pImageInstance)->RenderCoolTime(fCoolTimePct, m_bCoolTimeInverse);
			}
			else
#ifdef ENABLE_WINDOW_SLIDE_EFFECT
				((CGraphicExpandedImageInstance*)m_pImageInstance)->Render(GetParentSlidingFactor());
#else
				((CGraphicExpandedImageInstance*)m_pImageInstance)->Render();
#endif

		}

	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////

	DWORD CAniImageBox::Type()
	{
		static DWORD s_dwType = GetCRC32("CAniImageBox", strlen("CAniImageBox"));
		return (s_dwType);
	}

		struct FSetScale
	{
		float fx, fy;
		void operator () (CGraphicExpandedImageInstance* pInstance)
		{
			pInstance->SetScale(fx, fy);
		}
	};

	BOOL CAniImageBox::OnIsType(DWORD dwType)
	{
		if (CAniImageBox::Type() == dwType)
			return TRUE;

		return FALSE;
	}

	void CAniImageBox::SetScale(float fx, float fy)
	{
		FSetScale scale;
		scale.fx = fx;
		scale.fy = fy;
		for_each(m_ImageVector.begin(), m_ImageVector.end(), scale);
	}

	CAniImageBox::CAniImageBox(PyObject * ppyObject)
		:	CWindow(ppyObject),
			m_bycurDelay(0),
			m_byDelay(4),
			m_bycurIndex(0)
#ifdef ENABLE_NEW_FISHING_SYSTEM
		, m_RotationProcess(false)
#endif
	{
		m_ImageVector.clear();
	}
	CAniImageBox::~CAniImageBox()
	{
		for_each(m_ImageVector.begin(), m_ImageVector.end(), CGraphicExpandedImageInstance::DeleteExpandedImageInstance);
	}

	void CAniImageBox::SetDelay(int iDelay)
	{
		m_byDelay = iDelay;
	}
#if defined(ENABLE_ACCE_COSTUME_SYSTEM) || defined(ENABLE_CHANGELOOK_SYSTEM)
	void CAniImageBox::AppendImage(const char * c_szFileName, float r, float g, float b, float a)
#else
	void CAniImageBox::AppendImage(const char * c_szFileName)
#endif
	{
		CResource * pResource = CResourceManager::Instance().GetResourcePointer(c_szFileName);
		if (!pResource->IsType(CGraphicImage::Type()))
			return;

		CGraphicExpandedImageInstance * pImageInstance = CGraphicExpandedImageInstance::New();

		pImageInstance->SetImagePointer(static_cast<CGraphicImage*>(pResource));
#if defined(ENABLE_ACCE_COSTUME_SYSTEM) || defined(ENABLE_CHANGELOOK_SYSTEM)
		pImageInstance->SetDiffuseColor(r, g, b, a);
#endif
		if (pImageInstance->IsEmpty())
		{
			CGraphicExpandedImageInstance::Delete(pImageInstance);
			return;
		}

		m_ImageVector.push_back(pImageInstance);

		m_bycurIndex = rand() % m_ImageVector.size();
//		SetSize(pImageInstance->GetWidth(), pImageInstance->GetHeight());
//		UpdateRect();
	}


	void CAniImageBox::AppendImageScale(const char * c_szFileName, float scale_x, float scale_y)
	{
		CResource * pResource = CResourceManager::Instance().GetResourcePointer(c_szFileName);
		if (!pResource->IsType(CGraphicImage::Type()))
			return;

		CGraphicExpandedImageInstance * pImageInstance = CGraphicExpandedImageInstance::New();

		pImageInstance->SetImagePointer(static_cast<CGraphicImage*>(pResource));
		pImageInstance->SetScale(scale_x, scale_y);
		if (pImageInstance->IsEmpty())
		{
			CGraphicExpandedImageInstance::Delete(pImageInstance);
			return;
		}

		m_ImageVector.push_back(pImageInstance);

		m_bycurIndex = rand() % m_ImageVector.size();
		//		SetSize(pImageInstance->GetWidth(), pImageInstance->GetHeight());
		//		UpdateRect();
	}

#ifdef ENABLE_NEW_FISHING_SYSTEM
	void CAniImageBox::SetRotation(float r) {
		int iSize = m_ImageVector.size();
		if (iSize > 0) {
			m_RotationProcess = true;
			for (int i = 0; i < iSize; i++) {
				m_ImageVector[i]->SetRotation(r);
			}
		}

		m_RotationProcess = false;
	}
#endif

	struct FSetRenderingRect
	{
		float fLeft, fTop, fRight, fBottom;
		void operator () (CGraphicExpandedImageInstance * pInstance)
		{
			pInstance->SetRenderingRect(fLeft, fTop, fRight, fBottom);
		}
	};
	void CAniImageBox::SetRenderingRect(float fLeft, float fTop, float fRight, float fBottom)
	{
		FSetRenderingRect setRenderingRect;
		setRenderingRect.fLeft = fLeft;
		setRenderingRect.fTop = fTop;
		setRenderingRect.fRight = fRight;
		setRenderingRect.fBottom = fBottom;
		for_each(m_ImageVector.begin(), m_ImageVector.end(), setRenderingRect);
	}

	struct FSetRenderingMode
	{
		int iMode;
		void operator () (CGraphicExpandedImageInstance * pInstance)
		{
			pInstance->SetRenderingMode(iMode);
		}
	};
	void CAniImageBox::SetRenderingMode(int iMode)
	{
		FSetRenderingMode setRenderingMode;
		setRenderingMode.iMode = iMode;
		for_each(m_ImageVector.begin(), m_ImageVector.end(), setRenderingMode);
	}

	void CAniImageBox::ResetFrame()
	{
		m_bycurIndex = 0;
	}

	void CAniImageBox::OnUpdate()
	{
#ifdef ENABLE_NEW_FISHING_SYSTEM
		if (m_RotationProcess) {
			return;
		}
#endif

		++m_bycurDelay;
#ifdef ENABLE_FPS
		if (g_fGameFPS > 65)
		{
			if (m_bycurDelay < m_byDelay + (m_byDelay * ((g_fGameFPS / 60.0) - 1)))
				return;
		}
		else
#endif
		{

			if (m_bycurDelay < m_byDelay)
				return;
		}

		m_bycurDelay = 0;

		++m_bycurIndex;
		if (m_bycurIndex >= m_ImageVector.size())
		{
			m_bycurIndex = 0;

			OnEndFrame();
		}
	}
	void CAniImageBox::OnRender()
	{

#ifdef ENABLE_NEW_FISHING_SYSTEM
		if (m_RotationProcess) {
			return;
		}
#endif

		if (m_bycurIndex < m_ImageVector.size())
		{
			CGraphicExpandedImageInstance* pImage = m_ImageVector[m_bycurIndex];
#ifdef ENABLE_WINDOW_SLIDE_EFFECT
			pImage->Render(m_dScalingForce != 1.0 ? m_dScalingForce : GetParentSlidingFactor());
#else
			pImage->Render();
#endif
		}
	}

	struct FChangePosition
	{
		float fx, fy;
		void operator () (CGraphicExpandedImageInstance * pInstance)
		{
			pInstance->SetPosition(fx, fy);
		}
	};

	void CAniImageBox::OnChangePosition()
	{
		FChangePosition changePosition;
		changePosition.fx = m_rect.left;
		changePosition.fy = m_rect.top;
		for_each(m_ImageVector.begin(), m_ImageVector.end(), changePosition);
	}

	void CAniImageBox::OnEndFrame()
	{
		PyCallClassMemberFunc(m_poHandler, "OnEndFrame", BuildEmptyTuple());
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////

#ifdef ENABLE_NEW_FISHING_SYSTEM
	DWORD CFishBox::Type() {
		static DWORD s_dwType = GetCRC32("CFishBox", strlen("CFishBox"));
		return (s_dwType);
	}

	BOOL CFishBox::OnIsType(DWORD dwType) {
		if (CFishBox::Type() == dwType)
			return TRUE;

		return FALSE;
	}

	CFishBox::CFishBox(PyObject* ppyObject)
		: CWindow(ppyObject),
		m_v2SrcPos(0.0f, 0.0f),
		m_v2DstPos(0.0f, 0.0f),
		m_v2NextPos(0.0f, 0.0f),
		m_v2Direction(0.0f, 0.0f),
		m_v2NextDistance(0.0f, 0.0f),
		m_fDistance(0.0f),
		m_fMoveSpeed(3.0f),
		m_bIsMove(false),
		m_left(false),
		m_right(false),
		m_lastRandom(0),
		m_direction(0) {
	}

	CFishBox::~CFishBox() {
	}

	void CFishBox::OnEndMove() {
		PyCallClassMemberFunc(m_poHandler, "OnEndMove", BuildEmptyTuple());
		SetRandomPosition();
	}

	void CFishBox::SetRandomPosition() {
		m_v2Direction.x = 0.0f;
		m_v2Direction.y = 0.0f;

		if (m_left == false) {
			m_v2DstPos.x = listFishLeft[m_lastRandom][0];
			m_v2DstPos.y = listFishLeft[m_lastRandom][1];
			m_direction = listFishLeft[m_lastRandom][2];
		}
		else {
			m_v2DstPos.x = listFishRight[m_lastRandom][0];
			m_v2DstPos.y = listFishRight[m_lastRandom][1];
			m_direction = listFishRight[m_lastRandom][2];
		}

		D3DXVec2Subtract(&m_v2Direction, &m_v2DstPos, &m_v2SrcPos);
		m_fDistance = (m_v2Direction.x * m_v2Direction.x + m_v2Direction.y * m_v2Direction.y);
		D3DXVec2Normalize(&m_v2Direction, &m_v2Direction);

		m_bIsMove = true;
	}

	void CFishBox::MoveStart() {
		m_lastRandom = 0;
		m_bIsMove = false;
		m_left = false;
		m_right = false;
		m_v2SrcPos.x = m_rect.left;
		m_v2SrcPos.y = m_rect.top;
		SetRandomPosition();
	}

	void CFishBox::MoveStop() {
		m_bIsMove = false;
	}

	bool CFishBox::GetMove() {
		return m_bIsMove;
	}

	void CFishBox::GetPosition(int* x, int* y) {
		*x = m_v2NextPos.x;
		*y = m_v2NextPos.y;
	}

	void CFishBox::OnUpdate() {
		if (IsShow() && GetMove()) {
			D3DXVec2Add(&m_v2NextPos, &m_v2NextPos, &(m_v2Direction * m_fMoveSpeed));
			D3DXVec2Subtract(&m_v2NextDistance, &m_v2NextPos, &m_v2SrcPos);

			float fNextDistance = (m_v2NextDistance.x * m_v2NextDistance.x + m_v2NextDistance.y * m_v2NextDistance.y);
			if (fNextDistance >= m_fDistance)
			{
				if (m_left == false) {
					m_left = true;
				}
				else if (m_right == false) {
					m_right = true;

					int r = 0;
					bool same = true;
					while (same) {
						r = rand() % MAX_FISHING_WAYS;
						if (r != m_lastRandom) {
							same = false;
							m_lastRandom = r;
						}
					}

					m_left = false;
					m_right = false;
				}

				m_v2SrcPos = m_v2NextPos;
				MoveStop();
				OnEndMove();
				SetRandomPosition();
				return;
			}

			SetPosition(m_v2NextPos.x, m_v2NextPos.y);
			PyCallClassMemberFunc(m_poHandler, "OnRotation", Py_BuildValue("(f)", m_direction));
		}
	}
#endif

	CButton::CButton(PyObject * ppyObject)
		:	CWindow(ppyObject),
			m_pcurVisual(NULL),
			m_bEnable(TRUE),
			m_isPressed(FALSE),
			m_isFlash(FALSE), m_isFlashEx(FALSE)
	{
		CWindow::AddFlag(CWindow::FLAG_NOT_CAPTURE);
	}
	CButton::~CButton() = default;

	BOOL CButton::SetUpVisual(const char * c_szFileName)
	{
		CResource * pResource = CResourceManager::Instance().GetResourcePointer(c_szFileName);
		if (!pResource->IsType(CGraphicImage::Type()))
			return FALSE;

		m_upVisual.SetImagePointer(static_cast<CGraphicImage*>(pResource));
		if (m_upVisual.IsEmpty())
			return FALSE;

		SetSize(m_upVisual.GetWidth(), m_upVisual.GetHeight());
		//
		SetCurrentVisual(&m_upVisual);
		//

		return TRUE;
	}
	BOOL CButton::SetOverVisual(const char * c_szFileName)
	{
		CResource * pResource = CResourceManager::Instance().GetResourcePointer(c_szFileName);
		if (!pResource->IsType(CGraphicImage::Type()))
			return FALSE;

		m_overVisual.SetImagePointer(static_cast<CGraphicImage*>(pResource));
		if (m_overVisual.IsEmpty())
			return FALSE;

		SetSize(m_overVisual.GetWidth(), m_overVisual.GetHeight());

		return TRUE;
	}
	BOOL CButton::SetDownVisual(const char * c_szFileName)
	{
		CResource * pResource = CResourceManager::Instance().GetResourcePointer(c_szFileName);
		if (!pResource->IsType(CGraphicImage::Type()))
			return FALSE;

		m_downVisual.SetImagePointer(static_cast<CGraphicImage*>(pResource));
		if (m_downVisual.IsEmpty())
			return FALSE;

		SetSize(m_downVisual.GetWidth(), m_downVisual.GetHeight());

		return TRUE;
	}
	BOOL CButton::SetDisableVisual(const char * c_szFileName)
	{
		CResource * pResource = CResourceManager::Instance().GetResourcePointer(c_szFileName);
		if (!pResource->IsType(CGraphicImage::Type()))
			return FALSE;

		m_disableVisual.SetImagePointer(static_cast<CGraphicImage*>(pResource));
		if (m_downVisual.IsEmpty())
			return FALSE;

		SetSize(m_disableVisual.GetWidth(), m_disableVisual.GetHeight());

		return TRUE;
	}

	const char * CButton::GetUpVisualFileName()

	{
		return m_upVisual.GetGraphicImagePointer()->GetFileName();
	}

	const char * CButton::GetOverVisualFileName()
	{
		return m_overVisual.GetGraphicImagePointer()->GetFileName();
	}
	const char * CButton::GetDownVisualFileName()
	{
		return m_downVisual.GetGraphicImagePointer()->GetFileName();
	}

	void CButton::SetButtonColor(float r, float g, float b, float alpha)
	{
		if (!m_upVisual.IsEmpty())
			m_upVisual.SetDiffuseColor(r, g, b, alpha);
		if (!m_overVisual.IsEmpty())
			m_overVisual.SetDiffuseColor(r, g, b, alpha);
		if (!m_downVisual.IsEmpty())
			m_downVisual.SetDiffuseColor(r, g, b, alpha);
	}

	void CButton::Flash()
	{
		m_isFlash = TRUE;
	}

	void CButton::EnableFlash()
	{
		m_isFlash = true;
		if (!m_overVisual.IsEmpty())
			SetCurrentVisual(&m_overVisual);
	}

	void CButton::DisableFlash()
	{
		m_isFlash = false;
		if (!m_upVisual.IsEmpty())
			SetCurrentVisual(&m_upVisual);
	}


	void CButton::Enable()
	{
		if (!IsDisable()) {
			SetUp();
			m_bEnable = TRUE;
		}
	}

	void CButton::FlashEx()
	{
		m_isFlashEx = TRUE;
	}

	void CButton::Disable()
	{
		if (IsDisable()) {
			m_bEnable = FALSE;
			if (!m_disableVisual.IsEmpty())
				SetCurrentVisual(&m_disableVisual);
		}
	}

	BOOL CButton::IsDisable()
	{
		return m_bEnable;
	}

	void CButton::SetUp()
	{
		SetCurrentVisual(&m_upVisual);
		m_isPressed = FALSE;
	}
	void CButton::Up()
	{
		if (IsIn())
			SetCurrentVisual(&m_overVisual);
		else
			SetCurrentVisual(&m_upVisual);

		PyCallClassMemberFunc(m_poHandler, "CallEvent", BuildEmptyTuple());
	}
	void CButton::Over()
	{
		SetCurrentVisual(&m_overVisual);
	}
	void CButton::Down()
	{
		m_isPressed = TRUE;
		SetCurrentVisual(&m_downVisual);
		PyCallClassMemberFunc(m_poHandler, "DownEvent", BuildEmptyTuple());
	}

	void CButton::OnUpdate()
	{
	}
	void CButton::OnRender()
	{
		if (!IsShow())
			return;

		if (m_pcurVisual)
		{
			if (m_isFlash)
			{
				if ((int(timeGetTime() / 500) % 2) && !IsIn() && !IsPressed())
				{
					return;
					//if (!m_overVisual.IsEmpty())
					//	SetCurrentVisual(&m_overVisual);
				}
				else
				{
					if (IsIn() && !IsPressed())
					{
						if (!m_overVisual.IsEmpty())
							SetCurrentVisual(&m_overVisual);
				}
					else if (IsPressed())
					{
						if (!m_downVisual.IsEmpty())
							SetCurrentVisual(&m_downVisual);
					}
					else
					{
						if (!m_upVisual.IsEmpty())
							SetCurrentVisual(&m_upVisual);
					}
			}
		}

			if (m_isFlashEx && !IsIn())
			{
				if (int(timeGetTime() / 500) % 2)
					Over();
				if (int(timeGetTime() / (500 - 15)) % 2)
					Down();
			}

#ifdef ENABLE_WINDOW_SLIDE_EFFECT
			m_pcurVisual->Render(m_dScalingForce != 1.0 ? m_dScalingForce : GetParentSlidingFactor());
#else
			m_pcurVisual->Render();
#endif
	}

		PyCallClassMemberFunc(m_poHandler, "OnRender", BuildEmptyTuple());
	}

	void CButton::OnChangePosition()
	{
		if (m_pcurVisual)
			m_pcurVisual->SetPosition(m_rect.left, m_rect.top);
	}

	BOOL CButton::OnMouseLeftButtonDown()
	{
		if (!IsEnable())
			return TRUE;

		m_isPressed = TRUE;
		Down();

		return TRUE;
	}
	BOOL CButton::OnMouseLeftButtonDoubleClick()
	{
		if (!IsEnable())
			return TRUE;

		OnMouseLeftButtonDown();

		return TRUE;
	}
	BOOL CButton::OnMouseLeftButtonUp()
	{
		if (!IsEnable())
			return TRUE;
		if (!IsPressed())
			return TRUE;

		m_isPressed = FALSE;
		Up();

		return TRUE;
	}
	void CButton::OnMouseOverIn()
	{
		if (!IsEnable())
			return;

		Over();
		PyCallClassMemberFunc(m_poHandler, "ShowToolTip", BuildEmptyTuple());
	}
	void CButton::OnMouseOverOut()
	{
		if (!IsEnable())
			return;

		SetUp();
		PyCallClassMemberFunc(m_poHandler, "HideToolTip", BuildEmptyTuple());
	}

#ifdef ENABLE_NEW_GAMEOPTION
	DWORD CButton::Type()
	{
		static DWORD s_dwType = GetCRC32("CButton", strlen("CButton"));
		return (s_dwType);
	}
	BOOL CButton::OnIsType(DWORD dwType)
	{
		if (CButton::Type() == dwType)
			return TRUE;
		return FALSE;
	}
	void CButton::SetRenderingRect(float fLeft, float fTop, float fRight, float fBottom)
	{
		m_upVisual.SetRenderingRect(fLeft, fTop, fRight, fBottom);
		m_overVisual.SetRenderingRect(fLeft, fTop, fRight, fBottom);
		m_downVisual.SetRenderingRect(fLeft, fTop, fRight, fBottom);
	}
#endif

#ifdef INGAME_WIKI
	void CButton::SetCurrentVisual(CGraphicExpandedImageInstance* pVisual)
#else
	void CButton::SetCurrentVisual(CGraphicImageInstance* pVisual)
#endif
	{
		m_pcurVisual = pVisual;
		m_pcurVisual->SetPosition(m_rect.left, m_rect.top);
#ifdef INGAME_WIKI
		if (m_pcurVisual == &m_upVisual)
			PyCallClassMemberFunc(m_poHandler, "OnSetUpVisual", BuildEmptyTuple());
		else if (m_pcurVisual == &m_overVisual)
			PyCallClassMemberFunc(m_poHandler, "OnSetOverVisual", BuildEmptyTuple());
		else if (m_pcurVisual == &m_downVisual)
			PyCallClassMemberFunc(m_poHandler, "OnSetDownVisual", BuildEmptyTuple());
		else if (m_pcurVisual == &m_disableVisual)
			PyCallClassMemberFunc(m_poHandler, "OnSetDisableVisual", BuildEmptyTuple());
#endif
	}

	BOOL CButton::IsEnable()
	{
		return m_bEnable;
	}

	BOOL CButton::IsPressed()
	{
		return m_isPressed;
	}

#ifdef INGAME_WIKI
	void CButton::OnSetRenderingRect()
	{
		m_upVisual.iSetRenderingRect(m_renderingRect.left, m_renderingRect.top, m_renderingRect.right, m_renderingRect.bottom);
		m_overVisual.iSetRenderingRect(m_renderingRect.left, m_renderingRect.top, m_renderingRect.right, m_renderingRect.bottom);
		m_downVisual.iSetRenderingRect(m_renderingRect.left, m_renderingRect.top, m_renderingRect.right, m_renderingRect.bottom);
		m_disableVisual.iSetRenderingRect(m_renderingRect.left, m_renderingRect.top, m_renderingRect.right, m_renderingRect.bottom);
	}
#endif

	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////

	CRadioButton::CRadioButton(PyObject* ppyObject) : CButton(ppyObject) {}
	CRadioButton::~CRadioButton() = default;

#ifdef ENABLE_NEW_GAMEOPTION
	DWORD CRadioButton::Type()
	{
		static DWORD s_dwType = GetCRC32("CRadioButton", strlen("CRadioButton"));
		return (s_dwType);
	}
	BOOL CRadioButton::OnIsType(DWORD dwType)
	{
		if (CRadioButton::Type() == dwType)
			return TRUE;
		return FALSE;
	}
#endif

	BOOL CRadioButton::OnMouseLeftButtonDown()
	{
		if (!IsEnable())
			return TRUE;

		if (!m_isPressed)
		{
			Down();
			PyCallClassMemberFunc(m_poHandler, "CallEvent", BuildEmptyTuple());
		}

		return TRUE;
	}
	BOOL CRadioButton::OnMouseLeftButtonUp()
	{
		return TRUE;
	}
	void CRadioButton::OnMouseOverIn()
	{
		if (!IsEnable())
			return;

		if (!m_isPressed)
		{
			SetCurrentVisual(&m_overVisual);
		}

		PyCallClassMemberFunc(m_poHandler, "ShowToolTip", BuildEmptyTuple());
	}
	void CRadioButton::OnMouseOverOut()
	{
		if (!IsEnable())
			return;

		if (!m_isPressed)
		{
			SetCurrentVisual(&m_upVisual);
		}

		PyCallClassMemberFunc(m_poHandler, "HideToolTip", BuildEmptyTuple());
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////

	CToggleButton::CToggleButton(PyObject* ppyObject) : CButton(ppyObject) {}
	CToggleButton::~CToggleButton() = default;

#ifdef ENABLE_NEW_GAMEOPTION
	DWORD CToggleButton::Type()
	{
		static DWORD s_dwType = GetCRC32("CToggleButton", strlen("CToggleButton"));
		return (s_dwType);
	}
	BOOL CToggleButton::OnIsType(DWORD dwType)
	{
		if (CToggleButton::Type() == dwType)
			return TRUE;
		return FALSE;
	}
#endif

	BOOL CToggleButton::OnMouseLeftButtonDown()
	{
		if (!IsEnable())
			return TRUE;

		if (m_isPressed)
		{
			SetUp();
			if (IsIn())
				SetCurrentVisual(&m_overVisual);
			else
				SetCurrentVisual(&m_upVisual);
			PyCallClassMemberFunc(m_poHandler, "OnToggleUp", BuildEmptyTuple());
		}
		else
		{
			Down();
			PyCallClassMemberFunc(m_poHandler, "OnToggleDown", BuildEmptyTuple());
		}

		return TRUE;
	}
	BOOL CToggleButton::OnMouseLeftButtonUp()
	{
		return TRUE;
	}

	void CToggleButton::OnMouseOverIn()
	{
		if (!IsEnable())
			return;

		if (!m_isPressed)
		{
			SetCurrentVisual(&m_overVisual);
		}

		PyCallClassMemberFunc(m_poHandler, "ShowToolTip", BuildEmptyTuple());
	}
	void CToggleButton::OnMouseOverOut()
	{
		if (!IsEnable())
			return;

		if (!m_isPressed)
		{
			SetCurrentVisual(&m_upVisual);
		}

		PyCallClassMemberFunc(m_poHandler, "HideToolTip", BuildEmptyTuple());
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////

	CDragButton::CDragButton(PyObject * ppyObject) : CButton(ppyObject)
	{
		CWindow::RemoveFlag(CWindow::FLAG_NOT_CAPTURE);
		m_restrictArea.left = 0;
		m_restrictArea.top = 0;
		m_restrictArea.right = CWindowManager::Instance().GetScreenWidth();
		m_restrictArea.bottom = CWindowManager::Instance().GetScreenHeight();
	}
	CDragButton::~CDragButton() = default;

	void CDragButton::SetRestrictMovementArea(int ix, int iy, int iwidth, int iheight)
	{
		m_restrictArea.left = ix;
		m_restrictArea.top = iy;
		m_restrictArea.right = ix + iwidth;
		m_restrictArea.bottom = iy + iheight;
	}

	void CDragButton::OnChangePosition()
	{
		m_x = max(m_x, m_restrictArea.left);
		m_y = max(m_y, m_restrictArea.top);
		m_x = min(m_x, max(0, m_restrictArea.right - m_lWidth));
		m_y = min(m_y, max(0, m_restrictArea.bottom - m_lHeight));

		m_rect.left = m_x;
		m_rect.top = m_y;

		if (m_pParent)
		{
			const RECT & c_rRect = m_pParent->GetRect();
			m_rect.left += c_rRect.left;
			m_rect.top += c_rRect.top;
		}

		m_rect.right = m_rect.left + m_lWidth;
		m_rect.bottom = m_rect.top + m_lHeight;

		std::for_each(m_pChildList.begin(), m_pChildList.end(), std::mem_fn(&CWindow::UpdateRect));

		if (m_pcurVisual)
			m_pcurVisual->SetPosition(m_rect.left, m_rect.top);

		if (IsPressed())
			PyCallClassMemberFunc(m_poHandler, "OnMove", BuildEmptyTuple());
	}

	void CDragButton::OnMouseOverIn()
	{
		if (!IsEnable())

			return;

		CButton::OnMouseOverIn();
		PyCallClassMemberFunc(m_poHandler, "OnMouseOverIn", BuildEmptyTuple());
	}

	void CDragButton::OnMouseOverOut()
	{
		if (!IsEnable())
			return;

		CButton::OnMouseOverIn();
		PyCallClassMemberFunc(m_poHandler, "OnMouseOverOut", BuildEmptyTuple());
	}
};
