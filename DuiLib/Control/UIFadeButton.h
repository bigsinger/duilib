#ifndef __UIFADEBUTTON_H__
#define __UIFADEBUTTON_H__

#pragma once

#if DUI_HAS_FULL_CONTROLS

namespace DuiLib {

	class UILIB_API CFadeButtonUI : public CButtonUI
	{
		DECLARE_DUICONTROL(CFadeButtonUI)
	public:
		CFadeButtonUI();
		virtual ~CFadeButtonUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		void SetNormalImage(LPCTSTR pStrImage);

		void DoEvent(TEventUI& event);
		void OnTimer( int nTimerID );
		void PaintStatusImage(HDC hDC);
		
	protected:
		tstring m_sLastImage;
	};

}

#endif

#endif
