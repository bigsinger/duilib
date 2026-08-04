#include "StdAfx.h"
#include "UISvg.h"

#pragma push_macro("NTDDI_VERSION")
#undef NTDDI_VERSION
#define NTDDI_VERSION NTDDI_WIN10_RS2
#include <d2d1_3.h>
#pragma pop_macro("NTDDI_VERSION")
#include <d3d11.h>
#include <dxgi1_2.h>

#include <cstring>
#include <mutex>
#include <utility>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

namespace DuiLib {

namespace {

constexpr UINT kSvgRasterWidth = 64;
constexpr UINT kSvgRasterHeight = 64;

template<typename T>
class ComPtr final
{
public:
    ComPtr() noexcept = default;
    ~ComPtr() { Reset(); }
    ComPtr(const ComPtr&) = delete;
    ComPtr& operator=(const ComPtr&) = delete;
    ComPtr(ComPtr&& other) noexcept : value_(std::exchange(other.value_, nullptr)) {}
    ComPtr& operator=(ComPtr&& other) noexcept
    {
        if (this != &other) {
            Reset();
            value_ = std::exchange(other.value_, nullptr);
        }
        return *this;
    }

    T* Get() const noexcept { return value_; }
    T* operator->() const noexcept { return value_; }
    T** Put() noexcept
    {
        Reset();
        return &value_;
    }

    void Reset() noexcept
    {
        if (value_ != nullptr) value_->Release();
        value_ = nullptr;
    }

    template<typename U>
    HRESULT As(ComPtr<U>& result) const noexcept
    {
        if (value_ == nullptr) return E_POINTER;
        return value_->QueryInterface(
            __uuidof(U), reinterpret_cast<void**>(result.Put()));
    }

private:
    T* value_ = nullptr;
};

class SvgRasterizer final
{
public:
    TImageInfo* Rasterize(const BYTE* data, DWORD size)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!Initialize()) return nullptr;

        ComPtr<IStream> stream;
        HGLOBAL memory = ::GlobalAlloc(GMEM_MOVEABLE, size);
        if (memory == nullptr) return nullptr;
        void* destination = ::GlobalLock(memory);
        if (destination == nullptr) {
            ::GlobalFree(memory);
            return nullptr;
        }
        std::memcpy(destination, data, size);
        ::GlobalUnlock(memory);
        if (FAILED(::CreateStreamOnHGlobal(memory, TRUE, stream.Put()))) {
            ::GlobalFree(memory);
            return nullptr;
        }

        D3D11_TEXTURE2D_DESC renderDescription = {};
        renderDescription.Width = kSvgRasterWidth;
        renderDescription.Height = kSvgRasterHeight;
        renderDescription.MipLevels = 1;
        renderDescription.ArraySize = 1;
        renderDescription.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        renderDescription.SampleDesc.Count = 1;
        renderDescription.Usage = D3D11_USAGE_DEFAULT;
        renderDescription.BindFlags = D3D11_BIND_RENDER_TARGET;

        ComPtr<ID3D11Texture2D> renderTexture;
        if (FAILED(d3dDevice_->CreateTexture2D(
            &renderDescription, nullptr, renderTexture.Put()))) {
            return nullptr;
        }

        ComPtr<IDXGISurface> surface;
        if (FAILED(renderTexture.As(surface))) return nullptr;

        D2D1_BITMAP_PROPERTIES1 bitmapProperties = {};
        bitmapProperties.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
        bitmapProperties.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
        bitmapProperties.dpiX = 96.0f;
        bitmapProperties.dpiY = 96.0f;
        bitmapProperties.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET |
            D2D1_BITMAP_OPTIONS_CANNOT_DRAW;

        ComPtr<ID2D1Bitmap1> target;
        if (FAILED(d2dContext_->CreateBitmapFromDxgiSurface(
            surface.Get(), &bitmapProperties, target.Put()))) {
            return nullptr;
        }

        ComPtr<ID2D1SvgDocument> document;
        if (FAILED(d2dContext_->CreateSvgDocument(stream.Get(),
            D2D1_SIZE_F{ static_cast<float>(kSvgRasterWidth),
                static_cast<float>(kSvgRasterHeight) }, document.Put()))) {
            return nullptr;
        }

        // Fixed root dimensions (for example 24x24) would otherwise occupy only
        // part of the raster surface. Let the viewBox fill the requested image.
        ComPtr<ID2D1SvgElement> root;
        document->GetRoot(root.Put());
        if (root.Get() != nullptr) {
            const D2D1_SVG_LENGTH fullSize = {
                100.0f, D2D1_SVG_LENGTH_UNITS_PERCENTAGE
            };
            root->SetAttributeValue(L"width", fullSize);
            root->SetAttributeValue(L"height", fullSize);
        }

        d2dContext_->SetTarget(target.Get());
        d2dContext_->SetTransform(D2D1_MATRIX_3X2_F{ 1, 0, 0, 1, 0, 0 });
        d2dContext_->BeginDraw();
        d2dContext_->Clear(D2D1_COLOR_F{ 0, 0, 0, 0 });
        d2dContext_->DrawSvgDocument(document.Get());
        const HRESULT drawResult = d2dContext_->EndDraw();
        d2dContext_->SetTarget(nullptr);
        if (FAILED(drawResult)) return nullptr;

        D3D11_TEXTURE2D_DESC stagingDescription = renderDescription;
        stagingDescription.Usage = D3D11_USAGE_STAGING;
        stagingDescription.BindFlags = 0;
        stagingDescription.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
        ComPtr<ID3D11Texture2D> stagingTexture;
        if (FAILED(d3dDevice_->CreateTexture2D(
            &stagingDescription, nullptr, stagingTexture.Put()))) {
            return nullptr;
        }
        d3dContext3d_->CopyResource(stagingTexture.Get(), renderTexture.Get());

