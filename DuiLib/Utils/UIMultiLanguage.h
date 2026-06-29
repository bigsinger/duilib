#pragma once

#include "UIStringUtil.h"

#include <string_view>
#include <unordered_map>

namespace DuiLib
{
	class CControlUI;
	class CPaintManagerUI;

	class UILIB_API CMultiLanguageUI
	{
	public:
		void Clear();
		bool LoadFromFile(LPCTSTR pstrFile);
		bool LoadFromUtf8Text(std::string_view text);
		void SetTextUtf8(std::string_view key, std::string_view value);
		bool GetText(LPCTSTR pstrKey, tstring& value) const;
		int Apply(CControlUI* pRoot) const;
		int Apply(CPaintManagerUI* pManager) const;

	private:
		static DuiUtf8String TrimUtf8(std::string_view text);
		static DuiUtf8String UnescapeUtf8(std::string_view text);
		int ApplyControl(CControlUI* pControl) const;

	private:
		std::unordered_map<tstring, tstring> m_items;
	};
}
