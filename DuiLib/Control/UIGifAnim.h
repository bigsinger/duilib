#ifndef GifAnimUI_h__
#define GifAnimUI_h__

#pragma once

#if DUI_HAS_STANDARD_CONTROLS

namespace DuiLib
{
	class UILIB_API CGifAnimUI : public CControlUI
	{
		DECLARE_DUICONTROL(CGifAnimUI)
	public:
		CGifAnimUI(void);
		~CGifAnimUI(void);

		LPCTSTR	GetClass() const;
		LPVOID	GetInterface(LPCTSTR pstrName);
		void	DoInit();
		bool	DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);
		void	DoEvent(TEventUI& event);
		void	SetVisible(bool bVisible = true );
		void	SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		void	SetBkImage(LPCTSTR pStrImage);
		LPCTSTR GetBkImage();

		void	SetAutoPlay(bool bIsAuto = true );
		bool	IsAutoPlay() const;
		void	SetAutoSize(bool bIsAuto = true );
		bool	IsAutoSize() const;
		void	PlayGif();
		void	PauseGif();
		void	StopGif();

	private:
		void	InitGifImage();
		void	DeleteGif();
		void    OnTimer( UINT_PTR idEvent );
		void	DrawFrame( HDC hDC );

	private:
		tstring		m_sBkImage;
		bool			m_bIsAutoPlay;
		bool			m_bIsAutoSize;
		bool			m_bIsPlaying;
	};
}

#endif

#endif
