#pragma once
#include <cstdint>
class CPythonSystem : public CSingleton<CPythonSystem>
{
	public:
		enum EWindow
		{
			WINDOW_STATUS,
			WINDOW_INVENTORY,
			WINDOW_ABILITY,
			WINDOW_SOCIETY,
			WINDOW_JOURNAL,
			WINDOW_COMMAND,

			WINDOW_QUICK,
			WINDOW_GAUGE,
			WINDOW_MINIMAP,
			WINDOW_CHAT,

			WINDOW_MAX_NUM,
		};

		enum
		{
			FREQUENCY_MAX_NUM  = 30,
			RESOLUTION_MAX_NUM = 100
		};
#ifdef ULTIMATE_PICKUP
		enum EPickupFilter
		{
			PICKUP_FILTER_WEAPON = (1 << 0),
			PICKUP_FILTER_ARMOR = (1 << 1),
			PICKUP_FILTER_EAR = (1 << 2),
			PICKUP_FILTER_NECK = (1 << 3),
			PICKUP_FILTER_FOOTS = (1 << 4),
			PICKUP_FILTER_SHIELD = (1 << 5),
			PICKUP_FILTER_HEAD = (1 << 6),
			PICKUP_FILTER_WRIST = (1 << 7),
			PICKUP_FILTER_BOOK = (1 << 8),
			PICKUP_FILTER_STONE = (1 << 9),
			PICKUP_FILTER_ETC = (1 << 10),
		};
#endif

		typedef struct SResolution
		{
			DWORD	width;
			DWORD	height;
			DWORD	bpp;		// bits per pixel (high-color = 16bpp, true-color = 32bpp)

			DWORD	frequency[20];
			BYTE	frequency_count;
		} TResolution;

		typedef struct SWindowStatus
		{
			int		isVisible;
			int		isMinimized;

			int		ixPosition;
			int		iyPosition;
			int		iHeight;
		} TWindowStatus;

		typedef struct SConfig
		{
			DWORD			width;
			DWORD			height;
			DWORD			bpp;
			DWORD			frequency;

			bool			is_software_cursor;
			bool			is_object_culling;
			int				iDistance;
			int				iShadowLevel;

			FLOAT			music_volume;
			BYTE			voice_volume;

			int				gamma;

			int				isSaveID;
			char			SaveID[20];
			char			TransLangKey[5];

			bool			bWindowed;
			bool			bDecompressDDS;
			bool			bNoSoundCard;
			bool			bUseDefaultIME;
			BYTE			bSoftwareTiling;
#ifdef ENABLE_REFINE_RENEWAL		
			bool			bRefineStatus;
			bool			bRefineStatusFlag;
#endif	
			bool			bViewChat;
			int				bAlwaysShowName;
			bool			bShowDamage;
#ifdef ENABLE_FPS
			BYTE				iFps;
#endif
			bool			bShowSalesText;
#ifdef ENABLE_DOG_MODE
			bool			bDogMode;
#endif
#ifdef ENABLE_REMEMBER_LAST_SPLIT
			int				iRememberSplit;
#endif
#ifdef ENABLE_CHAT_COLOR_SYSTEM
			bool			bChatColor;
#endif
#ifdef ENABLE_ENB_MODE
			bool bENBModeStatus;
#endif
#if defined(__BL_HIDE_EFFECT__)
			bool			bBuffEffect;
			bool			bSkillEffect;
#endif
#ifdef ENABLE_HIDE_PET
			bool			bHidePets;
#endif
#ifdef ENABLE_HIDE_MOUNT
			bool			bHideMounts;
#endif
#ifdef ENABLE_HIDE_SHOPS
			bool			bHideShops;
#endif
#ifdef ENABLE_SHOPNAMES_RANGE
			float	shopnames_range;
#endif
#ifdef ENABLE_FOG_FIX
			bool			bFogMode;
#endif
#if defined(WJ_SHOW_MOB_INFO) && defined(ENABLE_SHOW_MOBAIFLAG)
			bool			bShowMobAIFlag;
#endif
#if defined(WJ_SHOW_MOB_INFO) && defined(ENABLE_SHOW_MOBLEVEL)
			bool			bShowMobLevel;
#endif
#ifdef ULTIMATE_PICKUP
			bool			bPickupMode;
			bool			bPickupAutoMode;
			DWORD			bPickupFilter;
#endif
#if defined(ENABLE_FOV_OPTION)
			float fFOV;
#endif
		} TConfig;

	public:
		CPythonSystem();
		virtual ~CPythonSystem();

		void Clear();
		void SetInterfaceHandler(PyObject * poHandler);
		void DestroyInterfaceHandler();

		// Config
		void							SetDefaultConfig();
		bool							LoadConfig();
		bool							SaveConfig();
		void							ApplyConfig();
		void							SetConfig(TConfig * set_config);
		TConfig *						GetConfig();
		void							ChangeSystem();

