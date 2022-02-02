/** @file
 * @brief グラフィックスレンダラ(DX11)
 * @author Takaaki Sato
 * @copyright (c) 2022 Demiquartz &lt;info@demiquartz.jp&gt; @n
 * Distributed under the MIT License (See accompanying file LICENSE
 * or copy at https://opensource.org/licenses/MIT)
 */
#include <graphene/graphics/renderer/dx11.hpp>
#include "../../config.hpp"

#include <wrl/client.h>
#include <dxgi1_6.h>
#include <d3d11.h>

#ifndef NDEBUG
#include <iostream>
#endif

namespace Graphene::Graphics {

class RendererDX11 final : public Renderer {
public:
    RendererDX11(UniqueWindow&& window) :
    Window_(std::move(window)),
    ClearColorR_(0.0f),
    ClearColorG_(0.0f),
    ClearColorB_(0.0f),
    ClearColorA_(1.0f) {
        InitializeDX11();
        InitializeDXGI();
        Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
        SwapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
        Device_->CreateRenderTargetView(backBuffer.Get(), nullptr, RenderTargetView_.GetAddressOf());
        DeviceContext_->OMSetRenderTargets(1, RenderTargetView_.GetAddressOf(), nullptr);
        Window_->SetResizeCallback([this](std::size_t width, std::size_t height) {
            BOOL fullscreen;
            if (SUCCEEDED(SwapChain_->GetFullscreenState(&fullscreen, nullptr))) {
                if (fullscreen) Window_->HideCursor();
                else            Window_->ShowCursor();
            }
            DeviceContext_->OMSetRenderTargets(0, nullptr, nullptr);
            RenderTargetView_.Reset();
            SwapChain_->ResizeBuffers(2, width, height, DXGI_FORMAT_UNKNOWN, 0);
            Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
            SwapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
            Device_->CreateRenderTargetView(backBuffer.Get(), nullptr, RenderTargetView_.GetAddressOf());
            DeviceContext_->OMSetRenderTargets(1, RenderTargetView_.GetAddressOf(), nullptr);
        });
        Window_->ShowWindow();
    }

    virtual ~RendererDX11() override {
    }

    virtual void SetClearColor(float red, float green, float blue, float alpha) override {
        ClearColorR_ = red;
        ClearColorG_ = green;
        ClearColorB_ = blue;
        ClearColorA_ = alpha;
    }

    virtual bool ShouldQuit(void) override {
        return Window_->ShouldClose();
    }

    virtual void CancelQuit(void) override {
        Window_->CancelClose();
    }

    virtual void Present(void) override {
        auto hr = SwapChain_->Present(1, 0);
        if (FAILED(hr)) {
            throw std::system_error(hr, std::system_category());
        }
        Window_->PollEvents();
        float clearColor[4] = { ClearColorR_, ClearColorG_, ClearColorB_, ClearColorA_ };
        DeviceContext_->ClearRenderTargetView(RenderTargetView_.Get(), clearColor);
    }

private:
    void InitializeDX11(void) {
        D3D_FEATURE_LEVEL featureLevels[] = {
            D3D_FEATURE_LEVEL_12_1,
            D3D_FEATURE_LEVEL_12_0,
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0
        };
#ifndef NDEBUG
        D3D_FEATURE_LEVEL featureLevel;
#endif
        auto hr = D3D11CreateDevice(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
#ifndef NDEBUG
            D3D11_CREATE_DEVICE_DEBUG,
#else
            0,
#endif
            featureLevels,
            sizeof(featureLevels) / sizeof(D3D_FEATURE_LEVEL),
            D3D11_SDK_VERSION,
            Device_.GetAddressOf(),
#ifndef NDEBUG
            &featureLevel,
#else
            nullptr,
#endif
            DeviceContext_.GetAddressOf()
        );
        if (FAILED(hr)) {
            throw std::system_error(hr, std::system_category());
        }
#ifndef NDEBUG
        std::clog << "FeatureLevel: " << featureLevel << std::endl;
#endif
    }

    void InitializeDXGI(void) {
        Microsoft::WRL::ComPtr<IDXGIFactory7> factory;
        auto hr = CreateDXGIFactory2(
#ifndef NDEBUG
            DXGI_CREATE_FACTORY_DEBUG,
#else
            0,
#endif
            IID_PPV_ARGS(factory.GetAddressOf())
        );
        if (FAILED(hr)) {
            throw std::system_error(hr, std::system_category());
        }
        DXGI_SWAP_CHAIN_DESC1 desc;
        desc.Width              = 0;
        desc.Height             = 0;
        desc.Format             = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.Stereo             = FALSE;
        desc.SampleDesc.Count   = 1;
        desc.SampleDesc.Quality = 0;
        desc.BufferUsage        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        desc.BufferCount        = 2;
        desc.Scaling            = DXGI_SCALING_STRETCH;
        desc.SwapEffect         = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        desc.AlphaMode          = DXGI_ALPHA_MODE_UNSPECIFIED;
        desc.Flags              = 0;
        hr = factory->CreateSwapChainForHwnd(
            Device_.Get(),
            std::any_cast<HWND>(Window_->GetHandle()),
            &desc,
            nullptr,
            nullptr,
            reinterpret_cast<IDXGISwapChain1**>(SwapChain_.GetAddressOf())
        );
        if (FAILED(hr)) {
            throw std::system_error(hr, std::system_category());
        }
    }

private:
    UniqueWindow                                   Window_;
    Microsoft::WRL::ComPtr<IDXGISwapChain4>        SwapChain_;
    Microsoft::WRL::ComPtr<ID3D11Device>           Device_;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext>    DeviceContext_;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> RenderTargetView_;
    float                                          ClearColorR_;
    float                                          ClearColorG_;
    float                                          ClearColorB_;
    float                                          ClearColorA_;
};

SharedRenderer RendererBuilderDX11::Build(WindowBuilder& windowBuilder) const {
    windowBuilder.SetVisible(false);
    return std::make_unique<RendererDX11>(windowBuilder.Build());
}

} // Graphene::Graphics