        D3D11_MAPPED_SUBRESOURCE mapped = {};
        if (FAILED(d3dContext3d_->Map(stagingTexture.Get(), 0,
            D3D11_MAP_READ, 0, &mapped))) {
            return nullptr;
        }

        BITMAPINFO bitmapInfo = {};
        bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bitmapInfo.bmiHeader.biWidth = kSvgRasterWidth;
        bitmapInfo.bmiHeader.biHeight = -static_cast<LONG>(kSvgRasterHeight);
        bitmapInfo.bmiHeader.biPlanes = 1;
        bitmapInfo.bmiHeader.biBitCount = 32;
        bitmapInfo.bmiHeader.biCompression = BI_RGB;
        bitmapInfo.bmiHeader.biSizeImage =
            kSvgRasterWidth * kSvgRasterHeight * 4;

        BYTE* bitmapBits = nullptr;
        HBITMAP bitmap = ::CreateDIBSection(nullptr, &bitmapInfo,
            DIB_RGB_COLORS, reinterpret_cast<void**>(&bitmapBits), nullptr, 0);
        if (bitmap != nullptr && bitmapBits != nullptr) {
            const BYTE* source = static_cast<const BYTE*>(mapped.pData);
            const size_t rowBytes = static_cast<size_t>(kSvgRasterWidth) * 4;
            for (UINT row = 0; row < kSvgRasterHeight; ++row) {
                std::memcpy(bitmapBits + row * rowBytes,
                    source + row * mapped.RowPitch, rowBytes);
            }
        }
        d3dContext3d_->Unmap(stagingTexture.Get(), 0);
        if (bitmap == nullptr) return nullptr;

        TImageInfo* image = new TImageInfo;
        image->hBitmap = bitmap;
        image->nX = kSvgRasterWidth;
        image->nY = kSvgRasterHeight;
        image->alphaChannel = true;
        image->dwMask = 0;
        return image;
    }

private:
    bool Initialize()
    {
        if (initialized_) return available_;
        initialized_ = true;

        static const D3D_FEATURE_LEVEL featureLevels[] = {
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_1,
            D3D_FEATURE_LEVEL_10_0,
        };
        D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_10_0;
        HRESULT result = ::D3D11CreateDevice(nullptr,
            D3D_DRIVER_TYPE_HARDWARE, nullptr,
            D3D11_CREATE_DEVICE_BGRA_SUPPORT,
            featureLevels, _countof(featureLevels), D3D11_SDK_VERSION,
            d3dDevice_.Put(), &featureLevel, d3dContext3d_.Put());
        if (FAILED(result)) {
            result = ::D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_WARP,
                nullptr, D3D11_CREATE_DEVICE_BGRA_SUPPORT,
                featureLevels, _countof(featureLevels), D3D11_SDK_VERSION,
                d3dDevice_.Put(), &featureLevel, d3dContext3d_.Put());
        }
        if (FAILED(result)) return false;

        ComPtr<IDXGIDevice> dxgiDevice;
        if (FAILED(d3dDevice_.As(dxgiDevice))) return false;

        D2D1_FACTORY_OPTIONS factoryOptions = {};
        ComPtr<ID2D1Factory1> factory;
        if (FAILED(::D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,
            __uuidof(ID2D1Factory1), &factoryOptions,
            reinterpret_cast<void**>(factory.Put())))) {
            return false;
        }

        ComPtr<ID2D1Device> device;
        if (FAILED(factory->CreateDevice(dxgiDevice.Get(), device.Put()))) return false;

        ComPtr<ID2D1DeviceContext> context;
        if (FAILED(device->CreateDeviceContext(
            D2D1_DEVICE_CONTEXT_OPTIONS_NONE, context.Put()))) {
            return false;
        }
        if (FAILED(context.As(d2dContext_))) return false;

        available_ = true;
        return true;
    }

    std::mutex mutex_;
    bool initialized_ = false;
    bool available_ = false;
    ComPtr<ID3D11Device> d3dDevice_;
    ComPtr<ID3D11DeviceContext> d3dContext3d_;
    ComPtr<ID2D1DeviceContext5> d2dContext_;
};

} // namespace

bool IsSvgImageData(const BYTE* data, DWORD size) noexcept
{
    if (data == nullptr || size < 5) return false;
    DWORD offset = size >= 3 && data[0] == 0xEF && data[1] == 0xBB &&
        data[2] == 0xBF ? 3 : 0;
    while (offset < size && (data[offset] == ' ' || data[offset] == '\t' ||
        data[offset] == '\r' || data[offset] == '\n')) {
        ++offset;
    }
    if (offset + 4 >= size || data[offset] != '<') return false;

    const DWORD probeEnd = size < offset + 256 ? size : offset + 256;
    for (DWORD i = offset; i + 4 < probeEnd; ++i) {
        if (data[i] == '<' &&
            (data[i + 1] == 's' || data[i + 1] == 'S') &&
            (data[i + 2] == 'v' || data[i + 2] == 'V') &&
            (data[i + 3] == 'g' || data[i + 3] == 'G')) {
            return true;
        }
    }
    return false;
}

TImageInfo* LoadSvgImageFromMemory(const BYTE* data, DWORD size)
{
    static SvgRasterizer rasterizer;
    return rasterizer.Rasterize(data, size);
}

} // namespace DuiLib
