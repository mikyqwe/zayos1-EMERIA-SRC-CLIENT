#pragma once

#include "PythonWindow.h"
#include "../UserInterface/Locale_inc.h"
namespace UI
{
	enum
	{
		ITEM_WIDTH = 32,
		ITEM_HEIGHT = 32,

		SLOT_NUMBER_NONE = 0xffffffff,
	};

	enum ESlotStyle
	{
		SLOT_STYLE_NONE,
		SLOT_STYLE_PICK_UP,
		SLOT_STYLE_SELECT,
	};

	enum ESlotState
	{
		SLOT_STATE_LOCK		= (1 << 0),
		SLOT_STATE_CANT_USE	= (1 << 1),
		SLOT_STATE_DISABLE	= (1 << 2),
		SLOT_STATE_ALWAYS_RENDER_COVER = (1 << 3),			// 현재 Cover 버튼은 슬롯에 무언가 들어와 있을 때에만 렌더링 하는데, 이 flag가 있으면 빈 슬롯이어도 커버 렌더링
#ifdef WJ_ENABLE_TRADABLE_ICON
		SLOT_STATE_CANT_MOUSE_EVENT = (1 << 4),
		SLOT_STATE_UNUSABLE = (1 << 5),
#endif
	};

	class CSlotWindow : public CWindow
	{
		public:
			static DWORD Type();

		public:
			class CSlotButton;
			class CCoverButton;
#ifdef GLOW_EFFECT
			class CHighLightImage;
#endif
			class CCoolTimeFinishEffect;

			friend class CSlotButton;
			friend class CCoverButton;

			typedef struct SSlot
			{
				DWORD	dwState;
				DWORD	dwSlotNumber;
				DWORD	dwCenterSlotNumber;		// NOTE : 사이즈가 큰 아이템의 경우 아이템의 실제 위치 번호
				DWORD	dwItemIndex;			// NOTE : 여기서 사용되는 Item이라는 단어는 좁은 개념의 것이 아닌,
				BOOL	isItem;					//        "슬롯의 내용물"이라는 포괄적인 개념어. 더 좋은 것이 있을까? - [levites]
				DWORD	dwItemID;
				
				// CoolTime
				float	fCoolTime;
				float	fStartCoolTime;

				// Toggle
				BOOL	bActive;
#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
				BOOL	bActiveOfflineShop;
#endif
				int		ixPosition;
				int		iyPosition;

				int		ixCellSize;
				int		iyCellSize;

				BYTE	byxPlacedItemSize;
				BYTE	byyPlacedItemSize;

				CGraphicImageInstance * pInstance;
				CGraphicImageInstance * pInstanceLevel;
				CNumberLine * pNumberLine;

				bool	bRenderBaseSlotImage;
				CCoverButton * pCoverButton;
				CSlotButton * pSlotButton;
				CImageBox * pSignImage;
				CAniImageBox * pFinishCoolTimeEffect;
#if defined(ENABLE_ACCE_COSTUME_SYSTEM) || defined(ENABLE_CHANGELOOK_SYSTEM)
				CAniImageBox*	pActiveSlotEffect[3];
#endif
#ifdef GLOW_EFFECT
				BOOL bEnableHighLight;
				CHighLightImage* pHighLightImage;
#endif
			} TSlot;
			typedef std::list<TSlot> TSlotList;
			typedef TSlotList::iterator TSlotListIterator;
			typedef struct SStoreCoolDown { float fCoolTime; float fElapsedTime; bool bActive; };
		public:
			CSlotWindow(PyObject * ppyObject);
			virtual ~CSlotWindow();

			void Destroy();
			void SetSlotLevelImage(DWORD dwIndex, CGraphicImage * levelImage);

			// Manage Slot
			void SetSlotType(DWORD dwType);
			void SetSlotStyle(DWORD dwStyle);

