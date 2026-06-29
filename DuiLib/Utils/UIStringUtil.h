#pragma once

#include "Utils.h"

#include <functional>
#include <string>
#include <string_view>

namespace DuiLib
{
	using DuiUtf8String = std::string;
	using DuiWideString = std::wstring;

#ifdef _UNICODE
	using DuiNativeString = tstring;
#else
	using DuiNativeString = tstring;
#endif

	using DuiTextGetter = std::function<DuiUtf8String()>;
	using DuiTextSetter = std::function<void(std::string_view)>;

	UILIB_API DuiWideString DuiUtf8ToWide(std::string_view text);
	UILIB_API DuiUtf8String DuiWideToUtf8(std::wstring_view text);
	UILIB_API DuiNativeString DuiWideToNative(std::wstring_view text);
	UILIB_API DuiNativeString DuiUtf8ToNative(std::string_view text);
	UILIB_API DuiUtf8String DuiNativeToUtf8(LPCTSTR text);
	UILIB_API DuiUtf8String DuiStringToUtf8(tstring_view text);
	UILIB_API tstring DuiStringFromUtf8(std::string_view text);
}
