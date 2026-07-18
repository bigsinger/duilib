#include "stdafx.h"
#include "UIChildLayout.h"

namespace DuiLib
{
	CChildLayoutUI::CChildLayoutUI()
		: m_pBuilderCallback(NULL)
	{

	}

	void CChildLayoutUI::Init()
	{
		if (!m_pstrXMLFile.empty())
		{
			CDialogBuilder builder;
			CContainerUI* pChildWindow = static_cast<CContainerUI*>(builder.Create(m_pstrXMLFile.c_str(), NULL, m_pBuilderCallback, m_pManager));
			if (pChildWindow)
			{
				this->Add(pChildWindow);
			}
			else
			{
				this->RemoveAll();
			}
		}
	}

	void CChildLayoutUI::SetAttribute( LPCTSTR pstrName, LPCTSTR pstrValue )
	{
		if( _tcscmp(pstrName, _T("xmlfile")) == 0 )
			SetChildLayoutXML(pstrValue);
		else
			CContainerUI::SetAttribute(pstrName,pstrValue);
	}

	void CChildLayoutUI::SetChildLayoutXML( DuiLib::tstring pXML )
	{
		m_pstrXMLFile=pXML;
	}

	DuiLib::tstring CChildLayoutUI::GetChildLayoutXML()
	{
		return m_pstrXMLFile.c_str();
	}

	void CChildLayoutUI::SetBuilderCallback(IDialogBuilderCallback* callback)
	{
		m_pBuilderCallback = callback;
	}

	IDialogBuilderCallback* CChildLayoutUI::GetBuilderCallback() const
	{
		return m_pBuilderCallback;
	}

	LPVOID CChildLayoutUI::GetInterface( LPCTSTR pstrName )
	{
		if( _tcscmp(pstrName, DUI_CTR_CHILDLAYOUT) == 0 ) return static_cast<CChildLayoutUI*>(this);
		return CControlUI::GetInterface(pstrName);
	}

	LPCTSTR CChildLayoutUI::GetClass() const
	{
		return _T("ChildLayoutUI");
	}
} // namespace DuiLib
