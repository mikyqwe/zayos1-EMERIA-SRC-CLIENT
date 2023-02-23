#include "StdAfx.h"
#include "GrpImageInstance.h"
#include "StateManager.h"

#include "../eterBase/CRC32.h"
//STATEMANAGER.SaveRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTCOLOR);
//STATEMANAGER.SaveRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
//STATEMANAGER.RestoreRenderState(D3DRS_SRCBLEND);
//STATEMANAGER.RestoreRenderState(D3DRS_DESTBLEND);

CDynamicPool<CGraphicImageInstance>		CGraphicImageInstance::ms_kPool;

void CGraphicImageInstance::CreateSystem(UINT uCapacity)
{
	ms_kPool.Create(uCapacity);
}

void CGraphicImageInstance::DestroySystem()
{
	ms_kPool.Destroy();
}

CGraphicImageInstance* CGraphicImageInstance::New()
{
	return ms_kPool.Alloc();
}

void CGraphicImageInstance::Delete(CGraphicImageInstance* pkImgInst)
{
	pkImgInst->Destroy();
	ms_kPool.Free(pkImgInst);
}

#ifdef ENABLE_WINDOW_SLIDE_EFFECT
void CGraphicImageInstance::Render(const double scaling)
#else
void CGraphicImageInstance::Render()
#endif
{
	if (IsEmpty())
		return;

	assert(!IsEmpty());
#ifdef ENABLE_WINDOW_SLIDE_EFFECT
	OnRender(scaling);
#else
	OnRender();
#endif
}


