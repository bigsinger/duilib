#include "stdafx.h"
#include "UIMultiLanguage.h"

#include <filesystem>
#include <fstream>
#include <iterator>

namespace DuiLib
{
	void CMultiLanguageUI::Clear()
	{
		m_items.clear();
	}

	bool CMultiLanguageUI::LoadFromFile(LPCTSTR pstrFile)
	{
		if( pstrFile == NULL || *pstrFile == _T('\0') ) return false;

		std::ifstream input(std::filesystem::path(pstrFile), std::ios::binary);
		if( !input.is_open() ) return false;

		DuiUtf8String text((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
		return LoadFromUtf8Text(text);
	}

	bool CMultiLanguageUI::LoadFromUtf8Text(std::string_view text)
	{
		Clear();
		if( text.size() >= 3 &&
			static_cast<unsigned char>(text[0]) == 0xEF &&
			static_cast<unsigned char>(text[1]) == 0xBB &&
			static_cast<unsigned char>(text[2]) == 0xBF ) {
			text.remove_prefix(3);
		}

		size_t lineBegin = 0;
		while( lineBegin <= text.size() ) {
			size_t lineEnd = text.find_first_of("\r\n", lineBegin);
			if( lineEnd == std::string_view::npos ) lineEnd = text.size();

			DuiUtf8String line = TrimUtf8(text.substr(lineBegin, lineEnd - lineBegin));
			if( !line.empty() && line[0] != '#' && line[0] != ';' ) {
				size_t eq = line.find('=');
				if( eq != DuiUtf8String::npos ) {
					DuiUtf8String key = TrimUtf8(std::string_view(line).substr(0, eq));
					DuiUtf8String value = TrimUtf8(std::string_view(line).substr(eq + 1));
					if( !key.empty() ) SetTextUtf8(key, UnescapeUtf8(value));
				}
			}

			if( lineEnd == text.size() ) break;
			lineBegin = lineEnd + 1;
			if( lineBegin < text.size() && text[lineEnd] == '\r' && text[lineBegin] == '\n' ) ++lineBegin;
		}
		return true;
	}

	void CMultiLanguageUI::SetTextUtf8(std::string_view key, std::string_view value)
	{
		if( key.empty() ) return;
		m_items[DuiUtf8ToNative(key)] = DuiUtf8ToNative(value);
	}

	bool CMultiLanguageUI::GetText(LPCTSTR pstrKey, tstring& value) const
	{
		if( pstrKey == NULL || *pstrKey == _T('\0') ) return false;
		auto it = m_items.find(pstrKey);
		if( it == m_items.end() || it->second.empty() ) return false;
		value = it->second;
		return true;
	}

	int CMultiLanguageUI::Apply(CControlUI* pRoot) const
	{
		if( pRoot == NULL ) return 0;
		return ApplyControl(pRoot);
	}

	int CMultiLanguageUI::Apply(CPaintManagerUI* pManager) const
	{
		if( pManager == NULL ) return 0;
		return Apply(pManager->GetRoot());
	}

	DuiUtf8String CMultiLanguageUI::TrimUtf8(std::string_view text)
	{
		size_t begin = 0;
		while( begin < text.size() && (text[begin] == ' ' || text[begin] == '\t') ) ++begin;
		size_t end = text.size();
		while( end > begin && (text[end - 1] == ' ' || text[end - 1] == '\t') ) --end;
		return DuiUtf8String(text.substr(begin, end - begin));
	}

	DuiUtf8String CMultiLanguageUI::UnescapeUtf8(std::string_view text)
	{
		DuiUtf8String result;
		result.reserve(text.size());
		for( size_t i = 0; i < text.size(); ++i ) {
			if( text[i] == '\\' && i + 1 < text.size() ) {
				++i;
				if( text[i] == 'n' ) result.push_back('\n');
				else if( text[i] == 'r' ) result.push_back('\r');
				else if( text[i] == 't' ) result.push_back('\t');
				else result.push_back(text[i]);
			}
			else {
				result.push_back(text[i]);
			}
		}
		return result;
	}

	int CMultiLanguageUI::ApplyControl(CControlUI* pControl) const
	{
		int count = 0;
		const tstring name = pControl->GetName();
		if( !name.empty() ) {
			tstring value;
			if( GetText(name.c_str(), value) ) {
				pControl->SetText(value.c_str());
				++count;
			}

			tstring tipKey = name + _T("_tip");
			if( GetText(tipKey.c_str(), value) ) {
				pControl->SetToolTip(value.c_str());
				++count;
			}
		}

		IContainerUI* pContainer = static_cast<IContainerUI*>(pControl->GetInterface(_T("IContainer")));
		if( pContainer != NULL ) {
			for( int i = 0; i < pContainer->GetCount(); ++i ) {
				CControlUI* pChild = pContainer->GetItemAt(i);
				if( pChild != NULL ) count += ApplyControl(pChild);
			}
		}
		return count;
	}
}
