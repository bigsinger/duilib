#pragma once

namespace DuiLib {

bool IsSvgImageData(const BYTE* data, DWORD size) noexcept;
TImageInfo* LoadSvgImageFromMemory(const BYTE* data, DWORD size);

} // namespace DuiLib