#ifdef RENDER_TARGET
#ifdef ENABLE_WINDOW_SLIDE_EFFECT
void CGraphicImageInstance::OnRender(const double scaling)
#else
void CGraphicImageInstance::OnRender()
#endif
{
	CGraphicTexture* graphicTexture = m_roImage->GetTexturePointer();

	float fimgWidth = m_roImage->GetWidth() * m_v2Scale.x;
	float fimgHeight = m_roImage->GetHeight() * m_v2Scale.y;

	const RECT& c_rRect = m_roImage->GetRectReference();
	float texReverseWidth = 1.0f / float(graphicTexture->GetWidth());
	float texReverseHeight = 1.0f / float(graphicTexture->GetHeight());
	float su = c_rRect.left * texReverseWidth;
	float sv = c_rRect.top * texReverseHeight;
	float eu = (c_rRect.left + (c_rRect.right - c_rRect.left)) * texReverseWidth;
	float ev = (c_rRect.top + (c_rRect.bottom - c_rRect.top)) * texReverseHeight;


	TPDTVertex vertices[4];
	vertices[0].position.x = m_v2Position.x - 0.5f;
	vertices[0].position.y = m_v2Position.y - 0.5f;
	vertices[0].position.z = 0.0f;
	vertices[0].texCoord = TTextureCoordinate(su, sv);
	vertices[0].diffuse = m_DiffuseColor;

	vertices[1].position.x = m_v2Position.x + fimgWidth - 0.5f;
	vertices[1].position.y = m_v2Position.y - 0.5f;
	vertices[1].position.z = 0.0f;
	vertices[1].texCoord = TTextureCoordinate(eu, sv);
	vertices[1].diffuse = m_DiffuseColor;

	vertices[2].position.x = m_v2Position.x - 0.5f;
	vertices[2].position.y = m_v2Position.y + fimgHeight - 0.5f;
	vertices[2].position.z = 0.0f;
	vertices[2].texCoord = TTextureCoordinate(su, ev);
	vertices[2].diffuse = m_DiffuseColor;

	vertices[3].position.x = m_v2Position.x + fimgWidth - 0.5f;
	vertices[3].position.y = m_v2Position.y + fimgHeight - 0.5f;
	vertices[3].position.z = 0.0f;
	vertices[3].texCoord = TTextureCoordinate(eu, ev);
	vertices[3].diffuse = m_DiffuseColor;

#ifdef ENABLE_WINDOW_SLIDE_EFFECT
	if (scaling != 1.0) {
		const auto realwidth = vertices[3].position.x - vertices[0].position.x;
		const auto realheight = vertices[3].position.y - vertices[0].position.y;

		const auto xoff = (realwidth * (1.0 - scaling)) / 2;
		const auto yoff = (realheight * (1.0 - scaling)) / 2;

		vertices[0].position.x += xoff;
		vertices[0].position.y += yoff;

		vertices[1].position.x -= xoff;
		vertices[1].position.y += yoff;

		vertices[2].position.x += xoff;
		vertices[2].position.y -= yoff;

		vertices[3].position.x -= xoff;
		vertices[3].position.y -= yoff;

		D3DXCOLOR new_color = m_DiffuseColor;
		new_color.a *= scaling * scaling;

		for (auto& vert : vertices)
			vert.diffuse = new_color;
	}
#endif

	if (CGraphicBase::SetPDTStream(vertices, 4))
	{
		CGraphicBase::SetDefaultIndexBuffer(CGraphicBase::DEFAULT_IB_FILL_RECT);

		STATEMANAGER.SetTexture(0, graphicTexture->GetD3DTexture());
		STATEMANAGER.SetTexture(1, NULL);
		STATEMANAGER.SetVertexShader(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);
		STATEMANAGER.DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 4, 0, 2);
	}
}
#else
void CGraphicImageInstance::OnRender()
{
	CGraphicImage * pImage = m_roImage.GetPointer();
	CGraphicTexture * pTexture = pImage->GetTexturePointer();

	float fimgWidth = pImage->GetWidth()*m_vScale;
	float fimgHeight = pImage->GetHeight()*m_vScale;

	const RECT& c_rRect = pImage->GetRectReference();
	float texReverseWidth = 1.0f / float(pTexture->GetWidth());
	float texReverseHeight = 1.0f / float(pTexture->GetHeight());
	float su = c_rRect.left * texReverseWidth;
	float sv = c_rRect.top * texReverseHeight;
	float eu = (c_rRect.left + (c_rRect.right-c_rRect.left)) * texReverseWidth;
	float ev = (c_rRect.top + (c_rRect.bottom-c_rRect.top)) * texReverseHeight;


	TPDTVertex vertices[4];
	vertices[0].position.x	= m_v2Position.x-0.5f;
	vertices[0].position.y	= m_v2Position.y-0.5f;
	vertices[0].position.z	= 0.0f;
	vertices[0].texCoord	= TTextureCoordinate(su, sv);
	vertices[0].diffuse		= m_DiffuseColor;

	vertices[1].position.x	= m_v2Position.x + fimgWidth-0.5f;
	vertices[1].position.y	= m_v2Position.y-0.5f;
	vertices[1].position.z	= 0.0f;
	vertices[1].texCoord	= TTextureCoordinate(eu, sv);
	vertices[1].diffuse		= m_DiffuseColor;

	vertices[2].position.x	= m_v2Position.x-0.5f;
	vertices[2].position.y	= m_v2Position.y + fimgHeight-0.5f;
	vertices[2].position.z	= 0.0f;
	vertices[2].texCoord	= TTextureCoordinate(su, ev);
	vertices[2].diffuse		= m_DiffuseColor;

	vertices[3].position.x	= m_v2Position.x + fimgWidth-0.5f;
	vertices[3].position.y	= m_v2Position.y + fimgHeight-0.5f;
	vertices[3].position.z	= 0.0f;
	vertices[3].texCoord	= TTextureCoordinate(eu, ev);
	vertices[3].diffuse		= m_DiffuseColor;

	// 2004.11.18.myevan.ctrl+alt+del 반복 사용시 튕기는 문제
	if (CGraphicBase::SetPDTStream(vertices, 4))
	{
		CGraphicBase::SetDefaultIndexBuffer(CGraphicBase::DEFAULT_IB_FILL_RECT);

		STATEMANAGER.SetTexture(0, pTexture->GetD3DTexture());
		STATEMANAGER.SetTexture(1, NULL);
		STATEMANAGER.SetVertexShader(D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1);
		STATEMANAGER.DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 4, 0, 2);
	}
	//OLD: STATEMANAGER.DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 4, 2, c_FillRectIndices, D3DFMT_INDEX16, vertices, sizeof(TPDTVertex));
	////////////////////////////////////////////////////////////
}
#endif

