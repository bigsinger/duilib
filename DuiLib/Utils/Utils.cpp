#include "stdafx.h"
#include "Utils.h"

#include <algorithm>
#include <cctype>
#include <cwctype>
#include <utility>
#include <vector>

namespace DuiLib
{

	/////////////////////////////////////////////////////////////////////////////////////
	//
	//

	CDuiRect::CDuiRect()
	{
		left = top = right = bottom = 0;
	}

	CDuiRect::CDuiRect(const RECT& src)
	{
		left = src.left;
		top = src.top;
		right = src.right;
		bottom = src.bottom;
	}

	CDuiRect::CDuiRect(int iLeft, int iTop, int iRight, int iBottom)
	{
		left = iLeft;
		top = iTop;
		right = iRight;
		bottom = iBottom;
	}

	int CDuiRect::GetWidth() const
	{
		return right - left;
	}

	int CDuiRect::GetHeight() const
	{
		return bottom - top;
	}

	void CDuiRect::Empty()
	{
		left = top = right = bottom = 0;
	}

	bool CDuiRect::IsNull() const
	{
		return (left == 0 && right == 0 && top == 0 && bottom == 0); 
	}

	void CDuiRect::Join(const RECT& rc)
	{
		if( rc.left < left ) left = rc.left;
		if( rc.top < top ) top = rc.top;
		if( rc.right > right ) right = rc.right;
		if( rc.bottom > bottom ) bottom = rc.bottom;
	}

	void CDuiRect::ResetOffset()
	{
		::OffsetRect(this, -left, -top);
	}

	void CDuiRect::Normalize()
	{
		if( left > right ) { int iTemp = left; left = right; right = iTemp; }
		if( top > bottom ) { int iTemp = top; top = bottom; bottom = iTemp; }
	}

	void CDuiRect::Offset(int cx, int cy)
	{
		::OffsetRect(this, cx, cy);
	}

	void CDuiRect::Inflate(int cx, int cy)
	{
		::InflateRect(this, cx, cy);
	}

	void CDuiRect::Deflate(int cx, int cy)
	{
		::InflateRect(this, -cx, -cy);
	}

	void CDuiRect::Union(CDuiRect& rc)
	{
		::UnionRect(this, this, &rc);
	}


	/////////////////////////////////////////////////////////////////////////////////////
	//
	//

	CStdPtrArray::CStdPtrArray(int iPreallocSize) : m_ppVoid(NULL), m_nCount(0), m_nAllocated(iPreallocSize)
	{
		ASSERT(iPreallocSize>=0);
		if( iPreallocSize > 0 ) m_ppVoid = static_cast<LPVOID*>(malloc(iPreallocSize * sizeof(LPVOID)));
	}

	CStdPtrArray::CStdPtrArray(const CStdPtrArray& src) : m_ppVoid(NULL), m_nCount(0), m_nAllocated(0)
	{
		for(int i=0; i<src.GetSize(); i++)
			Add(src.GetAt(i));
	}

	CStdPtrArray::~CStdPtrArray()
	{
		if( m_ppVoid != NULL ) free(m_ppVoid);
	}

	void CStdPtrArray::Empty()
	{
		if( m_ppVoid != NULL ) free(m_ppVoid);
		m_ppVoid = NULL;
		m_nCount = m_nAllocated = 0;
	}

	void CStdPtrArray::Resize(int iSize)
	{
		Empty();
		m_ppVoid = static_cast<LPVOID*>(malloc(iSize * sizeof(LPVOID)));
		::ZeroMemory(m_ppVoid, iSize * sizeof(LPVOID));
		m_nAllocated = iSize;
		m_nCount = iSize;
	}

	bool CStdPtrArray::IsEmpty() const
	{
		return m_nCount == 0;
	}

	bool CStdPtrArray::Add(LPVOID pData)
	{
		if( ++m_nCount >= m_nAllocated) {
			int nAllocated = m_nAllocated * 2;
			if( nAllocated == 0 ) nAllocated = 11;
			LPVOID* ppVoid = static_cast<LPVOID*>(realloc(m_ppVoid, nAllocated * sizeof(LPVOID)));
			if( ppVoid != NULL ) {
				m_nAllocated = nAllocated;
				m_ppVoid = ppVoid;
			}
			else {
				--m_nCount;
				return false;
			}
		}
		m_ppVoid[m_nCount - 1] = pData;
		return true;
	}

