#include "StdAfx.h"
#include "GlobalVariable.h"

bool CGlobalVariable::m_bIsProjectExist = false;
CString CGlobalVariable::m_strProjectName;
CString CGlobalVariable::m_strProjectPath;
CString CGlobalVariable::m_strCurPath;
CString CGlobalVariable::m_strTemplatesDir;
CString CGlobalVariable::m_strStylesDir;

CGlobalVariable::CGlobalVariable(void)
{
}

CGlobalVariable::~CGlobalVariable(void)
{
}

CString& CGlobalVariable::GetCurPath()
{
	if(m_strCurPath.IsEmpty())
	{
		TCHAR szFileName[MAX_PATH] = {0};
		::GetModuleFileName(NULL, szFileName, MAX_PATH);
		m_strCurPath = szFileName;
		int nPos = m_strCurPath.ReverseFind('\\');
		if(nPos != -1)
			m_strCurPath = m_strCurPath.Left(nPos + 1);

        CString strPath = m_strCurPath;
        strPath.TrimRight(_T("\\/"));
        nPos = strPath.ReverseFind('\\');
        if(nPos != -1)
        {
            CString strDirName = strPath.Mid(nPos + 1);
            if(strDirName.CompareNoCase(_T("Full")) == 0
                || strDirName.CompareNoCase(_T("x64")) == 0
                || strDirName.CompareNoCase(_T("x64Full")) == 0)
            {
                m_strCurPath = strPath.Left(nPos + 1);
            }
        }
	}

	return m_strCurPath;
}

CString& CGlobalVariable::GetTemplatesDir()
{
	if(m_strTemplatesDir.IsEmpty())
	{
		m_strTemplatesDir = GetCurPath() + DIR_TEMPLATES;
		CreateDirectory(m_strTemplatesDir, NULL);
	}

	return m_strTemplatesDir;
}

CString& CGlobalVariable::GetStylesDir()
{
	if(m_strStylesDir.IsEmpty())
	{
		m_strStylesDir = GetCurPath() + DIR_STYLES;
		CreateDirectory(m_strStylesDir, NULL);
	}

	return m_strStylesDir;
}