			void AppendSlot(DWORD dwIndex, int ixPosition, int iyPosition, int ixCellSize, int iyCellSize);
			void SetCoverButton(DWORD dwIndex, const char * c_szUpImageName, const char * c_szOverImageName, const char * c_szDownImageName, const char * c_szDisableImageName, BOOL bLeftButtonEnable, BOOL bRightButtonEnable);
			void SetSlotBaseImage(const char * c_szFileName, float fr, float fg, float fb, float fa);
			void SetSlotBaseImageScale(const char * c_szFileName, float fr, float fg, float fb, float fa, float sx, float sy);
			void SetSlotScale(DWORD dwIndex, DWORD dwVirtualNumber, BYTE byWidth, BYTE byHeight, CGraphicImage * pImage, float sx, float sy, D3DXCOLOR& diffuseColor);			
			void AppendSlotButton(const char * c_szUpImageName, const char * c_szOverImageName, const char * c_szDownImageName);
			void AppendRequirementSignImage(const char * c_szImageName);

			void EnableCoverButton(DWORD dwIndex);
			void DisableCoverButton(DWORD dwIndex);
			void SetAlwaysRenderCoverButton(DWORD dwIndex, bool bAlwaysRender = false);
#ifdef GLOW_EFFECT
			void AppendHighLightImage(DWORD dwIndex, const char* c_sImageName, float fAlphaSpeed = 0.0f, float fRotationSpeed = 0.0f, float fCurAlpha = 1.0f, DWORD dwDiffuse = DWORD(-1));
			void EnableHighLightImage(DWORD dwIndex);
			void DisableHighLightImage(DWORD dwIndex);
#endif
			void ShowSlotBaseImage(DWORD dwIndex);
			void HideSlotBaseImage(DWORD dwIndex);
			BOOL IsDisableCoverButton(DWORD dwIndex);
			BOOL HasSlot(DWORD dwIndex);

			void ClearAllSlot();
			void ClearSlot(DWORD dwIndex);
			void SetSlot(DWORD dwIndex, DWORD dwVirtualNumber, BYTE byWidth, BYTE byHeight, CGraphicImage * pImage, D3DXCOLOR& diffuseColor);
			void SetSlotCount(DWORD dwIndex, DWORD dwCount);
			void SetSlotCountNew(DWORD dwIndex, DWORD dwGrade, DWORD dwCount);
			void SetSlotCoolTime(DWORD dwIndex, float fCoolTime, float fElapsedTime = 0.0f);
			void StoreSlotCoolTime(DWORD dwKey, DWORD dwSlotIndex, float fCoolTime, float fElapsedTime = .0f);
			void RestoreSlotCoolTime(DWORD dwKey);
			void ActivateSlot(DWORD dwIndex);
			void DeactivateSlot(DWORD dwIndex);
			void RefreshSlot();
#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
			void ActivateOfflineShopSlot(DWORD dwIndex);
			void DeactivateOfflineShopSlot(DWORD dwIndex);
#endif
			DWORD GetSlotCount();

			void LockSlot(DWORD dwIndex);
			void UnlockSlot(DWORD dwIndex);
			BOOL IsLockSlot(DWORD dwIndex);
			void SetCantUseSlot(DWORD dwIndex);
			void SetUseSlot(DWORD dwIndex);
			BOOL IsCantUseSlot(DWORD dwIndex);
			void EnableSlot(DWORD dwIndex);
			void DisableSlot(DWORD dwIndex);
			BOOL IsEnableSlot(DWORD dwIndex);
#ifdef WJ_ENABLE_TRADABLE_ICON
			void SetCanMouseEventSlot(DWORD dwIndex);
			void SetCantMouseEventSlot(DWORD dwIndex);
			void SetUsableSlotOnTopWnd(DWORD dwIndex);
			void SetUnusableSlotOnTopWnd(DWORD dwIndex);
#endif
			// Select
			void ClearSelected();
			void SelectSlot(DWORD dwSelectingIndex);
			BOOL isSelectedSlot(DWORD dwIndex);
			DWORD GetSelectedSlotCount();
			DWORD GetSelectedSlotNumber(DWORD dwIndex);

			// Slot Button
			void ShowSlotButton(DWORD dwSlotNumber);
			void HideAllSlotButton();
			void OnPressedSlotButton(DWORD dwType, DWORD dwSlotNumber, BOOL isLeft = TRUE);

			// Requirement Sign
			void ShowRequirementSign(DWORD dwSlotNumber);
			void HideRequirementSign(DWORD dwSlotNumber);

