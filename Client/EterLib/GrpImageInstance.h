#pragma once

#include "GrpImage.h"
#include "GrpIndexBuffer.h"
#include "GrpVertexBufferDynamic.h"
#include "Pool.h"

class CGraphicImageInstance
{
	public:
		static DWORD Type();
		BOOL IsType(DWORD dwType);

	public:
		CGraphicImageInstance();
		virtual ~CGraphicImageInstance();

		void Destroy();

#ifdef ENABLE_WINDOW_SLIDE_EFFECT
		void Render(const double scaling = 1.0);
#else
		void Render();
#endif

		void SetDiffuseColor(float fr, float fg, float fb, float fa);
		#ifdef RENDER_TARGET
		void SetScale(float fx, float fy);
		#else
		void SetScale(float sx, float sy);
		#endif
		void SetPosition(float fx, float fy);

		void SetImagePointer(CGraphicImage* pImage);
		void ReloadImagePointer(CGraphicImage* pImage);
		bool IsEmpty() const;

		void RenderCoolTime(float fCoolTime, bool bIsInverse);
		void RenderRectCircle(float fRectCircle, bool bIsInverse);

		

		int GetWidth();
		int GetHeight();

		CGraphicTexture * GetTexturePointer();
		const CGraphicTexture &	GetTextureReference() const;
		CGraphicImage * GetGraphicImagePointer();

		bool operator == (const CGraphicImageInstance & rhs) const;

	protected:
		void Initialize();

#ifdef ENABLE_WINDOW_SLIDE_EFFECT
		virtual void OnRender(const double scaling = 1.0);
#else
		virtual void OnRender();
#endif
		virtual void OnSetImagePointer();

		virtual BOOL OnIsType(DWORD dwType);

		virtual void OnRenderCoolTime(float fCoolTime, bool bIsInverse);
		virtual void OnRenderRectCircle(float fRectCircle, bool bIsInverse);

	protected:
		D3DXCOLOR m_DiffuseColor;
		D3DXVECTOR2 m_v2Position;
#ifdef RENDER_TARGET
		D3DXVECTOR2 m_v2Scale;
#endif
		float m_vScale;
		CGraphicImage::TRef m_roImage;

	public:
		static void CreateSystem(UINT uCapacity);
		static void DestroySystem();

		static CGraphicImageInstance* New();
		static void Delete(CGraphicImageInstance* pkImgInst);

		static CDynamicPool<CGraphicImageInstance>		ms_kPool;
};
