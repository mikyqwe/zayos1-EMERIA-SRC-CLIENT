#pragma once

#include "GrpImage.h"
#include "Pool.h"

class CGraphicMarkInstance
{
	public:
		static DWORD Type();
		BOOL IsType(DWORD dwType);

		void SetImageFileName(const char* c_szFileName);
		const std::string& GetImageFileName();

	public:
		CGraphicMarkInstance();
		virtual ~CGraphicMarkInstance();

		void Destroy();

#ifdef ENABLE_WINDOW_SLIDE_EFFECT
		void Render(const double scaling = 1.0);
#else
		void Render();
#endif

		void SetDepth(float fDepth);
		void SetDiffuseColor(float fr, float fg, float fb, float fa);
		void SetPosition(float fx, float fy);
		void SetIndex(UINT uIndex);
		void SetScale(float fScale);

		void Load();
		bool IsEmpty() const;

		int GetWidth();
		int GetHeight();

		CGraphicTexture * GetTexturePointer();
		const CGraphicTexture &	GetTextureReference() const;
		CGraphicImage * GetGraphicImagePointer();

		bool operator == (const CGraphicMarkInstance & rhs) const;

	protected:
		enum
		{
			MARK_WIDTH = 16,
			MARK_HEIGHT = 12,
		};

		void Initialize();

#ifdef ENABLE_WINDOW_SLIDE_EFFECT
		virtual void OnRender(const double scaling = 1.0);
#else
		virtual void OnRender();
#endif
		virtual void OnSetImagePointer();

		virtual BOOL OnIsType(DWORD dwType);

		void SetImagePointer(CGraphicImage * pImage);

	protected:
		D3DXCOLOR m_DiffuseColor;
		D3DXVECTOR2 m_v2Position;
		D3DXVECTOR3 m_v3Position;

		UINT m_uIndex;

		FLOAT m_fScale;
		FLOAT m_fDepth;

		CGraphicImage::TRef m_roImage;
		std::string m_stImageFileName;

	public:
		static void CreateSystem(UINT uCapacity);
		static void DestroySystem();

		static CGraphicMarkInstance* New();
		static void Delete(CGraphicMarkInstance* pkImgInst);

		static CDynamicPool<CGraphicMarkInstance> ms_kPool;
};