	bool CStdPtrArray::InsertAt(int iIndex, LPVOID pData)
	{
		if( iIndex == m_nCount ) return Add(pData);
		if( iIndex < 0 || iIndex > m_nCount ) return false;
		if( ++m_nCount >= m_nAllocated) {
			int nAllocated = m_nAllocated * 2;
			if( nAllocated == 0 ) nAllocated = 11;
			LPVOID* ppVoid = static_cast<LPVOID*>(realloc(m_ppVoid, nAllocated * sizeof(LPVOID)));
			if( ppVoid != NULL ) {
				m_nAllocated = nAllocated;
				m_ppVoid = ppVoid;
			}
			else {
				--m_nCount;
				return false;
			}
		}
		memmove(&m_ppVoid[iIndex + 1], &m_ppVoid[iIndex], (m_nCount - iIndex - 1) * sizeof(LPVOID));
		m_ppVoid[iIndex] = pData;
		return true;
	}

	bool CStdPtrArray::SetAt(int iIndex, LPVOID pData)
	{
		if( iIndex < 0 || iIndex >= m_nCount ) return false;
		m_ppVoid[iIndex] = pData;
		return true;
	}

	bool CStdPtrArray::Remove(int iIndex)
	{
		if( iIndex < 0 || iIndex >= m_nCount ) return false;
		if( iIndex < --m_nCount ) ::CopyMemory(m_ppVoid + iIndex, m_ppVoid + iIndex + 1, (m_nCount - iIndex) * sizeof(LPVOID));
		return true;
	}

	int CStdPtrArray::Find(LPVOID pData) const
	{
		for( int i = 0; i < m_nCount; i++ ) if( m_ppVoid[i] == pData ) return i;
		return -1;
	}

	int CStdPtrArray::GetSize() const
	{
		return m_nCount;
	}

	LPVOID* CStdPtrArray::GetData()
	{
		return m_ppVoid;
	}

	LPVOID CStdPtrArray::GetAt(int iIndex) const
	{
		if( iIndex < 0 || iIndex >= m_nCount ) return NULL;
		return m_ppVoid[iIndex];
	}

	LPVOID CStdPtrArray::operator[] (int iIndex) const
	{
		ASSERT(iIndex>=0 && iIndex<m_nCount);
		return m_ppVoid[iIndex];
	}


	/////////////////////////////////////////////////////////////////////////////////////
	//
	//

	CStdValArray::CStdValArray(int iElementSize, int iPreallocSize /*= 0*/) : 
	m_pVoid(NULL), 
		m_nCount(0), 
		m_iElementSize(iElementSize), 
		m_nAllocated(iPreallocSize)
	{
		ASSERT(iElementSize>0);
		ASSERT(iPreallocSize>=0);
		if( iPreallocSize > 0 ) m_pVoid = static_cast<LPBYTE>(malloc(iPreallocSize * m_iElementSize));
	}

	CStdValArray::~CStdValArray()
	{
		if( m_pVoid != NULL ) free(m_pVoid);
	}

	void CStdValArray::Empty()
	{   
		m_nCount = 0;
	}

	bool CStdValArray::IsEmpty() const
	{
		return m_nCount == 0;
	}

	bool CStdValArray::Add(LPCVOID pData)
	{
		if( ++m_nCount >= m_nAllocated) {
			int nAllocated = m_nAllocated * 2;
			if( nAllocated == 0 ) nAllocated = 11;
			LPBYTE pVoid = static_cast<LPBYTE>(realloc(m_pVoid, nAllocated * m_iElementSize));
			if( pVoid != NULL ) {
				m_nAllocated = nAllocated;
				m_pVoid = pVoid;
			}
			else {
				--m_nCount;
				return false;
			}
		}
		::CopyMemory(m_pVoid + ((m_nCount - 1) * m_iElementSize), pData, m_iElementSize);
		return true;
	}

	bool CStdValArray::Remove(int iIndex)
	{
		if( iIndex < 0 || iIndex >= m_nCount ) return false;
		if( iIndex < --m_nCount ) ::CopyMemory(m_pVoid + (iIndex * m_iElementSize), m_pVoid + ((iIndex + 1) * m_iElementSize), (m_nCount - iIndex) * m_iElementSize);
		return true;
	}

	int CStdValArray::GetSize() const
	{
		return m_nCount;
	}

	LPVOID CStdValArray::GetData()
	{
		return static_cast<LPVOID>(m_pVoid);
	}