		// Interface
		bool							LoadInterfaceStatus();
		void							SaveInterfaceStatus();
		bool							isInterfaceConfig();
		const TWindowStatus &			GetWindowStatusReference(int iIndex);
#ifdef ENABLE_FPS
		void							SetFPS(BYTE iFlag);
		BYTE								GetFPS() { return m_Config.iFps; }
#endif
		DWORD							GetWidth();
		DWORD							GetHeight();
		DWORD							GetBPP();
		DWORD							GetFrequency();
		bool							IsSoftwareCursor();
		bool							IsWindowed();
		bool							IsViewChat();
		int								IsAlwaysShowName();
		bool							IsShowDamage();
		bool							IsShowSalesText();
		bool							IsUseDefaultIME();
#ifdef ENABLE_REFINE_RENEWAL	
		bool							IsRefineStatusShow();
		void							SetRefineStatus(int iFlag);
#endif
		bool							IsNoSoundCard();
		bool							IsAutoTiling();
		bool							IsSoftwareTiling();
		void							SetSoftwareTiling(bool isEnable);
		void							SetViewChatFlag(int iFlag);
		void							SetAlwaysShowNameFlag(int iFlag);
		void							SetShowDamageFlag(int iFlag);
		void							SetShowSalesTextFlag(int iFlag);
#ifdef ENABLE_DOG_MODE
		void							SetDogMode(int iFlag);
		bool							GetDogMode();
#endif
#if defined(ENABLE_CHAT_COLOR_SYSTEM)
		void							SetChatColor(int iFlag);
		bool							GetChatColor();
#endif
#ifdef ENABLE_ENB_MODE
		bool							IsENBModeStatus();
		void							SetENBModeStatusFlag(int iFlag);
#endif
#if defined(__BL_HIDE_EFFECT__)
		bool							GetBLEffectOption(std::uint8_t idx);
		void							SetBLEffectOption(std::uint8_t idx, bool ret);
#endif
#ifdef ENABLE_HIDE_PET
		bool							IsHidePets()									{ return m_Config.bHidePets; }
		void							SetHidePets(bool iFlag)							{ m_Config.bHidePets = iFlag; }
#endif
#ifdef ENABLE_HIDE_MOUNT
		bool							IsHideMounts()										{ return m_Config.bHideMounts; }
		void							SetHideMounts(bool iFlag)							{ m_Config.bHideMounts = iFlag; }
#endif
#ifdef ENABLE_HIDE_SHOPS
		bool							IsHideShops()										{ return m_Config.bHideShops; }
		void							SetHideShops(bool iFlag)							{ m_Config.bHideShops = iFlag; }
#endif

#ifdef ENABLE_FOG_FIX
		void							SetFogMode(int iFlag);
		bool							IsFogMode();
#endif
#ifdef ENABLE_REMEMBER_LAST_SPLIT
		int                               GetLastRememberSplitData();
		void                            SetLastRemeberSplitData(int iFlag);
#endif
#if defined(WJ_SHOW_MOB_INFO) && defined(ENABLE_SHOW_MOBAIFLAG)
		bool							IsShowMobAIFlag();
		void							SetShowMobAIFlagFlag(int iFlag);
#endif
#if defined(WJ_SHOW_MOB_INFO) && defined(ENABLE_SHOW_MOBLEVEL)
		bool							IsShowMobLevel();
		void							SetShowMobLevelFlag(int iFlag);
#endif

#ifdef ULTIMATE_PICKUP
		bool							GetPickupMode() { return m_Config.bPickupMode; }
		void							SetPickupMode(bool iFlag) { m_Config.bPickupMode = iFlag; }

		bool							GetPickupAutoMode() { return m_Config.bPickupAutoMode; }
		void							SetPickupAutoMode(bool iFlag) { m_Config.bPickupAutoMode = iFlag; }

		DWORD							GetPickupFilter() { return m_Config.bPickupFilter; }
		void							SetPickupFilter(DWORD iFlag) { m_Config.bPickupFilter = iFlag; }
#endif

		// Window
		void							SaveWindowStatus(int iIndex, int iVisible, int iMinimized, int ix, int iy, int iHeight);

		// SaveID
		int								IsSaveID();
		const char *					GetSaveID();
		void							SetSaveID(int iValue, const char * c_szSaveID);
		const char *					GetTransLangKey();
		void							SetTransLangKey(const char * c_szTransLangKey);

		/// Display
		void							GetDisplaySettings();

		int								GetResolutionCount();
		int								GetFrequencyCount(int index);
		bool							GetResolution(int index, OUT DWORD *width, OUT DWORD *height, OUT DWORD *bpp);
		bool							GetFrequency(int index, int freq_index, OUT DWORD *frequncy);
		int								GetResolutionIndex(DWORD width, DWORD height, DWORD bpp);
		int								GetFrequencyIndex(int res_index, DWORD frequency);
		bool							isViewCulling();

		// Sound
		float							GetMusicVolume();
		int								GetSoundVolume();
		void							SetMusicVolume(float fVolume);
		void							SetSoundVolumef(float fVolume);

		int								GetDistance();
#if defined(ENABLE_FOV_OPTION)
		// FoV
		float							GetFOV();
		void							SetFOV(float c_fValue);
#endif
		int								GetShadowLevel();
		void							SetShadowLevel(unsigned int level);

#ifdef ENABLE_SHOPNAMES_RANGE
		void	SetShopNamesRange(float fRange);
		float	GetShopNamesRange();
#endif
	protected:
		TResolution						m_ResolutionList[RESOLUTION_MAX_NUM];
		int								m_ResolutionCount;

		TConfig							m_Config;
		TConfig							m_OldConfig;

		bool							m_isInterfaceConfig;
		PyObject *						m_poInterfaceHandler;
		TWindowStatus					m_WindowStatus[WINDOW_MAX_NUM];
};