const CGraphicTexture & CGraphicImageInstance::GetTextureReference() const
{
	return m_roImage->GetTextureReference();
}

CGraphicTexture * CGraphicImageInstance::GetTexturePointer()
{
	CGraphicImage* pkImage = m_roImage.GetPointer();
	return pkImage ? pkImage->GetTexturePointer() : NULL;
}

CGraphicImage * CGraphicImageInstance::GetGraphicImagePointer()
{
	return m_roImage.GetPointer();
}

int CGraphicImageInstance::GetWidth()
{
	if (IsEmpty())
		return 0;

	return m_roImage->GetWidth();
}

int CGraphicImageInstance::GetHeight()
{
	if (IsEmpty())
		return 0;

	return m_roImage->GetHeight();
}

void CGraphicImageInstance::SetDiffuseColor(float fr, float fg, float fb, float fa)
{
	m_DiffuseColor.r = fr;
	m_DiffuseColor.g = fg;
	m_DiffuseColor.b = fb;
	m_DiffuseColor.a = fa;
}
void CGraphicImageInstance::SetPosition(float fx, float fy)
{
	m_v2Position.x = fx;
	m_v2Position.y = fy;
}

#ifdef RENDER_TARGET
void CGraphicImageInstance::SetScale(float fx, float fy)
{
	m_v2Scale.x = fx;
	m_v2Scale.y = fy;
}

#else
void CGraphicImageInstance::SetScale(float fx, float fy)
{
	m_vScale = fx;
}
#endif

void CGraphicImageInstance::SetImagePointer(CGraphicImage * pImage)
{
	m_roImage.SetPointer(pImage);

	OnSetImagePointer();
}

void CGraphicImageInstance::ReloadImagePointer(CGraphicImage * pImage)
{
	if (m_roImage.IsNull())
	{
		SetImagePointer(pImage);
		return;
	}

	CGraphicImage * pkImage = m_roImage.GetPointer();

	if (pkImage)
		pkImage->Reload();
}

bool CGraphicImageInstance::IsEmpty() const
{
	if (!m_roImage.IsNull() && !m_roImage->IsEmpty())
		return false;

	return true;
}

bool CGraphicImageInstance::operator == (const CGraphicImageInstance & rhs) const
{
	return (m_roImage.GetPointer() == rhs.m_roImage.GetPointer());
}

DWORD CGraphicImageInstance::Type()
{
	static DWORD s_dwType = GetCRC32("CGraphicImageInstance", strlen("CGraphicImageInstance"));
	return (s_dwType);
}

BOOL CGraphicImageInstance::IsType(DWORD dwType)
{
	return OnIsType(dwType);
}

BOOL CGraphicImageInstance::OnIsType(DWORD dwType)
{
	if (CGraphicImageInstance::Type() == dwType)
		return TRUE;

	return FALSE;
}

void CGraphicImageInstance::OnSetImagePointer()
{
}

void CGraphicImageInstance::Initialize()
{
	m_DiffuseColor.r = m_DiffuseColor.g = m_DiffuseColor.b = m_DiffuseColor.a = 1.0f;
	m_v2Position.x = m_v2Position.y = 0.0f;
#ifdef RENDER_TARGET
	m_v2Scale.x = m_v2Scale.y = 1.0f;
#endif
	m_vScale = 1.0f;
}