	LPVOID CStdValArray::GetAt(int iIndex) const
	{
		if( iIndex < 0 || iIndex >= m_nCount ) return NULL;
		return m_pVoid + (iIndex * m_iElementSize);
	}

	LPVOID CStdValArray::operator[] (int iIndex) const
	{
		ASSERT(iIndex>=0 && iIndex<m_nCount);
		return m_pVoid + (iIndex * m_iElementSize);
	}


	/////////////////////////////////////////////////////////////////////////////////////
	//
	//

	LPCTSTR DuiSafeString(LPCTSTR text)
	{
		return text ? text : _T("");
	}

	int DuiStringLength(const tstring& text)
	{
		return static_cast<int>(text.length());
	}

	tstring DuiStringAssign(LPCTSTR text, int length)
	{
		text = DuiSafeString(text);
		const int textLength = length < 0 ? static_cast<int>(_tcslen(text)) : length;
		return tstring(text, static_cast<tstring::size_type>(textLength));
	}

	int DuiStringFind(tstring_view text, TCHAR ch, int pos)
	{
		if (pos < 0 || pos > static_cast<int>(text.length())) return -1;
		const auto found = text.find(ch, static_cast<tstring_view::size_type>(pos));
		return found == tstring_view::npos ? -1 : static_cast<int>(found);
	}

	int DuiStringFind(tstring_view text, LPCTSTR value, int pos)
	{
		if (!value || pos < 0 || pos > static_cast<int>(text.length())) return -1;
		const auto found = text.find(value, static_cast<tstring_view::size_type>(pos));
		return found == tstring_view::npos ? -1 : static_cast<int>(found);
	}

	int DuiStringReverseFind(tstring_view text, TCHAR ch)
	{
		const auto found = text.rfind(ch);
		return found == tstring_view::npos ? -1 : static_cast<int>(found);
	}

	tstring DuiStringLeft(tstring_view text, int length)
	{
		if (length < 0) length = 0;
		if (length > static_cast<int>(text.length())) length = static_cast<int>(text.length());
		return tstring(text.substr(0, static_cast<tstring_view::size_type>(length)));
	}

	tstring DuiStringMid(tstring_view text, int pos, int length)
	{
		if (pos < 0) pos = 0;
		if (pos > static_cast<int>(text.length())) return {};
		if (length < 0 || pos + length > static_cast<int>(text.length())) length = static_cast<int>(text.length()) - pos;
		if (length <= 0) return {};
		return tstring(text.substr(static_cast<tstring_view::size_type>(pos), static_cast<tstring_view::size_type>(length)));
	}

	tstring DuiStringRight(tstring_view text, int length)
	{
		if (length < 0) length = 0;
		if (length > static_cast<int>(text.length())) length = static_cast<int>(text.length());
		return tstring(text.substr(text.length() - static_cast<tstring_view::size_type>(length)));
	}

	int DuiStringReplace(tstring& text, LPCTSTR from, LPCTSTR to)
	{
		from = DuiSafeString(from);
		to = DuiSafeString(to);
		const tstring::size_type fromLength = _tcslen(from);
		if (fromLength == 0) return 0;

		const tstring::size_type toLength = _tcslen(to);
		int count = 0;
		tstring::size_type pos = text.find(from);
		while (pos != tstring::npos) {
			text.replace(pos, fromLength, to);
			pos = text.find(from, pos + toLength);
			++count;
		}
		return count;
	}

	void DuiStringMakeUpper(tstring& text)
	{
		std::transform(text.begin(), text.end(), text.begin(), [](TCHAR ch) -> TCHAR {
#ifdef _UNICODE
			return static_cast<TCHAR>(std::towupper(ch));
#else
			return static_cast<TCHAR>(std::toupper(static_cast<unsigned char>(ch)));
#endif
		});
	}

	void DuiStringMakeLower(tstring& text)
	{
		std::transform(text.begin(), text.end(), text.begin(), [](TCHAR ch) -> TCHAR {
#ifdef _UNICODE
			return static_cast<TCHAR>(std::towlower(ch));
#else
			return static_cast<TCHAR>(std::tolower(static_cast<unsigned char>(ch)));
#endif
		});
	}

