#pragma once

#include "UIStringUtil.h"

#include <string_view>
#include <unordered_map>
#include <functional>

namespace DuiLib
{
	class CControlUI;
	class CPaintManagerUI;

	class UILIB_API CMultiLanguageUI
	{
	public:
		using TextResolver = std::function<bool(LPCTSTR, tstring&)>;

		void Clear();
		bool LoadFromFile(LPCTSTR pstrFile);
		bool LoadFromUtf8Text(std::string_view text);
		void SetTextUtf8(std::string_view key, std::string_view value);
		bool GetText(LPCTSTR pstrKey, tstring& value) const;
		int Apply(CControlUI* pRoot) const;
		int Apply(CPaintManagerUI* pManager) const;
		// Applies text supplied by a host-owned language store while keeping tree
		// traversal and the name/name_tip convention inside DuiLib.
		static int ApplyResolved(CControlUI* pRoot, const TextResolver& resolver);
		static int ApplyResolved(CPaintManagerUI* pManager, const TextResolver& resolver);

	private:
		static DuiUtf8String TrimUtf8(std::string_view text);
		static DuiUtf8String UnescapeUtf8(std::string_view text);
		int ApplyControl(CControlUI* pControl) const;
		static int ApplyResolvedControl(CControlUI* pControl, const TextResolver& resolver);

	private:
		std::unordered_map<tstring, tstring> m_items;
	};
}