void CGraphicImageInstance::Destroy()
{
	m_roImage.SetPointer(NULL); // CRef 에서 레퍼런스 카운트가 떨어져야 함.
	Initialize();
}

CGraphicImageInstance::CGraphicImageInstance()
{
	Initialize();
}

CGraphicImageInstance::~CGraphicImageInstance()
{
	Destroy();
}

void CGraphicImageInstance::RenderCoolTime(float fCoolTime, bool bIsInverse)
{
	if (IsEmpty())
		return;

	assert(!IsEmpty());

	OnRenderCoolTime(fCoolTime, bIsInverse);
}

void CGraphicImageInstance::OnRenderCoolTime(float fCoolTime, bool bIsInverse)
{
	if (fCoolTime >= 1.0f)
		fCoolTime = 1.0f;

	CGraphicImage* pImage = m_roImage.GetPointer();
	const CGraphicTexture* pTexture = pImage->GetTexturePointer();

	const float fimgWidth = pImage->GetWidth() * m_v2Scale.x;
	const float fimgHeight = pImage->GetHeight() * m_v2Scale.y;
	const float fimgWidthHalf = fimgWidth * 0.5f;
	const float fimgHeightHalf = fimgHeight * 0.5f;

	const RECT& c_rRect = pImage->GetRectReference();
	const float texReverseWidth = 1.0f / float(pTexture->GetWidth());
	const float texReverseHeight = 1.0f / float(pTexture->GetHeight());
	const float su = c_rRect.left * texReverseWidth;
	const float sv = c_rRect.top * texReverseHeight;
	const float eu = c_rRect.right * texReverseWidth;
	const float ev = c_rRect.bottom * texReverseHeight;
	const float euh = eu * 0.5f;
	const float evh = ev * 0.5f;
	const float fxCenter = m_v2Position.x - 0.5f + fimgWidthHalf;
	const float fyCenter = m_v2Position.y - 0.5f + fimgHeightHalf;

	if (fCoolTime < 1.0f)
	{
		if (fCoolTime < 0.0) fCoolTime = 0.0;

		constexpr int c_iTriangleCountPerBox = 8;
		D3DXVECTOR2 v2BoxPos[c_iTriangleCountPerBox] = {
			D3DXVECTOR2(-1.0f, -1.0f),
			D3DXVECTOR2(-1.0f,  0.0f),
			D3DXVECTOR2(-1.0f, +1.0f),
			D3DXVECTOR2(0.0f, +1.0f),
			D3DXVECTOR2(+1.0f, +1.0f),
			D3DXVECTOR2(+1.0f,  0.0f),
			D3DXVECTOR2(+1.0f, -1.0f),
			D3DXVECTOR2(0.0f, -1.0f),
		};

		D3DXVECTOR2 v2TexPos[c_iTriangleCountPerBox] = {
			D3DXVECTOR2(su,  sv),
			D3DXVECTOR2(su, evh),
			D3DXVECTOR2(su,  ev),
			D3DXVECTOR2(euh,  ev),
			D3DXVECTOR2(eu,  ev),
			D3DXVECTOR2(eu, evh),
			D3DXVECTOR2(eu,  sv),
			D3DXVECTOR2(euh,  sv),
		};

		if (bIsInverse)
		{
			v2TexPos[0].x = eu;
			v2TexPos[1].x = eu;
			v2TexPos[2].x = eu;
			v2TexPos[4].x = su;
			v2TexPos[5].x = su;
			v2TexPos[6].x = su;

			v2BoxPos[0] = D3DXVECTOR2(+1.0f, -1.0f);
			v2BoxPos[1] = D3DXVECTOR2(+1.0f, 0.0f);
			v2BoxPos[2] = D3DXVECTOR2(+1.0f, +1.0f);
			v2BoxPos[3] = D3DXVECTOR2(0.0f, +1.0f);
			v2BoxPos[4] = D3DXVECTOR2(-1.0f, +1.0f);
			v2BoxPos[5] = D3DXVECTOR2(-1.0f, 0.0f);
			v2BoxPos[6] = D3DXVECTOR2(-1.0f, -1.0f);
			v2BoxPos[7] = D3DXVECTOR2(0.0f, -1.0f);
		}

		int iTriCount = static_cast<int>(c_iTriangleCountPerBox - c_iTriangleCountPerBox * fCoolTime);
		const float fLastPercentage = (c_iTriangleCountPerBox - c_iTriangleCountPerBox * fCoolTime) - iTriCount;

		std::vector<TPDTVertex> vertices;
		TPDTVertex vertex;
		vertex.position = TPosition(fxCenter, fyCenter, 0.0f);
		vertex.texCoord = TTextureCoordinate(euh, evh);
		vertex.diffuse = m_DiffuseColor;
		vertices.push_back(vertex);

		vertex.position = TPosition(fxCenter, fyCenter - fimgHeightHalf - 0.5f, 0.0f);
		vertex.texCoord = TTextureCoordinate(euh, sv);
		vertex.diffuse = m_DiffuseColor;
		vertices.push_back(vertex);

		if (iTriCount > 0)
		{
			for (int j = 0; j < iTriCount; ++j)
			{
				vertex.position = TPosition(fxCenter + (v2BoxPos[j].x * fimgWidthHalf) - 0.5f,
					fyCenter + (v2BoxPos[j].y * fimgHeightHalf) - 0.5f,
					0.0f);
				vertex.texCoord = TTextureCoordinate(v2TexPos[j & (c_iTriangleCountPerBox - 1)].x,
					v2TexPos[j & (c_iTriangleCountPerBox - 1)].y);
				vertex.diffuse = m_DiffuseColor;
				vertices.push_back(vertex);
			}
		}

		if (fLastPercentage > 0.0f)
		{
			D3DXVECTOR2* pv2Pos;
			D3DXVECTOR2* pv2LastPos;
			assert((iTriCount + 8) % 8 >= 0 && (iTriCount + 8) % 8 < 8);
			assert((iTriCount + 7) % 8 >= 0 && (iTriCount + 7) % 8 < 8);
			pv2LastPos = &v2BoxPos[(iTriCount + 8) % 8];
			pv2Pos = &v2BoxPos[(iTriCount + 7) % 8];
			float fxShit = (pv2LastPos->x - pv2Pos->x) * fLastPercentage + pv2Pos->x;
			float fyShit = (pv2LastPos->y - pv2Pos->y) * fLastPercentage + pv2Pos->y;
			vertex.position = TPosition(fimgWidthHalf * fxShit + fxCenter - 0.5f,
				fimgHeightHalf * fyShit + fyCenter - 0.5f,
				0.0f);
			vertex.texCoord = TTextureCoordinate(euh * fxShit + euh,
				evh * fyShit + evh);
			vertex.diffuse = m_DiffuseColor;
			vertices.push_back(vertex);
			++iTriCount;
		}

		if (vertices.empty())
			return;

		DWORD dwRS = D3DCULL_NONE;
		if (bIsInverse)
		{
			STATEMANAGER.GetRenderState(D3DRS_CULLMODE, &dwRS);
			STATEMANAGER.SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		}
		else
			STATEMANAGER.SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
		if (CGraphicBase::SetPDTStream(&vertices[0], vertices.size()))
		{
			CGraphicBase::SetDefaultIndexBuffer(CGraphicBase::DEFAULT_IB_FILL_TRI);
			STATEMANAGER.SetTexture(0, pTexture->GetD3DTexture());
			STATEMANAGER.SetTexture(1, NULL);
#ifdef ENABLE_D3DX9
			STATEMANAGER.SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);
#else
			STATEMANAGER.SetVertexShader(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);
#endif
			STATEMANAGER.DrawPrimitive(D3DPT_TRIANGLEFAN, 0, iTriCount);
		}
		if (bIsInverse)
			STATEMANAGER.SetRenderState(D3DRS_CULLMODE, dwRS);
		else
			STATEMANAGER.SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	}
}
void CGraphicImageInstance::RenderRectCircle(float fRectCircle, bool bIsInverse)
{
	if (IsEmpty())
		return;
	assert(!IsEmpty());
	OnRenderRectCircle(fRectCircle, bIsInverse);
}