	int DuiStringFormatV(tstring& text, LPCTSTR format, va_list args)
	{
		if (!format) {
			text.clear();
			return 0;
		}

		va_list argsCopy;
		va_copy(argsCopy, args);
		const int length = _vsctprintf(format, argsCopy);
		va_end(argsCopy);
		if (length < 0) {
			text.clear();
			return length;
		}

		std::vector<TCHAR> buffer(static_cast<size_t>(length) + 1, _T('\0'));
		int written = _vsntprintf_s(buffer.data(), buffer.size(), _TRUNCATE, format, args);
		if (written < 0) written = static_cast<int>(_tcslen(buffer.data()));
		text.assign(buffer.data(), static_cast<tstring::size_type>(written));
		return written;
	}

	int DuiStringFormat(tstring& text, LPCTSTR format, ...)
	{
		va_list args;
		va_start(args, format);
		const int result = DuiStringFormatV(text, format, args);
		va_end(args);
		return result;
	}

	int DuiStringSmallFormat(tstring& text, LPCTSTR format, ...)
	{
		TCHAR buffer[64] = { 0 };
		va_list args;
		va_start(args, format);
		const int written = _vsntprintf_s(buffer, _countof(buffer), _TRUNCATE, format, args);
		va_end(args);
		text.assign(buffer);
		return written;
	}

	/////////////////////////////////////////////////////////////////////////////
	//
	//

	CStdStringPtrMap::CStdStringPtrMap(int nSize) : m_nBuckets(0), m_bEnabled(false)
	{
		Resize(nSize);
	}

	CStdStringPtrMap::~CStdStringPtrMap()
	{
		RemoveAll();
	}

	void CStdStringPtrMap::RemoveAll()
	{
		m_items.clear();
		m_keys.clear();
	}

	void CStdStringPtrMap::Resize(int nSize)
	{
		RemoveAll();
		if( nSize <= 0 ) {
			m_nBuckets = 0;
			m_bEnabled = false;
			return;
		}

		if( nSize < 16 ) nSize = 16;
		m_nBuckets = nSize;
		m_bEnabled = true;
		m_items.reserve(static_cast<size_t>(m_nBuckets));
		m_keys.reserve(static_cast<size_t>(m_nBuckets));
	}

	LPVOID CStdStringPtrMap::Find(LPCTSTR key, bool optimize) const
	{
		(void)optimize;
		if( !m_bEnabled || key == nullptr || GetSize() == 0 ) return NULL;

		auto it = m_items.find(tstring(key));
		return it == m_items.end() ? NULL : it->second;
	}

	bool CStdStringPtrMap::Insert(LPCTSTR key, LPVOID pData)
	{
		if( !m_bEnabled || key == nullptr ) return false;

		tstring itemKey(key);
		auto inserted = m_items.emplace(itemKey, pData);
		if( !inserted.second ) return false;

		m_keys.push_back(std::move(itemKey));
		return true;
	}

	LPVOID CStdStringPtrMap::Set(LPCTSTR key, LPVOID pData)
	{
		if( !m_bEnabled || key == nullptr ) return pData;

		tstring itemKey(key);
		auto it = m_items.find(itemKey);
		if( it != m_items.end() ) {
			LPVOID pOldData = it->second;
			it->second = pData;
			return pOldData;
		}

		m_items.emplace(itemKey, pData);
		m_keys.push_back(std::move(itemKey));
		return NULL;
	}

	bool CStdStringPtrMap::Remove(LPCTSTR key)
	{
		if( !m_bEnabled || key == nullptr || GetSize() == 0 ) return false;

		tstring itemKey(key);
		if( m_items.erase(itemKey) == 0 ) return false;

		auto it = std::find(m_keys.begin(), m_keys.end(), itemKey);
		if( it != m_keys.end() ) {
			m_keys.erase(it);
		}
		return true;
	}

	int CStdStringPtrMap::GetSize() const
	{
		return static_cast<int>(m_keys.size());
	}

	LPCTSTR CStdStringPtrMap::GetAt(int iIndex) const
	{
		if( !m_bEnabled || iIndex < 0 || static_cast<size_t>(iIndex) >= m_keys.size() ) return nullptr;
		return m_keys[static_cast<size_t>(iIndex)].c_str();
	}

	LPCTSTR CStdStringPtrMap::operator[] (int nIndex) const
	{
		return GetAt(nIndex);
	}


	/////////////////////////////////////////////////////////////////////////////////////
	//
	//

	CWaitCursor::CWaitCursor()
	{
		m_hOrigCursor = ::SetCursor(::LoadCursor(NULL, IDC_WAIT));
	}

	CWaitCursor::~CWaitCursor()
	{
		::SetCursor(m_hOrigCursor);
	}

}
