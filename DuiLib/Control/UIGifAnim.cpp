#include "StdAfx.h"
#include "UIGifAnim.h"

#if DUI_HAS_STANDARD_CONTROLS

///////////////////////////////////////////////////////////////////////////////////////
namespace DuiLib
{
	IMPLEMENT_DUICONTROL(CGifAnimUI)

		CGifAnimUI::CGifAnimUI(void)
	{
		m_bIsAutoPlay		=	true;
		m_bIsAutoSize		=	false;
		m_bIsPlaying		=	false;

	}


	CGifAnimUI::~CGifAnimUI(void)
	{
		DeleteGif();
	}

	LPCTSTR CGifAnimUI::GetClass() const
	{
		return _T("GifAnimUI");
	}

	LPVOID CGifAnimUI::GetInterface( LPCTSTR pstrName )
	{
		if( _tcsicmp(pstrName, DUI_CTR_GIFANIM) == 0 ) return static_cast<CGifAnimUI*>(this);
		return CControlUI::GetInterface(pstrName);
	}

	void CGifAnimUI::DoInit()
	{
		InitGifImage();
	}

	bool CGifAnimUI::DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl)
	{
		if( !::IntersectRect( &m_rcPaint, &rcPaint, &m_rcItem ) ) return true;
		DrawFrame( hDC );
		return true;
	}

	void CGifAnimUI::DoEvent( TEventUI& event )
	{
		CControlUI::DoEvent(event);
	}

	void CGifAnimUI::SetVisible(bool bVisible)
	{
		CControlUI::SetVisible(bVisible);
		if (bVisible)
			PlayGif();
		else
			StopGif();
	}

	void CGifAnimUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcsicmp(pstrName, _T("bkimage")) == 0 ) SetBkImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("autoplay")) == 0 ) {
			SetAutoPlay(_tcsicmp(pstrValue, _T("true")) == 0);
		}
		else if( _tcsicmp(pstrName, _T("autosize")) == 0 ) {
			SetAutoSize(_tcsicmp(pstrValue, _T("true")) == 0);
		}
		else
			CControlUI::SetAttribute(pstrName, pstrValue);
	}

	void CGifAnimUI::SetBkImage(LPCTSTR pStrImage)
	{
		if( m_sBkImage == pStrImage || NULL == pStrImage) return;

		m_sBkImage = pStrImage;

		StopGif();
		Invalidate();

	}

	LPCTSTR CGifAnimUI::GetBkImage()
	{
		return m_sBkImage.c_str();
	}

	void CGifAnimUI::SetAutoPlay(bool bIsAuto)
	{
		m_bIsAutoPlay = bIsAuto;
	}

	bool CGifAnimUI::IsAutoPlay() const
	{
		return m_bIsAutoPlay;
	}

	void CGifAnimUI::SetAutoSize(bool bIsAuto)
	{
		m_bIsAutoSize = bIsAuto;
	}

	bool CGifAnimUI::IsAutoSize() const
	{
		return m_bIsAutoSize;
	}

	void CGifAnimUI::PlayGif()
	{
		m_bIsPlaying = true;
		Invalidate();
	}

	void CGifAnimUI::PauseGif()
	{
		m_bIsPlaying = false;
		Invalidate();
	}

	void CGifAnimUI::StopGif()
	{
		m_bIsPlaying = false;
		Invalidate();
	}

	void CGifAnimUI::InitGifImage()
	{
		if (m_bIsAutoPlay)
		{
			PlayGif();
		}
	}

	void CGifAnimUI::DeleteGif()
	{
	}

	void CGifAnimUI::OnTimer( UINT_PTR idEvent )
	{
	}

	void CGifAnimUI::DrawFrame( HDC hDC )
	{
		if ( NULL == hDC || m_sBkImage.empty() ) return;
		DrawImage(hDC, m_sBkImage);
	}
}

#endif
