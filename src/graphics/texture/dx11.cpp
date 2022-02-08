/** @file
 * @brief テクスチャオブジェクト(DX11)
 * @author Takaaki Sato
 * @copyright (c) 2022 Demiquartz &lt;info@demiquartz.jp&gt; @n
 * Distributed under the MIT License (See accompanying file LICENSE
 * or copy at https://opensource.org/licenses/MIT)
 */
#include "dx11.hpp"

namespace Graphene::Graphics {

TextureDX11::TextureDX11(
    SharedImage                          image,
    Microsoft::WRL::ComPtr<ID3D11Device> device
) {
    DXGI_FORMAT format;
    switch (image->Format()) {
    case RGBAFP32: format = DXGI_FORMAT_R32G32B32A32_FLOAT; break;
    case RGBAFP16: format = DXGI_FORMAT_R16G16B16A16_FLOAT; break;
    case RGBAUN16: format = DXGI_FORMAT_R16G16B16A16_UNORM; break;
    case RGBA8888: format = DXGI_FORMAT_R8G8B8A8_UNORM;     break;
    case BGRA4444: format = DXGI_FORMAT_B4G4R4A4_UNORM;     break;
    case BGRA5551: format = DXGI_FORMAT_B5G5R5A1_UNORM;     break;
    case BGRA5650: format = DXGI_FORMAT_B5G6R5_UNORM;       break;
    default: throw std::runtime_error("GenerateTexture: Unsupported format.");
    }
    D3D11_SUBRESOURCE_DATA data;
    data.pSysMem          = image->Data();
    data.SysMemPitch      = image->Stride();
    data.SysMemSlicePitch = image->Stride() * image->Length(1);
    switch (image->Rank()) {
    case 1:
        {
            D3D11_TEXTURE1D_DESC desc;
            desc.Width              = image->Length(0);
            desc.MipLevels          = 1;
            desc.ArraySize          = 1;
            desc.Format             = format;
            desc.Usage              = D3D11_USAGE_IMMUTABLE;
            desc.BindFlags          = D3D11_BIND_SHADER_RESOURCE;
            desc.CPUAccessFlags     = 0;
            desc.MiscFlags          = 0;
            Microsoft::WRL::ComPtr<ID3D11Texture1D> texture;
            auto hr = device->CreateTexture1D(&desc, &data, texture.GetAddressOf());
            if (FAILED(hr)) throw std::system_error(hr, std::system_category());
            Texture_ = texture;
        }
        {
            D3D11_SHADER_RESOURCE_VIEW_DESC desc;
            desc.Format                    = format;
            desc.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE1D;
            desc.Texture1D.MostDetailedMip = 0;
            desc.Texture1D.MipLevels       = 1;
            auto hr = device->CreateShaderResourceView(Texture_.Get(), &desc, TextureView_.GetAddressOf());
            if (FAILED(hr)) throw std::system_error(hr, std::system_category());
        }
        break;
    case 2:
        {
            D3D11_TEXTURE2D_DESC desc;
            desc.Width              = image->Length(0);
            desc.Height             = image->Length(1);
            desc.MipLevels          = 1;
            desc.ArraySize          = 1;
            desc.Format             = format;
            desc.SampleDesc.Count   = 1;
            desc.SampleDesc.Quality = 0;
            desc.Usage              = D3D11_USAGE_IMMUTABLE;
            desc.BindFlags          = D3D11_BIND_SHADER_RESOURCE;
            desc.CPUAccessFlags     = 0;
            desc.MiscFlags          = 0;
            Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
            auto hr = device->CreateTexture2D(&desc, &data, texture.GetAddressOf());
            if (FAILED(hr)) throw std::system_error(hr, std::system_category());
            Texture_ = texture;
        }
        {
            D3D11_SHADER_RESOURCE_VIEW_DESC desc;
            desc.Format                    = format;
            desc.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE2D;
            desc.Texture2D.MostDetailedMip = 0;
            desc.Texture2D.MipLevels       = 1;
            auto hr = device->CreateShaderResourceView(Texture_.Get(), &desc, TextureView_.GetAddressOf());
            if (FAILED(hr)) throw std::system_error(hr, std::system_category());
        }
        break;
    case 3:
        {
            D3D11_TEXTURE3D_DESC desc;
            desc.Width              = image->Length(0);
            desc.Height             = image->Length(1);
            desc.Depth              = image->Length(2);
            desc.MipLevels          = 1;
            desc.Format             = format;
            desc.Usage              = D3D11_USAGE_IMMUTABLE;
            desc.BindFlags          = D3D11_BIND_SHADER_RESOURCE;
            desc.CPUAccessFlags     = 0;
            desc.MiscFlags          = 0;
            Microsoft::WRL::ComPtr<ID3D11Texture3D> texture;
            auto hr = device->CreateTexture3D(&desc, &data, texture.GetAddressOf());
            if (FAILED(hr)) throw std::system_error(hr, std::system_category());
            Texture_ = texture;
        }
        {
            D3D11_SHADER_RESOURCE_VIEW_DESC desc;
            desc.Format                    = format;
            desc.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE3D;
            desc.Texture3D.MostDetailedMip = 0;
            desc.Texture3D.MipLevels       = 1;
            auto hr = device->CreateShaderResourceView(Texture_.Get(), &desc, TextureView_.GetAddressOf());
            if (FAILED(hr)) throw std::system_error(hr, std::system_category());
        }
        break;
    default:
        throw std::runtime_error("GenerateTexture: Unsupported format.");
    }
    {
        D3D11_SAMPLER_DESC desc;
        desc.Filter         = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        desc.AddressU       = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.AddressV       = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.AddressW       = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.MipLODBias     = 0;
        desc.MaxAnisotropy  = 1;
        desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        desc.BorderColor[0] = 0.0f;
        desc.BorderColor[1] = 0.0f;
        desc.BorderColor[2] = 0.0f;
        desc.BorderColor[3] = 0.0f;
        desc.MinLOD         = 0.0f;
        desc.MaxLOD         = D3D11_FLOAT32_MAX;
        auto hr = device->CreateSamplerState(&desc, SamplerState_.GetAddressOf());
        if (FAILED(hr)) throw std::system_error(hr, std::system_category());
    }
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>
TextureDX11::GetTexture(void) {
    return TextureView_;
}

Microsoft::WRL::ComPtr<ID3D11SamplerState>
TextureDX11::GetSampler(void) {
    return SamplerState_;
}

} // namespace Graphene::Graphics
