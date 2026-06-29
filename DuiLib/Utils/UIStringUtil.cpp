#include "stdafx.h"
#include "UIStringUtil.h"

namespace DuiLib
{
	namespace
	{
		std::string WideToCodePage(std::wstring_view text, UINT codePage)
		{
			if (text.empty()) return {};

			const int size = ::WideCharToMultiByte(
				codePage,
				0,
				text.data(),
				static_cast<int>(text.size()),
				nullptr,
				0,
				nullptr,
				nullptr);
			if (size <= 0) return {};

			std::string result(static_cast<size_t>(size), '\0');
			::WideCharToMultiByte(
				codePage,
				0,
				text.data(),
				static_cast<int>(text.size()),
				result.data(),
				size,
				nullptr,
				nullptr);
			return result;
		}

		std::wstring CodePageToWide(std::string_view text, UINT codePage)
		{
			if (text.empty()) return {};

			const int size = ::MultiByteToWideChar(
				codePage,
				0,
				text.data(),
				static_cast<int>(text.size()),
				nullptr,
				0);
			if (size <= 0) return {};

			std::wstring result(static_cast<size_t>(size), L'\0');
			::MultiByteToWideChar(
				codePage,
				0,
				text.data(),
				static_cast<int>(text.size()),
				result.data(),
				size);
			return result;
		}
	}

	DuiWideString DuiUtf8ToWide(std::string_view text)
	{
		return CodePageToWide(text, CP_UTF8);
	}

	DuiUtf8String DuiWideToUtf8(std::wstring_view text)
	{
		return WideToCodePage(text, CP_UTF8);
	}

	DuiNativeString DuiWideToNative(std::wstring_view text)
	{
#ifdef _UNICODE
		return DuiWideString(text);
#else
		return WideToCodePage(text, CP_ACP);
#endif
	}

	DuiNativeString DuiUtf8ToNative(std::string_view text)
	{
#ifdef _UNICODE
		return DuiUtf8ToWide(text);
#else
		return WideToCodePage(DuiUtf8ToWide(text), CP_ACP);
#endif
	}

	DuiUtf8String DuiNativeToUtf8(LPCTSTR text)
	{
		if (text == nullptr || *text == _T('\0')) return {};

#ifdef _UNICODE
		return DuiWideToUtf8(text);
#else
		return DuiWideToUtf8(CodePageToWide(text, CP_ACP));
#endif
	}

	DuiUtf8String DuiStringToUtf8(tstring_view text)
	{
		if (text.empty()) return {};
#ifdef _UNICODE
		return DuiWideToUtf8(text);
#else
		return DuiWideToUtf8(CodePageToWide(std::string_view(text.data(), text.size()), CP_ACP));
#endif
	}

	tstring DuiStringFromUtf8(std::string_view text)
	{
		return DuiUtf8ToNative(text);
	}
}