void CGraphicImageInstance::OnRenderRectCircle(float fRectCircle, bool bIsInverse)
{
	if (fRectCircle >= 1.0f)
		fRectCircle = 1.0f;

	CGraphicImage* pImage = m_roImage.GetPointer();
	const CGraphicTexture* pTexture = pImage->GetTexturePointer();

	const float fimgWidth = pImage->GetWidth() * m_v2Scale.x;
	const float fimgHeight = pImage->GetHeight() * m_v2Scale.y;
	const float fimgWidthHalf = fimgWidth * 0.5f;
	const float fimgHeightHalf = fimgHeight * 0.5f;

	const RECT& c_rRect = pImage->GetRectReference();
	const float texReverseWidth = 1.0f / float(pTexture->GetWidth());
	const float texReverseHeight = 1.0f / float(pTexture->GetHeight());
	const float su = c_rRect.left * texReverseWidth;
	const float sv = c_rRect.top * texReverseHeight;
	const float eu = c_rRect.right * texReverseWidth;
	const float ev = c_rRect.bottom * texReverseHeight;
	const float euh = eu * 0.5f;
	const float evh = ev * 0.5f;
	const float fxCenter = m_v2Position.x - 0.5f + fimgWidthHalf;
	const float fyCenter = m_v2Position.y - 0.5f + fimgHeightHalf;

	if (fRectCircle < 1.0f)
	{
		if (fRectCircle < 0.0) fRectCircle = 0.0;

		constexpr int c_iTriangleCountPerBox = 8;
		D3DXVECTOR2 v2BoxPos[c_iTriangleCountPerBox] = {
			D3DXVECTOR2(-1.0f, -1.0f),
			D3DXVECTOR2(-1.0f,  0.0f),
			D3DXVECTOR2(-1.0f, +1.0f),
			D3DXVECTOR2(0.0f, +1.0f),
			D3DXVECTOR2(+1.0f, +1.0f),
			D3DXVECTOR2(+1.0f,  0.0f),
			D3DXVECTOR2(+1.0f, -1.0f),
			D3DXVECTOR2(0.0f, -1.0f),
		};

		D3DXVECTOR2 v2TexPos[c_iTriangleCountPerBox] = {
			D3DXVECTOR2(su,  sv),
			D3DXVECTOR2(su, evh),
			D3DXVECTOR2(su,  ev),
			D3DXVECTOR2(euh,  ev),
			D3DXVECTOR2(eu,  ev),
			D3DXVECTOR2(eu, evh),
			D3DXVECTOR2(eu,  sv),
			D3DXVECTOR2(euh,  sv),
		};

		if (bIsInverse)
		{
			v2TexPos[0].x = eu;
			v2TexPos[1].x = eu;
			v2TexPos[2].x = eu;
			v2TexPos[4].x = su;
			v2TexPos[5].x = su;
			v2TexPos[6].x = su;

			v2BoxPos[0] = D3DXVECTOR2(+1.0f, -1.0f);
			v2BoxPos[1] = D3DXVECTOR2(+1.0f, 0.0f);
			v2BoxPos[2] = D3DXVECTOR2(+1.0f, +1.0f);
			v2BoxPos[3] = D3DXVECTOR2(0.0f, +1.0f);
			v2BoxPos[4] = D3DXVECTOR2(-1.0f, +1.0f);
			v2BoxPos[5] = D3DXVECTOR2(-1.0f, 0.0f);
			v2BoxPos[6] = D3DXVECTOR2(-1.0f, -1.0f);
			v2BoxPos[7] = D3DXVECTOR2(0.0f, -1.0f);
		}

		int iTriCount = static_cast<int>(c_iTriangleCountPerBox - c_iTriangleCountPerBox * fRectCircle);
		const float fLastPercentage = (c_iTriangleCountPerBox - c_iTriangleCountPerBox * fRectCircle) - iTriCount;

		std::vector<TPDTVertex> vertices;
		TPDTVertex vertex;
		vertex.position = TPosition(fxCenter, fyCenter, 0.0f);
		vertex.texCoord = TTextureCoordinate(euh, evh);
		vertex.diffuse = m_DiffuseColor;
		vertices.push_back(vertex);

		vertex.position = TPosition(fxCenter, fyCenter - fimgHeightHalf - 0.5f, 0.0f);
		vertex.texCoord = TTextureCoordinate(euh, sv);
		vertex.diffuse = m_DiffuseColor;
		vertices.push_back(vertex);

		if (iTriCount > 0)
		{
			for (int j = 0; j < iTriCount; ++j)
			{
				vertex.position = TPosition(fxCenter + (v2BoxPos[j].x * fimgWidthHalf) - 0.5f,
					fyCenter + (v2BoxPos[j].y * fimgHeightHalf) - 0.5f,
					0.0f);
				vertex.texCoord = TTextureCoordinate(v2TexPos[j & (c_iTriangleCountPerBox - 1)].x,
					v2TexPos[j & (c_iTriangleCountPerBox - 1)].y);
				vertex.diffuse = m_DiffuseColor;
				vertices.push_back(vertex);
			}
		}

		if (fLastPercentage > 0.0f)
		{
			D3DXVECTOR2* pv2Pos;
			D3DXVECTOR2* pv2LastPos;
			assert((iTriCount + 8) % 8 >= 0 && (iTriCount + 8) % 8 < 8);
			assert((iTriCount + 7) % 8 >= 0 && (iTriCount + 7) % 8 < 8);
			pv2LastPos = &v2BoxPos[(iTriCount + 8) % 8];
			pv2Pos = &v2BoxPos[(iTriCount + 7) % 8];
			float fxShit = (pv2LastPos->x - pv2Pos->x) * fLastPercentage + pv2Pos->x;
			float fyShit = (pv2LastPos->y - pv2Pos->y) * fLastPercentage + pv2Pos->y;
			vertex.position = TPosition(fimgWidthHalf * fxShit + fxCenter - 0.5f,
				fimgHeightHalf * fyShit + fyCenter - 0.5f,
				0.0f);
			vertex.texCoord = TTextureCoordinate(euh * fxShit + euh,
				evh * fyShit + evh);
			vertex.diffuse = m_DiffuseColor;
			vertices.push_back(vertex);
			++iTriCount;
		}

		if (vertices.empty())
			return;

		DWORD dwRS = D3DCULL_NONE;
		if (bIsInverse)
		{
			STATEMANAGER.GetRenderState(D3DRS_CULLMODE, &dwRS);
			STATEMANAGER.SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		}
		else
			STATEMANAGER.SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
		if (CGraphicBase::SetPDTStream(&vertices[0], vertices.size()))
		{
			CGraphicBase::SetDefaultIndexBuffer(CGraphicBase::DEFAULT_IB_FILL_TRI);
			STATEMANAGER.SetTexture(0, pTexture->GetD3DTexture());
			STATEMANAGER.SetTexture(1, NULL);
#ifdef ENABLE_D3DX9
			STATEMANAGER.SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);
#else
			STATEMANAGER.SetVertexShader(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);
#endif
			STATEMANAGER.DrawPrimitive(D3DPT_TRIANGLEFAN, 0, iTriCount);
		}
		if (bIsInverse)
			STATEMANAGER.SetRenderState(D3DRS_CULLMODE, dwRS);
		else
			STATEMANAGER.SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	}
}