			// ToolTip
			BOOL OnOverInItem(DWORD dwSlotNumber);
			void OnOverOutItem();

#ifdef GLOW_EFFECT
			BOOL OnOverIn(DWORD dwSlotNumber);
			void OnOverOut();
#endif

			// For Usable Item
			void SetUseMode(BOOL bFlag);
			void SetUsableItem(BOOL bFlag);
#ifdef ENABLE_SWAPITEM_SYSTEM
			void SetSwapItem(BOOL bFlag);
#endif

#ifdef ENABLE_SWAPITEM_SYSTEM
			BOOL m_isSwapItem;
#endif

			// CallBack
			void ReserveDestroyCoolTimeFinishEffect(DWORD dwSlotIndex);
			void ActivateSlotEffect(DWORD dwSlotIndex, float r, float g, float b, float a);
			void DeactivateSlotEffect(DWORD dwSlotIndex);

			void SetSlotID(DWORD dwIndex, DWORD dwID);
#if defined(ENABLE_ACCE_COSTUME_SYSTEM) || defined(ENABLE_CHANGELOOK_SYSTEM)
			void	ActivateEffect(DWORD dwSlotIndex, float r, float g, float b, float a);
			void	DeactivateEffect(DWORD dwSlotIndex);
#endif
			
		protected:
			void __Initialize();
			void __CreateToggleSlotImage();
			void __CreateSlotEnableEffect();
			void __CreateFinishCoolTimeEffect(TSlot * pSlot);
			void __CreateBaseImageScale(const char * c_szFileName, float fr, float fg, float fb, float fa, float sx, float sy);
			void __CreateBaseImage(const char * c_szFileName, float fr, float fg, float fb, float fa);

			void __DestroyToggleSlotImage();
			void __DestroySlotEnableEffect();
			void __DestroyFinishCoolTimeEffect(TSlot * pSlot);
			void __DestroyBaseImage();

			// Event
			void OnUpdate();
			void OnRender();
			BOOL OnMouseLeftButtonDown();
			BOOL OnMouseLeftButtonUp();
			BOOL OnMouseRightButtonDown();
			BOOL OnMouseLeftButtonDoubleClick();
			void OnMouseOverOut();
			void OnMouseOver();
			void RenderSlotBaseImage();
			void RenderLockedSlot();
			virtual void OnRenderPickingSlot();
			virtual void OnRenderSelectedSlot();

			// Select
			void OnSelectEmptySlot(int iSlotNumber);
			void OnSelectItemSlot(int iSlotNumber);
			void OnUnselectEmptySlot(int iSlotNumber);
			void OnUnselectItemSlot(int iSlotNumber);
			void OnUseSlot();

			// Manage Slot
			BOOL GetSlotPointer(DWORD dwIndex, TSlot ** ppSlot);
			BOOL GetSelectedSlotPointer(TSlot ** ppSlot);
			virtual BOOL GetPickedSlotPointer(TSlot ** ppSlot);
			void ClearSlot(TSlot * pSlot);
			virtual void OnRefreshSlot();

			// ETC
			BOOL OnIsType(DWORD dwType);

		protected:
			DWORD m_dwSlotType;
			DWORD m_dwSlotStyle;
			std::list<DWORD> m_dwSelectedSlotIndexList;
			TSlotList m_SlotList;
#ifdef GLOW_EFFECT
			DWORD m_dwOverInSlotNumber;
#endif
			DWORD m_dwToolTipSlotNumber;
			std::map<DWORD, std::map<DWORD, SStoreCoolDown>>	m_CoolDownStore;
			BOOL m_isUseMode;
			BOOL m_isUsableItem;

			CGraphicImageInstance * m_pBaseImageInstance;
			CImageBox * m_pToggleSlotImage;
#ifdef ENABLE_HIGHLIGHT_SYSTEM
			CAniImageBox * m_apSlotActiveEffect[3];
#else
			CAniImageBox * m_pSlotActiveEffect;
#endif
#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
			CAniImageBox* m_apOfflineShopSlotActiveEffect[3];
#endif
#ifdef ENABLE_HIGHLIGHT_SYSTEM
			CAniImageBox * m_pSlotActiveEffect;         //COMMENTANTO ORA PER FIXARE LUCCICHIO TEST
#endif
			std::deque<DWORD> m_ReserveDestroyEffectDeque;
	};
};