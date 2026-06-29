#ifndef __UTILS_H__
#define __UTILS_H__

#pragma once

#include <cstdarg>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace DuiLib
{
#ifdef _UNICODE
	using tstring_base = std::wstring;
#else
	using tstring_base = std::string;
#endif
	using tstring = tstring_base;
	using tstring_view = std::basic_string_view<TCHAR>;

	/////////////////////////////////////////////////////////////////////////////////////
	//

	UILIB_API LPCTSTR DuiSafeString(LPCTSTR text);
	UILIB_API int DuiStringLength(const tstring& text);
	UILIB_API tstring DuiStringAssign(LPCTSTR text, int length = -1);
	UILIB_API int DuiStringFind(tstring_view text, TCHAR ch, int pos = 0);
	UILIB_API int DuiStringFind(tstring_view text, LPCTSTR value, int pos = 0);
	UILIB_API int DuiStringReverseFind(tstring_view text, TCHAR ch);
	UILIB_API tstring DuiStringLeft(tstring_view text, int length);
	UILIB_API tstring DuiStringMid(tstring_view text, int pos, int length = -1);
	UILIB_API tstring DuiStringRight(tstring_view text, int length);
	UILIB_API int DuiStringReplace(tstring& text, LPCTSTR from, LPCTSTR to);
	UILIB_API void DuiStringMakeUpper(tstring& text);
	UILIB_API void DuiStringMakeLower(tstring& text);
	UILIB_API int DuiStringFormatV(tstring& text, LPCTSTR format, va_list args);
	UILIB_API int __cdecl DuiStringFormat(tstring& text, LPCTSTR format, ...);
	UILIB_API int __cdecl DuiStringSmallFormat(tstring& text, LPCTSTR format, ...);

	/////////////////////////////////////////////////////////////////////////////////////
	//

	class STRINGorID
	{
	public:
		STRINGorID(LPCTSTR lpString) : m_lpstr(lpString)
		{ }
		STRINGorID(UINT nID) : m_lpstr(MAKEINTRESOURCE(nID))
		{ }
		LPCTSTR m_lpstr;
	};

	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API CDuiRect : public tagRECT
	{
	public:
		CDuiRect();
		CDuiRect(const RECT& src);
		CDuiRect(int iLeft, int iTop, int iRight, int iBottom);

		int GetWidth() const;
		int GetHeight() const;
		void Empty();
		bool IsNull() const;
		void Join(const RECT& rc);
		void ResetOffset();
		void Normalize();
		void Offset(int cx, int cy);
		void Inflate(int cx, int cy);
		void Deflate(int cx, int cy);
		void Union(CDuiRect& rc);
	};

	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API CStdPtrArray
	{
	public:
		CStdPtrArray(int iPreallocSize = 0);
		CStdPtrArray(const CStdPtrArray& src);
		~CStdPtrArray();

		void Empty();
		void Resize(int iSize);
		bool IsEmpty() const;
		int Find(LPVOID iIndex) const;
		bool Add(LPVOID pData);
		bool SetAt(int iIndex, LPVOID pData);
		bool InsertAt(int iIndex, LPVOID pData);
		bool Remove(int iIndex);
		int GetSize() const;
		LPVOID* GetData();

		LPVOID GetAt(int iIndex) const;
		LPVOID operator[] (int nIndex) const;

	protected:
		LPVOID* m_ppVoid;
		int m_nCount;
		int m_nAllocated;
	};


	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API CStdValArray
	{
	public:
		CStdValArray(int iElementSize, int iPreallocSize = 0);
		~CStdValArray();

		void Empty();
		bool IsEmpty() const;
		bool Add(LPCVOID pData);
		bool Remove(int iIndex);
		int GetSize() const;
		LPVOID GetData();

		LPVOID GetAt(int iIndex) const;
		LPVOID operator[] (int nIndex) const;

	protected:
		LPBYTE m_pVoid;
		int m_iElementSize;
		int m_nCount;
		int m_nAllocated;
	};


	/////////////////////////////////////////////////////////////////////////////////////
	//

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif
	class UILIB_API CStdStringPtrMap
	{
	public:
		CStdStringPtrMap(int nSize = 83);
		~CStdStringPtrMap();

		void Resize(int nSize = 83);
		LPVOID Find(LPCTSTR key, bool optimize = true) const;
		bool Insert(LPCTSTR key, LPVOID pData);
		LPVOID Set(LPCTSTR key, LPVOID pData);
		bool Remove(LPCTSTR key);
		void RemoveAll();
		int GetSize() const;
		LPCTSTR GetAt(int iIndex) const;
		LPCTSTR operator[] (int nIndex) const;

	protected:
		struct TstringHash
		{
			size_t operator()(const tstring& value) const noexcept
			{
				return std::hash<tstring_base>{}(value);
			}
		};

		int m_nBuckets;
		bool m_bEnabled;
		std::unordered_map<tstring, LPVOID, TstringHash> m_items;
		std::vector<tstring> m_keys;
	};
#ifdef _MSC_VER
#pragma warning(pop)
#endif

	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API CWaitCursor
	{
	public:
		CWaitCursor();
		~CWaitCursor();

	protected:
		HCURSOR m_hOrigCursor;
	};

	/////////////////////////////////////////////////////////////////////////////////////
	//

	class CVariant : public VARIANT
	{
	public:
		CVariant() 
		{ 
			VariantInit(this); 
		}
		CVariant(int i)
		{
			VariantInit(this);
			this->vt = VT_I4;
			this->intVal = i;
		}
		CVariant(float f)
		{
			VariantInit(this);
			this->vt = VT_R4;
			this->fltVal = f;
		}
		CVariant(LPOLESTR s)
		{
			VariantInit(this);
			this->vt = VT_BSTR;
			this->bstrVal = s;
		}
		CVariant(IDispatch *disp)
		{
			VariantInit(this);
			this->vt = VT_DISPATCH;
			this->pdispVal = disp;
		}

		~CVariant() 
		{ 
			VariantClear(this); 
		}
	};

}

#endif // __UTILS_H__
