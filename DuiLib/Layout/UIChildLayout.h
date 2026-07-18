#ifndef __UICHILDLAYOUT_H__
#define __UICHILDLAYOUT_H__

#pragma once

namespace DuiLib
{
	class UILIB_API CChildLayoutUI : public CContainerUI
	{
	public:
		CChildLayoutUI();

		void Init();
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		void SetChildLayoutXML(tstring pXML);
		DuiLib::tstring GetChildLayoutXML();
		void SetBuilderCallback(IDialogBuilderCallback* callback);
		IDialogBuilderCallback* GetBuilderCallback() const;
		virtual LPVOID GetInterface(LPCTSTR pstrName);
		virtual LPCTSTR GetClass() const;

	private:
		DuiLib::tstring m_pstrXMLFile;
		IDialogBuilderCallback* m_pBuilderCallback;
	};
} // namespace DuiLib
#endif // __UICHILDLAYOUT_H__
